/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 09:10:43 by aachfenn          #+#    #+#             */
/*   Updated: 2023/12/20 09:10:46 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"

int main() {
	parce_conf_file();
	
	int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket < 0) {
		cout << "Could not create socket";
		return 1;
	}

	sockaddr_in serverAddress;
	std::memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT);

	if (bind(listenSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		cout << "Bind failed";
		return 1;
	}

	if (listen(listenSocket, 3) < 0) {
		cout << "Listen failed";
		return 1;
	}

	struct pollfd fds[1];
	fds[0].fd = listenSocket;
	fds[0].events = POLLIN;

	sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);
	int clientSocket;

	for (;;) {
		int ret = poll(fds, 1, -1); // -1 means wait indefinitely

		if (ret > 0) {
			if (fds[0].revents & POLLIN) {
				clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
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
							 "\r\n\r\n";				ssize_t recvSize;
				while ((recvSize = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
					buffer[recvSize] = '\0';
					std::cout << buffer << std::endl;
					send(clientSocket, buf, strlen(buf), 0);
				}
				close(clientSocket);
			}
		} else if (ret < 0) {
			cout << "Poll failed";
			return 1;
		}
	}

	close(listenSocket);

	return 0;
}