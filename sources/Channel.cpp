/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:34:51 by gaducurt          #+#    #+#             */
/*   Updated: 2026/04/29 10:38:18 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <sys/types.h>
#include <algorithm>

Channel::Channel()
{
}

Channel::Channel(std::string channel_name, Client *op) : _name(channel_name), _nbMembers(1), _invitOnly(false), _restrictionTopic(false), _hasPassword(false), _hasTopic(false), _hasLimit(false) 
{
	this->addOperator(op);
	this->addUser(op);
	// Initialiser la map de modes.
	
}

Channel::~Channel()
{
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

std::vector<Client*> Channel::getUsers() const
{
	this->displayUsers();
	return (this->_users);
}

void Channel::displayUsers() const
{
	for (int i = 0; i < this->_users.size(); i++)
		std::cout << "users = " << this->_users[i]->getNickname() << std::endl;
}

std::vector<Client*> Channel::getOperators() const
{
	this->displayOps();
	return (this->_operator);
}

void Channel::displayOps() const
{
	for (int i = 0; i < this->_operator.size(); i++)
		std::cout << "operator = " << this->_operator[i]->getNickname() << std::endl;
}

std::string Channel::getTopic() const
{
	std::cout << getHasTopic() << std::endl;
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

unsigned int Channel::getNbOp() const
{
	return (this->_nbOp);
}

bool Channel::getInvitOnly() const
{
	return (this->_invitOnly);
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
	setHasPassword(true);
}

void Channel::rmPassword()
{
	if (this->getHasPassword())
	{
		this->_password.clear();
		this->setHasPassword(false);
	}
}

void Channel::addUser(Client *target)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_users.begin(), this->_users.end(), target);
	if (it == this->_users.end())
		this->_users.push_back(target);
	this->setNbMembers();
}

void Channel::ejectClient(Client *target, Client *op)
{
	if (target == op)
		return ;
	std::vector<Client*>::iterator it;
	it = std::find(this->_operator.begin(), this->_operator.end(), op);
	if (it != this->_operator.end())
	{
		std::cout << "ok" << std::endl;
		it = std::find(this->_users.begin(), this->_users.end(), target);
		if (it != this->_users.end())
			this->_users.erase(it);
		it = std::find(this->_operator.begin(), this->_operator.end(), target);
		if (it != this->_operator.end())
			this->_operator.erase(it);
	}
	this->setNbMembers();
	this->setNbOp();
}

void Channel::addOperator(Client *target)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_operator.begin(), this->_operator.end(), target);
	if (it == this->_operator.end())
		this->_operator.push_back(target);
	this->setNbOp();
}

void Channel::rmOperator(Client *target) // Called by another operator with mode
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_operator.begin(), this->_operator.end(), target);
	if (it != this->_operator.end())
		this->_operator.erase(it);
	this->setNbOp();
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

void Channel::setNbMembers()
{
	this->_nbMembers = this->_users.size();
}

void Channel::setNbOp()
{
	this->_nbOp = this->_operator.size();
}
