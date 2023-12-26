/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 09:10:43 by aachfenn          #+#    #+#             */
/*   Updated: 2023/12/26 09:25:21 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../INC/server.hpp"
// #include <fcntl.h>

// int main() {
// 	parceConfFile cf;
// 	parce_conf_file(cf);

// 	int j = 0;
// 	int listen_size = cf.server[j].listen.size();
// 	int listenSocket[listen_size];
// 	int ports[listen_size];
// 	for (int i = 0;i < listen_size;i++) {
// 		ports[i] = std::atoi((cf.server[j].listen[i]).c_str());
// 	}
// 	for (int i = 0;i < listen_size;i++) {
		
// 		listenSocket[i] = socket(AF_INET, SOCK_STREAM, 0);
// 		if (listenSocket[i] < 0) {
// 			cout << "Could not create socket";
// 			return 1;
// 		}
	
// 		sockaddr_in serverAddress;
// 		std::memset(&serverAddress, 0, sizeof(serverAddress));
// 		serverAddress.sin_family = AF_INET;
// 		serverAddress.sin_addr.s_addr = INADDR_ANY;
// 		serverAddress.sin_port = htons(ports[i]);

// 		int optval = 1;
// 		if (setsockopt(listenSocket[i], SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
// 		    std::cerr << "setsockopt failed";
// 		    return 1;
// 		}
		
// 		if (bind(listenSocket[i], (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
// 			cout << "Bind failed";
// 			return 1;
// 		}
	
// 		if (listen(listenSocket[i], 3) < 0) {
// 			cout << "Listen failed";
// 			return 1;
// 		}
// 	}

// 	// struct pollfd fds[listen_size];
// 	std::vector<pollfd> fds;
// 	for (int l = 0;l <listen_size;l++) {

// 		pollfd pfd = { listenSocket[l], POLLIN, 0 };
// 		fds.push_back(pfd);
// 		// fds[l].fd = listenSocket[l];
// 		// fds[l].events = POLLIN;
// 	}

// 	sockaddr_in clientAddress;
// 	socklen_t clientAddressLength = sizeof(clientAddress);
// 	int clientSocket;

// 	cout << "--------waiting for connections--------" << endl;
// 	for (;;) {
// 		// pollfd pfd = { listenSocket[listen_size], POLLIN, 0 };
// 		// fds.push_back(pfd);
// 		int ret = poll(fds.data(), listen_size, -1);
// 		cout << "poll triggered"<< endl;
// 		if (ret > 0) {
			
// 			for (size_t i = 0; i < fds.size();i++) {
// 				if (fds[i].revents & POLLIN) {
// 					clientSocket = accept(listenSocket[i], (struct sockaddr*)&clientAddress, &clientAddressLength);
// 					if (clientSocket < 0) {
// 						cout << "Accept failed";
// 						return 1;
// 					}
// 					pollfd pfd = { clientSocket, POLLIN, 0 };
// 					fds.push_back(pfd);
// 					cout << fds.size() << endl;
// 					// int flags = fcntl(clientSocket, F_GETFL, 0);
// 					// fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);
// 					char buffer[50];
// 					// string tmp = buffer;
// 					// string contain;
// 					// // if (tmp.length() < 4)
// 					// // 	return 1;
// 					// cout << tmp.substr(tmp.length() - 4, 4) << endl;
// 					// if (tmp.substr(tmp.length() - 4, 4) == "\r\n\r\n") {
						
// 						// char buf[] = "HTTP/1.1 200 OK\r\nd""Content-Type: text/html\r\n""Connection: close\r\n""\r\n""<!DOCTYPE html>""<html>""<head>""    <title>My Title</title>""</head>""<body>""    <h1>Hello, World!</h1>""    <h1>Hello, World!</h1>""    <h1>Hello, World!</h1>""    <h1>Hello, World!</h1>""</body>""</html>""\r\n\r\n";
// 							//
// 							//
// 						ssize_t recvSize;
// 						recvSize = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
// 						buffer[recvSize] = '\0';
// 						cout << "-----------\n";
// 						std::cout << buffer << std::endl;
// 						cout << "-----------\n";
// 						// send(clientSocket, buf, strlen(buf), 0);
// 						close(clientSocket);
// 					// }
// 					// else
// 					// 	contain += string(buffer);
// 					break;
// 				}
// 			}
// 		} else if (ret < 0) {
// 			cout << "Poll failed";
// 			return 1;
// 		}
// 	}

// 	for (int l = 0;l <listen_size;l++) {
// 		close(listenSocket[l]);
// 	}

// 	return 0;
// }