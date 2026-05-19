/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FunctionError.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 11:22:44 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/19 16:39:18 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FunctionError.hpp"

const char* FunctionError::what() const throw()
{
	return "An error occured during send()";
}

