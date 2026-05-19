/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 18:03:39 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/19 18:04:41 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::MOTD(Client* client)
{
	std::ifstream	motd("files/motd_3.txt");
	std::string		buffer;

	RPL_MOTDSTART(_name, client);
	if (!motd.is_open())
	{
		ERR_NOMOTD(_name, client);
		return ;
	}
	while (std::getline(motd, buffer, '\n'))
	{
		RPL_MOTD(_name, client, buffer);
	}
	RPL_ENDOFMOTD(_name, client);
}
