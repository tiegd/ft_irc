/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 14:00:09 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/11 13:50:13 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <algorithm>

std::vector<std::string>	split( std::string & str, char c );

void Server::MODE(std::string const& line, Client* op)
{
	// /mode seul => affiche les modes activés sur le channel

	// Faire un parsing comme dans join car il peut y avoir plusieurs options
	// en meme temps et les parametres doivent etres dans l'ordre.
	// Split la string en un vector d'options et un vector de parametres.
	// Envoyer dans l'ordes dans les differents methodes.
	
	std::string	temp(line);
	if (temp.size() <= 5)
	{
		ERR_NEEDMOREPARAMS(_name, op, "MODE");
		return ;
	}
	temp.erase(0, 5);

	std::vector<std::string>	splitArgs = split(temp, SPACE);
	std::string					channelTarget = splitArgs[0];
	if (splitArgs.size() == 1)
	{
		if (!_channels[channelTarget])
			ERR_NOSUCHCHANNEL(_name, op, channelTarget);
		else
			RPL_CHANNELMODEIS(_name, op, _channels[channelTarget]);
		return ;
	}
	std::string					options;
	bool						toDo = true;
	options = splitArgs[1];
	if (options[0] == '-' || options[0] == '+')
	{
		if (options[0] == '-')
			toDo = false;
		if (options.size() == 1)
		{
			ERR_NEEDMOREPARAMS(_name, op, "MODE");
			return ;	
		}
		options.erase(0, 1);
	}
	if (!parseOptions(options, op))
		return ;
	splitArgs.erase(splitArgs.begin(), splitArgs.begin() + 2);
	for (int i = 0; i < options.size(); i++)
	{
		switch (options[i])
		{
			case 'i':
				modeInviteOnly(op, _channels[channelTarget], toDo);
			case 't':
				modeRestrictionTopic(op, _channels[channelTarget], toDo);
			case 'k':
				if (splitArgs.size() > 0)
				{
					modePassword(op, _channels[channelTarget], toDo, splitArgs[0]);
					splitArgs.erase(splitArgs.begin());
				}
				else
					ERR_NEEDMOREPARAMS(_name, op, "MODE");
			case 'o':
				if (splitArgs.size() > 0)
				{
					modeOpPrivilege(op, _channels[channelTarget], toDo, splitArgs[0]);
					splitArgs.erase(splitArgs.begin());
				}
				else
					ERR_NEEDMOREPARAMS(_name, op, "MODE");
			case 'l':
				if (splitArgs.size() > 0)
				{
					modeLimitUser(op, _channels[channelTarget], toDo, splitArgs[0]);
					splitArgs.erase(splitArgs.begin());
				}
				else
					ERR_NEEDMOREPARAMS(_name, op, "MODE");
		}
	}
	RPL_CHANNELMODEIS(_name, op, _channels[channelTarget]);
}

void Server::modeInviteOnly(Client* op, Channel* channel, bool toDo)
{
	if (channel->isOperator(op))
	{
		if (toDo && !channel->getInvitOnly())
			channel->setInvitOnly(toDo);
		else if (!toDo && channel->getInvitOnly())
			channel->setInvitOnly(toDo);
		else
			return;
	}
	else
		ERR_CHANOPRIVSNEEDED(_name, op, channel->getName());
}

void Server::modeRestrictionTopic(Client* op, Channel* channel, bool toDo)
{
	if (channel->isOperator(op))
	{
		if (toDo && !channel->getResTopic())
			channel->setHasRestrictionTopic(toDo);
		else if (!toDo && channel->getResTopic())
			channel->setHasRestrictionTopic(toDo);
		else
			return;
	}
	else
		ERR_CHANOPRIVSNEEDED(_name, op, channel->getName());
}

void Server::modePassword(Client* op, Channel* channel, bool toDo, std::string password)
{
	if (!parseChannelPassword(op, channel, password))
		return ;
	if (channel->isOperator(op))
	{
		if (toDo)
			channel->setPassword(op, password);
		else if (!toDo && channel->getHasPassword())
		{
			if (channel->getPassword() == password)
				channel->rmPassword(op);
			else
				ERR_PASSWDMISMATCH(_name, op);
		}
	}
	else
		ERR_CHANOPRIVSNEEDED(_name, op, channel->getName());
}

void Server::modeOpPrivilege(Client* op, Channel* channel, bool toDo, std::string user)
{
	if (channel->isOperator(op))
	{
		for (std::map<SOCKET, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if (it->second->getNickname() == user)
			{
				if (!channel->isOperator(it->second) || !channel->isUser(it->second))
				{
					ERR_NOTONCHANNEL(_name, it->second, channel->getName());
					return;
				}
				if (toDo)
					channel->addOperator(it->second);
				else if (!toDo)
					channel->rmOperator(it->second);
			}
		}
	}
	else
		ERR_CHANOPRIVSNEEDED(_name, op, channel->getName());
}

void Server::modeLimitUser(Client* op, Channel* channel, bool toDo, std::string limit)
{
	if (channel->isOperator(op))
	{
		u_int64_t	nb;
		
		if (toDo)
		{
			for (int i = 0; i < limit.size(); i++)
			{
				if (!std::isdigit(limit[i]))
					ERR_INVALIDMODEPARAM(_name, op, channel->getName(), 'o', limit, "limit can be only numeric characters");
					return ;
			}
			nb = std::atoi(limit.c_str());
			if (limit[0] != '0' && limit.size() == 1 && nb == 0)
				return ;
		}
		channel->setUserLimit(nb, toDo);
	}
	else
		ERR_CHANOPRIVSNEEDED(_name, op, channel->getName());
}

bool Server::parseOptions(std::string options, Client *client)
{
	for (int i = 0; i < options.size(); i++)
	{
		if (options[i] != 'i' && options[i] != 't' && options[i] != 'k' && options[i] != 'o' && options[i] != 'l')
		{
			ERR_UMODEUNKNOWNFLAG(_name, client);
			return (false);
		}
	}
	return (true);
}

bool Server::parseChannelPassword(Client* op, Channel* channel, std::string password)
{
	for (int i = 0; i < password.size(); i++)
	{
		if (password[i] < 33 || password[i] > 126)
		{
			ERR_INVALIDMODEPARAM(_name, op, channel->getName(), 'k', password, "the characters in the keyword must be printable");
			return (false);
		}
	}
	return (true);
}
