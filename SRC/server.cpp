
#include "../INC/server.hpp"

// int main()
// {
//     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if(sockfd == -1)
//     {
//         cerr << "Failed to Create Socket"<< endl;
//         return 1;
//     }
//     struct sockaddr_in address;
//     socklen_t socket_lenght  = sizeof(address);
//     bzero(&address, sizeof(struct sockaddr_in));
//     address.sin_family = AF_INET;
//     address.sin_port = htons(PORT);
//     address.sin_addr.s_addr = INADDR_ANY;

//     if(bind(sockfd, (struct sockaddr*)&address, sizeof(address)) == -1)
//     {
//         cerr << "Failed to Bind"<< endl;
//         return 1;
//     }

//     //listen to incomming connections
//     if(listen(sockfd, 10) == -1)
//     {
//         cerr << "Failed to listen"<< endl;
//         return 1;        
//     }

//     while (1)
//     {
//         cout << "waiting for new connections"<< endl;
//         int new_socket;
//         if((new_socket = accept(sockfd, (struct sockaddr* )&address, (socklen_t* )&socket_lenght))==-1)
//         {
//             cerr << "failed to accept a new connection"<< endl;
//             return 1;
//         }
//         char buffer[1024] = {0};
//         int nb_readed = read(new_socket, buffer, 1024);
//         // the request
//         // write()
//     }
    



// }