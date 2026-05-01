/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:34:51 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/01 13:19:13 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <sys/types.h>
#include <algorithm>
#include "FunctionError.hpp"

Channel::Channel()
{
}

Channel::Channel(std::string channel_name, Client *op) : _name(channel_name), _nbMembers(1), _hasTopic(false), _modeUsed(0)
{
	this->addOperator(op);
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
	// this->displayUsers();
	return (this->_users);
}

void Channel::displayUsers() const
{
	for (int i = 0; i < this->_users.size(); i++)
		std::cout << "users = " << this->_users[i]->getNickname() << std::endl;
}

std::vector<Client*> Channel::getOperators() const
{
	// this->displayOps();
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
	return (this->_modeUsed >> 4 & 1);
}

bool Channel::getResTopic() const
{
	return (this->_modeUsed >> 3 & 1);
}

bool Channel::getHasPassword() const
{
	return (this->_modeUsed >> 2 & 1);
}

bool Channel::getHasTopic() const
{
	return (this->_hasTopic);
}

bool Channel::getHasLimit() const
{
	return (this->_modeUsed & 1);
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

void Channel::rmUser(Client *target)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_users.begin(), this->_users.end(), target);
		if (it != this->_users.end())
			this->_users.erase(it);
	this->setNbMembers();
}

void Channel::kickUser(Client *target, Client *op, std::string msg)
{
	if (target == op)
		return ;
	std::vector<Client*>::iterator it;
	it = std::find(this->_operator.begin(), this->_operator.end(), op);
	if (it != this->_operator.end())
	{
		this->rmOperator(target);
		this->rmUser(target);
		this->sendChannelMsg(msg);
	}
}

void Channel::addOperator(Client *target)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_operator.begin(), this->_operator.end(), target);
	if (it == this->_operator.end())
	{
		this->_operator.push_back(target);
		it = std::find(this->_users.begin(), this->_users.end(), target);
		if (it != this->_users.end())
			this->_users.erase(it);
	}
	this->setNbOp();
}

void Channel::rmOperator(Client *target) // Called by another operator with mode
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_operator.begin(), this->_operator.end(), target);
	if (it != this->_operator.end())
	{
		this->_operator.erase(it);
		this->_operator.push_back(target);
	}
	this->setNbOp();
}

void Channel::setTopic(std::string topic)
{
	// parseTopic(topic)
	if (!this->getResTopic())
	{
		this->_topic = topic;
		setHasTopic(true);
	}
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
	if (arg)
		this->_modeUsed += 1 << 4;
	else
		this->_modeUsed -= 1 << 4;
}

void Channel::setHasRestrictionTopic(bool arg)
{
	if (arg)
		this->_modeUsed += 1 << 3;
	else
		this->_modeUsed -= 1 << 3;
}

void Channel::setHasPassword(bool arg)
{
	if (arg)
		this->_modeUsed += 1 << 2;
	else
		this->_modeUsed -= 1 << 2;
}

void Channel::setHasTopic(bool arg)
{
	this->_hasTopic = arg;
}

void Channel::setHasLimit(bool arg)
{
	if (arg)
		this->_modeUsed += 1;
	else
		this->_modeUsed -= 1;
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

void	Channel::sendChannelMsg( std::string const& message )
{
	for (std::vector<Client*>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (send((*it)->getSocketClient(), message.c_str(), message.size(), 0))
			throw FunctionError();
	}
}

std::string	Channel::getStrAllUsersNames( void )
{
	std::string allUser;

	for (int i = 0; i < _users.size(); i++)
	{
		allUser = _users[i]->getNickname() + " ";
	}
	allUser.erase(allUser.find_last_of(' '), 1);
	return allUser;
}

std::string	Channel::getStrAllOperatorsNames( void )
{
	std::string allUser;

	for (int i = 0; i < _operator.size(); i++)
	{
		allUser = _operator[i]->getNickname() + " ";
	}
	allUser.erase(allUser.find_last_of(' '), 1);
	return allUser;
}
