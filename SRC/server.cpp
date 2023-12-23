#include "../INC/server.hpp"

int getMaxFd()
{
	int tmp = -1;
    if(fdMapRead.size() >= 1)
	{
		//TODO: TRY TO USE RBEGIN() INSTEAD OF END()
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
	cout << "max is :" << getMaxFd()<< endl;
	int count_send = 0;



	while (1)
	{
		debute:
		refresh_fd_set(theFdSetRead, theFdSetWrite);
		cout << "waiting to select trigger "<< getMaxFd()+1<< endl;
		
		select(getMaxFd()+1, theFdSetRead, theFdSetWrite, NULL, NULL);
		cout << "select triggered"<< endl;	
		if(FD_ISSET(sockfd, theFdSetRead))
		{
			// cout << "new connection to setup"<< endl;
			int datasocket = accept(sockfd, NULL, NULL);
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
				//************
			if(datasocket == -1)
			{
				cerr << "accept error" << endl;
				exit(1);
			}
			cerr << "connection accepted -_-"<<endl;
			fdMapRead[datasocket] = httpRequest();
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
					cout << "waiting for data from client :"<< it->first << endl;
					int size_readed = recv(commSocket, buffer, 200, 0);
					cout << "readed : "<< size_readed<< endl;
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
						cout << "-------------------" << endl;
						it->second.request = it->second.request + string(buffer);
						if(it->second.request.size() > 4  && it->second.request.substr(it->second.request.size() - 4) == "\r\n\r\n")
						{
							cout << "full request reveived!!!"<<endl;
							fdMapWrite[commSocket] = httpResponse(it->second);
							fdMapRead.erase(commSocket);
							refresh_fd_set(theFdSetRead, theFdSetWrite);
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
					cout << (it->second.request.substr(0, 50))<< endl;
							string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
												"<!DOCTYPE html>\n"
												"<html> \n"
												"<body>\n"
												"<form action=\"/action_page.php\" method=\"get\">\n"
												"  First name:<br>\n"
												"  <input type=\"text\" name=\"firstname\" value=\"Mickey\">\n"
												"  <br>\n"
												"  Last name:<br>\n"
												"  <input type=\"text\" name=\"lastname\" value=\"Mouse\">\n"
												"  <br><br>\n"
												"  <input type=\"submit\" value=\"Submit\">\n"
												"</form> \n"
												"</body>\n"
												"</html>\n";
												
							full_request.clear();
							int test = send(commSocket, httpResponse.c_str()+ (120 * count_send), 120, 0);
							cout <<endl<< count_send << " is sent"<< endl;
							if(test == -1)
							{
								cout << "sending error "<< test << endl;
								exit(11);
							}
							if(count_send++ >= 2)
							{
								// cout << "+++++1" << endl;
								close(commSocket);
								count_send = 0;
								
								fdMapWrite.erase(commSocket);
								refresh_fd_set(theFdSetRead, theFdSetWrite);
								break;
							}
				}
			}
		}
	}

	return 0;
}