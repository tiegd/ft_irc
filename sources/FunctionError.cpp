/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FunctionError.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 11:22:44 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/29 13:20:12 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FunctionError.hpp"

const char* FunctionError::what() const throw()
{
	return "An error occured during send()";
}

