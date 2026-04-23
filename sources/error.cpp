/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:27:20 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/21 17:24:28 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_IRC.hpp"
#include "Client.hpp"
// #include <string>

/* FORMAT ERREUR : { :PREFIX CODE TARGET :message de l'erreur\r\n }
- client: Client to whom the error is being sent.
- prefix: Name of the server
- codeError: Error code to be processed by the client
- messageError: Message of the error.
*/
void	sendError( Client *client, std::string prefix, std::string codeError ,std::string messageError)
{
	std::string msgToCLient = ":" + prefix + SPACE + codeError + SPACE + client->getNickname() + messageError;
	send(client->getSocketClient(), msgToCLient.c_str(), msgToCLient.size(), 0);
}
