/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:48:05 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/07 13:35:57 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
#define RPL_HPP

#include "Client.hpp"
#include "Channel.hpp"

void	sendRpl( Client *client, std::string const& msgToClient );

void	RPL_NOTOPIC(std::string serverName, Client* client, std::string const& channel);
void	RPL_TOPIC(std::string const& serverName, Client* client, std::string const& channel, std::string const& topic);
void	RPL_ENDOFNAMES(std::string const& serverName, Client* client, std::string const& channel);
void	RPL_NAMREPLY(std::string const& serverName, Client* client, Channel* channel);
void    RPL_CHANNELMODEIS(std::string const& serverName, Client* client, Channel* channel);

#endif
