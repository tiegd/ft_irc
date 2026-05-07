/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:03:42 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/07 18:56:09 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream>

Client::Client( void ) {}

Client::Client( SOCKET sockClient ) : _clientSocket(sockClient), _nickname("*"), _username("*"), _registered(false) {}

/* { SETTER } */
void		Client::setNickname( std::string const & newNickname )	{ _nickname = newNickname; }
void		Client::setRegister( bool val )							{ _registered = val; }
void		Client::setUsername( std::string const & username )		{ _username = username; }
void		Client::setHostname( std::string const & hostname )		{ _hostname = hostname; }

/* { GETTER } */
SOCKET		Client::getSocketClient( void ) const					{ return _clientSocket; }
std::string	Client::getNickname( void ) const						{ return _nickname; }
bool		Client::getRegister( void ) const						{ return _registered; }
std::string	Client::getUsername( void ) const						{ return _username; }
std::string	Client::getHostname( void ) const						{ return _hostname; }

std::string	Client::getFullName( void ) const // ":nick!user@host"
{
	std::string	fullName = _nickname + "!" + _username + "@" + _hostname;
	return fullName;
}

void		Client::sendNotif( std::string notification )
{
	if (send(_clientSocket, notification.c_str(), notification.size(), 0) < 0)
	{
		throw FunctionError();
	}
}

Client::~Client( void ) {}
