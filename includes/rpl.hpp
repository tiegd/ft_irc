/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:48:05 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/30 11:00:01 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
#define RPL_HPP

#define	RPL_TOPIC		"332"
#define	RPL_NAMREPLY	"353"
#define RPL_ENDOFNAMES	"366"

void	sendReply( Client *client, std::string prefix, std::string codeError ,std::string messageError);

#endif
