/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FunctionError.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 11:22:44 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/29 10:39:34 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FunctionError.hpp"

FunctionError::FunctionError( std::string functionName ) : _functionName(functionName) {}


const char* FunctionError::what() const throw()
{
	std::string whatMsg = "An error occured during: " + _functionName;
	return whatMsg.c_str();
}

