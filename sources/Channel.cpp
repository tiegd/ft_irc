/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:34:51 by gaducurt          #+#    #+#             */
/*   Updated: 2026/04/24 14:02:36 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <sys/types.h>

Channel::Channel()
{
}

Channel::Channel(std::string channel_name, Client op) : _name(channel_name), _nbMembers(1), _inviteOnly(false), _restrictionTopic(false), _hasPassword(false), _hasTopic(false), _hasLimit(false) 
{
	this->addOperator(op);
	// Ajouter l'operator dans la liste des operator et celle des users
	// Initialiser la map de modes.
	
}

Channel::~Channel()
{
	// Delete les vectors et maps.
}

/*
----------Getters----------
*/

std::string Channel::getName() const
{
	return (this->_name);
}

std::string Channel::getPassword() const
{
	return (this->_password);
}

std::vector<Client> Channel::getUsers() const
{
	return (this->_users);
}

std::vector<Client> Channel::getOperators() const
{
	return (this->_operator);
}

std::string Channel::getTopic() const
{
	return (this->_topic);
}

std::map<std::string, void (*)()> Channel::getMode() const
{
	return (this->_mode);
}

unsigned int Channel::getNbMembers() const
{
	return (this->_nbMembers);
}

bool Channel::getInviteOnly() const
{
	return (this->_inviteOnly);
}

bool Channel::getResTopic() const
{
	return (this->_restrictionTopic);
}

bool Channel::getHasPassword() const
{
	return (this->_hasPassword);
}

bool Channel::getHasTopic() const
{
	return (this->_hasTopic);
}

bool Channel::getHasLimit() const
{
	return (this->_hasLimit);
}

u_int64_t Channel::getUserLimit() const
{
	return (this->_userLimit);
}

/*
----------Setters----------
*/

void Channel::setName(std::string name)
{
	this->_name = name;
}

void Channel::setPassword(std::string password)
{
	this->_password = password;
}

void Channel::rmPassword()
{
	if (this->getHasPassword())
	{
		this->_password.clear();
		this->setHasPassword(false);
	}
}

void Channel::addUser(Client target)
{
	for (int i = 0; i <= this->_users.size; i++)
	{
		if (this->_users[i] == target)
			return ;
	}
	this->_users.push_back(const &target);
}

void Channel::kick(Client target, Client op)
{
	for (int i = 0; i <= this->_operator.size; i++)
	{
		if (this->_operator[i] == op && this->_users.size > 0) // Check if the op is an operator
		{
			for (int i = 0; i <= this->_users.size; i++) // Delete target from the user's list
			{
				if (this->_users[i] == user)
					this->_users.erase(i);
			}
			for (int i = 0; i <= this->_operator.size; i++) // Delete target from the op's list
			{
				if (this->_operator[i] == target)
					this->_operator.erase(i);
			}
		}
	}
}

void Channel::addOperator(Client target)
{
	for (int i = 0; i <= this->_operator.size; i++) // Check if the target is allready op
	{
		if (this->_operator[i] == target)
			return ;
	}
	this->_operator.push_back(const &target);
}

void Channel::rmOperator(Client target)
{
	for (int i = 0; i <= this->_operator.size; i++) // Check if the target is allready op
	{
		if (this->_operator[i] == target)
			this->_operator.erase(i);
	}
}

void Channel::setTopic(std::string topic)
{
	// parseTopic(topic)
	this->_topic = topic;
	setHasTopic(true);
}

void Channel::rmTopic()
{
	if (this->getHasTopic())
	{
		this->_topic.clear();
		this->setHasTopic(false);
	}
}

void Channel::setInvitOnly(bool arg)
{
	this->_invitOnly = arg;
}

void Channel::setRestrictionTopic(bool arg)
{
	this->_restrictionTopic = arg;
}

void Channel::setHasPassword(bool arg)
{
	this->_hasPassword = arg;
}

void Channel::setHasTopic(bool arg)
{
	this->_hasTopic = arg;
}

void Channel::setHasLimit(bool arg)
{
	this->_hasLimit = arg;
}

void Channel::setUserLimit(u_int64_t nb)
{
	this->_userLimit = nb;
}

