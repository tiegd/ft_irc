/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:00:04 by gaducurt          #+#    #+#             */
/*   Updated: 2026/05/28 14:34:40 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

class Client;

class Channel
{
	private:
		std::string							_name;
		std::string							_password;
		std::vector<Client*>				_users;
		std::vector<Client*>				_operator;
		std::vector<Client*>				_invited;
		std::string							_topic;
		unsigned int						_nbMembers;
		unsigned int						_nbOp;
		unsigned int						_totClient;

		char								_modeUsed; 
		/*	The modes are stocked on the 5th bits.
			Need to be checked with binary operator.
			0000itkl */
		bool								_hasTopic;
		u_int64_t							_userLimit;

		Channel();
		Channel( Channel const& src );
		Channel & operator=(Channel const& rightSide);

	public:
		Channel(std::string channel_name, Client *op);
		~Channel();

		/* GETTER */
		std::string							getName() const;
		std::string							getPassword() const;
		std::vector<Client*>				getUsers() const;
		std::vector<Client*>				getOperators() const;
		std::string							getTopic() const;
		std::string							getModeString() const;
		std::string							getModeArgs() const;
		unsigned int						getNbMembers() const;
		unsigned int						getNbOp() const;
		unsigned int						getTotClient() const;
		bool								getInvitOnly() const;
		bool								getResTopic() const;
		bool								getHasPassword() const;
		bool								getHasTopic() const;
		bool								getHasLimit() const;
		u_int64_t							getUserLimit() const;
		bool								isOperator(Client *op);
		bool								isUser(Client *target);
		bool								isInvited(Client *target);
		bool								clientIsOnChannel( Client* client );

		/* SETTER */
		void	addUser(Client *target);
		void	rmUser(Client *target);
		void	setPassword(Client *op, std::string password);
		void	rmPassword(Client *op);
		void	kickUser(Client *target, Client *op);
		void	addOperator(Client *target);
		void	rmOperator(Client *target);
		void	setTopic(Client *op, std::string topic);
		void	rmTopic(Client *op);
		void	setInvitOnly(bool arg);
		void	setHasRestrictionTopic(bool arg);
		void	setHasPassword(bool arg);
		void	setHasTopic(bool arg);
		void	setHasLimit(bool arg);
		void	setUserLimit(u_int64_t nb, bool arg);
		void	setNbMembers();
		void	setNbOp();
		void	setTotClient();
		void	addBackInvite(Client *target);
		void	rmInvite(Client *target);

		void	broadcastToAll( std::string const& message, Client* sender );
		void	nameRplToAll(std::string serverName);	

		std::string	getStrAllUsersNames( void );
		std::string	getStrAllOperatorsNames( void );
};

#endif

