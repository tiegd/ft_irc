/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:59:44 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/11 15:32:58 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
// #include "Client.hpp"
// #include "Channel.hpp"

std::vector<std::string>	split( std::string & str, char c );

void Server::KICK(std::string const& line, Client* op)
{
	// Client send : [KICK <channel from> <channel target> :target]

	std::string temp(line);
	temp.erase(0, 5);
	
	std::vector<std::string>	splitArgs = split(temp, SPACE);
	std::string					msg;
	std::string 				channelTarget = splitArgs[0];
	std::string					clientToKick = splitArgs[1];

	clientToKick.erase(0, 1);
	msg = ":" + op->getNickname() + " KICK " + channelTarget + " " + clientToKick;
	if (splitArgs.size() == 3)
		msg += " :" + splitArgs[2];
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it->second->getName() == channelTarget)
			break;
		if (it == _channels.end())
		{
			ERR_NOSUCHCHANNEL(_name, op, channelTarget);
			return;
		}
	}
	for (std::map<SOCKET, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname() == clientToKick)
		{
			if (it->second == op)
				return;
			if (_channels[channelTarget]->isOperator(op))
			{
				if (!_channels[channelTarget]->isOperator(it->second) || !_channels[channelTarget]->isUser(it->second))
				{
					ERR_NOTONCHANNEL(_name, it->second, _channels[channelTarget]->getName());
					return;
				}
				_channels[channelTarget]->kickUser(it->second, op, msg);
			}
		}
	}
}
