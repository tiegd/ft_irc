/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NOTICE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 14:21:16 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/12 16:04:31 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
// #include "Client.hpp"
// #include "tools.hpp"
// #include "error_IRC.hpp"
// #include "FunctionError.hpp"
// #include "Channel.hpp"
#include <algorithm>

void	sendNoticeToUser(Client* client, std::string const& target, SOCKET sockTarget, std::string const& message);
void	sendNoticeToChannel(Channel* channel, Client* client, std::string const& message);

void	Server::NOTICE( std::string const& line, Client* client)
{
	if (line.size() <= 7)
	{
		ERR_NORECIPIENT(_name, client);
		throw std::invalid_argument("Channel or nickname missing to send msg");
	}
	std::string	temp(line);
	temp.erase(0, 8);
	
	std::vector<std::string>	splitArgs = split(temp, SPACE);
	std::string					strMessage = splitArgs[1];

	std::vector<std::string>	recipient = split(splitArgs[0], ',');

	if (strMessage.size() > 1 && strMessage[0] == ':')
	{
		for(std::vector<std::string>::iterator it = recipient.begin(); it != recipient.end(); it++)
		{
			sendMessage(client, *it, strMessage);
		}
	}
	else
	{
		ERR_NOTEXTTOSEND(_name, client);
		throw std::invalid_argument("Missing :<text to send>");
	}
}

/*
- Send std::string message to std::string recipient.
- Throw invalid_argument exception if not found or FunctionError exception if send fails.
*/
void	Server::sendNotice( Client *client, std::string recipient, std::string message)
{
	if (recipient[0] == '#')
	{
		sendNoticeToChannel(_channels[recipient], client, message); // fonction qui envoit un message a tous le channel.
	}
	else
	{
		SOCKET	sockRecipient = searchClientSocket(recipient);
		sendNoticeToUser(client, recipient, sockRecipient, message);
	}
}

/*
	Message format: :Angel!wings@irc.org PRIVMSG Wiz :Are you receiving this message!
*/
void	sendNoticeToUser(Client* client, std::string const& target, SOCKET sockTarget, std::string const& message)
{
	std::string fullMsg = ":" + client->getFullName() + " PRIVMSG " + target + " :" + message + "\r\n";
	if (send(sockTarget, fullMsg.c_str(), fullMsg.size(), 0) < 0)
			throw FunctionError();
}

void	sendNoticeToChannel(Channel* channel, Client* client, std::string const& message)
{
	std::string fullMsg = ":" + client->getFullName() + " PRIVMSG " + channel->getName() + " :" + message + "\r\n";
	channel->broadcastToAll(fullMsg, client);
}

