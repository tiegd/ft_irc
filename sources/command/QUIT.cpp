/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:29:23 by jpiquet           #+#    #+#             */
/*   Updated: 2026/06/13 11:28:17 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::QUIT( std::string const& line, Client* client )
{
	size_t		pos = line.find(':');
	std::string	message;
	if (pos == line.npos)
		message = "Leaving";
	else
		message = line.substr(pos + 1);

	std::string	fullMsg = ":" + client->getFullName() + " QUIT " + ":" + message + "\r\n";
	client->broadcastToMyChannels(fullMsg);
	client->setDeleted(true);
}
