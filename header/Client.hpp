/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amerzone <amerzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 12:18:51 by amerzone          #+#    #+#             */
/*   Updated: 2026/04/09 17:46:53 by amerzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Channel.hpp"

class Client
{
	private:
		/*Le nickname doit etre unique a tout le server et comporter uniquement 9 char*/
		std::string						_nickname;
		/*Username ne peut pas etre modifier mais n'est pas forcement unique*/
		std::string						_username;
		std::string						_hostname;

		/*Map de tous les channels rejoint par le client.*/
		std::map<std::string, Channel>	_joined;

		SOCKET							_clientSocket;

	public:
		/* Fonction necessaire
		
		- Changer son nickname.
		
		*/
		
			
};

#endif