/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FunctionError.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 11:14:29 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/29 13:19:42 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THROW_ERROR_HPP
#define THROW_ERROR_HPP

#include <exception>
#include <string>
#include <iostream>

class	FunctionError : public std::exception
{
	public:
		const char* what() const throw();
};

#endif
