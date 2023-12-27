/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eboulhou <eboulhou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 10:06:31 by aachfenn          #+#    #+#             */
/*   Updated: 2023/12/27 15:53:37 by eboulhou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"

extern std::map<int, httpRequest> fdMapRead;
extern std::map<int, httpResponse> fdMapWrite;
extern fd_set theFdSetRead[NBOFCLIENTS];
extern fd_set theFdSetWrite[NBOFCLIENTS];


httpRequest& httpRequest::operator=(const httpRequest& obj)
{
	socket = obj.socket;
	server_socket = obj.server_socket;
	request = obj.request;
	method = obj.method;
	uri = obj.uri;
	http_version = obj.http_version;
	hostname = obj.hostname;
	port = obj.port;
	connection = obj.connection;
	return *this;
}

void	httpRequest::generate_response() {
	// cout << request << endl;

	string first_line = request.substr(0, request.find("\n"));
	{
		int pos_1 = request.find(" ");
		int pos_2 = request.find(" ", pos_1 + 1);
		method = request.substr(0, pos_1);
		uri = request.substr(pos_1 + 1, pos_2 - pos_1 - 1);
		http_version = request.substr(pos_2 + 1, request.find("\n") - pos_2 - 2);
	}
	{
		size_t pos_1 = request.find("Host");
		if (pos_1 == string::npos)
			exit (1);
		pos_1 += 6;
		size_t pos_2 = request.find(":", pos_1);
		if (pos_2 == string::npos)
			exit (1);
		hostname = request.substr(pos_1, pos_2 - pos_1);
		port = request.substr(pos_2 + 1, request.find("\r", pos_2) - pos_2 - 1);
	}
	{
		size_t pos_1 = request.find("Connection");
		if (pos_1 == string::npos)
			exit (1);
		pos_1 += 12;
		size_t pos_2 = request.find("\r", pos_1);
		if (pos_2 == string::npos)
			exit (1);

		if (request.substr(pos_1, pos_2 - pos_1) == "keep-alive")
			connection = true;
	}
	
	// cout << first_line << endl;
	
	cout << "method is >> |" << method  << "|" << endl;
	cout << "uri is >> |" << uri  << "|" << endl;
	cout << "http_version is >> |" << http_version  << "|" << endl;
	cout << "hostname is >> |" << hostname  << "|" << endl;
	cout << "port is >> |" << port  << "|" << endl;
	cout << "connection is >> |" << connection  << "|" << endl;
	cout << "***********************************\n";
}



int  readTheRequest(std::map<int, httpRequest>::iterator& it)
{
	int commSocket;
	char buffer[BUFFER_SIZE];
	int size_readed;

	commSocket = it->first;
	bzero(buffer, BUFFER_SIZE);
	size_readed = recv(commSocket, buffer, BUFFER_SIZE, 0);
	if(size_readed == -1)
	{
		cerr << "error at reading from socket"<< endl;
		exit(1);
	}
	else if(size_readed == 0)
	{
		cout << "connection ended"<< endl;
		close(commSocket);
		fdMapRead.erase(commSocket);
		return 0;
	}
	else 
	{
		// cout << "-------------------" << endl;
		it->second.request = it->second.request + string(buffer);
		if(it->second.request.size() > 4  && it->second.request.substr(it->second.request.size() - 4) == "\r\n\r\n")
		{
			cout << "full request received!!!"<<endl;
			it->second.generate_response();
			fdMapWrite.insert(std::make_pair(commSocket, httpResponse(it->second, "")));
			fdMapRead.erase(commSocket);
			return 0;
		}
	}
	cout << "impossible to reach"<< endl;
	return 0;
}