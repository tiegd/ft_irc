/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:59:44 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/13 17:04:03 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<std::string>	split( std::string & str, char c );

void Server::KICK(std::string const& line, Client* op)
{
	// Client send : [KICK <channel> :target [:comment]]

	std::string temp(line);
	temp.erase(0, 5);
	
	std::vector<std::string>	splitArgs = split(temp, SPACE);
	
	if (splitArgs.size() < 2)
	{
		ERR_NEEDMOREPARAMS(_name, op, "KICK");
		return ;
	}
	// splitArgs[1].erase(0);
	std::cout << "channelTarget = " << splitArgs[0] << std::endl;
	std::cout << "client to kick = " << splitArgs[1] << std::endl;
	std::cout << "comment = " << splitArgs[2] << std::endl;
	std::string				 	channelTarget = splitArgs[0];
	std::vector<std::string>	clientToKick = split(splitArgs[1], ',');
	std::string					comment;
	// if (splitArgs.size() == 3)
	// 	comment = parseComment(splitArgs[2]);
	// else
	// 	comment = '\0';
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
	for (int i = 0; i < clientToKick.size(); i++)
	{
		for (std::map<SOCKET, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if (it->second->getNickname() == clientToKick[i])
			{
				if (it->second == op)
					return;
				if (_channels[channelTarget]->isOperator(op))
				{
					std::cout << "yes" << std::endl;
					if (!_channels[channelTarget]->isOperator(it->second) && !_channels[channelTarget]->isUser(it->second))
					{
						ERR_USERNOTINCHANNEL(_name, op, _channels[channelTarget]->getName());
						return;
					}
					_channels[channelTarget]->kickUser(it->second, op);
					if (splitArgs.size() == 3)
						RPL_CHANNELKICK(_name, op, it->second, _channels[channelTarget]); // reply avec le comment
					else
						RPL_CHANMSGKICK(_name, op, it->second, _channels[channelTarget], splitArgs[2]);
					return ;
				}
			}
		}
		ERR_USERNOTINCHANNEL(_name, op, _channels[channelTarget]->getName());
	}
}

std::string	Server::parseComment(std::string str)
{
	if (str[0] != ':')
	{
		std::vector<std::string>	newStr = split(str, SPACE);
		return (newStr[0]);
	}
	else
	{
		str.erase(0, 1);
		return (str);
	}
}
