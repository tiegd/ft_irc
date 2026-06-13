/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 18:33:23 by jpiquet           #+#    #+#             */
/*   Updated: 2026/06/13 13:05:42 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	sendPrivmsgToUser(Client* client, Client* target, std::string const& message);
void	sendPrivmsgToChannel(Channel* channel, Client* client, std::string const& message);
int		sameRecipientMultipleTimes(std::vector<std::string> nicknames);

/*
PRIVMSG <receiver>{,<receiver>} <:text to be sent>
*/
void	Server::PRIVMSG( std::string const& line, Client* client)
{
	if (line.size() <= 7)
	{
		ERR_NORECIPIENT(_name, client);
		throw std::invalid_argument("Channel or nickname missing to send msg");
	}
	std::string	temp(line);
	temp.erase(0, 8);

	std::vector<std::string>	splitArgs = splitStr(temp, " :");

	if (splitArgs.size() < 2)
	{
		ERR_NOTEXTTOSEND(_name, client);
		throw std::invalid_argument("Missing :<text to send>");
	}

	std::string					strMessage = splitArgs[1];

	std::vector<std::string>	recipient = split(splitArgs[0], ',');
	int i = sameRecipientMultipleTimes(recipient);
	if (i != -1)
	{
		ERR_TOOMANYTARGETS(_name, client, recipient[i]);
		throw std::invalid_argument("Multiple repetiton of recipient");
	}
	if (strMessage.size() > 1 && strMessage[0] == ':')
	{
		strMessage.erase(0, 1);
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
void	Server::sendMessage( Client *client, std::string recipient, std::string message)
{
	if (recipient[0] == '#')
	{
		if (channelExist(recipient) == true)
		{
			if (_channels[recipient]->clientIsOnChannel(client))
			{
				sendPrivmsgToChannel(_channels[recipient], client, message);
			}
			else
			{
				ERR_CANNOTSENDTOCHAN(_name, client, recipient);
				throw std::invalid_argument("Client is not on channel");
			}
		}
		else
		{
			ERR_NOSUCHNICK(_name, client, recipient);
			throw std::invalid_argument("Socket for channel given can't be found");
		}
	}
	else
	{
		SOCKET	sockRecipient = searchClientSocket(recipient);
		if (sockRecipient == -1) // si on trouve pas le client
		{
			ERR_NOSUCHNICK(_name, client, recipient);
			throw std::invalid_argument("Socket for nickname given can't be found");
		}
		else
		{
			sendPrivmsgToUser(client, _clients[sockRecipient], message);
		}
	}
}

/*
	Return the SOCKET associated to std::string nicknameRecipient or -1 if it's not found
*/
SOCKET	Server::searchClientSocket( std::string nicknameRecipient )
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

/*
	Message format: :Angel!wings@irc.org PRIVMSG Wiz :Are you receiving this message!
*/
void	sendPrivmsgToUser(Client* client, Client* target, std::string const& message)
{
	std::string fullMsg = ":" + client->getFullName() + " PRIVMSG " + client->getNickname() + " :" + message + "\r\n";
	target->outBuff += fullMsg;
	// if (send(sockTarget, fullMsg.c_str(), fullMsg.size(), 0) < 0)
	// 		std::cerr << "send() error" << std::endl;
}

void	sendPrivmsgToChannel(Channel* channel, Client* client, std::string const& message)
{
	std::string fullMsg = ":" + client->getFullName() + " PRIVMSG " + channel->getName() + " :" + message + "\r\n";
	channel->broadcastToAll(fullMsg, client);
}

/*
	Return the index of the recipient that occure multiple times or -1 if not found.
*/
int	sameRecipientMultipleTimes(std::vector<std::string> nicknames)
{
	std::string	temp;

	for (size_t y = 0; y < nicknames.size(); ++y)
	{
		for (size_t i = y + 1; i < nicknames.size(); ++i)
		{
			if (nicknames[y] == nicknames[i])
				return (int)y;
		}
	}
	return -1;
}
