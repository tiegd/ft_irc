/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_IRC.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:33:30 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/07 16:33:50 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_IRC_HPP
#define ERROR_IRC_HPP

class Client;

#include <string>

void	sendError( Client *client, std::string const& msgToClient );

// { NICK }
void	ERR_NONICKNAMEGIVEN(std::string const& serverName, Client* client);
void	ERR_ERRONEUSNICKNAME(std::string const& serverName, Client* client);
void	ERR_NICKNAMEINUSE(std::string const& serverName, Client* client);
void	ERR_ALREADYREGISTRED(std::string const& serverName, Client* client);

// { PASS }
void    ERR_PASSWDMISMATCH(std::string const& serverName, Client* client);
void    ERR_NEEDMOREPARAMS(std::string const& serverName, Client* client, std::string const& command);

// { JOIN }
void    ERR_BADCHANMASK(std::string const& serverName, Client* client, std::string const& channel);
void    ERR_BADCHANNELKEY(std::string const& serverName, Client* client, std::string const& channel);

// { PRIVMSG }
void    ERR_NORECIPIENT(std::string const& serverName, Client* client);
void    ERR_NOTEXTTOSEND(std::string const& serverName, Client* client);
void    ERR_NOSUCHNICK(std::string const& serverName, Client* client, std::string const& nick);
void    ERR_NOSUCHCHAN(std::string const& serverName, Client* client, std::string const& channel);

void    ERR_INVITEONLYCHAN(std::string const& serverName, Client* client, std::string const& channel);

// {TOPIC}
void	ERR_NOTONCHANNEL(std::string const& serverName, Client* client, std::string const& channel);
void	ERR_CHANOPRIVSNEEDED(std::string const& serverName, Client* client, std::string const& channel);

void	ERR_NOSUCHCHANNEL(std::string const& serverName, Client* client, std::string channel);

// {MODE}
void	ERR_UMODEUNKNOWNFLAG(std::string const& serverName, Client* client);
void	ERR_NOPRIVILEGES(std::string const& serverName, Client* client);
void	ERR_INVALIDMODEPARAM(std::string const& serverName, Client* client, std::string const& channel, char mode, std::string limit, std::string str);

#endif