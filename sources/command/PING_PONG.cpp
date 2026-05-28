/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING_PONG.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 16:15:59 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/19 16:37:11 by gaducurt         ###   ########.fr       */
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
	if(send(client->getSocketClient(), strPong.c_str(), strPong.size(), 0) < 0)
	{
		std::cout << "send() error ocurred" << std::endl;
	}
}
