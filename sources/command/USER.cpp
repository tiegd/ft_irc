/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 09:44:33 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/07 16:33:09 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
// #include "Client.hpp"
// #include "tools.hpp"
// #include "error_IRC.hpp"

/*
Format : USER <username> <hostname> <servername> :<realname>
Il faut qu'il y ait les 4 parametres sinon : ERR_NEEDMOREPARAMS
Pas que le client soit deja enregistré (que nick et user est dejà été recu): ERR_ALREADYREGISTRED
*/
void	Server::USER( std::string const & line, Client* client )
{
	if (client->getRegister() == true)
	{
		ERR_ALREADYREGISTRED(_name, client);
		throw std::invalid_argument("already registered");
	}

	std::string temp(line);
	temp.erase(0, 5);	
	size_t pos = temp.find_first_of(':', 0);
	if (pos == std::string::npos)
	{
		ERR_NEEDMOREPARAMS(_name, client, "USER");
		throw std::invalid_argument("Need more params");
	}

	std::string first_part = temp.substr(0, pos);
	std::string realname = temp.substr(pos + 1, temp.size() - pos );

	std::cout << first_part << std::endl;
	std::cout << realname << std::endl;

	std::vector<std::string>	splitParams = split(first_part, SPACE);
	if (splitParams.size() != 3)
	{
		ERR_NEEDMOREPARAMS(_name, client, "USER");
		throw std::invalid_argument("Need more params");
	}
	client->setUsername(splitParams[0]);
	client->setHostname(splitParams[1]);
}

// std::vector<std::string>	split( std::string & str, char c )
// {
// 	std::vector<std::string>	strvec;
// 	size_t index = 0;
// 	size_t	pos = 0;

// 	while (index < str.size())
// 	{
// 		pos = str.find(c, index);
// 		if (pos == std::string::npos)
// 		{
// 			strvec.push_back(str.substr(index, str.size()));
// 			break;
// 		}
// 		strvec.push_back(str.substr(index, pos - index));
// 		index = pos + 1;
// 	}
// 	return strvec;
// }

// FUNCTION TESTER
// #include <string>
// #include <iostream>
// #include <exception>
// #include <stdexcept>
// #include <vector>
// #include <algorithm>

// std::vector<std::string>	split( std::string & str );

// int	main( void )
// {
// 	std::string const & line = "jpiquet 0 * :Jocelyn Piquet";
// 	size_t pos = line.find_first_of(':', 0);
// 	if (pos == std::string::npos)
// 	{
// 		sendError(client, _name, ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
// 		throw std::invalid_argument("Need more params");
// 	}
// 	std::string first_part = line.substr(0, pos);
// 	std::string realname = line.substr(pos + 1, line.size() - pos );

// 	std::cout << first_part << std::endl;
// 	std::cout << realname << std::endl;

// 	std::vector<std::string>	splitedArgs = split(first_part);
// 	if (splitedArgs.size() != 3)
// 	{
// 		sendError(client, _name, ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
// 		throw std::invalid_argument("Need more params");
// 	}
	
// 	return 0;
// }