/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 14:56:10 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/11 19:01:22 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
	- Si channel
*/
void	Server::WHO( std::string const& line, Client* client )
{
	std::string temp(line);
	temp.erase(0, 5);

	std::vector<std::string>	params = split(temp, ' ');
	std::string					mask = params[0];
	bool						onlyOps = false;
	if (params.size() > 1)
	{
		if (params[1].compare("o") == 0)
			onlyOps = true;
	}
	if (mask[0] = '#')
	{
		if (_channels[mask])
		{
			std::vector<Client*> opList = _channels[mask]->getOperators();
			for (int i = 0; i < opList.size(); ++i)
				RPL_WHOREPLY(_name, client, opList[i], mask);
			if (onlyOps == false)
			{
				std::vector<Client*> userList = _channels[mask]->getUsers();
				for (int i = 0; i < userList.size(); ++i)
					RPL_WHOREPLY(_name, client, userList[i], mask);
			}
		}
	}
	
}
