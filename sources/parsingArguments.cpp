/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingArguments.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 12:34:11 by amerzone          #+#    #+#             */
/*   Updated: 2026/05/19 11:50:13 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "function.hpp"
#include <algorithm>
#include "tools.hpp"

void	parseArg( int ac, char **av, u_int16_t & port, std::string & password)
{
	if (ac != 3)
		throw std::invalid_argument("Bad number of arguments.");

	char*	end_ptr;
	long tport = std::strtol(av[1], &end_ptr, 10);
	if (*end_ptr != '\0')
		throw std::invalid_argument("Port must contain only numbers.");
	if ( tport < 1024 || tport > 49151)
		throw std::invalid_argument("Port must be between 1024 & 49151 included.");
	port = static_cast<u_int16_t>(tport);

	password = av[2];
	if (password.size() < 1 || password.size() > 50)
		throw std::invalid_argument("Password must contain between 1 & 50 characters");

	for (std::string::const_iterator it = password.begin(); it != password.end(); ++it)
	{
		if (isPrintable(*it) == false)
			throw std::invalid_argument("Password must contain only printable characters.");
	}
}
