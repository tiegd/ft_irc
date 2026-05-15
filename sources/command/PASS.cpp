/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:34:09 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/12 11:54:14 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
// #include "Client.hpp"
// #include "error_IRC.hpp"

bool	correctPassword( std::string const & line, std::string const & password);

int	Server::PASS(std::string const & line, Client* client)
{
	if (client->getRegister() == true)
	{
		ERR_ALREADYREGISTRED(_name, client);
		std::cout << "already registered" << std::endl;
		// throw std::invalid_argument("already registered");
		return -1;
	}
	if (correctPassword(line, _password) == false)
	{
		ERR_PASSWDMISMATCH(_name, client);
		std::cout << "wrong pass" << std::endl;
		// throw std::invalid_argument("Wrong PASS");
		return -1;
	}
	return 1;
}

bool	correctPassword( std::string const & line, std::string const & password)
{
	std::string temp(line);
	
	temp.erase(0, 5);
	std::cout << temp << std::endl;
	if (temp.compare(password) == 0 && temp.size() == password.size())
	{
		return true;
	}
	else
		return false;
}
