/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 12:42:23 by amerzone          #+#    #+#             */
/*   Updated: 2026/05/07 10:32:48 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "function.hpp"

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
						std::cout << "Client disconnected" << std::endl;
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
							// std::cout << "Client " << i << " dit : "  << line << std::endl;
							std::cout << "Recu : "<< line << std::endl;
							parseCommand(line, _clients[_fds[i].fd]);
						}
						if (_clients[_fds[i].fd]->getRegister() == true && _clients[_fds[i].fd]->getNickname() != "*" && _clients[_fds[i].fd]->getUsername() != "*")
						{
							
							_clients[_fds[i].fd]->setRegister(true);
						}
					}
				}
			}
		}
	}
}

void	Server::parseCommand( std::string const & line , Client* client )
{
	try
	{
		if (line.size() >= 4)
		{
			if (!line.compare(0, 4, "PASS") && (line[4] == ' ' || line.size() == 4))
			{
				PASS(line, client);
			}
			if (!line.compare(0, 4, "NICK") && (line[4] == ' ' || line.size() == 4))
			{
				NICK(line, client);
			}
			if (!line.compare(0, 4, "USER") && (line[4] == ' ' || line.size() == 4))
			{
				USER(line, client);
			}
			if (!line.compare(0, 4, "JOIN") && (line[4] == ' ' || line.size() == 4))
			{
				JOIN(line, client);
			}
			if (!line.compare(0, 7, "PRIVMSG") && (line[7] == ' ' || line.size() == 7))
			{
				PRIVMSG(line, client);
			}
			if (!line.compare(0, 5, "TOPIC") && (line[5] == ' ' || line.size() == 5))
			{
				TOPIC(line, client);
			if (line.compare(0, 5, "KICK") && line[4] == ' ')
			{
				KICK(line, client);
			}
			if (line.compare(0, 5, "MODE") && line[4] == ' ')
			{
				MODE(line, client);
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
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

	Client* newClient = new Client(client);
	_clients[client] = newClient;

	std::cout << "New client connected" << std::endl;
}
