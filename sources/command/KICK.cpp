/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:59:44 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/13 11:48:24 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<std::string>	split( std::string & str, char c );

void Server::KICK(std::string const& line, Client* op)
{
	// Client send : [KICK <channel> :target]

	std::string temp(line);
	temp.erase(0, 5);
	
	std::vector<std::string>	splitArgs = split(temp, SPACE);
	
	if (splitArgs.size() < 2)
	{
		ERR_NEEDMOREPARAMS(_name, op, "KICK");
		return ;
	}
	std::vector<std::string> 	channelTarget = split(splitArgs[0], ',');
	std::vector<std::string>	clientToKick = split(splitArgs[1], ',');
	std::string					comment = parseComment(splitArgs[2]);

	if (channelTarget.size() != clientToKick.size())
	{
		ERR_NEEDMOREPARAMS(_name, op, "KICK");
		return ;
	}
	for (int i = 0; i < channelTarget.size(); i++)
	{
		for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
		{
			if (it->second->getName() == channelTarget[i])
				break;
			if (it == _channels.end())
			{
				ERR_NOSUCHCHANNEL(_name, op, channelTarget[i]);
				return;
			}
		}
		for (std::map<SOCKET, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if (it->second->getNickname() == clientToKick[i])
			{
				if (it->second == op)
				{
					std::cout << "autoban" << std::endl;
					return;
				}
				if (_channels[channelTarget[i]]->isOperator(op))
				{
					if (!_channels[channelTarget[i]]->isOperator(it->second) && !_channels[channelTarget[i]]->isUser(it->second))
					{
						ERR_USERNOTINCHANNEL(_name, op, _channels[channelTarget[i]]->getName());
						return;
					}
					_channels[channelTarget[i]]->kickUser(it->second, op, comment);
				}
			}
		}
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
