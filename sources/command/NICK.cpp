/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:34:09 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/12 15:23:59 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
// #include "Client.hpp"
// #include "error_IRC.hpp"

bool	validNickname( std::string line);
bool	nicknameIsAvailable( std::string line, std::map<SOCKET, Client*> clients );
bool	noNicknameGiven( std::string line);

/*
- Checker si c'est le bon format:
1 à 9 caractere max
caractere autorisé en plus de a-z : { } \| [ ] \ ^ _
il ne doit pas commencer par un chiffre.
caratere interdit : :, espace, *, ?, @, !
- Checker si il existe deja. Sinon renvoyer une erreur ERR_NICKNAMEINUSE.
*/
int	Server::NICK( std::string const & line, Client* client )
{
	std::string temp(line);

	temp.erase(0, 5);
	if (noNicknameGiven(temp) == true)
	{
		ERR_NONICKNAMEGIVEN(_name, client);
		return -1;
		// throw std::invalid_argument("Nickname is empty");
	}
	if (validNickname(temp) == false)
	{
		ERR_ERRONEUSNICKNAME(_name, client);
		return -1;
		// throw std::invalid_argument("Wrong nickname format");
	}
	if (nicknameExist(temp) == true)
	{
		ERR_NICKNAMEINUSE(_name, client);
		return -1;
		// throw std::invalid_argument("Nickname already in use");
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
	// std::cout << "NOTIF CHANGEMENT DE NICK : " << notification << std::endl;
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
		if (line.find_first_of(INVALID_CHAR, 0) != std::string::npos //trouve le 1er mauvais caractere
		|| line.find_first_not_of(FIRST_VALID_CHAR, 0) == 0 //trouve le 1er caractere qui n'est pas valide, si l'index renvoyé est 0 c'est pas bon
		|| line.find_first_not_of(ALL_VALID_CHAR, 0) != std::string::npos) // cette ligne est inutile normalement puisqu'ele fait le meme check que le 1er(cependant regarde plus de charactere donc peut etre mieux)
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
