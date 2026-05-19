/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:03:42 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/19 17:42:40 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <algorithm>
#include <iostream>

Client::Client( void ) {}

Client::Client( SOCKET sockClient ) : _clientSocket(sockClient), _nickname("*"), _username("*"),
									_registered(false), _passValid(0), _nickValid(0), _userValid(0), _deleted(false) {}

/* { SETTER } */
void		Client::setNickname( std::string const & newNickname )	{ _nickname = newNickname; }
void		Client::setRegister( bool val )							{ _registered = val; }
void		Client::setUsername( std::string const & username )		{ _username = username; }
void		Client::setHostname( std::string const & hostname )		{ _hostname = hostname; }
void		Client::setRealname( std::string const & realname )		{ _realname = realname; }

void		Client::setPassValid( int val )							{ _passValid = val; }
void		Client::setNickValid( int val )							{ _nickValid = val; }
void		Client::setUserValid( int val )							{ _userValid = val; }
void		Client::setDeleted( bool val )							{ _deleted = val; }


/* { GETTER } */
SOCKET		Client::getSocketClient( void ) const					{ return _clientSocket; }
std::string	Client::getNickname( void ) const						{ return _nickname; }
bool		Client::getRegister( void ) const						{ return _registered; }
std::string	Client::getUsername( void ) const						{ return _username; }
std::string	Client::getHostname( void ) const						{ return _hostname; }
std::string	Client::getRealname( void ) const						{ return _realname; }
bool		Client::getDeleted( void ) const						{ return _deleted; }

std::string	Client::getFullName( void ) const // ":nick!user@host"
{
	std::string	fullName = _nickname + "!" + _username + "@" + _hostname;
	return fullName;
}

void		Client::sendNotif( std::string notification )
{
	if (send(_clientSocket, notification.c_str(), notification.size(), 0) < 0)
	{
		std::cout << "sendNotif() error" << std::endl;
	}
}

void		Client::broadcastToMyChannels( std::string notification )
{
	std::vector<Channel*>::iterator it;
	for (it = _chanJoined.begin(); it != _chanJoined.end(); ++it)
	{
		(*it)->broadcastToAll(notification, this);
	}
}

void		Client::addChanJoined( Channel *newChannel )
{
	_chanJoined.push_back(newChannel);
}

void		Client::rmChanJoined( Channel *newChannel )
{
	std::vector<Channel*>::iterator it;
	it = std::find(_chanJoined.begin(), _chanJoined.end(), newChannel);
	if (it != _chanJoined.end())
		_chanJoined.erase(it);
}

bool		Client::canBeRegistered() const
{
	if (_passValid == 1 && _nickValid == 1 && _userValid == 1)
		return true;
	else
		return false;
}

/* TESTS */

void		Client::printChanJoined() const
{
	std::cout << getNickname() << "._chanJoined = " << std::endl;
	for (int i = 0; i < _chanJoined.size(); i++)
		std::cout << _chanJoined[i] << std::endl;
	std::cout << std::endl;
}

void		Client::quitChannels()
{
	std::vector<Channel*>::iterator it;
	for (it = _chanJoined.begin(); it != _chanJoined.end(); ++it)
	{
		(*it)->rmOperator(this);
		(*it)->rmUser(this);
	}
}

Client::~Client( void ) {}
