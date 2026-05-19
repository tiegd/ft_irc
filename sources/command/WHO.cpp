/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 14:56:10 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/19 16:14:21 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	sendWhoFromChannel( Channel* channel, Client* client, std::string const& servName, bool onlyOps );

void	Server::WHO( std::string const& line, Client* client )
{
	if (line.size() == 3 || (line[3] == ' ' && line.size() == 4))
	{
		for (std::map<SOCKET, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			RPL_WHOREPLY(_name, client, it->second, "*");
		}
		RPL_ENDOFWHO(_name, client, "*");
		return ;
	}
	std::string temp(line);
	temp.erase(0, 4);

	std::vector<std::string>	params = split(temp, ' ');
	std::string					mask = params[0];
	bool						onlyOps = false;
	if (params.size() > 1)
	{
		if (params[1].compare("o") == 0)
			onlyOps = true;
	}
	if (mask[0] == '#')
	{
		if (_channels[mask])
		{
			sendWhoFromChannel(_channels[mask], client, _name, onlyOps);
			RPL_ENDOFWHO(_name, client, mask);
			return ;
		}
	}
	if (mask.compare("*") == 0 || mask.compare("0") == 0)
	{
		for (std::map<SOCKET, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			RPL_WHOREPLY(_name, client, it->second, mask);
		}
		RPL_ENDOFWHO(_name, client, mask);
		return ;
	}
	for (std::map<SOCKET, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (mask.compare(it->second->getNickname()) == 0)
		{
			RPL_WHOREPLY(_name, client, it->second, mask);
		}
		else if (mask.compare(it->second->getHostname()) == 0)
		{
			RPL_WHOREPLY(_name, client, it->second, mask);
		}
		else if (mask.compare(it->second->getRealname()) == 0)
		{
			RPL_WHOREPLY(_name, client, it->second, mask);
		}
	}
	RPL_ENDOFWHO(_name, client, mask);
}

void	sendWhoFromChannel( Channel* channel, Client* client, std::string const& servName, bool onlyOps )
{
	
	std::vector<Client*> opList = channel->getOperators();
	for (size_t i = 0; i < opList.size(); ++i)
	{
		if (opList[i] != client)
			RPL_WHOREPLY(servName, client, opList[i], channel->getName());
	}
	if (onlyOps == false)
	{
		std::vector<Client*> userList = channel->getUsers();
		for (size_t i = 0; i < userList.size(); ++i)
		{
			if (userList[i] != client)
				RPL_WHOREPLY(servName, client, userList[i], channel->getName());
		}
	}
}
