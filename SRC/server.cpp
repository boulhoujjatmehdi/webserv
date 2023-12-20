#include "../INC/server.hpp"


int main()
{
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
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

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

    fd_set theFdSet[NBOFCLIENTS];
    FD_ZERO(theFdSet);
    fd_vec.push_back(sockfd);
    FD_SET(sockfd, theFdSet);
    cout << "max is :" << getMaxFd()<< endl;
    while (1)
    {
        select(getMaxFd()+1, theFdSet, NULL, NULL, NULL);
        if(FD_ISSET(sockfd, theFdSet))
        {
            cout << "new connection to setup"<< endl;
            int datasocket = accept(sockfd, NULL, NULL);
            if(datasocket == -1)
            {
                cerr << "accept error" << endl;
                exit(1);
            }
            cerr << "connection accepted -_-"<<endl;
        }
        else
        {
            int commSocket;
            char buffer[1024];
            for (int i = 0; i < NBOFCLIENTS; i++)
            {
                if(FD_ISSET(fd_vec.at(i), theFdSet))
                {
                    string str;
                    commSocket = fd_vec.at(i);
                    bzero(buffer, 1024);
                    cout << "waiting for data from client"<< endl;
                    int size_readed = read(commSocket, buffer, 1024);
                    if(size_readed == -1)
                    {
                        cerr << "error at reading from socket"<< endl;
                        exit(1);
                    }
                    int data;
                    memcpy(&data, buffer, sizeof(int));
                    
                    if(data == 0)
                    {
                        
                    }
                    else
                    {
                        str = str + string(buffer);
                    }

                }
            }
        }
    }

    return 0;
}