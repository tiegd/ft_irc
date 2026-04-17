/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 12:42:23 by amerzone          #+#    #+#             */
/*   Updated: 2026/04/17 15:58:01 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

#define FIRST_VALID_CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}|\\[]^_"
#define ALL_VALID_CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789{}|\\[]^_"

Server::Server( void ) {};

Server::Server( std::string name, u_int16_t port, std::string password )
: _name(name), _port(port), _password(password)
{
	setSocketServ();
	struct pollfd	t_fd;
	t_fd.fd = _socketServ;
	t_fd.events = POLLIN;
	t_fd.revents = 0;
	_fds.push_back(t_fd);
}

Server::~Server( void ) {};

void	Server::setSocketServ( void )
{
	struct sockaddr_in	servaddr;
	
	if ((_socketServ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Socket error" << std::endl;
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family			= AF_INET;
	servaddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	servaddr.sin_port			= htons(_port);

	int opt = 1;
	setsockopt(_socketServ, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if ((bind (_socketServ, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0)
		std::cout << "Bind error" << std::endl;

	if (listen(_socketServ, MAXCLIENTS) < 0)
		std::cout << "Listen error" << std::endl;
	
	std::cout << "Waiting for a connection on port " << _port << std::endl;
}

void	Server::runServer( void )
{
	char	buff[1024];
	// SOCKET	socketServ = _fds[0].fd;
	
	while (true)
	{
		if (poll(_fds.data(), _fds.size(), -1) < 0)
		{
			std::cout << "Error occured during poll().";
			continue;
		}
		
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _fds[0].fd)
				{
					addClientSocket();
				}
				else
				{
					int bytes = recv(_fds[i].fd, buff, sizeof(buff), 0);
					if (bytes == 0)
					{
						std::cout << "Client disconnected" << std::cout;
						close(_fds[i].fd);
						_fds.erase(_fds.begin() + i);
						i--;
					}
					else if (bytes < 0)
					{
						std::cout << "recv error" << std::endl;
						continue;
					}
					else
					{
						std::string	message;
						buff[bytes] = '\0';
						message += buff;
						size_t pos;
						while ((pos = message.find("\r\n")) != std::string::npos) 
						{
							std::string line = message.substr(0, pos);
							message.erase(0, pos + 2);
							parseCommand(line, _clients[_fds[i].fd]);
							std::cout << "Client " << i << " dit : "  << line << std::endl;
						}
					}
				}
			}
		}
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

bool	validNickname( std::string const & line, std::vector<std::string> const & nicknameAlreadyUsed)
{
	std::string temp(line);
	
	temp = temp.erase(0, 4);
	if (temp.size() >= 1 || temp.size() <= 9)
	{
		if (temp.find_first_of(": *?@!", 0) != std::string::npos //trouve le 1er mauvais caractere
		|| temp.find_first_not_of(FIRST_VALID_CHAR, 0) == 0 //trouve le 1er caractere qui n'est pas valide, si l'index renvoyé est 0 c'est pas bon
		|| temp.find_first_not_of(ALL_VALID_CHAR) != std::string::npos) //trouve le 1er caracter qui n'es pas valide si c'est differnet de npos c'est pas bon
			return false;
	}
	return false;
}

void	Server::parseCommand( std::string const & line , Client *client )
{

	if (client->getRegister() == false)
	{
		/* VERIFY PASSWORD */
		if (line.substr(0, 3) == "PASS" && line[4] == ' ')
		{
			if (correctPassword(line, _password) == false)
				throw std::invalid_argument("Wrong PASS");
		}
		if (line.substr(0, 3) == "NICK" && line[4] == ' ')
		{
			/*
			- Checker si c'est le bon format:
			1 à 9 caractere max
			caractere autorisé en plus de a-z : { } \| [ ] \ ^ _
			il ne doit pas commencer par un chiffre.
			caratere interdit : :, espace, *, ?, @, !
			- Checker si il existe deja. Sinon renvoyer une erreur ERR_NICKNAMEINUSE.
			*/
		}
		if (line.substr(0, 3) == "USER" && line[4] == ' ')
		{
			
		}
	}
	
}

void	Server::addClientSocket( void )
{
	struct sockaddr_in	client_addr;
	socklen_t 			addr_len = sizeof(client_addr);
	SOCKET				client;
	struct pollfd		t_fd;

	client = accept(_socketServ, (struct sockaddr *) &client_addr, &addr_len);
	if (client < 0)
	{
		std::cout << "Error occured during accept()" << std::endl;
		throw ;
	}
	t_fd.fd = client;
	t_fd.events = POLLIN;
	t_fd.revents = 0;
	_fds.push_back(t_fd);

	Client *newClient = new Client(client);
	_clients[client] = newClient;

	std::cout << "New client connected" << std::endl;

	// /* test reception de message */
	
	// std::string	message;
	// char	buff[1024];
	// int bytes = recv(_fds[1].fd, buff, sizeof(buff), 0);
	// buff[bytes] = '\0';
	// message += buff;
	// size_t pos;
	// while ((pos = message.find("\n")) != std::string::npos) 
	// {
	// 	std::string line = message.substr(0, pos);
	// 	message.erase(0, pos + 1);
	// 	createClient(line);
	// }
}
