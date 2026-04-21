/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:00:04 by gaducurt          #+#    #+#             */
/*   Updated: 2026/04/21 17:00:06 by gaducurt         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

class Channel
{
	private:
		std::string				_name;
		std::string				_password;
		std::vector<Client>			_users;
		std::vector<Client>			_operator;
		std::string				_topic;
		std::map<std::string, void (*)()>	_mode;
		unsigned int				_n_members;
	public:
		Channel();
		~Channel();

		void kick(Client client); //Remove the client specified in parameter and call the destructor if _n_members == 0
		void invite(Client client);
		void changeTopic();
		void setPassword(std::string password); //set the password if mode's parameter is +k
		void rmPassword(); //remove the password if mode's parameter is -k
		void addOperator(Client client);
		void rmOperator(Client client);
		void limitNbUsers(int nb_user);
};

