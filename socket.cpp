#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <vector>

#define SERVER_PORT 1800
#define	SOCKET int
#define MAXLINE	1024

int main(void)
{
	SOCKET				listenfd, connfd, n;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	uint8_t				recvline[MAXLINE];

	// AF_INET => IPv4 Internet Protocol & SOCK_STREAM => it's a TCP socket
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	
		std::cout << "Socket error" << std::endl;
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family			= AF_INET;
	servaddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	servaddr.sin_port			= htons(SERVER_PORT);
	// INADDR_ANY addresse (0) permetant d'accepter n'importe quel message.
	// htonl => Convertit un entier 32 bits de l'ordre dela machine vers l'ordre réseau.
	// htons => Convertit un entier 16 bits de l'ordre dela machine vers l'ordre réseau.

	int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	/* Permet de reutiliser immediatement le port meme si une ancienne instance du server est encre dans l'etat TIME_WAIT.
	SOL_SOCKET option qui permet de cibler le socket.
	SO_REUSADDR permet de redemarrer rapidement le server pour qu'il soit joignable directemnet sans attendre 2/3 min.
	*/

	if ((bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0)
		std::cout << "Bind error" << std::endl;
	/* bind() associe une address IP et un port a un socket.
	C'est l'étape qui nomme le server, par exemple pour faire écouter sur un port precis.
	*/

	if (listen(listenfd, 5) < 0)
		std::cout << "Listen error" << std::endl;
	/* listen() met un socket en mode écoute.
	Elle indique que le socket est pret à recevoir des connexions entrantes
	(le 5 limite le nombre de connexion a 5 clients).*/

	//Vecteur de pollfd dynamique
	std::vector<struct pollfd>	fds;
	fds.push_back({listenfd ,POLLIN ,0}); // Ajouter socket d'écoute en position 0.

	std::cout << "Waiting for a connection on port " << SERVER_PORT << std::endl;
	
	for ( ; ; )
	{
		// Attendre un evenement sur les fds
		if (poll(fds.data(), fds.size(), -1) < 0)
		{
			std::cout << "poll error" << std::endl;
			continue;
		}
		
		// On parcours tous les descripteurs de fichier
		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN) //Verifie dans revents si le POLLIN events est arrivé.
			{
				if (fds[i].fd == listenfd) // si on est sur le fd d'ecoute ca veut dire qu'un nouveau client essaye de communiquer
				{
					// on creer le nouveau client.
					struct sockaddr_in	client_addr;
					socklen_t 			addr_len = sizeof(client_addr);
					SOCKET				client;

					client = accept(listenfd, (struct sockaddr *) &client_addr, &addr_len);

					// on l'ajoute au vector de tous nos client.
					fds.push_back({client, POLLIN, 0});
					std::cout << "New client connected" << std::endl;
				}
				else // si le client existe deja
				{
					int bytes = recv(fds[i].fd, buff, sizeof(buff) - 1, 0);
					if (bytes == 0)
					{
						std::cout << "Client " << i << " deconnected" << std::endl;
						close(fds[i].fd);
						fds.erase(fds.begin() + i);
						i--; // On corrige l'index apres la supression pour pas sauter d'element dans le vecteur.
					}
					else if ( bytes < 0)
					{
						std::cout << "recv error" << std::endl;
						continue;
					}
					else
					{
						std::string	message;
						buff[bytes] = '\0';
						message += buff;
            			size_t pos;
            			while ((pos = message.find('\n')) != std::string::npos) 
						{
            			    std::string line = message.substr(0, pos);
            			    message.erase(0, pos + 1);
						
            			    std::cout << "Client " << i << " dit : "  << line << std::endl;
						}
					}
				}
			}
		}
	}
}
