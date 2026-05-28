/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:34:09 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/19 16:35:53 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	validNickname( std::string line);
bool	nicknameIsAvailable( std::string line, std::map<SOCKET, Client*> clients );
bool	noNicknameGiven( std::string line);

int	Server::NICK( std::string const & line, Client* client )
{
	std::string temp(line);

	temp.erase(0, 5);
	if (noNicknameGiven(temp) == true)
	{
		ERR_NONICKNAMEGIVEN(_name, client);
		return -1;
	}
	if (validNickname(temp) == false)
	{
		ERR_ERRONEUSNICKNAME(_name, client);
		return -1;
	}
	if (nicknameExist(temp) == true)
	{
		ERR_NICKNAMEINUSE(_name, client);
		return -1;
	}
	if (client->getRegister() == true)
	{
		for (std::vector<std::string>::iterator it = _nicknameAlreadyUsed.begin(); it != _nicknameAlreadyUsed.end(); it++)
		{
			if (temp == *it)
			{
				_nicknameAlreadyUsed.erase(it);
				break;
			}
		}
	}
	std::string	notification = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " NICK " + temp + "\r\n";
	client->sendNotif(notification);
	client->broadcastToMyChannels(notification);
	client->setNickname(temp);
	_nicknameAlreadyUsed.push_back(temp);
	return 1;
}

bool	validNickname( std::string line )
{
	if (line.size() >= 1 || line.size() <= 9)
	{
		if (line.find_first_of(INVALID_CHAR, 0) != std::string::npos
		|| line.find_first_not_of(FIRST_VALID_CHAR, 0) == 0
		|| line.find_first_not_of(ALL_VALID_CHAR, 0) != std::string::npos)
			return false;
		else
			return true;
	}
	return false;
}

bool	nicknameIsAvailable( std::string line, std::map<SOCKET, Client*> clients )
{
	for (std::map<SOCKET, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getNickname() == line)
		{
			return false;
		}
	}
	return true;
}

bool	noNicknameGiven( std::string line)
{
	if (line.size() <= 0)
	{
		return true;
	}
	return false;
}
