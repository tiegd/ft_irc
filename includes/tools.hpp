/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 13:40:55 by jpiquet           #+#    #+#             */
/*   Updated: 2026/05/19 16:31:16 by gaducurt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <vector>
#include <string>

std::vector<std::string>	split( std::string & str, char c );
std::vector<std::string>	splitStr( std::string & str, std::string c_set );
bool						isPrintable( char c );

#endif