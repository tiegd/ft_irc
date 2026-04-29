/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:03:42 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/29 09:57:18 by jpiquet          ###   ########.fr       */
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

Client::~Client( void ) {}
