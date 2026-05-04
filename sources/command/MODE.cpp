/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 14:00:09 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/04 13:38:10 by gaducurt         ###   ########.fr       */
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
	temp.erase(0, 5);

	std::vector<std::string>	splitArgs = split(temp, SPACE);
	std::string					channelTarget = splitArgs[0];
	std::string					options;
	bool						toDo;

	options = splitArgs[1];
	if (options[0] == '+')
		toDo = true;
	else if (options[0] == '-')
		toDo = false;
	else
		return; //rajouter l'erreur
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
				modePassword(op, _channels[channelTarget], toDo, splitArgs[0]);
				splitArgs.erase(splitArgs.begin());
			case 'o':
				modeOpPrivilege(op, _channels[channelTarget], toDo, splitArgs[0]);
				splitArgs.erase(splitArgs.begin());
			case 'l':
				modeLimitUser(op, _channels[channelTarget], toDo, splitArgs[0]);
				splitArgs.erase(splitArgs.begin());
		}
	}
}

void Server::modeInviteOnly(Client* op, Channel* channel, bool toDo)
{
	if (toDo && !channel->getInvitOnly())
		channel->setInvitOnly(op, toDo);
	else if (!toDo && channel->getInvitOnly())
		channel->setInvitOnly(op, toDo);
	else
		return;
}

void Server::modeRestrictionTopic(Client* op, Channel* channel, bool toDo)
{
	if (toDo && !channel->getResTopic())
		channel->setHasRestrictionTopic(op, toDo);
	else if (!toDo && channel->getResTopic())
		channel->setHasRestrictionTopic(op, toDo);
	else
		return;
}

void Server::modePassword(Client* op, Channel* channel, bool toDo, std::string password)
{
	if (toDo)
		channel->setPassword(op, password);
	else if (!toDo && !channel->getHasPassword())
		channel->rmPassword(op);
}

void Server::modeOpPrivilege(Client* op, Channel* channel, bool toDo, std::string user)
{
	if (toDo)
		channel->addOperator(op, target);
}

void Server::modeLimitUser(Client* op, Channel* channel, bool toDo, std::string limit)
{
	
}
