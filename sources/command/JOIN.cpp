/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:13:50 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/30 11:06:13 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "tools.hpp"
#include "error_IRC.hpp"
#include "Channel.hpp"
#include "rpl.hpp"

bool	nameChannelWellFormated( std::string nameChannel );

// std::string line = "JOIN #newchannel, #otherchannel password";
// JOIN <channel>{,<channel>} [<key>{,<key>}]
void	Server::JOIN(std::string const& line, Client* client)
{
	/* CHECK
	- Verifier qu'il y a assez de parametre.(au moins 1)
	- Verifier si le channel existe deja ou pas.
	- Si il existe checker si il a un mot de passe, checker si il est mode invite only, et checker si c'est le bon mot de pass si il y en a un.
	- Creer le channel.
	- Verifier que le nom du channel a le bon format.(commance par #, 200 char max et pas de SPACE, pas de ',' et pas de (G^))
	- Checker si il y a une ',' apres le nom du channel ou le password car on peut rejoindre plusieur channels en meme temps.
	
	- Split pour chaque "," si il y en a une, ensuite parser chaque string et faire les verifs.
	*/

	if (line.size() <= 5)
	{
		sendError(client, _name, ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters");
		throw std::invalid_argument("Not enough parameters");		
	}

	std::string	temp(line);
	temp.erase(0, 5);

	std::vector<std::string>	splitArgs = split(temp, SPACE);
	std::string					strChannel = splitArgs[0];
	std::string					strPassword = splitArgs[1];

	std::vector<std::string>	channels = split(strChannel, ',');
	std::vector<std::string>	passwords = split(strPassword, ',');

	std::map<std::string, Channel*>::iterator	it_end = _channels.end();
	for(size_t i = 0; i < channels.size(); i++)
	{
		std::string nameChannel(channels[i]);
		if (nameChannel[0] != '#')
		{
			sendError(client, _name, ERR_BADCHANMASK, "JOIN :Bad channel mask");
			throw std::invalid_argument("# is missing for the channel name");
		}
		if (it_end == _channels.find(nameChannel)) // regarde si le channel n'existe pas
		{
			//si il existe pas checker que le nom u channel a le bon format
			if (nameChannelWellFormated(nameChannel) == true) // faire la fonction
			{
				// créer le channel
				Channel*	newChannel = new Channel(nameChannel, client);
				_channels[nameChannel] = newChannel;
				sendJoinNotification(client, nameChannel);
			}
		}
		else // si il existe
		{
			//checker si c'est en mode invite-only
			if (_channels[nameChannel]->getInvitOnly() == true)
			{
				// checker si il a un password
				if (_channels[nameChannel]->getHasPassword()) //mettre getHasPassword()
				{
					// si il en a un verifier si c'est le bon.
					if  (i < passwords.size()) // si l'index du vector de passwords est plus petit on compare
					{
						if (passwords[i].compare(_channels[nameChannel]->getPassword()) == 0) //si le password est correct
						{
							_channels[nameChannel]->addUser(client);
							sendJoinNotification(client, nameChannel);
						}
					}
					else //sinon ca veut dire qu'il manque un parametre password pour le channel donc renvoyer badchannelkey
						sendError(client, _name, ERR_BADCHANNELKEY, nameChannel + " :Cannot join channel (+k)");
				}
				else //si il y a pas de password faire addUser
				{
					_channels[nameChannel]->addUser(client);
					sendJoinNotification(client, nameChannel);
				}
			}
			else // renvoyer une erreur car le mode invite only est activé
			{
				sendError(client, _name, ERR_INVITEONLYCHAN, nameChannel + " :Cannot join channel (+i)");
			}
		}
	}
}

void	Server::sendJoinNotification(Client *client, std::string nameChannel)
{
	/*
	-envoyer notif a tous le channel
	-envoyer le topic si il y en a un
	-envoyer la list des gens qu'il y a sur le channel
	-envoyer "fin de list" au client
	*/
	_channels[nameChannel]->sendChannelMsg(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " JOIN " + nameChannel + "\r\n");

	// Channel TOPIC:
	if (_channels[nameChannel]->getHasTopic() == true)
		client->sendNotif(":" + _name + RPL_TOPIC + client->getNickname() + " " + nameChannel + " :" + _channels[nameChannel]->getTopic() + "\r\n");
	else
		client->sendNotif(":" + _name + RPL_TOPIC + client->getNickname() + " " + nameChannel + " :No topic exists" + "\r\n");

	//NAMES REPLY
	std::string names = ":" + _name + RPL_NAMREPLY + client->getHostname() + " = " + nameChannel + " :" + _channels[nameChannel]->getStrAllOperatorsNames() + _channels[nameChannel]->getStrAllUsersNames();
	client->sendNotif(names + "\r\n");
	
	//END LIST
	client->sendNotif(":" + _name + RPL_ENDOFNAMES + client->getNickname() + " :End of /NAMES list\r\n");
}

bool	nameChannelWellFormated( std::string nameChannel )
{
	if (nameChannel.size() <= 200)
	{
		for (std::string::const_iterator it = nameChannel.begin(); it != nameChannel.end(); ++it)
		{
			if (isPrintable(*it) == false)
				return false;
		}
		return true;
	}
	return false;
}
