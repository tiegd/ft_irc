/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 12:20:30 by amerzone          #+#    #+#             */
/*   Updated: 2026/04/20 19:02:31 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

/* GLOBAL */

#include <string>
#include <vector>
#include <map>
#include <cstdbool>

/* SOCKET */
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <poll.h>

// #define SERVER_PORT 1800
#define	SOCKET int
#define MAXLINE	4096
#define MAXCLIENTS 10

#define SPACE ' '

#define FIRST_VALID_CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}|\\[]^_"
#define ALL_VALID_CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789{}|\\[]^_"
#define	INVALID_CHAR ": *?@!"

class Client;
// class Channel;

class Server
{
	private:
		std::string						_name;
		u_int16_t						_port;
		std::string						_password;

		SOCKET							_socketServ;

		std::vector<struct pollfd>		_fds;
		// std::string						_addressIP;

		// /*Une liste d'operator, a voir si on creer une class operator ou on utilise client*/
		std::map<SOCKET, Client*>		_clients;
		// std::map<std::string, Channel>	_channels;
		std::vector<std::string>		_nicknameAlreadyUsed;
		
		// u_int64_t						_numberClients;

		Server( void );
		Server( Server & copy );
		Server & operator=( Server & rightSide );

	public:
		Server( std::string name, u_int16_t port, std::string password );
		~Server( void );

		/* SETTER */
		void	setSocketServ( void );
		// void	setPort( unsigned int port );
		// void	setPassword( std::string password );

		void	runServer( void );
		void	addClientSocket( void );
		
		// { PARSING }
		void	parseCommand( std::string const & line, Client* client );
		
		// { COMMAND }
		
		void	PASS( std::string const & line, Client* client );
		void	NICK( std::string const & line, Client* client );
		// bool	validNickname( std::string const & line);
		// bool	nicknameIsAvailable( std::string const & line );
		// bool	noNicknameGiven( std::string const & line);

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
