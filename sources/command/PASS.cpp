/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:34:09 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/19 16:37:06 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	correctPassword( std::string const & line, std::string const & password);

int	Server::PASS(std::string const & line, Client* client)
{
	if (client->getRegister() == true)
	{
		ERR_ALREADYREGISTRED(_name, client);
		return -1;
	}
	if (correctPassword(line, _password) == false)
	{
		ERR_PASSWDMISMATCH(_name, client);
		return -1;
	}
	return 1;
}

bool	correctPassword( std::string const & line, std::string const & password)
{
	std::string temp(line);
	
	temp.erase(0, 5);
	if (temp.compare(password) == 0 && temp.size() == password.size())
	{
		return true;
	}
	else
		return false;
}
