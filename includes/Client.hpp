/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 12:18:51 by amerzone          #+#    #+#             */
/*   Updated: 2026/04/17 10:57:46 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
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

		bool							_registered;
		/*Map de tous les channels rejoint par le client.*/
		// std::map<std::string, Channel>	_joined;

		Client( void );
		
	public:

		Client( SOCKET sockClient );
		~Client( void );
		
		/* GETTER */
		SOCKET		getSocketClient( void ) const;
		std::string	getNickname( void ) const;
		bool		getRegister( void ) const;

		/* SETTER */
		void	setNickname( std::string const & newNickname );
		void	setRegister( bool val );
			
};

#endif