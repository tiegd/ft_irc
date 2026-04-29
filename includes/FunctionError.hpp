/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FunctionError.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 11:14:29 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/29 10:48:57 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THROW_ERROR_HPP
#define THROW_ERROR_HPP

#include <exception>
#include <string>
#include <iostream>

class	FunctionError : public std::exception
{
	private:
		std::string	_functionName;
	public:
		FunctionError( std::string functionName );
		const char* what() const throw();
};

#endif
