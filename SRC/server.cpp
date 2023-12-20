/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 09:10:43 by aachfenn          #+#    #+#             */
/*   Updated: 2023/12/20 13:22:22 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"
#include <fcntl.h>

int main() {
	parceConfFile cf;
	parce_conf_file(cf);

	int j = 0;
	int listen_size = cf.server[j].listen.size();
	int listenSocket[listen_size];
	int ports[listen_size];
	for (int i = 0;i < listen_size;i++) {
		ports[i] = std::atoi((cf.server[j].listen[i]).c_str());
	}
	for (int i = 0;i < listen_size;i++) {
		
		listenSocket[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (listenSocket[i] < 0) {
			cout << "Could not create socket";
			return 1;
		}
	
		sockaddr_in serverAddress;
		std::memset(&serverAddress, 0, sizeof(serverAddress));
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = INADDR_ANY;
		serverAddress.sin_port = htons(ports[i]);

		int optval = 1;
		if (setsockopt(listenSocket[i], SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		    std::cerr << "setsockopt failed";
		    return 1;
		}
		
		if (bind(listenSocket[i], (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
			cout << "Bind failed";
			return 1;
		}
	
		if (listen(listenSocket[i], 3) < 0) {
			cout << "Listen failed";
			return 1;
		}
	}

	struct pollfd fds[listen_size];
	for (int l = 0;l <listen_size;l++) {

		fds[l].fd = listenSocket[l];
		fds[l].events = POLLIN;
	}

	sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);
	int clientSocket;

	for (;;) {
		int ret = poll(fds, listen_size, -1);

		if (ret > 0) {
			
			for (int i = 0; i < listen_size;i++) {
				if (fds[i].revents & POLLIN) {
					clientSocket = accept(listenSocket[i], (struct sockaddr*)&clientAddress, &clientAddressLength);
					if (clientSocket < 0) {
						cout << "Accept failed";
						return 1;
					}
	
					char buffer[1024];
					char buf[] = "HTTP/1.1 200 OK\r\n"
					             "Content-Type: text/html\r\n"
					             "Connection: close\r\n"
					             "\r\n"
					             "<!DOCTYPE html>"
					             "<html>"
					             "<head>"
					             "    <title>My Title</title>"
					             "</head>"
					             "<body>"
					             "    <h1>Hello, World!</h1>"
					             "</body>"
					             "</html>"
								 "\r\n\r\n";
					ssize_t recvSize;
					while ((recvSize = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
						cout << recvSize << endl;
						buffer[recvSize] = '\0';
						std::cout << buffer << std::endl;
						send(clientSocket, buf, strlen(buf), 0);
						close(clientSocket);
						// close(listenSocket[i]);
					}
				}
			}
		} else if (ret < 0) {
			cout << "Poll failed";
			return 1;
		}
	}

	for (int l = 0;l <listen_size;l++) {
		close(listenSocket[l]);
	}

	return 0;
}