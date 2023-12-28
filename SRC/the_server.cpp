#include "../INC/server.hpp"

int getMaxFd()
{
	int tmp = -1;
    if(fdMapRead.size() >= 1)
	{
		//PARSE: TRY TO USE RBEGIN() INSTEAD OF END
		std::map<int, httpRequest>::iterator it = fdMapRead.end();
		it--;
		if( it->first > tmp)
			tmp = it->first;
	}
	if(fdMapWrite.size() >= 1)
	{
		std::map<int, httpResponse>::iterator it = fdMapWrite.end();
		it--;
		if( it->first > tmp)
			tmp = it->first;
	}

    return tmp;
}

void refresh_fd_set(fd_set *fdRead, fd_set *fdWrite)
{
    // FD_ZERO(fdRead);
	bzero(fdRead, sizeof(fdRead) * NBOFCLIENTS);
	bzero(fdWrite, sizeof(fdWrite) * NBOFCLIENTS);

    for (std::map<int, httpRequest>::iterator it = fdMapRead.begin(); it != fdMapRead.end(); it++)
    {
        FD_SET(it->first, fdRead);
    }
    for (std::map<int, httpResponse>::iterator it = fdMapWrite.begin(); it != fdMapWrite.end(); it++)
    {
        FD_SET(it->first, fdWrite);
    }
}


int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	string full_request;

	if(sockfd == -1)
	{
		cerr << "Failed to Create Socket"<< endl;
		return 1;
	}
	struct sockaddr_in address;
	// socklen_t socket_lenght  = sizeof(address);
	bzero(&address, sizeof(struct sockaddr_in));
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT1);
	address.sin_addr.s_addr = INADDR_ANY;
	int optval = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
		cerr << "Error setting socket options" << endl;
		exit(1);
	}

	// Set the socket to non-blocking mode
	int flags = fcntl(sockfd, F_GETFL, 0);
	if (flags == -1) {
		cerr << "Can't get flags for socket" << endl;
		return -1;
	}

	flags |= O_NONBLOCK;
	if (fcntl(sockfd, F_SETFL, flags) == -1) {
		cerr << "Can't set socket to non-blocking mode" << endl;
		return -1;
	}

	if(bind(sockfd, (struct sockaddr*)&address, sizeof(address)) == -1)
	{
		cerr << "Failed to Bind"<< endl;
		return 1;
	}

	if(listen(sockfd, 10) == -1)
	{
		cerr << "Failed to listen"<< endl;
		return 1;        
	}

	fd_set theFdSetRead[NBOFCLIENTS];
	fd_set theFdSetWrite[NBOFCLIENTS];
	FD_ZERO(theFdSetRead);
	fdMapRead[sockfd] = httpRequest();
	int count_send = 0;

	while (1)
	{
		debute:
		refresh_fd_set(theFdSetRead, theFdSetWrite);
		select(getMaxFd()+1, theFdSetRead, theFdSetWrite, NULL, NULL);
		// cout << "***********************************"<< endl;//test
		// cout << "select triggered"<< endl;	
		if(FD_ISSET(sockfd, theFdSetRead))
		{
			// cout << "new connection to setup"<< endl;
			int datasocket = accept(sockfd, NULL, NULL);
			if(datasocket == -1)
			{
				cerr << "accept error" << endl;
				exit(1);
			}
			int optval =1 ;
			if(setsockopt(datasocket, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval)) == -1)
			{
				cerr << "set Socket Options error"<<endl;
				exit(1);
			}
			// else
			// {
			// 	// cout << "socket " << datasocket << " is on nosigpipe" << endl;//test
			// }
			// Set the socket to non-blocking mode
			int flags = fcntl(datasocket, F_GETFL, 0);
			if (flags == -1) {
				cerr << "Can't get flags for socket" << endl;
				return -1;
			}
			flags |= O_NONBLOCK;
			if (fcntl(datasocket, F_SETFL, flags) == -1) {
				cerr << "Can't set socket to non-blocking mode" << endl;
				return -1;
			}
			fdMapRead[datasocket] = httpRequest(datasocket, "");
			refresh_fd_set(theFdSetRead, theFdSetWrite);
			
		}
		else
		{
			int commSocket;

			char buffer[200];

			for (std::map<int, httpRequest>::iterator it = fdMapRead.begin(); it != fdMapRead.end(); it++)
			{
				if(FD_ISSET(it->first, theFdSetRead))
				{
					
					commSocket = it->first;
					bzero(buffer, 200);
					int size_readed = recv(commSocket, buffer, 200, 0);
					if(size_readed == -1)
					{
						cerr << "error at reading from socket"<< endl;
						exit(1);
					}
					else if(size_readed == 0)
					{
						cout << "connection ended"<< endl;
						full_request.clear();
						close(commSocket);
						fdMapRead.erase(commSocket);
						//TODO: TRY CONTINUE
					}
					else 
					{
						// cout << "-------------------" << endl;
						it->second.request = it->second.request + string(buffer);
						if(it->second.request.size() > 4  && it->second.request.substr(it->second.request.size() - 4) == "\r\n\r\n")
						{
							// cout << "full request received!!!"<<endl;
							it->second.generate_response();
							fdMapWrite.insert(std::make_pair(commSocket, httpResponse(it->second)));
							fdMapRead.erase(commSocket);
							refresh_fd_set(theFdSetRead, theFdSetWrite); //TODO: OPTIMIZE
							goto debute;
						}
					}
				}
			}
			for (std::map<int, httpResponse>::iterator it = fdMapWrite.begin(); it != fdMapWrite.end(); it++)
			{
				if(FD_ISSET(it->first, theFdSetWrite))
				{
					commSocket = it->first;			
							full_request.clear();
							if (it->second.sendChunk())
							{
								close(commSocket);
								count_send = 0;//delete
								
								// cout << "closed" << commSocket<<endl;//test
								fdMapWrite.erase(commSocket);
								break;

							}

				}
			}
		}
	}

	return 0;
}