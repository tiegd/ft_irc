/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_IRC.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:33:30 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/26 17:20:18 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_IRC_HPP
#define ERROR_IRC_HPP

class Client;
#include "Client.hpp"

void	sendError( Client *client, std::string const& msgToClient );

// { NICK }
void	ERR_NONICKNAMEGIVEN( std::string const& serverName, Client* client );
void	ERR_ERRONEUSNICKNAME( std::string const& serverName, Client* client );
void	ERR_NICKNAMEINUSE( std::string const& serverName, Client* client );
void	ERR_ALREADYREGISTRED( std::string const& serverName, Client* client );
void	ERR_NOTREGISTERED( std::string const& serverName, Client* client );

// { PASS }
void    ERR_PASSWDMISMATCH( std::string const& serverName, Client* client );
void    ERR_NEEDMOREPARAMS( std::string const& serverName, Client* client, std::string const& command );

// { JOIN }
void    ERR_BADCHANMASK( std::string const& serverName, Client* client, std::string const& channel );
void    ERR_BADCHANNELKEY( std::string const& serverName, Client* client, std::string const& channel );
void    ERR_CHANNELISFULL( std::string const& serverName, Client* client, std::string const& channel );

// { PRIVMSG }
void    ERR_NORECIPIENT( std::string const& serverName, Client* client );
void    ERR_NOTEXTTOSEND( std::string const& serverName, Client* client );
void    ERR_NOSUCHNICK( std::string const& serverName, Client* client, std::string const& nick );
void    ERR_NOSUCHCHAN( std::string const& serverName, Client* client, std::string const& channel );
void	ERR_USERONCHANNEL( std::string const& serverName, Client* client, std::string const& target, std::string const& channelName );
void    ERR_INVITEONLYCHAN( std::string const& serverName, Client* client, std::string const& channel );
void	ERR_TOOMANYTARGETS ( std::string const& serverName, Client* client, std::string const& target ); // "<target> :Duplicate recipients.
void	ERR_CANNOTSENDTOCHAN(std::string const& serverName, Client* client, std::string const& channelName);

// {TOPIC}
void	ERR_NOTONCHANNEL( std::string const& serverName, Client* client, std::string const& channel );
void	ERR_CHANOPRIVSNEEDED( std::string const& serverName, Client* client, std::string const& channel );

void	ERR_NOSUCHCHANNEL(std::string const& serverName, Client* client, std::string channel);

// {MODE}
void	ERR_NOPRIVILEGES(std::string const& serverName, Client* client);
void	ERR_INVALIDMODEPARAM(std::string const& serverName, Client* client, std::string const& channel, char mode, std::string limit, std::string str);
void	ERR_USERNOTINCHANNEL(std::string const& serverName, Client* client, std::string channel);
void	ERR_UMODEUNKNOWNFLAG( std::string const& serverName, Client* client );
void	ERR_NOMOTD( std::string const& serverName, Client* client );

void    ERR_UNKNOWNCOMMAND(std::string const & serverName, Client* client, std::string const & command);

#endif