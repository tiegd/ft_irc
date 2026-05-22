/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:59:44 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/20 14:47:29 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<std::string>	split( std::string & str, char c );

// KICK <channel> <user> *( "," <user> ) [<:comment>]
void Server::KICK(std::string const& line, Client* op)
{
	std::string temp(line);
	temp.erase(0, 5);
	
	std::vector<std::string>	splitArgs = split(temp, SPACE);
	
	if (splitArgs.size() < 2)
	{
		ERR_NEEDMOREPARAMS(_name, op, "KICK");
		throw std::invalid_argument("More parameter needed");
	}
	
	std::string				 	channelTarget = splitArgs[0];
	std::vector<std::string>	clientToKick = split(splitArgs[1], ',');
	std::string					comment;
	std::map<std::string, Channel*>::iterator it;

	for (it = _channels.begin(); it != _channels.end(); it++)
	{	
		if (it->second->getName() == channelTarget)
			break;
	}
	if (it == _channels.end())
	{
		ERR_NOSUCHCHANNEL(_name, op, channelTarget);
		throw std::invalid_argument("Channel can't be found");
	}
	for (size_t i = 0; i < clientToKick.size(); i++)
	{
		bool									kicked = false;
		std::map<SOCKET, Client *>::iterator	it;
		for (it = _clients.begin(); it != _clients.end(); it++)
		{
			if (it->second->getNickname() == clientToKick[i])
			{
				if (it->second == op)
					return;
				if (_channels[channelTarget]->isOperator(op))
				{
					if (!_channels[channelTarget]->isOperator(it->second) && !_channels[channelTarget]->isUser(it->second))
					{
						ERR_USERNOTINCHANNEL(_name, op, _channels[channelTarget]->getName());
						throw std::invalid_argument("Client not on that channel");
					}
					_channels[channelTarget]->kickUser(it->second, op);
					kicked = true;
					if (splitArgs.size() >= 3)
					{
						comment = parseComment(splitArgs);
						RPL_CHANMSGKICK(op, it->second, _channels[channelTarget], comment); // reply avec le comment
					}
					else
						RPL_CHANNELKICK(op, it->second, _channels[channelTarget]);
				}
				else
				{
					ERR_CHANOPRIVSNEEDED(_name, op, _channels[channelTarget]->getName());
					throw std::invalid_argument("Client is not channel operator");
				}
			}
		}
		if (it == _clients.end() && !kicked)
		{
			ERR_USERNOTINCHANNEL(_name, op, _channels[channelTarget]->getName());
			throw std::invalid_argument("Client not on that channel");
		}
	}
}

std::string	Server::parseComment(std::vector<std::string> args)
{
	std::string	str = args[2];

	for (size_t i = 3; i < args.size(); i++)
		str += " " + args[i];
	return (str);
}
