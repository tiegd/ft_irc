/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:00:04 by gaducurt          #+#    #+#             */
/*   Updated: 2026/04/23 16:34:42 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <sys/types.h>

class Channel
{
	private:
		std::string				_name;
		std::string				_password;
		std::vector<Client>			_users;
		std::vector<Client>			_operator;
		std::string				_topic;
		std::map<std::string, void (*)()>	_mode;
		unsigned int				_nbMembers;

		bool					_inviteOnly; //i
		bool					_restrictionTopic; //t
		bool					_hasPassword; //k
		bool					_hasTopic;
		bool					_hasLimit;
		u_int64_t				_userLimit; //l

		Channel();
	public:
		Channel(std::string channel_name, Client op);
		~Channel();

		// void invite(Client client);
		// void displayTopic();
		// void changeTopic();
		
		// void limitNbUsers(int nb_user);
		// void listUsers();

		//getters
		std::string				getName() const;
		std::string				getPassword() const;
		std::vector<Client>			getUsers() const;
		std::vector<Client>			getOperators() const;
		std::string				getTopic() const;
		std::map<std::string, void (*)()>	getMode() const;
		unsigned int				getNbMembers() const;
		bool					getInviteOnly() const;
		bool					getResTopic() const;
		bool					getHasPassword() const;
		bool					getHasTopic() const;
		bool					getHasLimit() const;
		u_int64_t				getUserLimit() const;

		//setters
		void	setName(std::string name);
		void	setPassword(std::string password); //set the password if mode's parameter is +k
		void	rmPassword(); //remove the password if mode's parameter is -k
		void	addUser(Client user); //add a new user whene a client use join
		void	kick(Client user); //Remove the client specified in parameter and call the destructor if _n_members == 0
		void	addOperator(Client client);
		void	rmOperator(Client client);
		void	setTopic(std::string topic);
		void	setInvitOnly(bool arg);
		void	setResTopic(bool arg);
		void	setHasPassword(bool arg);
		void	setHasTopic(bool arg);
		void	setHasLimit(bool arg);
		void	setUserLimit(u_int64_t nb);
};

#endif

