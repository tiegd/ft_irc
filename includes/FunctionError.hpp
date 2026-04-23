/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FunctionError.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 11:14:29 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/23 11:28:45 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THROW_ERROR_HPP
#define THROW_ERROR_HPP

#include <exception>
#include <string>

class	FunctionError : public std::exception
{
	private:
		std::string	_functionName;
	public:
		FunctionError( std::string functionName );
		const char* what( void ) const throw();
		virtual ~FunctionError() throw();
};

#endif
