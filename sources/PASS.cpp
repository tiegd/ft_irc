/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:34:09 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/20 19:06:01 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "error.hpp"

void	Server::PASS(std::string const & line, Client* client)
{
	if (client->getRegister() == true)
		sendError(client, _name, ERR_ALREADYREGISTRED, ":You may not reregister");
	if (correctPassword(line, _password) == false)
	{
		sendError(client, _name, ERR_PASSWDMISMATCH, ":Password incorrect");
		throw std::invalid_argument("Wrong PASS");
	}
}

bool	correctPassword( std::string const & line, std::string const & password)
{
	std::string temp(line);
	
	temp = temp.erase(0, 4);
	if (temp.compare(password) == 0)
	{
		return true;
	}
	else
		return false;
}
