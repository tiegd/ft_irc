/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 14:00:09 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/01 13:16:56 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

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
	else
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
		channel->setInvitOnly(toDo);
	else if (!toDo && channel->getInvitOnly())
		channel->setInvitOnly(toDo);
	else
		return;
}

void Server::modeRestrictionTopic(Client* op, Channel* channel, bool toDo)
{
	if (toDo && !channel->getResTopic())
		channel->setHasRestrictionTopic(toDo);
	else if (!toDo && channel->getResTopic())
		channel->setHasRestrictionTopic(toDo);
	else
		return;
}

void Server::modePassword(Client* op, Channel* channel, bool toDo, std::string password)
{
	
}

void Server::modeOpPrivilege(Client* op, Channel* channel, bool toDo, std::string user)
{
	
}

void Server::modeLimitUser(Client* op, Channel* channel, bool toDo, std::string limit)
{
	
}
