/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NOTICE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 14:21:16 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/28 14:12:43 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "tools.hpp"
#include <algorithm>

/* Same as PRIVMSG but we doesn't send error if the channel or tue user can't be found*/
void	Server::NOTICE( std::string const& line, Client* client)
{
	if (line.size() <= 5)
	{
		sendError(client, _name, ERR_NORECIPIENT, ":No recipient given (NOTICE)");
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
			sendNotice(client, *it, strMessage);
		}
	}
	else
	{
		sendError(client, _name, ERR_NOTEXTTOSEND, ":No text to send");
		throw std::invalid_argument("Missing :<text to send>");
	}
}

void	Server::sendNotice( Client *client, std::string recipient, std::string message)
{
	if (recipient[0] == '#')
	{
		if (channelExist(recipient) == true) // si on trouve le channel
			_channels[recipient].sendChannelMsg(message); // fonction qui envoit un message a tous le channel.
		else // sinon revoyer une erreur
			throw std::invalid_argument("Socket for channel given can't be found");
	}
	else
	{
		SOCKET	sockRecipient = searchClient(recipient);
		if (sockRecipient == -1) // si on trouve pas le client
			throw std::invalid_argument("Socket for nickname given can't be found");
		if (send(client->getSocketClient(), message.c_str(), message.size(), 0) < 0) // si on le trouve on envoie le message au socket du nickname associé
			throw FunctionError("send");
	}
}
