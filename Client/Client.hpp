/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 12:18:51 by amerzone          #+#    #+#             */
/*   Updated: 2026/05/27 17:14:15 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Channel;

#define	SOCKET int

class Client
{
	private:
		SOCKET							_clientSocket;
		/*The nickname must be unique across the entire server and consist of exactly 9 characters.*/
		std::string						_nickname;
		/*The username cannot be changed, but it is not necessarily unique*/
		std::string						_username;
		std::string						_hostname;
		std::string						_realname;

		bool							_registered;
		bool							_deleted;

		std::vector<Channel*>			_chanJoined;

		int								_passValid;
		int								_nickValid;
		int								_userValid;

		Client( Client const& src );
		Client & operator=(Client const& rightSide);
		
	public:

		Client( void );
		Client( SOCKET sockClient );
		~Client( void );

		std::string						_inBuff;
		
		void	sendNotif( std::string notification );
		
		/* GETTER */
		SOCKET		getSocketClient( void ) const;
		std::string	getNickname( void ) const;
		std::string	getUsername( void ) const;
		std::string	getHostname( void ) const;
		bool		getRegister( void ) const;
		std::string	getFullName( void ) const;
		std::string	getRealname( void ) const;
		bool		getDeleted( void ) const;

		/* SETTER */
		void	setNickname( std::string const & newNickname );
		void	setUsername( std::string const & newNickname );
		void	setHostname( std::string const & newNickname );
		void	setRealname( std::string const & realname );
		void	setRegister( bool val );
		void	setDeleted( bool val );

		void	setPassValid( int val );
		void	setNickValid( int val );
		void	setUserValid( int val );
		
		void	addChanJoined( Channel* newChannel );
		void	rmChanJoined( Channel* newChannel );
		void	broadcastToMyChannels( std::string notification );
		bool	canBeRegistered( void ) const;
		void	quitChannels( std::map<std::string, Channel*> & channels );
};

#endif
