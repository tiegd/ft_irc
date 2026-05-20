/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 14:00:09 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/19 16:35:43 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
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
	if (temp.size() <= 6) // check qu'il y ait bien une #channel après MODE
	{
		ERR_NEEDMOREPARAMS(_name, op, "MODE");
		return ;
	}
	temp.erase(0, 5);

	std::vector<std::string>	splitArgs = split(temp, SPACE);
	std::string					channelTarget = splitArgs[0];
	if (!_channels[channelTarget]) // check l'existance du #channel
	{
		ERR_NOSUCHCHANNEL(_name, op, channelTarget);
		return ;	
	}
	if (splitArgs.size() == 1) // s'il n'y a aucun parametre après #channel
	{
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
	for (size_t i = 0; i < options.size(); i++)
	{
		switch (options[i])
		{
			case 'o':
				if (splitArgs.size() > 0)
				{
					modeOpPrivilege(op, _channels[channelTarget], toDo, splitArgs[0]);
					splitArgs.erase(splitArgs.begin());
				}
				else
					ERR_NEEDMOREPARAMS(_name, op, "MODE");
				break;
			case 'i':
				modeInviteOnly(op, _channels[channelTarget], toDo);
				break;
			case 't':
				modeRestrictionTopic(op, _channels[channelTarget], toDo);
				break;
			case 'l':
				if (splitArgs.size() > 0)
				{
					modeAddLimitUser(op, _channels[channelTarget], toDo, splitArgs[0]);
					splitArgs.erase(splitArgs.begin());
				}
				else if (!toDo)
					modeRmLimitUser(op, _channels[channelTarget], toDo);
				else
					ERR_NEEDMOREPARAMS(_name, op, "MODE");
				break;
			case 'k':
				if (splitArgs.size() > 0)
				{
					modePassword(op, _channels[channelTarget], toDo, splitArgs[0]);
					splitArgs.erase(splitArgs.begin());
				}
				else
					ERR_NEEDMOREPARAMS(_name, op, "MODE");
				break;
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
				if (!channel->isOperator(it->second) && !channel->isUser(it->second))
				{
					ERR_USERNOTINCHANNEL(_name, op, channel->getName());
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

void Server::modeAddLimitUser(Client* op, Channel* channel, bool toDo, std::string limit)
{
	if (channel->isOperator(op))
	{
		if (toDo)
		{
			u_int64_t	nb;
			for (size_t i = 0; i < limit.size(); i++)
			{
				if (!std::isdigit(limit[i]))
				{
					ERR_INVALIDMODEPARAM(_name, op, channel->getName(), 'l', limit, "limit can be only numeric characters");
					return ;
				}
			}
			nb = std::atoi(limit.c_str());
			if (limit[0] != '0' && limit.size() == 1 && nb == 0)
				return ;
			channel->setUserLimit(nb, toDo);
		}
	}
	else
		ERR_CHANOPRIVSNEEDED(_name, op, channel->getName());
}

void Server::modeRmLimitUser(Client* op, Channel* channel, bool toDo)
{
	if (channel->isOperator(op))
		channel->setUserLimit(0, toDo);
	else
		ERR_CHANOPRIVSNEEDED(_name, op, channel->getName());
}

bool Server::parseOptions(std::string options, Client *client)
{
	std::string 			str = "oitkl";
	std::string::size_type	n;
	for (size_t i = 0; i < options.size(); i++)
	{
		if (std::count(options.begin(), options.end(), options[i]) > 1)
			return (false);
		n = str.find(options[i]);
		if (n == std::string::npos)
		{
			ERR_UMODEUNKNOWNFLAG(_name, client);
			return (false);
		}
	}
	return (true);
}

bool Server::parseChannelPassword(Client* op, Channel* channel, std::string password)
{
	for (size_t i = 0; i < password.size(); i++)
	{
		if (password[i] < 33 || password[i] > 126)
		{
			ERR_INVALIDMODEPARAM(_name, op, channel->getName(), 'k', password, "the characters in the keyword must be printable");
			return (false);
		}
	}
	return (true);
}
