/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 18:33:23 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/23 14:15:04 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "tools.hpp"
#include <algorithm>

/*
PRIVMSG <receiver>{,<receiver>} <:text to be sent>
*/
void	Server::PRIVMSG( std::string const& line, Client* client)
{
	if (line.size() <= 5)
	{
		sendError(client, _name, ERR_NORECIPIENT, ":No recipient given (PRIVMSG)");
		throw std::invalid_argument("Channel or nickname missing to send msg");
	}
	std::string	temp(line);
	temp.erase(0, 5);
	
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
		sendError(client, _name, ERR_NOTEXTTOSEND, ":No text to send");
		throw std::invalid_argument("Missing :<text to send>");
	}
}

/*
- Send std::string message to std::string recipient.
- Throw invalid_argument exception if not found or FunctionError exception if send fails.
*/
void	Server::sendMessage( Client *client, std::string recipient, std::string message)
{
	if (recipient[0] == '#')
	{
		if (channelExist(recipient) == true) // si on trouve le channel
			_channels[recipient].sendChannelMsg(message); // fonction qui envoit un message a tous le channel.
		else // sinon revoyer une erreur
		{
			sendError(client, _name, ERR_NOSUCHNICK, recipient + " :No such nick/channel");
			throw std::invalid_argument("Socket for channel given can't be found");
		}
	}
	else
	{
		SOCKET	sockRecipient = searchClient(recipient);
		if (sockRecipient == -1) // si on trouve pas le client
		{
			sendError(client, _name, ERR_NOSUCHNICK, recipient + " :No such nick/channel");
			throw std::invalid_argument("Socket for nickname given can't be found");
		}
		if (send(client->getSocketClient(), message.c_str(), message.size(), 0) < 0) // si on le trouve on envoie le message au socket du nickname associé
			throw FunctionError("send");
	}
}

/* Return the SOCKET associated to std::string nicknameRecipient or -1 if it's not found*/
SOCKET	const& Server::searchClient( std::string nicknameRecipient )
{
	for (std::map<SOCKET, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (nicknameRecipient.compare(it->second->getNickname()) == 0)
		{
			return it->second->getSocketClient();
		}
	}
	return -1;
}

/* Return true if the <channel>[channelName] exist or false if not*/
bool	Server::channelExist( std::string channelName )
{
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (channelName.compare(it->first) == 0)
		{
			return true;
		}
	}
	return false;
}
