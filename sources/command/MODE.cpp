/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 14:00:09 by gaducurt          #+#    #+#             */
/*   Updated: 2026/04/30 14:21:29 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::MODE(std::string const& line, Client* op)
{
	// /mode seul => affiche les modes activés sur le channel	
}

void Server::modeInviteOnly()
{
	// :MODE
}

void Server::modeResTopic()
{
	
}

void Server::modePassword()
{
	
}

void Server::modeOpPrivilege()
{
	
}

void Server::modeLimitUser()
{
	
}
