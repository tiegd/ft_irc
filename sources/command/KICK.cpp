/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 10:55:25 by gaducurt          #+#    #+#             */
/*   Updated: 2026/04/29 17:04:38 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

std::vector<std::string>	split( std::string & str, char c );

void Server::KICK(std::string const& line, Client* op)
{
    // Client send : [KICK <channel d'où on parle> <channel sur lequel on veut kick> :client à kick]
    // parsing

	std::string temp(line);
	
	std::vector<std::string>	splitArgs = split(temp, SPACE);
	std::string 				channelTarget = splitArgs[1];
	std::string					clientToKick = splitArgs[2];
	if (splitArgs.size() == 3)
		std::string				comment = splitArgs[3];

	clientToKick.erase(0, 1);
	for (std::map<SOCKET, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname() == clientToKick)
			_channels[channelTarget]->kickUser(it->second, op);
	}
	
}
