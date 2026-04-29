/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:34:09 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/29 17:35:10 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "error_IRC.hpp"

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
void	Server::NICK( std::string const & line, Client* client )
{
	std::string temp(line);

	temp.erase(0, 5);
	std::cout << "Receive in NICK() :" << temp << std::endl;
	if (noNicknameGiven(temp) == true)
	{
		sendError(client, _name, ERR_ERRONEUSNICKNAME, ":No nickname given");
		throw std::invalid_argument("Nickname is empty");
	}
	if (validNickname(temp) == false)
	{
		sendError(client, _name, ERR_ERRONEUSNICKNAME, ":Erroneus nickname");
		throw std::invalid_argument("Wrong nickname format");
	}
	if (nicknameIsAvailable(temp, _clients) == false)
	{
		sendError(client, _name, ERR_NICKNAMEINUSE, ":Nickname is already in use");
		throw std::invalid_argument("Nickname already in use");
	}
	if (client->getRegister() == true)
	{
		// std::cout << "IS REGISTERED" << std::endl;
		for (std::vector<std::string>::iterator it = _nicknameAlreadyUsed.begin(); it != _nicknameAlreadyUsed.end(); it++)
		{
			if (temp == *it)
			{
				_nicknameAlreadyUsed.erase(it);
			}
			std::string	notification = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " NICK " + temp;
			client->sendNotif(notification);
		}
	}
	client->setNickname(temp);
	_nicknameAlreadyUsed.push_back(temp);
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
