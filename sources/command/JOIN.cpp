/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:13:50 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/27 08:44:40 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	nameChannelWellFormated( std::string nameChannel );

// JOIN <channel>{,<channel>} [<key>{,<key>}]
void	Server::JOIN(std::string const& line, Client* client)
{
	if (line.size() <= 5)
	{
		ERR_NEEDMOREPARAMS(_name, client, "JOIN");
		throw std::invalid_argument("Not enough parameters");
	}

	std::string	temp(line);
	temp.erase(0, 5);

	//Split the string in <channel> / <password>
	std::vector<std::string>	splitArgs = split(temp, SPACE);

	std::string					strChannel = splitArgs[0];
	std::vector<std::string>	channels = split(strChannel, ',');
	std::string					strPassword;
	std::vector<std::string>	passwords;

	if (splitArgs.size() > 1)
	{
		strPassword = splitArgs[1];
		passwords = split(strPassword, ',');
	}

	for(size_t i = 0; i < channels.size(); i++)
	{
		std::string nameChannel(channels[i]);
		if (nameChannel[0] != '#')
		{
			ERR_BADCHANMASK(_name, client, nameChannel);
			throw std::invalid_argument("# is missing for the channel name");
		}
		if (_channels.size() == 0 || !_channels[nameChannel])
		{
			if (nameChannelWellFormated(nameChannel) == true)
			{
				Channel*	newChannel = new Channel(nameChannel, client);
				_channels[nameChannel] = newChannel;
				client->addChanJoined(_channels[nameChannel]);
				sendJoinNotification(client, _channels[nameChannel]);
			}
			else
			{
				ERR_BADCHANMASK(_name, client, nameChannel);
				throw std::invalid_argument("Bad channel name");
			}
		}
		else
		{
			if (_channels[nameChannel]->getInvitOnly() == false || (_channels[nameChannel]->getInvitOnly() && _channels[nameChannel]->isInvited(client)))
			{
				if (_channels[nameChannel]->getHasPassword())
				{
					if  (!passwords.empty()  && i < passwords.size())
					{
						if (passwords[i].compare(_channels[nameChannel]->getPassword()) == 0)
						{
							if (!_channels[nameChannel]->getHasLimit() || (_channels[nameChannel]->getHasLimit() && _channels[nameChannel]->getTotClient() < _channels[nameChannel]->getUserLimit()))
							{
								_channels[nameChannel]->addUser(client);
								client->addChanJoined(_channels[nameChannel]);
								sendJoinNotification(client, _channels[nameChannel]);
								if (_channels[nameChannel]->isInvited(client))
									_channels[nameChannel]->rmInvite(client);
							}
							else
							{
								ERR_CHANNELISFULL(_name, client, nameChannel);
								throw std::invalid_argument("Channel is full");
							}
						}
					}
					else
					{
						ERR_BADCHANNELKEY(_name, client, nameChannel);
						throw std::invalid_argument("Missing password");
					}
				}
				else
				{
					if (!_channels[nameChannel]->getHasLimit() || (_channels[nameChannel]->getHasLimit() && _channels[nameChannel]->getTotClient() < _channels[nameChannel]->getUserLimit()))
					{
						_channels[nameChannel]->addUser(client);
						client->addChanJoined(_channels[nameChannel]);
						sendJoinNotification(client, _channels[nameChannel]);
						if (_channels[nameChannel]->isInvited(client))
							_channels[nameChannel]->rmInvite(client);
					}
					else
					{
						ERR_CHANNELISFULL(_name, client, nameChannel);
						throw std::invalid_argument("Channel is full");
					}
				}
			}
			else
			{
				ERR_INVITEONLYCHAN(_name, client, nameChannel);
				throw std::invalid_argument("Invite-only mode is activated");
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
