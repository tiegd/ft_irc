/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:03:42 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/16 17:07:36 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream>

Client::Client( void ) {}

// Client::Client( Client const & src )
// {
// 	//std::cout << "Copy constructor called" << std::endl;
// }

// Client & Client::operator=( Client const & rightSide )
// {
// 	if (this != &rightSide)
// 	return *this;
// }

Client::Client( SOCKET sockClient ) : _clientSocket(sockClient) {}

Client::~Client( void ) {}
