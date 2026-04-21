/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:34:09 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/20 19:14:25 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "error.hpp"

bool	validNickname( std::string const & line);
bool	nicknameIsAvailable( std::string const & line, std::map<SOCKET, Client*> clients );
bool	noNicknameGiven( std::string const & line);

/*
- Checker si c'est le bon format:
1 à 9 caractere max
caractere autorisé en plus de a-z : { } \| [ ] \ ^ _
il ne doit pas commencer par un chiffre.
caratere interdit : :, espace, *, ?, @, !
- Checker si il existe deja. Sinon renvoyer une erreur ERR_NICKNAMEINUSE.
*/
void	Server::NICK(std::string const & line, Client *client)
{
	if (noNicknameGiven(line) == true)
	{
		sendError(client, _name, ERR_ERRONEUSNICKNAME, ":No nickname given");
		throw std::invalid_argument("Nickname is empty");
	}
	if (validNickname(line) == false)
	{
		sendError(client, _name, ERR_ERRONEUSNICKNAME, ":Erroneus nickname");
		throw std::invalid_argument("Wrong nickname format");
	}
	if (nicknameIsAvailable(line, _clients) == false)
	{
		sendError(client, _name, ERR_NICKNAMEINUSE, ":Nickname is already in use");
		throw std::invalid_argument("Nickname already in use");
	}
}

bool	validNickname( std::string const & line)
{
	std::string temp(line);

	if (temp.size() >= 1 || temp.size() <= 9)
	{
		if (temp.find_first_of(INVALID_CHAR, 0) != std::string::npos //trouve le 1er mauvais caractere
		|| temp.find_first_not_of(FIRST_VALID_CHAR, 0) == 0 //trouve le 1er caractere qui n'est pas valide, si l'index renvoyé est 0 c'est pas bon
		|| temp.find_first_not_of(ALL_VALID_CHAR, 0) != std::string::npos) //trouve le 1er caracter qui n'es pas valide, si c'est differnet de npos(donc qu'on en a trouvé un) c'est pas bon
			return false;
		else
			return true;
	}
	return false;
}

bool	nicknameIsAvailable( std::string const & line, std::map<SOCKET, Client*> clients )
{
	std::string temp(line);

	temp = temp.erase(0, 4);

	for (std::map<SOCKET, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getNickname() == temp)
		{
			return false;
		}
	}
	return true;
}

bool	noNicknameGiven( std::string const & line)
{
	std::string temp(line);

	temp = temp.erase(0, 4);
	if (temp.size() <= 0)
	{
		return true;
	}
	return false;
}
