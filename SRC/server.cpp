/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 09:45:04 by rennatiq          #+#    #+#             */
/*   Updated: 2024/02/06 11:45:57 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"

extern std::map<int, httpRequest> fdMapRead;
extern std::map<int, httpResponse> fdMapWrite;
extern std::map<int, Server> servers_sockets;
extern std::vector<int> deleteReadFd;
extern std::vector<int> deleteWriteFd;
extern fd_set theFdSetRead[NBOFCLIENTS];


void refresh_fd_set(fd_set *fdRead, fd_set *fdWrite)
{
    // FD_ZERO(fdRead);
	bzero(fdRead, sizeof(fdRead) * NBOFCLIENTS);
	bzero(fdWrite, sizeof(fdWrite) * NBOFCLIENTS);
	for (std::vector<int>::iterator it = deleteReadFd.begin(); it != deleteReadFd.end(); it++)
		if(fdMapRead.find(*it) != fdMapRead.end())
			fdMapRead.erase(*it);
	for (std::vector<int>::iterator it = deleteWriteFd.begin(); it != deleteWriteFd.end(); it++)
		if(fdMapWrite.find(*it) != fdMapWrite.end())
			fdMapWrite.erase(*it);
	
	deleteReadFd.clear();
	deleteWriteFd.clear();
	for (std::map<int, Server>::iterator it  = servers_sockets.begin(); it != servers_sockets.end(); it++)
	{
		FD_SET(it->first, fdRead);
	}
    for (std::map<int, httpRequest>::iterator it = fdMapRead.begin(); it != fdMapRead.end(); it++)
    {
        FD_SET(it->first, fdRead);
    }
    for (std::map<int, httpResponse>::iterator it = fdMapWrite.begin(); it != fdMapWrite.end(); it++)
    {
        FD_SET(it->first, fdWrite);
    }
}

int connectSockets(parceConfFile cf)
{
	int port;
	string host;
	int nbOfSockets = 0;

	for (int i = 0; i < cf.server_nb ; i ++)
	{
		for (size_t pot = 0; pot < cf.server[i].listen.size(); pot++)
		{
			port = std::atoi(cf.server[i].listen[pot].c_str());
			host = cf.server[i].host;
			int sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if(sockfd == -1)
			{
				cerr << "Failed to Create Socket"<< endl;
				return 1;
			}
			struct sockaddr_in address;
			// socklen_t socket_lenght  = sizeof(address);
			bzero(&address, sizeof(struct sockaddr_in));
			address.sin_family = AF_INET;
			address.sin_port = htons(port);
			inet_pton(AF_INET, host.c_str(), &(address.sin_addr));
			memset(address.sin_zero, '\0', sizeof address.sin_zero);
			
			//reuse the port number after closing
			int optval = 1;
			if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
				cerr << "Error setting socket options" << endl;
				continue;
			}

			fcntl(sockfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

			if(bind(sockfd, (struct sockaddr*)&address, sizeof(address)) == -1)
			{
				cerr << "\033[31mhost : ["<< host << "] and port : ["<< port <<"] failed to bind.\033[0m"<< endl;
				continue;
			}
			{
				cout << "\033[32mhost : ["<< host << "] and port : ["<< port <<"] has been binded.\033[0m"<< endl;
			}
			
// run this sysctl kern.ipc.somaxconn to see why 128
			if(listen(sockfd, 128) == -1)
			{
				cerr << "Failed to listen"<< endl;
				continue;
			}

			FD_ZERO(theFdSetRead);
			servers_sockets[sockfd] = cf.server[i];

			nbOfSockets ++;
		}
	}
	return nbOfSockets;
}

void acceptNewConnections(int sockfd)
{
	int datasocket = accept(sockfd, NULL, NULL);
		if(datasocket == -1)
		{
			cerr << "accept error" << endl;
			
			return ;
		}
struct linger linger_opt = {1, 0}; // Linger active, timeout 0
		setsockopt(datasocket, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));

		int optval =1 ;
		if(setsockopt(datasocket, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval)) == -1)
		{
			cerr << "set Socket Options error"<<endl;
			return ;
		}
		fcntl(sockfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
		
		
		fdMapRead.insert(std::make_pair(datasocket, httpRequest(datasocket, sockfd)));
}
