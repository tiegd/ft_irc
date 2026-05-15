/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 16:33:44 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/14 14:08:15 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	sendPartMsg( Client* client, std::string const& fullName, Channel* channel, std::string const& msg );

void	Server::PART(std::string const& line, Client* client)
{
	if (line.size() <= 5)
	{
		ERR_NEEDMOREPARAMS(_name, client, "PART");
		throw std::invalid_argument("Not enough parameters");		
	}

	std::string	temp(line);
	temp.erase(0, 5);

	std::cout << temp <<std::endl;
	std::vector<std::string>	splitArgs = splitStr(temp, " :");
	std::vector<std::string>	nameChannels = split(splitArgs[0], ',');
	std::string					message = splitArgs[1];

	// std::cout << "channel name : " << splitArgs[0] << std::endl;
	// std::cout << "message : " << message << std::endl;

	for (size_t i = 0; i < nameChannels.size(); ++i)
	{
		if (!_channels[nameChannels[i]])
		{
			ERR_NOSUCHCHAN(_name, client, nameChannels[i]);
			continue ;
		}
		if (!_channels[nameChannels[i]]->clientIsOnChannel(client))
		{
			ERR_NOTONCHANNEL(_name, client, nameChannels[i]);
			continue;
		}
		if (_channels[nameChannels[i]]->isUser(client))
		{
			_channels[nameChannels[i]]->rmUser(client);
		}
		if (_channels[nameChannels[i]]->isOperator(client))
		{
			_channels[nameChannels[i]]->rmOperator(client);
		}
		sendPartMsg(client, client->getFullName(), _channels[nameChannels[i]], message);
		if (_channels[nameChannels[i]]->getNbMembers() == 0 && _channels[nameChannels[i]]->getNbOp() == 0)
		{
			delete _channels[nameChannels[i]];
		}
	}
}

void	sendPartMsg( Client* client, std::string const& fullName, Channel* channel, std::string const& msg )
{
	std::string	fullMsg = ":" + fullName + " PART " + channel->getName() + " " + msg + "\r\n";
	sendRpl(client, fullMsg);
	channel->broadcastToAll(fullMsg, client);
}
