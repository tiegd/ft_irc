/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 14:46:20 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/15 16:42:45 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
// #include "Channel.hpp"
// #include "rpl.hpp"
// #include "error_IRC.hpp"

void	sendTopicNotif(Channel* channel, Client* client, std::string const& newTopic);

/*
	- +t : que les operateur peuvent modifier le topic du channel;
	- -t : tous le monde peut modifier le topic.
	- TOPIC + <channel> = afficher le topic du channel.
	- TOPIC + <channel> + [:le nom du topic] = changer le topic du channel.
	- Si le [:nom du topic] est une string vide, supprimer le topic.
*/
void	Server::TOPIC(std::string line, Client* client)
{

	std::string	temp(line); 
	temp.erase(0, 6);
	size_t endOfChannelName = temp.find_first_of(' ');

	std::string	channelName = temp.substr(0, endOfChannelName);
	std::string topic;
	if (endOfChannelName != std::string::npos)
	{
		topic = temp.substr(endOfChannelName + 1);
	}
	if (!_channels[channelName])
	{
		ERR_NOSUCHCHAN(_name, client, channelName);
		throw std::invalid_argument("User cannot access topic if channel doesn't exists");
	}
	if (_channels[channelName]->clientIsOnChannel(client) == false)
	{
		ERR_NOTONCHANNEL(_name, client, channelName);
		throw std::invalid_argument("User cannot access topic if his not in channel");
	}
	if (endOfChannelName == std::string::npos) // Ca veut dire qu'il n'y  pas de topic en parametre donc afficher le topic du channel(RPL TOPIC)
	{
		if(_channels[channelName]->getHasTopic() == true)
			RPL_TOPIC(_name, client, channelName,_channels[channelName]->getTopic());
		else
			RPL_NOTOPIC(_name, client, channelName);
	}
	else //il y a un topic en parametre
	{
		if (_channels[channelName]->getResTopic()) // checker si le mode +t est activé 
		{
			if (_channels[channelName]->isOperator(client) == false) // checker si le client est pas un operator & renvoyer erreur en fonction
			{
				ERR_CHANOPRIVSNEEDED(_name, client, channelName);
				throw std::invalid_argument("Channel mode is +t && user is not operator");
			}
		}
		if (topic.size() == 1)
		{
			_channels[channelName]->rmTopic(client);
			sendTopicNotif(_channels[channelName], client, topic);
		}
		else
		{
			topic.erase(0, 1);
			_channels[channelName]->setTopic(client, topic);
			_channels[channelName]->setHasTopic(true);
			sendTopicNotif(_channels[channelName], client, topic);
		}
	}
}

// :WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic
void	sendTopicNotif(Channel* channel, Client* client, std::string const& newTopic)
{
	std::string fullMsg = ":" + client->getFullName() + " TOPIC " + channel->getName() + " :" + newTopic + "\r\n";
	channel->broadcastToAll(fullMsg, client);
}
