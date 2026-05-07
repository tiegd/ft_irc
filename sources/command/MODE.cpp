/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 14:00:09 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/07 10:16:22 by gaducurt         ###   ########.fr       */
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
		// display mode
		return ;
	}
	temp.erase(0, 5);

	std::vector<std::string>	splitArgs = split(temp, SPACE);
	std::string					channelTarget = splitArgs[0];
	std::string					options;
	bool						toDo = true;

	if (splitArgs.size() < 2)
	{
		ERR_NEEDMOREPARAMS("MODE");
		return ;
	}
	options = splitArgs[1];
	if (!parseOptions(options))
		return ;
	if (options[0] == '-')
		toDo = false;
	options.erase(0, 1);
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
					ERR_NEEDMOREPARAMS("MODE");
			case 'o':
				if (splitArgs.size() > 0)
				{
					modeOpPrivilege(op, _channels[channelTarget], toDo, splitArgs[0]);
					splitArgs.erase(splitArgs.begin());
				}
				else
					ERR_NEEDMOREPARAMS("MODE");
			case 'l':
				if (splitArgs.size() > 0)
				{
					modeLimitUser(op, _channels[channelTarget], toDo, splitArgs[0]);
					splitArgs.erase(splitArgs.begin());
				}
				else
					ERR_NEEDMOREPARAMS("MODE");
		}
	}
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
}

void Server::modePassword(Client* op, Channel* channel, bool toDo, std::string password)
{
	if (!parseChannelPassword(password))
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
				ERR_PASSWDMISMATCH();
		}
	}
}

void Server::modeOpPrivilege(Client* op, Channel* channel, bool toDo, std::string user)
{
	if (channel->isOperator(op))
	{
		for (std::map<SOCKET, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if (it->second->getNickname() == user)
			{
				if (toDo)
					channel->addOperator(it->second);
				else if (!toDo)
					channel->rmOperator(it->second);
			}
		}
	}
}

void Server::modeLimitUser(Client* op, Channel* channel, bool toDo, std::string limit)
{
	
}

bool Server::parseOptions(std::string options)
{
	if (options[0] != '+' || options[0] != '-')
	{
		ERR_NEEDMOREPARAMS("MODE");
		return (false);
	}
	for (int i = 1; i < options.size(); i++)
	{
		if (options[i] != 'i' && options[i] != 't' && options[i] != 'k' && options[i] != 'o' && options[i] != 'l')
		{
			ERR_UMODEUNKNOWNFLAG(options[i]);
			return (false);
		}
	}
	return (true);
}

bool Server::parseChannelPassword(std::string password)
{
	for (int i = 0; i < password.size(); i++)
	{
		if (password[i] < 33 || password[i] > 126)
			return (false);
	}
	return (true);
}
