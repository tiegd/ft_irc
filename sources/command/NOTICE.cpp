/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NOTICE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 14:21:16 by jpiquet           #+#    #+#             */
/*   Updated: 2026/06/19 10:38:40 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	sendNoticeToUser(Client* client, Client* target, std::string const& message);
void	sendNoticeToChannel(Channel* channel, Client* client, std::string const& message);

void	Server::NOTICE( std::string const& line, Client* client)
{
	if (line.size() <= 7)
	{
		ERR_NORECIPIENT(_name, client);
		throw std::invalid_argument("Channel or nickname missing to send msg");
	}
	std::string	temp(line);
	temp.erase(0, 7);
	
	std::vector<std::string>	splitArgs = splitStr(temp, " :");

	if (splitArgs.size() < 2)
	{
		ERR_NOTEXTTOSEND(_name, client);
		throw std::invalid_argument("Missing :<text to send>");
	}

	std::string					strMessage = splitArgs[1];

	std::vector<std::string>	recipient = split(splitArgs[0], ',');

	if (strMessage.size() > 1 && strMessage[0] == ':')
	{
		strMessage.erase(0, 1);
		for(std::vector<std::string>::iterator it = recipient.begin(); it != recipient.end(); it++)
		{
			sendNotice(client, *it, strMessage);
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
		if (channelExist(recipient) == true)
		{
			if (_channels[recipient]->clientIsOnChannel(client))
			{
				sendNoticeToChannel(_channels[recipient], client, message);
			}
			else
			{
				ERR_CANNOTSENDTOCHAN(_name, client, recipient);
				throw std::invalid_argument("Client is not on channel");
			}
		}
		else
			throw std::invalid_argument("Socket for channel given can't be found");
	}
	else
	{
		SOCKET	sockRecipient = searchClientSocket(recipient);
		if (sockRecipient == -1)
		{
			throw std::invalid_argument("Socket for nickname given can't be found");
		}
		sendNoticeToUser(client, _clients[sockRecipient], message);
	}
}

/*
	Message format: :Angel!wings@irc.org NOTICE Wiz :Are you receiving this message!
*/
void	sendNoticeToUser(Client* client, Client* target, std::string const& message)
{
	std::string fullMsg = ":" + client->getFullName() + " NOTICE " + target->getNickname() + " :" + message + "\r\n";
	target->outBuff += fullMsg;
}

void	sendNoticeToChannel(Channel* channel, Client* client, std::string const& message)
{
	std::string fullMsg = ":" + client->getFullName() + " NOTICE " + channel->getName() + " :" + message + "\r\n";
	channel->broadcastToAll(fullMsg, client);
}

