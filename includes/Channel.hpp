/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:00:04 by gaducurt          #+#    #+#             */
/*   Updated: 2026/04/30 13:21:11 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <sys/types.h>

class Channel
{
	private:
		std::string							_name;
		std::string							_password;
		std::vector<Client*>				_users;
		std::vector<Client*>				_operator;
		std::string							_topic;
		std::map<std::string, void (*)()>	_mode;
		unsigned int						_nbMembers;
		unsigned int						_nbOp;

		char								_modeUsed; 
		// The modes are stocked on the 5th bits.
		// Need to be checked with binary operator.
		// 0000itkl
		// bool								_invitOnly; //i
		// bool								_restrictionTopic; //t
		// bool								_hasPassword; //k
		bool								_hasTopic;
		// bool								_hasLimit;
		u_int64_t							_userLimit; //l

		Channel();
	public:
		Channel(std::string channel_name, Client *op);
		~Channel();

		//getters
		std::string							getName() const;
		std::string							getPassword() const;
		std::vector<Client*>				getUsers() const;
		void								displayUsers() const;
		std::vector<Client*>				getOperators() const;
		void								displayOps() const;
		std::string							getTopic() const;
		std::map<std::string, void (*)()>	getMode() const;
		unsigned int						getNbMembers() const;
		unsigned int						getNbOp() const;
		bool								getInvitOnly() const;
		bool								getResTopic() const;
		bool								getHasPassword() const;
		bool								getHasTopic() const;
		bool								getHasLimit() const;
		u_int64_t							getUserLimit() const;

		//setters
		void	addUser(Client *target); //add a new user whene a client use join
		void	rmUser(Client *target);
		void	setName(std::string name);
		void	setPassword(std::string password); //set the password if mode's parameter is +k
		void	rmPassword(); //remove the password if mode's parameter is -k
		void	kickUser(Client *target, Client *op); //Remove the client specified in parameter and call the destructor if _n_members == 0
		void	addOperator(Client *target);
		void	rmOperator(Client *target);
		void	setTopic(std::string topic);
		void	rmTopic();
		void	setInvitOnly(bool arg);
		void	setHasRestrictionTopic(bool arg);
		void	setHasPassword(bool arg);
		void	setHasTopic(bool arg);
		void	setHasLimit(bool arg);
		void	setUserLimit(u_int64_t nb);
		void	setNbMembers();
		void	setNbOp();


		void		sendChannelMsg( std::string const& message );
		std::string	getStrAllUsersNames( void );
		std::string	getStrAllOperatorsNames( void );

};

#endif

