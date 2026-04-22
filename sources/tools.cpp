/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 13:40:18 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/22 13:40:40 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools.hpp"

std::vector<std::string>	split( std::string & str, char c )
{
	std::vector<std::string>	strvec;
	size_t index = 0;
	size_t	pos = 0;

	while (index < str.size())
	{
		pos = str.find(c, index);
		if (pos == std::string::npos)
		{
			strvec.push_back(str.substr(index, str.size()));
			break;
		}
		strvec.push_back(str.substr(index, pos - index));
		index = pos + 1;
	}
	return strvec;
}
