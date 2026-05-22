/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 09:44:33 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/21 11:32:53 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
/*
Format : USER <username> <hostname> <servername> :<realname>
*/
int	Server::USER( std::string const & line, Client* client )
{
	if (client->getRegister() == true)
	{
		ERR_ALREADYREGISTRED(_name, client);
		return -1;
	}

	std::string temp(line);
	temp.erase(0, 5);	
	size_t pos = temp.find_first_of(':', 0);
	if (pos == std::string::npos)
	{
		ERR_NEEDMOREPARAMS(_name, client, "USER");
		return -1;
	}

	std::string first_part = temp.substr(0, pos);
	std::string realname = temp.substr(pos + 1, temp.size() - pos );


	std::vector<std::string>	splitParams = split(first_part, SPACE);
	if (splitParams.size() != 3)
	{
		ERR_NEEDMOREPARAMS(_name, client, "USER");
		return -1;
	}
	client->setUsername(splitParams[0]);
	client->setHostname(splitParams[1]);
	client->setRealname(realname);
	return 1;
}
