/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 13:17:04 by amerzone          #+#    #+#             */
/*   Updated: 2026/04/14 13:27:15 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CHANNEL.HPP
#define CHANNEL.HPP

#include "Server.hpp"
#include "Client.hpp"

class Channel
{
	private:
		/*	Le nom d'un channel peut commencer par "#" max 200 char 
		et ne peut contenir aucun " ", controle G (^G) ou ","*/
		std::string							_name;
		std::string							_password;
		std::vector<Client>					_users;
		std::vector<Client>					_operator;
		std::string							_topic;
		std::map<std::string, void (*)()>	_mode;
		unsigned int						_n_members;

		/*MODE */
		
		bool								_inviteOnly; // i
		bool								_topicChange; // t
		u_int64_t							_userLimit; // l
		bool								_hasPassword; // k
		bool								_hasTopic;

	public:
		/* fonction possible:
		
		- Creer le channel
		- Checker le nombre de membre et detruire le channel en fonction;
		(si il y a 0 membre exemple)
		- Detruire le channel;

		- Kick un utilisateur du channel.
		- Inviter un nouvel utilisateur.
		- TOPIC: Changer le topic du channel donne en parametre. 
		  (si pas de parametre afficher le topic actuel)
		- Mettre un password au channel.
		- Mettre un Client en mode operator.
		- Enlever un Client du mode operator.
		- Fixer la limite d'utilisateur possible.
		
		- NAMES: afficher toutes la liste des personnes presentes sur le channel. (optionnel)

		*/
		
};

#endif