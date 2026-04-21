/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:27:20 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/20 18:42:44 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.hpp"
#include "Server.hpp"
#include "Client.hpp"

/* FORMAT ERREUR : { :PREFIX CODE TARGET :message de l'erreur\r\n }*/
void	sendError( Client *client, std::string prefix, std::string codeError ,std::string messageError)
{
	std::string msgToCLient = ":" + prefix + SPACE + codeError + SPACE + client->getNickname() + messageError;
	send(client->getSocketClient(), msgToCLient.c_str(), msgToCLient.size(), 0);
}
