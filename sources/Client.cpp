/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:03:42 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/20 17:53:44 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream>

Client::Client( void ) {}

// Client::Client( Client const & src )
// {
// 	//std::cout << "Copy constructor called" << std::endl;
// }

// Client & Client::operator=( Client const & rightSide )
// {
// 	if (this != &rightSide)
// 	return *this;
// }

Client::Client( SOCKET sockClient ) : _clientSocket(sockClient), _nickname("*") {}

SOCKET		Client::getSocketClient( void ) const
{
	return _clientSocket;
}

void		Client::setNickname( std::string const & newNickname )
{
	_nickname = newNickname;
}

std::string	Client::getNickname( void ) const
{
	return _nickname;
}

bool		Client::getRegister( void ) const
{
	return _registered;
}

void		Client::setRegister( bool val )
{
	_registered = val;
}



Client::~Client( void ) {}
