/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 16:22:12 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/13 14:30:06 by jpiquet          ###   ########.fr       */
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
	std::string msgToClient = ":" + serverName + " 353 " + client->getNickname() + " = " + channel->getName()
								+ " :" + channel->getStrAllOperatorsNames() + SPACE + channel->getStrAllUsersNames() + "\r\n";
	sendRpl(client, msgToClient);
}



/* :serveur 352 <nick_qui_a_fait_WHO> <canal> <user> <host> <serveur> <nick_cible> :<hopcount> <realname> */
void	RPL_WHOREPLY(std::string const& serverName, Client *client, Client* target, std::string const& channelName)
{
	std::cout << "rpl_whoreply\n";
	std::string	msgToClient = ":" + serverName + " 352 " + client->getNickname()
							+ SPACE + channelName
							+ SPACE + target->getUsername()
							+ SPACE + target->getHostname() 
							+ SPACE + serverName
							+ SPACE + target->getNickname() + " :" + "0 " + target->getRealname() + "\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_ENDOFWHO(std::string const& serverName, Client* client, std::string const& mask)
{
	std::string msgToClient = ":" + serverName + " 315 " + client->getNickname() + SPACE + mask + " :End of /WHO list\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_INVITING(std::string const& serverName, Client* client, std::string const& target, std::string const& channel)
{
	std::string msgToClient = ":" + serverName + " 341 " + client->getNickname() + SPACE + channel + SPACE + target + "\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_WELCOME(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 001 " + client->getNickname() + " :" + "Welcome to the Internet Relay Network " + client->getFullName() + "\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_YOURHOST(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 002 " + client->getNickname() + " :" + "Your host is " + serverName + ", running version PROUT.2\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_CREATED(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 003 " + client->getNickname() + " :" + "This server was created 2026/04/15\r\n";
	sendRpl(client, msgToClient);
}

void	RPL_MYINFO(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 004 " + client->getNickname() + SPACE + serverName + " PROUT.2 " + "" + " oitlk\r\n"; 
	sendRpl(client, msgToClient);
}

/*       001    RPL_WELCOME
              "Welcome to the Internet Relay Network
               <nick>!<user>@<host>"
       002    RPL_YOURHOST
              "Your host is <servername>, running version <ver>"
       003    RPL_CREATED
              "This server was created <date>"
       004    RPL_MYINFO
              "<servername> <version> <available user modes>
               <available channel modes>"

         - The server sends Replies 001 to 004 to a user upon
           successful registration.
*/