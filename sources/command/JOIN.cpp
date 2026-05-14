/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:13:50 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/14 14:12:24 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
// #include "Client.hpp"
// #include "tools.hpp"
// #include "error_IRC.hpp"
// #include "Channel.hpp"
// #include "rpl.hpp"

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
		ERR_NEEDMOREPARAMS(_name, client, "JOIN");
		throw std::invalid_argument("Not enough parameters");		
	}

	std::string	temp(line);
	temp.erase(0, 5);

	//Diviser la string en <channel> / <password>
	std::vector<std::string>	splitArgs = split(temp, SPACE);

	/*Checker si le params est 0 dans ce cas faire PART a tous les channels du client*/
	std::string					strChannel = splitArgs[0];
	std::vector<std::string>	channels = split(strChannel, ',');
	std::string					strPassword;
	std::vector<std::string>	passwords;

	if (splitArgs.size() > 1)
	{
		strPassword = splitArgs[1];
		passwords = split(strPassword, ',');
	}

	// std::map<std::string, Channel*>::iterator	it_end = _channels.end();
	for(size_t i = 0; i < channels.size(); i++) // boucle pour iteré sur tous les channels.
	{
		std::string nameChannel(channels[i]);
		if (nameChannel[0] != '#')
		{
			ERR_BADCHANMASK(_name, client, nameChannel);
			throw std::invalid_argument("# is missing for the channel name");
		}
		if (_channels.size() == 0 || !_channels[nameChannel]) // regarde si le channel n'existe pas
		{
			//si il existe pas checker que le nom u channel a le bon format
			if (nameChannelWellFormated(nameChannel) == true) // faire la fonction
			{
				// créer le channel
				Channel*	newChannel = new Channel(nameChannel, client);
				_channels[nameChannel] = newChannel;
				client->addChanJoined(_channels[nameChannel]);
				sendJoinNotification(client, _channels[nameChannel]);
			}
		}
		else // si il existe
		{
			//checker si c'est en mode invite-only
			if (_channels[nameChannel]->getInvitOnly() == false)
			{
				// checker si il a un password
				if (_channels[nameChannel]->getHasPassword())
				{
					// si il en a un verifier si c'est le bon.
					if  (!passwords.empty()  && i < passwords.size()) // si l'index du vector de passwords est plus petit on compare
					{
						if (passwords[i].compare(_channels[nameChannel]->getPassword()) == 0) //si le password est correct
						{
							_channels[nameChannel]->addUser(client);
							client->addChanJoined(_channels[nameChannel]);
							sendJoinNotification(client, _channels[nameChannel]);
						}
					}
					else //sinon ca veut dire qu'il manque un parametre password pour le channel donc renvoyer badchannelkey
						ERR_BADCHANNELKEY(_name, client, nameChannel);
				}
				else //si il y a pas de password faire addUser
				{
					_channels[nameChannel]->addUser(client);
					client->addChanJoined(_channels[nameChannel]);
					sendJoinNotification(client, _channels[nameChannel]);
				}
			}
			else // renvoyer une erreur car le mode invite only est activé
			{
				ERR_INVITEONLYCHAN(_name, client, nameChannel);
			}
		}
	}
}

void	Server::sendJoinNotification(Client *client, Channel* channel)
{
	std::string	channelMsg = ":" + client->getFullName() + " JOIN " + channel->getName() + "\r\n";

	sendRpl(client, channelMsg);
	channel->broadcastToAll(channelMsg, client);

	RPL_NAMREPLY(_name, client, channel);
	RPL_ENDOFNAMES(_name, client, channel->getName());

	if (channel->getHasTopic() == true)
		RPL_TOPIC(_name, client, channel->getName(), channel->getTopic());
	else
		RPL_NOTOPIC(_name, client, channel->getName());

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
