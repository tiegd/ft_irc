/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:48:05 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/19 11:44:36 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
#define RPL_HPP

// #include "Client.hpp"
// #include "Channel.hpp"
#include "Server.hpp"

class Channel;
class Client;

void	sendRpl( Client *client, std::string const& msgToClient );

void	RPL_NOTOPIC( std::string serverName, Client* client, std::string const& channel );
void	RPL_TOPIC( std::string const& serverName, Client* client, std::string const& channel, std::string const& topic );
void	RPL_ENDOFNAMES( std::string const& serverName, Client* client, std::string const& channel );
void	RPL_NAMREPLY( std::string const& serverName, Client* client, Channel* channel );
void    RPL_CHANNELMODEIS(std::string const& serverName, Client* client, Channel* channel);
void	RPL_WHOREPLY( std::string const& serverName, Client *client, Client* target, std::string const& channelName );
void	RPL_ENDOFWHO( std::string const& serverName, Client* client, std::string const& mask );
void	RPL_INVITING( std::string const& serverName, Client* client, std::string const& target, std::string const& channel );

void	RPL_WELCOME( std::string const& serverName, Client* client );
void	RPL_YOURHOST( std::string const& serverName, Client* client );
void	RPL_CREATED( std::string const& serverName, Client* client );
void	RPL_MYINFO( std::string const& serverName, Client* client );

void	RPL_MOTDSTART( std::string const& serverName, Client* client );
void	RPL_MOTD( std::string const& serverName, Client* client, std::string const& motdString );
void	RPL_ENDOFMOTD( std::string const& serverName, Client* client );

void	RPL_CHANNELKICK(std::string const& serverName, Client *op, Client* client, Channel* channel);
void	RPL_CHANMSGKICK(std::string const& serverName, Client *op, Client* client, Channel* channel, std::string comment);

#endif
