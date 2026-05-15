/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 12:18:51 by amerzone          #+#    #+#             */
/*   Updated: 2026/05/15 15:08:01 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Channel.hpp"
#include "FunctionError.hpp"

class Channel;

#define	SOCKET int
// #include "Channel.hpp"

class Client
{
	private:
		SOCKET							_clientSocket;
		/*Le nickname doit etre unique a tout le server et comporter uniquement 9 char*/
		std::string						_nickname;
		/*Username ne peut pas etre modifier mais n'est pas forcement unique*/
		std::string						_username;
		std::string						_hostname;
		std::string						_realname;

		bool							_registered;
		std::vector<Channel*>			_chanJoined;

		int								_passValid;
		int								_nickValid;
		int								_userValid;

		Client( Client const& src );
		// Client & operator=(Client const& rightSide);
		
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

		/* SETTER */
		void	setNickname( std::string const & newNickname );
		void	setUsername( std::string const & newNickname );
		void	setHostname( std::string const & newNickname );
		void	setRealname( std::string const & realname );
		void	setRegister( bool val );

		void	setPassValid( int val );
		void	setNickValid( int val );
		void	setUserValid( int val );
		
		void	addChanJoined( Channel* newChannel );
		void	broadcastToMyChannels( std::string notification );
		bool	canBeRegistered( void ) const;
};

#endif
