/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 16:22:12 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/07 18:59:11 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rpl.hpp"
// #include "Server.hpp"

void	sendRpl( Client *client, std::string const& msgToClient )
{
	send(client->getSocketClient(), msgToClient.c_str(), msgToClient.size(), 0);
}

void	RPL_NOTOPIC(std::string serverName, Client* client, std::string const& channel)
{
	std::string msgToClient = ":" + serverName + " 331 " + client->getNickname() + " " + channel + " :No topic is set\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_TOPIC(std::string const& serverName, Client* client, std::string const& channel, std::string const& topic)
{
	std::string msgToClient = ":" + serverName + " 332 " + client->getNickname() + " " + channel + " :" + topic + "\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_ENDOFNAMES(std::string const& serverName, Client* client, std::string const& channel)
{
	std::string msgToClient = ":" + serverName + " 366 " + client->getNickname() + " " + channel + " :End of NAMES list\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_NAMREPLY(std::string const& serverName, Client* client, Channel* channel)
{
	std::string msgToClient = ":" + serverName + " 353 " + client->getNickname() + " = " + channel->getName() + " :" + channel->getStrAllOperatorsNames() + SPACE + channel->getStrAllUsersNames() + "\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_WELCOME(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 001 " + client->getNickname() + " :" + "HEY SALUT T'ES SUR LE SERVER DE 2 GROSSES MERDES !\r\n";
	sendRpl(client, msgToClient);
}
