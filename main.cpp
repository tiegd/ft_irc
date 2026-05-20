/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 12:32:06 by amerzone          #+#    #+#             */
/*   Updated: 2026/05/20 15:34:21 by gaducurt         ###   ########.fr       */
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
	try
	{
		Server	serv("Intelligent_Rapid_Cacaboudin", port, pwd);
		serv.runServer();
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return (0);
}
