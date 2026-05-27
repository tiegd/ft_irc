/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 17:33:07 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/27 13:56:53 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	sendInviteToUser(Client* client, Client* target, std::string const& channel);

/*
INVITE <nickname> <channel>
*/
void	Server::INVITE(std::string const& line, Client* client)
{
	if (line.size() <= 7)
	{
		ERR_NEEDMOREPARAMS(_name, client, "INVITE");
		throw std::invalid_argument("Not enough parameters");
	}

	std::string					temp(line);
	temp.erase(0, 7);

	std::vector<std::string>	splitParam = split(temp, SPACE);
	if (splitParam.size() < 2)
	{
		ERR_NEEDMOREPARAMS(_name, client, "INVITE");
		throw std::invalid_argument("Not enough parameters");
	}
	std::string					target = splitParam[0];
	std::string					channel = splitParam[1];
	if (target.empty() == true || channel.empty() == true)
	{
		ERR_NEEDMOREPARAMS(_name, client, "INVITE");
		throw std::invalid_argument("Not enough parameters");
	}
	if (!channelExist(channel))
	{
		ERR_NOSUCHCHANNEL(_name, client, channel);
		throw std::invalid_argument("No such channel");
	}
	if (nicknameExist(target) == false)
	{
		ERR_NOSUCHNICK(_name, client, target);
		throw std::invalid_argument("Socket for nickname given can't be found");
	}
	if (_channels[channel])
	{
		if (!_channels[channel]->clientIsOnChannel(client))
		{
			ERR_NOTONCHANNEL(_name, client, channel);
			throw std::invalid_argument("Client not on that channel");
		}
		if (_channels[channel]->clientIsOnChannel(searchClient(target)))
		{
			ERR_USERONCHANNEL(_name, client, target, channel);
			throw std::invalid_argument("Client is already on channel");
		}
		if (_channels[channel]->getInvitOnly() == true)
		{
			if (!_channels[channel]->isOperator(client))
			{
				ERR_CHANOPRIVSNEEDED(_name, client, channel);
				throw std::invalid_argument("Client is not channel operator");
			}
		}
	}
	_channels[channel]->addBackInvite(searchClient(target));
	sendInviteToUser(client, searchClient(target), channel);
	RPL_INVITING(_name, client, target, channel);
}

void	sendInviteToUser(Client* client, Client* target, std::string const& channel)
{
	std::string fullMsg = ":" + client->getFullName() + " INVITE " + target->getNickname() + SPACE + channel + "\r\n";
	if (send(target->getSocketClient(), fullMsg.c_str(), fullMsg.size(), 0) < 0)
			std::cerr << "send() error" << std::endl;
}
