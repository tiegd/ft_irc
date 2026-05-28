/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 14:46:20 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/27 11:00:46 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	sendTopicNotif(Channel* channel, Client* client, std::string const& newTopic);

/*
	- +t : only operators can change the channel topic;
	- -t : everyone can change the channel topic.
	- TOPIC + <channel> = display the channel topic.
	- TOPIC + <channel> + [:topic name] = change the channel topic.
	- If the [:topic name] is an empty string, remove the topic.
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
		ERR_NOSUCHCHANNEL(_name, client, channelName);
		throw std::invalid_argument("User cannot access topic if channel doesn't exists");
	}

	if (_channels[channelName]->clientIsOnChannel(client) == false)
	{
		ERR_NOTONCHANNEL(_name, client, channelName);
		throw std::invalid_argument("User cannot access topic if his not in channel");
	}
	
	if (endOfChannelName == std::string::npos) 
	{
		if(_channels[channelName]->getHasTopic() == true)
			RPL_TOPIC(_name, client, channelName,_channels[channelName]->getTopic());
		else
			RPL_NOTOPIC(_name, client, channelName);
	}
	else
	{
		if (_channels[channelName]->getResTopic()) 
		{
			if (_channels[channelName]->isOperator(client) == false) 
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
