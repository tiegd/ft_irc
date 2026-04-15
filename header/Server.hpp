/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 12:20:30 by amerzone          #+#    #+#             */
/*   Updated: 2026/04/14 13:28:20 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

/* GLOBAL */

#include <string>
#include <vector>
#include <map>

/* SOCKET */
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

// #define SERVER_PORT 1800
#define	SOCKET int
#define MAXLINE	4096

class Client;
class Channel;

class Server
{
	private:
		SOCKET							_servSocket;
		std::string						_name;
		std::string						_password;
		std::map<std::string, Client>	_clients;

		/*Une liste d'operator, a voir si on creer une class operator ou on utilise client*/
		std::map<std::string, Channel>	_channels;
	
		u_int64_t						_numberClients;

		std::string						_addressIP;
		u_int8_t						_port;

	public:
	/* Fonction necessaire

	{ PARSING }
	 - Parsing et stockages des arguments (Port, password)
	 - Parsing des requetes du client, tokenisation.

	{ CLIENT }
	- Accepter la demande d'un client + le creer.
	- Checker si il a le bon password.
	- Checker si son nickname n'existe pas deja.
	- Envoyer un message prive venant d'un client vers un autre.

	{ CHANNEL }
	- Creer un channel si il existe pas.
	- Faire rejoindre le client si il existe deja.
	- Checker si le channel est en invite-only.
	- LIST : permet de connaitre tous les channels disponnible sur le server. (optionnel)
	- PART : quitter le channel actuel. (optionnel)

	*/
};

#endif