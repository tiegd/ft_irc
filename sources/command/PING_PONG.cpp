/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING_PONG.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 16:15:59 by jpiquet           #+#    #+#             */
/*   Updated: 2026/06/19 10:51:16 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::PING( std::string const& line, Client* client )
{
	PONG(line.substr(5), client);
}

void	Server::PONG( std::string params, Client* client )
{
	std::string	strPong =  ":" + _name + " PONG " + params + "\r\n";
	client->outBuff += strPong;
}
