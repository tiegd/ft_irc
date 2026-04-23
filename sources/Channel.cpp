/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:34:51 by gaducurt          #+#    #+#             */
/*   Updated: 2026/04/23 16:34:59 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(std::string channel_name, Client op) : _name(channel_name), _n_members(1), _invite_only(false), _n_members(1), _inviteOnly(false), _restrictionTopic(false), _hasPassword(false), _hasTopic(false), _hasLimit(false) 
{
	// Ajouter l'operator dans la liste des operator et celle des users
	// Initialiser la map de modes.
}

Channel::~Channel()
{
	// Delete les vectors et maps.
}

std::string Channel::setName(std::string name)
{
	this._name = name;
}

