/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 12:32:06 by amerzone          #+#    #+#             */
/*   Updated: 2026/04/17 10:43:22 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "function.hpp"

int main( int ac, char **av )
{
	u_int16_t	port = 0;
	std::string	pwd;

	try
	{
		parseArg(ac, av, port, pwd);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		exit(1);
	}
	// std::cout << " PORT : " << port << " PWD : " << pwd << std::endl;

	try
	{
		Server	serv("CACA", port, pwd);
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return (0);
}
