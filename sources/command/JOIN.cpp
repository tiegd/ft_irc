/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpiquet <jpiquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:13:50 by jpiquet           #+#    #+#             */
/*   Updated: 2026/04/22 18:25:58 by jpiquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "tools.hpp"

// std::string line = "JOIN #newchannel, #otherchannel password";
// JOIN <channel>{,<channel>} [<key>{,<key>}]
void	Server::JOIN(std::string const& line, Client* client)
{
	/* CHECK
	- Verifier qu'il y a assez de parametre.(au moins 1)
	- Verifier si le channel existe deja ou pas.
	- Si il existe checker si il a un mot de passe, checker si il est mode invite only, et checker si c'est le bon mot de pass si il y en a un.
	- Creer le channel.
	- Verifier que le nom du channel a le bon format.(commance par #, 200 char max et pas de SPACE, pas de ',' et pas de (G^))
	- Checker si il y a une ',' apres le nom du chennel ou le password car on peut rejoindre plusieur channels en meme temps.
	

	- Split pour chaque "," si il y en a une, ensuite parser chaque string et faire les verifs.
	*/

	if (line.size() <= 5)
	{
		sendError(client, _name, ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters");
		throw std::invalid_argument("Not enough parameters");		
	}

	std::string	temp = line.substr(0, 5);

	std::vector<std::string>	splitArgs = split(temp, SPACE);
	std::string					strChannel = splitArgs[0];
	std::string					strPassword = splitArgs[1];

	std::vector<std::string>	channels = split(strChannel, ',');
	std::vector<std::string>	passwords = split(strPassword, ',');

	std::map<std::string, Channel*>::iterator	it_end = _channels.end();
	for(size_t i = 0; i < channels.size(); i++)
	{
		std::string nameChannel(channels[i]);
		if (nameChannel[0] != '#')
		{
			sendError(client, _name, ERR_BADCHANMASK, "JOIN :Bad channel mask");
			throw std::invalid_argument("# is missing for the channel name");
		}
		if (it_end == _channels.find(nameChannel)) // regarde si le channel n'existe pas
		{
			// create channel
			createChannel(nameChannel);
			_channels[nameChannel].addUser(Client *);
		}
		else // si il existe
		{
			// checker si il a un password
			if (_channels[nameChannel]->_hasPassword) //mettre getHasPassword()
			{
				// si il en a un verifier si c'est le bon.
				if  (i < passwords.size()) // si l'index du vector de passwords est plus petit on compare
				{
					if (passwords[i].compare(_channels[nameChannel]._password) == 0) //si le password est correct | remplacer _password par getPassword
						_channels[nameChannel].addUser(Client *);
				}
				else //sinon ca veut dire qu'il manque un parametre password pour le channel donc renvoyer badchannelkey
					sendError(client, _name, ERR_BADCHANNELKEY, nameChannel + " :Cannot join channel (+k)");
			}
			else //si il y a pas de password faire addUser
				_channels[nameChannel].addUser(Client *);
		}
	}

	// if (line.find(',', 0) != std::string::npos)
	// {
	// 	std::vector<std::string>	multipleChannels = split(temp, ',');
	// 	// envoyer chaque split dans la fonction qui va les rajouter au channel etc.
	// }
	// else
	// {
	// 	std::vector<std::string>	soloChannel = split(temp, ' ');
	// 	if (soloChannel.size() == 1)
	// 	{
	// 		sendError(client, _name, ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters");
	// 		throw std::invalid_argument("Not enough parameters");
	// 	}
	// 	// else if
	// 	soloChannel.erase(soloChannel.begin());
	// }

}
