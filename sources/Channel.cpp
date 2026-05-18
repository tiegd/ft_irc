/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:34:51 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/18 08:57:55 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <sys/types.h>
#include <algorithm>
#include "FunctionError.hpp"
#include "sstream"
// #include <cstdint>

Channel::Channel()
{
}

Channel::Channel(std::string channel_name, Client *op) : _name(channel_name), _nbMembers(1), _hasTopic(false), _modeUsed(0)
{
	this->addOperator(op);
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
	return (this->_users);
}

void Channel::displayUsers() const
{
	for (int i = 0; i < this->_users.size(); i++)
		std::cout << "users = " << this->_users[i]->getNickname() << std::endl;
}

std::vector<Client*> Channel::getOperators() const
{
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

std::string Channel::getModeString() const
{
	std::string	str;
	if (this->getInvitOnly())
		str += 'i';
	if (this->getResTopic())
		str += 't';
	if (this->getHasLimit())
		str += 'l';
	if (this->getHasPassword())
		str += 'k';
	return (str);
}

std::string Channel::getModeArgs() const
{
	std::string	str;
	if (this->getHasLimit() && !this->getHasPassword())
	{
		std::stringstream	out;
		out << this->getUserLimit();
		str += out.str();
	}
	if (this->getHasPassword() && !this->getHasLimit())
		str += this->getPassword();
	if (this->getHasLimit() && this->getHasPassword())
	{
		std::stringstream	out;
		out << this->getUserLimit();
		str += out.str() + " " + this->getPassword();
	}
	return (str);
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

bool Channel::isOperator(Client *op)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_operator.begin(), this->_operator.end(), op);
	if (it != this->_operator.end())
		return (true);
	else
		return (false);
}

bool Channel::isUser(Client *target)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_users.begin(), this->_users.end(), target);
	if (it != this->_users.end())
		return (true);
	else
		return (false);
}

bool Channel::isInvited(Client *target)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_invited.begin(), this->_invited.end(), target);
	if (it != this->_invited.end())
		return (true);
	else
		return (false);
}

bool		Channel::clientIsOnChannel( Client* client )
{
	if (isUser(client) == true || isOperator(client) == true)
		return true;
	return false;
}

/*
----------Setters----------
*/

void Channel::setName(std::string name)
{
	this->_name = name;
}

void Channel::setPassword(Client *op, std::string password)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_users.begin(), this->_users.end(), op);
	if (it == this->_users.end())
	{
		this->_password = password;
		if (!this->getHasPassword())
			setHasPassword(true);
	}
	else
		return; // renvoyer une erreur.
}

void Channel::rmPassword(Client *op)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_users.begin(), this->_users.end(), op);
	if (it == this->_users.end())
	{
		if (this->getHasPassword())
				this->setHasPassword(false);
		else
			return; // renvoyer une erreur.
	}
	else
		return; // renvoyer une erreur.
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

void Channel::kickUser(Client *target, Client *op)
{
	if (this->isOperator(op))
	{
		this->rmOperator(target);
		this->rmUser(target);
		target->rmChanJoined(this);
	}
}

void Channel::addOperator(Client *target)
{
	this->_operator.push_back(target);
	std::vector<Client*>::iterator it;
	it = std::find(this->_users.begin(), this->_users.end(), target);
	if (it != this->_users.end())
		this->_users.erase(it);
	this->setNbOp();
	this->setNbMembers();
}

void Channel::rmOperator(Client *target) // Called by another operator with mode
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_operator.begin(), this->_operator.end(), target);
	if (it != this->_operator.end())
	{
		this->_operator.erase(it);
		this->_users.push_back(target);
	}
	this->setNbOp();
	this->setNbMembers();
}

void Channel::setTopic(Client *op, std::string topic)
{
	if (!this->getResTopic() || (this->getResTopic() && this->isOperator(op)))
	{
		this->_topic = topic;
		setHasTopic(true);
	}
}

void Channel::rmTopic(Client *op)
{
	if (!this->getResTopic() || (this->getResTopic() && this->isOperator(op)))
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

void Channel::setUserLimit(u_int64_t nb, bool arg)
{
	if (arg == true && nb >= this->getNbMembers())
	{
		std::cout << "set limit" << std::endl;
		this->_userLimit = nb;
		this->setHasLimit(true);
	}
	else if (arg == false || nb == 0)
		this->setHasLimit(false);
}

void Channel::setNbMembers()
{
	this->_nbMembers = this->_users.size();
}

void Channel::setNbOp()
{
	this->_nbOp = this->_operator.size();
}

void Channel::addBackInvite(Client *target)
{
	this->_invited.push_back(target);
}

void Channel::rmInvite(Client *target)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_invited.begin(), this->_invited.end(), target);
		if (it != this->_invited.end())
			this->_invited.erase(it);
}

void	Channel::broadcastToAll( std::string const& message, Client* sender )
{
	// (void)sender;
	for (std::vector<Client*>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if ((*it)->getSocketClient() == sender->getSocketClient())
			continue;
		if (send((*it)->getSocketClient(), message.c_str(), message.size(), 0) < 0)
			std::cout << "send() error" << std::endl;

	}
	for (std::vector<Client*>::iterator it = _operator.begin(); it != _operator.end(); it++)
	{
		if ((*it)->getSocketClient() == sender->getSocketClient())
			continue;
		if (send((*it)->getSocketClient(), message.c_str(), message.size(), 0) < 0)
			std::cout << "send() error" << std::endl;
	}
}

std::string	Channel::getStrAllUsersNames( void )
{
	std::string allUsers;

	for (size_t i = 0; i < _users.size(); ++i)
	{
		if (!allUsers.empty())
			allUsers += " ";
		allUsers += _users[i]->getNickname();
	}
	return allUsers;
}

std::string	Channel::getStrAllOperatorsNames( void )
{
	std::string allOps;

	for (size_t i = 0; i < _operator.size(); ++i)
	{
		if (!allOps.empty())
			allOps += " ";
		allOps += '@' + _operator[i]->getNickname() + " ";
	}
	return allOps;
}


void Channel::printUsers() const
{
	std::cout << "Users : " << std::endl;
	for (int i = 0; i < _users.size(); i++)
		std::cout << _users[i]->getNickname() << std::endl;
	std::cout << "Operators : " << std::endl;
	for (int i = 0; i < _operator.size(); i++)
		std::cout << _operator[i]->getNickname() << std::endl;
}
