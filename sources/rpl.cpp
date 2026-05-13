/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 16:22:12 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/13 17:13:48 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rpl.hpp"
// #include "Server.hpp"

void	sendRpl( Client *client, std::string const& msgToClient )
{
	// std::cout << "sendRpl socket = " << client->getSocketClient() << std::endl;
	if (send(client->getSocketClient(), msgToClient.c_str(), msgToClient.size(), 0) < 0)
		std::cout << "send() error" << std::endl;
}

void	RPL_NOTOPIC(std::string serverName, Client* client, std::string const& channel)
{
	// std::cout << "RPL_NOTOPIC socket = " << client->getSocketClient() << std::endl;
	std::string msgToClient = ":" + serverName + " 331 " + client->getNickname() + " " + channel + " :No topic is set\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_TOPIC(std::string const& serverName, Client* client, std::string const& channel, std::string const& topic)
{
	// std::cout << "RPL_TOPIC socket = " << client->getSocketClient() << std::endl;
	std::string msgToClient = ":" + serverName + " 332 " + client->getNickname() + " " + channel + " :" + topic + "\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_ENDOFNAMES(std::string const& serverName, Client* client, std::string const& channel)
{
	// std::cout << "RPL_ENDOFNAMES socket = " << client->getSocketClient() << std::endl;
	std::string msgToClient = ":" + serverName + " 366 " + client->getNickname() + " " + channel + " :End of NAMES list\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_NAMREPLY(std::string const& serverName, Client* client, Channel* channel)
{
	// std::cout << "RPL_NAMREPLY socket = " << client->getSocketClient() << std::endl;
	std::string msgToClient = ":" + serverName + " 353 " + client->getNickname() + " = " + channel->getName() + " :" + channel->getStrAllOperatorsNames() + SPACE + channel->getStrAllUsersNames() + "\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_WELCOME(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 001 " + client->getNickname() + " :" + "HEY SALUT T'ES SUR LE SERVER DE 2 GROSSES MERDES !\r\n";
	sendRpl(client, msgToClient);
}

void    RPL_CHANNELMODEIS(std::string const& serverName, Client* client, Channel* channel)
{
	std::string msgToClient = ":" + serverName + " 324 " + client->getNickname() + channel->getName() + " "  + channel->getModeString() + "\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_CHANNELKICK(std::string const& serverName, Client *op, Client* client, Channel* channel)
{
	std::string msgToClient = ":" + serverName + " 808 " + ":" + op->getFullName() + " KICK " + channel->getName() + " " + client->getNickname() + "\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_CHANMSGKICK(std::string const& serverName, Client *op, Client* client, Channel* channel, std::string comment)
{
	std::string msgToClient = ":" + serverName + " 909 " + ":" + op->getFullName() + " KICK " + channel->getName() + " " + client->getNickname() + " " + comment + "\r\n";
	sendRpl(client, msgToClient);
}
