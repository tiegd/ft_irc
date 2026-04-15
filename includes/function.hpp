/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   function.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amerzone <amerzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 12:35:18 by amerzone          #+#    #+#             */
/*   Updated: 2026/04/15 14:48:46 by amerzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <exception>
#include <stdexcept>
#include <iostream>
#include "Server.hpp"

void	parseArg( int ac, char **av, u_int16_t & port, std::string & password );

#endif