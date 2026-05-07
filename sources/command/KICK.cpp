/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:59:44 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/07 16:09:26 by jpiquet          ###   ########.fr       */
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
	for (std::map<SOCKET, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname() == clientToKick)
			_channels[channelTarget]->kickUser(it->second, op, msg);
			// Checker si la channel existe
	}
}
