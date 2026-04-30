/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:34:09 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/29 13:52:42 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "error_IRC.hpp"

bool	correctPassword( std::string const & line, std::string const & password);

void	Server::PASS(std::string const & line, Client* client)
{
	std::cout << "Pass receive : " << line << " | Password server : " << _password << std::endl;
	if (client->getRegister() == true)
	{
		sendError(client, _name, ERR_ALREADYREGISTRED, ":You may not reregister");
		throw std::invalid_argument("already registered");
	}
	if (correctPassword(line, _password) == false)
	{
		sendError(client, _name, ERR_PASSWDMISMATCH, ":Password incorrect");
		throw std::invalid_argument("Wrong PASS");
	}
}

bool	correctPassword( std::string const & line, std::string const & password)
{
	std::string temp(line);
	
	temp.erase(0, 5);
	// std::cout << "pass receive : " << temp << std::endl;
	if (temp.compare(password) == 0)
	{
		return true;
	}
	else
		return false;
}
