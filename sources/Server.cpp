/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amerzone <amerzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 12:42:23 by amerzone          #+#    #+#             */
/*   Updated: 2026/04/15 16:21:34 by amerzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

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
	
}

// void	Server::setPort( unsigned int port )
// {
// 	this->_port = port;
// }

// void	Server::setPassword( std::string password )
// {
// 	this->_password = password;
// }

