/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 12:20:30 by amerzone          #+#    #+#             */
/*   Updated: 2026/05/21 11:34:26 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

/* GLOBAL */

#include <iostream>
#include <vector>
#include <map>
#include <csignal>
#include <fstream>

/* SOCKET */
#include <netinet/in.h>
#include <poll.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define	SOCKET int
#define MAXLINE	512
#define MAXCLIENTS 10

#define SPACE ' '

#define FIRST_VALID_CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}|\\[]^_"
#define ALL_VALID_CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789{}|\\[]^_"
#define	INVALID_CHAR ": *?@!"

/* {MY HEADERS} */
#include "Channel.hpp"
#include "error_IRC.hpp"
#include "tools.hpp"
#include "rpl.hpp"


class Server
{
	private:
		std::string						_name;
		u_int16_t						_port;
		std::string						_password;
		SOCKET							_socketServ;
		std::vector<struct pollfd>		_fds;
		std::map<SOCKET, Client*>		_clients;
		std::map<std::string, Channel*>	_channels;
		std::vector<std::string>		_nicknameAlreadyUsed;
		

		Server( void );
		Server( Server & copy );
		Server & operator=( Server & rightSide );

	public:
		Server( std::string name, u_int16_t port, std::string password );
		~Server( void );

		void			setSocketServ( void );
		void			runServer( void );
		void			addClientSocket( void );
		bool			nicknameExist( std::string target );
		void			disconnectClient( size_t & i );
		Client*			searchClient( std::string target );

		void			parseCommand( std::string const & line, Client* client );

		int			PASS( std::string const & line, Client* client );

		int			NICK( std::string const & line, Client* client );

		int			USER( std::string const & line, Client* client );

		void			JOIN( std::string const & line, Client* client );
			void			sendJoinNotification( Client *client, Channel* Channel );
		
		void			PRIVMSG( std::string const& line, Client* client );
		void			NOTICE( std::string const& line, Client* client );
			void			sendNotice( Client *client, std::string recipient, std::string message );
			void			sendMessage( Client *client, std::string recipient, std::string message );
			SOCKET			searchClientSocket( std::string nicknameRecipient );
			bool			channelExist( std::string channelName );

		void			PING( std::string const& line, Client* client );
		void			PONG( std::string params, Client* client );

		void			TOPIC(std::string line, Client* client );

		void			PART( std::string const& line, Client* client);

		void			WHO( std::string const& line, Client* client );

		void			KICK(std::string const& line, Client* op);
			std::string		parseComment(std::vector<std::string> args);
		void 			MODE(std::string const& line, Client* op);
			void			modeInviteOnly( Client* op, Channel* channel, bool toDo );
			void			modeRestrictionTopic( Client* op, Channel* channel, bool toDo );
			void			modePassword( Client* op, Channel* channel, bool toDo, std::string password );
			void			modeOpPrivilege( Client* op, Channel* channel, bool toDo, std::string user );
			void			modeAddLimitUser( Client* op, Channel* channel, bool toDo, std::string limit );
			void			modeRmLimitUser( Client* op, Channel* channel, bool toDo );
			bool			parseOptions( std::string options, Client *client );
			bool			parseChannelPassword( Client* op, Channel* channel, std::string password );
		void			INVITE( std::string const& line, Client* client );

		void			MOTD( Client* client );

		void			QUIT( std::string const& line, Client* client );
};

#endif