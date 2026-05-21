/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 12:42:23 by amerzone          #+#    #+#             */
/*   Updated: 2026/05/20 15:33:49 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <algorithm>

volatile std::sig_atomic_t gSignalStatus = 0;

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

void	signalHandler(int signal)
{
	if (signal == SIGINT)
		gSignalStatus = 1;
}

void	Server::disconnectClient( size_t & i )
{
	int fd = _fds[i].fd;
 
	std::cout << _clients[fd]->getNickname() << " disconnected" << std::endl;
 
	close(fd);
	delete _clients[fd];
	_clients.erase(fd);
	_fds.erase(_fds.begin() + i);
	i--;
}

void	Server::setSocketServ( void )
{
	struct sockaddr_in	servaddr;
	
	if ((_socketServ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Socket error" << std::endl;
	}
	int flags = fcntl(_socketServ, F_GETFL);
	fcntl(_socketServ, F_SETFL, flags | O_NONBLOCK);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family			= AF_INET;
	servaddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	servaddr.sin_port			= htons(_port);

	int opt = 1;
	setsockopt(_socketServ, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if ((bind (_socketServ, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0)
		throw	std::runtime_error("Bind() error");

	if (listen(_socketServ, MAXCLIENTS) < 0)
		throw	std::runtime_error("Listen() error");	

	std::cout << "Waiting for a connection on port " << _port << "..." << std::endl;
}

void	Server::runServer( void )
{
	char	buff[512];

	std::signal(SIGINT, signalHandler);
	while (true)
	{
		
		if (poll(_fds.data(), _fds.size(), -1) < 0)
		{
			if (gSignalStatus == 1)
			{
				std::cout << "Quit server..." << std::endl;
				return ;
			}
			std::cout << "Error occured during poll()." << std::endl;
			continue;
		}
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _fds[0].fd)
				{
					addClientSocket();
					continue ;
				}

				int bytes = recv(_fds[i].fd, buff, sizeof(buff), 0);
				if (bytes <= 0)
				{
					disconnectClient(i);
					continue;
				}
				
				_clients[_fds[i].fd]->_inBuff.append(buff, bytes);

				size_t pos;
				while ((pos = _clients[_fds[i].fd]->_inBuff.find("\r\n")) != std::string::npos) 
				{
					std::string line = _clients[_fds[i].fd]->_inBuff.substr(0, pos);
					_clients[_fds[i].fd]->_inBuff.erase(0, pos + 2);

					std::cout << "Recu par " << _clients[_fds[i].fd]->getNickname() << ": " << line << std::endl;

					parseCommand(line, _clients[_fds[i].fd]);
					
					if (_clients.find(_fds[i].fd) != _clients.end() && _clients[_fds[i].fd]->getDeleted() == true)
					{
						disconnectClient(i);
						break;
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
		if (!line.compare(0, 4, "PASS") && (line[4] == ' ' || line.size() == 4))
		{
			client->setPassValid(PASS(line, client));
		}
		if (!line.compare(0, 4, "NICK") && (line[4] == ' ' || line.size() == 4))
		{
			client->setNickValid(NICK(line, client));
		}
		if (!line.compare(0, 4, "USER") && (line[4] == ' ' || line.size() == 4))
		{
			client->setUserValid(USER(line, client));
		}
		if (!line.compare(0, 4, "PING") && (line[4] == ' ' || line.size() > 4))
		{
			PING(line, client);
		}
		if (!line.compare(0, 4, "QUIT") && (line[4] == ' ' || line.size() == 4))
		{
			QUIT(line, client);
		}
		if (client->getRegister() == true)
		{
			if (!line.compare(0, 4, "JOIN") && (line[4] == ' ' || line.size() == 4))
			{
				JOIN(line, client);
			}
			if (!line.compare(0, 4, "PART") && (line[4] == ' ' || line.size() == 4))
			{
				PART(line, client);
			}
			if (!line.compare(0, 7, "PRIVMSG") && (line[7] == ' ' || line.size() == 7))
			{
				PRIVMSG(line, client);
			}
			if (!line.compare(0, 5, "TOPIC") && (line[5] == ' ' || line.size() == 5))
			{
				TOPIC(line, client);
			}
			if (!line.compare(0, 4, "KICK") && (line[4] == ' ' || line.size() == 5))
			{
				KICK(line, client);
			}
			if (!line.compare(0, 4, "MODE") && (line[4] == ' '  || line.size() == 5))
			{
				MODE(line, client);
			}
			if (!line.compare(0, 3, "WHO") && (line[3] == ' ' || line.size() == 3))
			{
				WHO(line, client);
			}
			if (!line.compare(0, 6, "INVITE") && (line[6] == ' ' || line.size() == 6))
			{
				INVITE(line, client);
			}
			if (!line.compare(0, 4, "MOTD") && (line[4] == ' ' || line.size() == 4))
			{
				MOTD(client);
			}
			if (!line.compare(0, 6, "NOTICE") && (line[6] == ' ' || line.size() == 6))
			{
				NOTICE(line, client);
			}
		}
		else
		{
			ERR_UNKNOWNCOMMAND(_name, client, line);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	if ( client->getRegister() != true && client->canBeRegistered() == true)
	{
		client->setRegister(true);
		RPL_WELCOME(_name, client);
		RPL_YOURHOST(_name, client);
		RPL_CREATED(_name, client);
		RPL_MYINFO(_name, client);
		MOTD(client);
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
	int flags = fcntl(client, F_GETFL);
	fcntl(client, F_SETFL, flags | O_NONBLOCK);
	t_fd.fd = client;
	t_fd.events = POLLIN;
	t_fd.revents = 0;
	_fds.push_back(t_fd);

	Client* newClient = new Client(client);
	_clients[client] = newClient;

	std::cout << "New client connected" << std::endl;
}

bool	Server::nicknameExist( std::string target)
{
	for (std::map<SOCKET, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname() == target)
		{
			return true;
		}
	}
	return false;
}

Client*	Server::searchClient( std::string target )
{
	for (std::map<SOCKET, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (target.compare(it->second->getNickname()) == 0)
		{
			return it->second;
		}
	}
	return NULL;
}

Server::~Server( void )
{
	for (std::map<SOCKET, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		delete it->second;
	}
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		delete it->second;
	}
}
