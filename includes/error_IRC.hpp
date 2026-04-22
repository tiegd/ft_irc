/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_IRC.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:33:30 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/22 17:33:05 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_IRC_HPP
#define ERROR_IRC_HPP

#define ERR_NONICKNAMEGIVEN		"431" // :No nickname given
#define ERR_ERRONEUSNICKNAME	"432" // :Erroneus nickname
#define ERR_NICKNAMEINUSE		"433" // :Nickname is already in use
#define ERR_ALREADYREGISTRED	"462" // :You may not reregister

#define ERR_PASSWDMISMATCH		"464" // :Password incorrect
#define ERR_NEEDMOREPARAMS		"461" // <command> :Not enough parameters

#define ERR_BADCHANMASK			"476" // :Bad channel mask
#define	ERR_BADCHANNELKEY		"475" // <channel> :Cannot join channel (+k)

class Client;
#include <string>

void	sendError( Client *client, std::string prefix, std::string codeError ,std::string messageError);

#endif