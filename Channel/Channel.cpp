/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:34:51 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/27 16:53:33 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>
#include "sstream"

Channel::Channel()
{
}

Channel::Channel(std::string channel_name, Client *op) : _name(channel_name), _nbMembers(1), _modeUsed(0), _hasTopic(false)
{
	this->addOperator(op);
}

Channel::~Channel()
{
}

/*----------Getters----------*/

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

std::vector<Client*> Channel::getOperators() const
{
	return (this->_operator);
}

std::string Channel::getTopic() const
{
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

unsigned int Channel::getTotClient() const
{
	return (this->_totClient);
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

/*----------Setters----------*/

void Channel::setPassword(Client *op, std::string password)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_operator.begin(), this->_operator.end(), op);
	if (it != this->_operator.end())
	{
		this->_password = password;
		if (!this->getHasPassword())
			setHasPassword(true);
	}
	else
		return;
}

void Channel::rmPassword(Client *op)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_operator.begin(), this->_operator.end(), op);
	if (it == this->_operator.end())
	{
		if (this->getHasPassword())
				this->setHasPassword(false);
		else
			return;
	}
	else
		return;
}

void Channel::addUser(Client *target)
{
	
	std::vector<Client*>::iterator it;
	it = std::find(this->_users.begin(), this->_users.end(), target);
	if (it == this->_users.end())
		this->_users.push_back(target);
	this->setTotClient();
}

void Channel::rmUser(Client *target)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_users.begin(), this->_users.end(), target);
		if (it != this->_users.end())
			this->_users.erase(it);
	this->setTotClient();
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
	this->setTotClient();
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
	this->setTotClient();
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
	if (arg && !getInvitOnly())
		this->_modeUsed += 1 << 4;
	else if (!arg && getInvitOnly())
		this->_modeUsed -= 1 << 4;
}

void Channel::setHasRestrictionTopic(bool arg)
{
	if (arg && !getResTopic())
		this->_modeUsed += 1 << 3;
	else if (!arg && getResTopic())
		this->_modeUsed -= 1 << 3;
}

void Channel::setHasPassword(bool arg)
{
	if (arg && !getHasPassword())
		this->_modeUsed += 1 << 2;
	else if (!arg && getHasPassword())
		this->_modeUsed -= 1 << 2;
}

void Channel::setHasTopic(bool arg)
{
	this->_hasTopic = arg;
}

void Channel::setHasLimit(bool arg)
{
	if (arg && !getHasLimit())
		this->_modeUsed += 1;
	else if (!arg && getHasLimit())
		this->_modeUsed -= 1;
}

void Channel::setUserLimit(u_int64_t nb, bool arg)
{
	if (arg == true && nb >= this->getTotClient())
	{
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

void Channel::setTotClient()
{
	this->setNbMembers();
	this->setNbOp();
	this->_totClient = getNbMembers() + getNbOp();
}

void Channel::addBackInvite(Client *target)
{
	std::vector<Client*>::iterator it;
	it = std::find(this->_invited.begin(), this->_invited.end(), target);
	if (it == this->_invited.end())
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

void Channel::nameRplToAll(std::string serverName)
{
	for (size_t	i = 0; i < _users.size(); i++)
	{
		RPL_NAMREPLY(serverName, this->_users[i], this);
		RPL_ENDOFNAMES(serverName, this->_users[i], this->_name);
	}
	for (size_t	i = 0; i < _operator.size(); i++)
	{
		RPL_NAMREPLY(serverName, this->_operator[i], this);
		RPL_ENDOFNAMES(serverName, this->_operator[i], this->_name);
	}
}
