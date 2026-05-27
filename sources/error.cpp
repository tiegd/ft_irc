/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:27:20 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/27 15:29:32 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_IRC.hpp"

/* FORMAT ERROR : { :PREFIX CODE TARGET :message error\r\n }
- Prefix: Name of the server
- CodeError: Error code to be processed by the client
- Target: Client to whom the error is being sent.
- Message error.
*/
void	sendError( Client *client, std::string const& msgToClient )
{
	send(client->getSocketClient(), msgToClient.c_str(), msgToClient.size(), 0);
}

// { -------------- NICK ------------------ }
void	ERR_NONICKNAMEGIVEN(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 431 " + client->getNickname() + " :No nickname given\r\n";
	sendError(client, msgToClient);
}

void	ERR_ERRONEUSNICKNAME(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 432 " + client->getNickname() + " :Erroneus nickname\r\n";
	sendError(client, msgToClient);
}

void	ERR_NICKNAMEINUSE(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 433 " + client->getNickname() + " :Nickname is already in use\r\n";
	sendError(client, msgToClient);
}

void	ERR_ALREADYREGISTRED(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 462 " + client->getNickname() + " :You may not reregister\r\n";
	sendError(client, msgToClient);
}

void	ERR_NOTREGISTERED( std::string const& serverName, Client* client )
{
	std::string msgToClient = ":" + serverName + " 451 " + client->getNickname() + " :You have not registered\r\n";
	sendError(client, msgToClient);
}

// { -------------- PASS ----------------- }
void	ERR_PASSWDMISMATCH(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 464 " + client->getNickname() + " :Password incorrect\r\n";
	sendError(client, msgToClient);
}

void	ERR_NEEDMOREPARAMS(std::string const& serverName, Client* client, std::string const& command)
{
	std::string msgToClient = ":" + serverName + " 461 " + client->getNickname() + SPACE + command + " :Not enough parameters\r\n";
	sendError(client, msgToClient);
}

// { -------------- JOIN ----------------- }
void	ERR_BADCHANMASK(std::string const& serverName, Client* client, std::string const& channel)
{
	std::string msgToClient = ":" + serverName + " 476 " + client->getNickname() + SPACE + channel + " :Bad Channel Mask\r\n";
	sendError(client, msgToClient);
}

void	ERR_BADCHANNELKEY(std::string const& serverName, Client* client, std::string const& channel)
{
	std::string msgToClient = ":" + serverName + " 475 " + client->getNickname() + SPACE + channel + " :Cannot join channel (+k)\r\n";
	sendError(client, msgToClient);
}

void    ERR_CHANNELISFULL( std::string const& serverName, Client* client, std::string const& channel )
{
	std::string msgToClient = ":" + serverName + " 471 " + client->getNickname() + SPACE + channel + " :Cannot join channel (+l)\r\n";
	sendError(client, msgToClient);
}

// { ------------------- PRIVMSG -------------- }
void	ERR_NORECIPIENT(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 411 " + client->getNickname() + " :No recipient given\r\n";
	sendError(client, msgToClient);
}

void	ERR_NOTEXTTOSEND(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 412 " + client->getNickname() + " :No text to send\r\n";
	sendError(client, msgToClient);
}

void	ERR_NOSUCHNICK(std::string const& serverName, Client* client, std::string const& nick)
{
	std::string msgToClient = ":" + serverName + " 401 " + client->getNickname() + SPACE + nick + " :No such nick/channel\r\n";
	sendError(client, msgToClient);
}

void	ERR_INVITEONLYCHAN(std::string const& serverName, Client* client, std::string const& channel)
{
	std::string msgToClient = ":" + serverName + " 473 " + client->getNickname() + SPACE + channel + " :Cannot join channel (+i)\r\n";
	sendError(client, msgToClient);
}

void	ERR_NOTONCHANNEL(std::string const& serverName, Client* client, std::string const& channel)
{
	std::string msgToClient = ":" + serverName + " 442 " + client->getNickname() + SPACE + channel + " :You're not on that channel\r\n";
	sendError(client, msgToClient);
}

void	 ERR_CHANOPRIVSNEEDED(std::string const& serverName, Client* client, std::string const& channel)
{
	std::string msgToClient = ":" + serverName + " 482 " + client->getNickname() + SPACE + channel + " :You're not channel operator\r\n";
	sendError(client, msgToClient);
}

// { ------------------- MODE -------------- }
void	ERR_UMODEUNKNOWNFLAG(std::string const& serverName, Client* client)
{
	std::string msgToClient = ":" + serverName + " 501 " + client->getNickname() + " :Unknown MODE flag\r\n";
	sendError(client, msgToClient);
}

void	ERR_INVALIDMODEPARAM(std::string const& serverName, Client* client, std::string const& channel, char mode, std::string params, std::string str)
{
	std::string msgToClient = ":" + serverName + " 696 " + client->getNickname() + SPACE + channel + SPACE + mode + SPACE + params + " :" + str + "\r\n";
	sendError(client, msgToClient);
}

void	ERR_NOSUCHCHANNEL(std::string const& serverName, Client* client, std::string channel)
{
	std::string msgToClient = ":" + serverName + " 403 " + client->getNickname() + SPACE + channel +  " :No such channel\r\n";
	sendError(client, msgToClient);
}

void	ERR_USERNOTINCHANNEL(std::string const& serverName, Client* client, std::string channel)
{
	std::string msgToClient = ":" + serverName + " 441 " + client->getNickname() + SPACE + channel +  " :They aren't on that channel\r\n";
	sendError(client, msgToClient);
}

void	ERR_USERONCHANNEL(std::string const& serverName, Client* client, std::string const& target, std::string const& channelName)
{
	std::string msgToClient = ":" + serverName + " 443 " + client->getNickname() + SPACE + target + SPACE + channelName + " :Is already on channel\r\n";
	sendError(client, msgToClient);
}

void	ERR_TOOMANYTARGETS(std::string const& serverName, Client* client, std::string const& target)
{
	std::string msgToClient = ":" + serverName + " 407 " + client->getNickname() + SPACE + target + " :Duplicate recipients. No message delivered\r\n";
	sendError(client, msgToClient);
}

void	ERR_NOMOTD( std::string const& serverName, Client* client )
{
	std::string msgToClient = ":" + serverName + " 422 " + client->getNickname() + " :MOTD File is missing\r\n";
	sendError(client, msgToClient);
}

void    ERR_UNKNOWNCOMMAND(std::string const & serverName, Client* client, std::string const & command)
{
	std::string msgToClient = ":" + serverName + " 421 " + client->getNickname() + SPACE + command + " :Unknown command\r\n";
	sendError(client, msgToClient);
}

void	ERR_CANNOTSENDTOCHAN(std::string const& serverName, Client* client, std::string const& channelName)
{

	std::string msgToClient = ":" + serverName + " 404 " + client->getNickname() + SPACE + channelName + " :Cannot send to channel\r\n";
	sendError(client, msgToClient);
}
