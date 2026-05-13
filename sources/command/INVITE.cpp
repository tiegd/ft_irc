/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 17:33:07 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/13 10:38:22 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	sendInviteToUser(Client* client, Client* target, std::string const& channel);

/*
INVITE <nickname> <channel>
- Le channel n'est pas obligé d'exister ni d'etre un channel valide pour que le nickname puisse etre invité.
- Checker qu'il y a bien 2 paramettre sinon renvoyer ERR_NEEDMOREPARAMS [x]
- Checker si le <nickname> existe sinon renvoyer ERR_NOSUCHNICK [x]
- Checker si le user qui invite est bien sur le channel sinon renvoyer ERR_NOTONCHANNEL [x]
- Checker si le <nickname> n'est pas déja sur le channel sinon renvoyer ERR_USERONCHANNEL [x]
- Si le channel est en invite only (+i) le USER qui invite doit être operateur du channel sinon renvoyer ERR_CHANOPRIVSNEEDED [x]
*/
void	Server::INVITE(std::string const& line, Client* client)
{
	if (line.size() <= 7)
	{
		ERR_NEEDMOREPARAMS(_name, client, "INVITE");
		std::cout << "Not enough parameters" << std::endl;
		return ;
	}

	std::string					temp(line);
	temp.erase(0, 7);

	std::vector<std::string>	splitParam = split(temp, SPACE);
	std::string					target = splitParam[0];
	std::string					channel = splitParam[1];
	std::cout << "Channel extract : " << channel << std::endl;

	if (target.empty() == true || channel.empty() == true)
	{
		ERR_NEEDMOREPARAMS(_name, client, "INVITE");
		std::cout << "Not enough parameters" << std::endl;
		return ;
	}
	if (nicknameExist(target) == false)
	{
		ERR_NOSUCHNICK(_name, client, target);
		std::cout << "Nickname doesn't exist" << std::endl;
		return ;
	}
	if (_channels[channel])
	{
		if (!_channels[channel]->clientIsOnChannel(client))
		{
			ERR_NOTONCHANNEL(_name, client, channel);
			return ;
		}
		if (_channels[channel]->clientIsOnChannel(searchClient(target)))
		{
			ERR_USERONCHANNEL(_name, client, target, channel);
			return ;
		}
		if (_channels[channel]->getInvitOnly() == true)
		{
			if (!_channels[channel]->isOperator(client))
			{
				ERR_CHANOPRIVSNEEDED(_name, client, channel);
				return ;
			}
		}
	}
	std::cout << channel << std::endl;
	sendInviteToUser(client, searchClient(target), channel);
	RPL_INVITING(_name, client, target, channel);
}

void	sendInviteToUser(Client* client, Client* target, std::string const& channel)
{
	std::string fullMsg = ":" + client->getFullName() + " INVITE " + target->getNickname() + SPACE + channel + "\r\n";
	if (send(target->getSocketClient(), fullMsg.c_str(), fullMsg.size(), 0) < 0)
			throw FunctionError();
}

// bool	isAlreadyOnChannel(std::map<SOCKET, Client*> clientList, std::string const& nickname, Channel* channel)
// {
	
// }
