/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:59:44 by gaducurt          #+#    #+#             */
/*   Updated: 2026/04/30 13:55:02 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

std::vector<std::string>	split( std::string & str, char c );

void Server::KICK(std::string const& line, Client* op)
{
	// Client send : [KICK <channel from> <channel target> :target]

	std::string temp(line);
	
	std::vector<std::string>	splitArgs = split(temp, SPACE);
	std::string					msg;
	std::string 				channelTarget = splitArgs[1];
	std::string					clientToKick = splitArgs[2];

	clientToKick.erase(0, 1);
	msg = ":" + op->getNickname() + " KICK " + channelTarget + " " + clientToKick;
	if (splitArgs.size() == 3)
		msg += " :" + splitArgs[3];
	for (std::map<SOCKET, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname() == clientToKick)
			_channels[channelTarget]->kickUser(it->second, op, msg);
	}
}
