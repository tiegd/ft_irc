#include "Server.hpp"

volatile std::sig_atomic_t gSignalStatus = 0;

Server::Server( void ) {};

Server::Server( std::string name, u_int16_t port, std::string password )
: _name(name), _port(port), _password(password)
{
	setSocketServ();
	struct pollfd	t_fd;
	t_fd.fd = _socketServ;
	t_fd.events = POLLIN;
	t_fd.revents = 0;
	_fds.push_back(t_fd);
}

void	signalHandler(int signal)
{
	(void)signal;
	gSignalStatus = 1;
}

void	Server::setSocketServ( void )
{
	struct sockaddr_in	servaddr;

	if ((_socketServ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("Socket error");

	int flags = fcntl(_socketServ, F_GETFL);
	fcntl(_socketServ, F_SETFL, flags | O_NONBLOCK);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family			= AF_INET;
	servaddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	servaddr.sin_port			= htons(_port);

	int opt = 1;
	setsockopt(_socketServ, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if ((bind(_socketServ, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0)
		throw std::runtime_error("Bind error");

	if (listen(_socketServ, MAXCLIENTS) < 0)
		throw std::runtime_error("Listen error");

	std::cout << "Waiting for a connection on port " << _port << std::endl;
}

// FIX #12 — Uppercase la commande pour être insensible à la casse
static std::string	toUpperCommand( std::string const & line )
{
	std::string upper = line;
	size_t i = 0;
	while (i < upper.size() && upper[i] != ' ')
	{
		upper[i] = std::toupper(upper[i]);
		i++;
	}
	return upper;
}

// FIX #5 — Déconnexion propre d'un client par fd + index
void	Server::disconnectClient( size_t & i, std::string const & reason )
{
	int fd = _fds[i].fd;

	std::cout << _clients[fd]->getNickname() << " disconnected"
			  << (reason.empty() ? "" : " (" + reason + ")") << " !" << std::endl;

	close(fd);
	delete _clients[fd];
	_clients.erase(fd);
	_fds.erase(_fds.begin() + i);
	i--;
}

void	Server::runServer( void )
{
	char	buff[512];

	std::signal(SIGINT, signalHandler);
	while (true)
	{
		if (poll(_fds.data(), _fds.size(), POLL_TIMEOUT_MS) < 0)
		{
			if (gSignalStatus == 1)
			{
				std::cout << "Quit server..." << std::endl;
				return ;
			}
			std::cout << "Error occured during poll()." << std::endl;
			continue;
		}

		// FIX #5 — Vérification du timeout d'inactivité sur chaque itération de poll
		time_t now = time(NULL);
		for (size_t i = 1; i < _fds.size(); i++)
		{
			if (now - _clients[_fds[i].fd]->getLastActivity() > CLIENT_TIMEOUT_SEC)
			{
				std::string nick = _clients[_fds[i].fd]->getNickname();
				std::cout << nick << " timed out." << std::endl;
				disconnectClient(i, "timeout");
			}
		}

		for (size_t i = 0; i < _fds.size(); i++)
		{
			// FIX #8 — Gestion de POLLHUP et POLLERR
			if (_fds[i].revents & (POLLHUP | POLLERR))
			{
				if (_fds[i].fd == _fds[0].fd)
				{
					std::cout << "Error on server socket." << std::endl;
					return ;
				}
				disconnectClient(i, "connection error");
				continue;
			}

			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _fds[0].fd)
				{
					addClientSocket();
					continue ;
				}

				// FIX #2 — Nettoyage du buffer avant recv
				memset(buff, 0, sizeof(buff));
				int bytes = recv(_fds[i].fd, buff, sizeof(buff) - 1, 0);
				if (bytes <= 0)
				{
					// FIX #1 — Sauvegarde du fd avant erase (use-after-free)
					disconnectClient(i, "recv error or closed");
					continue;
				}

				// Mise à jour du timestamp d'activité
				_clients[_fds[i].fd]->updateLastActivity();

				_clients[_fds[i].fd]->_inBuff.append(buff, bytes);

				// FIX #3 — Limite de taille du buffer entrant
				if (_clients[_fds[i].fd]->_inBuff.size() > MAX_INBUFF_SIZE)
				{
					std::cout << "Client " << _clients[_fds[i].fd]->getNickname()
							  << " exceeded buffer limit, disconnecting." << std::endl;
					disconnectClient(i, "buffer overflow");
					continue;
				}

				size_t pos;
				while ((pos = _clients[_fds[i].fd]->_inBuff.find("\r\n")) != std::string::npos)
				{
					std::string line = _clients[_fds[i].fd]->_inBuff.substr(0, pos);
					_clients[_fds[i].fd]->_inBuff.erase(0, pos + 2);

					if (line.empty())
						continue;

					std::cout << "Recu par " << _clients[_fds[i].fd]->getNickname()
							  << ": " << line << std::endl;

					parseCommand(line, _clients[_fds[i].fd]);

					if (_clients.find(_fds[i].fd) != _clients.end()
						&& _clients[_fds[i].fd]->getDeleted() == true)
					{
						disconnectClient(i, "QUIT");
						break;
					}
				}
			}
		}
	}
}

void	Server::parseCommand( std::string const & line , Client* client )
{
	// FIX #12 — Commandes insensibles à la casse
	std::string uline = toUpperCommand(line);

	try
	{
		if (!uline.compare(0, 4, "PASS") && (uline[4] == ' ' || uline.size() == 4))
		{
			client->setPassValid(PASS(line, client));
		}
		if (!uline.compare(0, 4, "NICK") && (uline[4] == ' ' || uline.size() == 4))
		{
			client->setNickValid(NICK(line, client));
		}
		if (!uline.compare(0, 4, "USER") && (uline[4] == ' ' || uline.size() == 4))
		{
			client->setUserValid(USER(line, client));
		}

		// FIX #13 — PING accessible avant enregistrement
		if (!uline.compare(0, 4, "PING") && (uline.size() >= 4))
		{
			PING(line, client);
		}

		// FIX #10 — QUIT accessible avant enregistrement
		if (!uline.compare(0, 4, "QUIT") && (uline[4] == ' ' || uline.size() == 4))
		{
			QUIT(line, client);
			return ;
		}

		if (client->getRegister() == true)
		{
			bool commandFound = false;

			if (!uline.compare(0, 4, "JOIN") && (uline[4] == ' ' || uline.size() == 4))
			{
				JOIN(line, client);
				commandFound = true;
			}
			if (!uline.compare(0, 4, "PART") && (uline[4] == ' ' || uline.size() == 4))
			{
				PART(line, client);
				commandFound = true;
			}
			if (!uline.compare(0, 7, "PRIVMSG") && (uline[7] == ' ' || uline.size() == 7))
			{
				PRIVMSG(line, client);
				commandFound = true;
			}
			if (!uline.compare(0, 5, "TOPIC") && (uline[5] == ' ' || uline.size() == 5))
			{
				TOPIC(line, client);
				commandFound = true;
			}
			if (!uline.compare(0, 4, "KICK") && (uline[4] == ' ' || uline.size() == 4))
			{
				KICK(line, client);
				commandFound = true;
			}
			if (!uline.compare(0, 4, "MODE") && (uline[4] == ' ' || uline.size() == 4))
			{
				MODE(line, client);
				commandFound = true;
			}
			if (!uline.compare(0, 3, "WHO") && (uline[3] == ' ' || uline.size() == 3))
			{
				WHO(line, client);
				commandFound = true;
			}
			if (!uline.compare(0, 6, "INVITE") && (uline[6] == ' ' || uline.size() == 6))
			{
				INVITE(line, client);
				commandFound = true;
			}
			if (!uline.compare(0, 4, "MOTD") && (uline[4] == ' ' || uline.size() == 4))
			{
				MOTD(client);
				commandFound = true;
			}
			if (!uline.compare(0, 6, "NOTICE") && (uline[6] == ' ' || uline.size() == 6))
			{
				NOTICE(line, client);
				commandFound = true;
			}

			// FIX #11 — ERR_UNKNOWNCOMMAND pour commandes inconnues
			if (!commandFound
				&& uline.compare(0, 4, "PASS") != 0
				&& uline.compare(0, 4, "NICK") != 0
				&& uline.compare(0, 4, "USER") != 0
				&& uline.compare(0, 4, "PING") != 0
				&& uline.compare(0, 4, "QUIT") != 0)
			{
				std::string cmd = uline.substr(0, uline.find(' '));
				ERR_UNKNOWNCOMMAND(_name, client, cmd);
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	if (client->getRegister() != true && client->canBeRegistered() == true)
	{
		client->setRegister(true);
		RPL_WELCOME(_name, client);
		RPL_YOURHOST(_name, client);
		RPL_CREATED(_name, client);
		RPL_MYINFO(_name, client);
		MOTD(client);
	}
}

void	Server::addClientSocket( void )
{
	struct sockaddr_in	client_addr;
	socklen_t 			addr_len = sizeof(client_addr);
	SOCKET				client;
	struct pollfd		t_fd;

	// FIX #4 — Limite du nombre de clients connectés
	if (_fds.size() >= static_cast<size_t>(MAXCLIENTS + 1))
	{
		std::cout << "Max clients reached, refusing connection." << std::endl;
		client = accept(_socketServ, (struct sockaddr *) &client_addr, &addr_len);
		if (client >= 0)
			close(client);
		return ;
	}

	client = accept(_socketServ, (struct sockaddr *) &client_addr, &addr_len);
	if (client < 0)
	{
		std::cout << "Error occured during accept()" << std::endl;
		// FIX #7 — throw propre avec exception active
		throw std::runtime_error("accept() failed");
	}

	int flags = fcntl(client, F_GETFL);
	fcntl(client, F_SETFL, flags | O_NONBLOCK);
	t_fd.fd = client;
	t_fd.events = POLLIN;
	t_fd.revents = 0;
	_fds.push_back(t_fd);

	Client* newClient = new Client(client);
	_clients[client] = newClient;

	std::cout << "New client connected" << std::endl;
}

bool	Server::nicknameExist( std::string target )
{
	for (std::map<SOCKET, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname() == target)
			return true;
	}
	return false;
}

Client*	Server::searchClient( std::string target )
{
	for (std::map<SOCKET, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (target.compare(it->second->getNickname()) == 0)
			return it->second;
	}
	return NULL;
}

Server::~Server( void )
{
	std::cout << "Server destructor called !" << std::endl;

	for (std::map<SOCKET, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		close(it->first);
		delete it->second;
	}
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		delete it->second;
	}
	close(_socketServ);
}