/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 10:06:31 by aachfenn          #+#    #+#             */
/*   Updated: 2024/01/06 12:15:50 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"

extern std::map<int, httpRequest> fdMapRead;
extern std::map<int, httpResponse> fdMapWrite;
extern std::map<int, Server> servers_sockets;
extern fd_set theFdSetRead[NBOFCLIENTS];
extern fd_set theFdSetWrite[NBOFCLIENTS];

httpRequest& httpRequest::operator=(const httpRequest& obj)
{
	socket = obj.socket;
	content_length = obj.content_length;
	server_socket = obj.server_socket;
	request = obj.request;
	method = obj.method;
	uri = obj.uri;
	http_version = obj.http_version;
	hostname = obj.hostname;
	port = obj.port;
	connection = obj.connection;
	status = obj.status;
	return *this;
}

void	httpRequest::checks_() {

	size_t start = request.find("\r\n\r\n");
	start += 4;
	body_size = request.size() - start;
	// cout << "body size is : " << body_size << " cf bs is : " << servers_sockets[this->server_socket].client_body_size << endl;
	// cout << "URI L ::: " << uri.length() << endl;
	if (method != "GET" && method != "POST" && method != "DELETE")
		throw (std::runtime_error("error 9"));
		
	if (body_size > servers_sockets[this->server_socket].client_body_size) {
		status = 413;
		throw (std::runtime_error("error 10")); 
	}
	if (uri.length() > 2048) {
		status = 414;
		throw (std::runtime_error("error 11"));
	}
}

void	httpRequest::extract_form_data() {
	
	size_t start = request.find("\r\n\r\n");
	if (start == string::npos)
		exit (1);
	start += 4;
	string data = request.substr(start, request.length());
	size_t pos = 0;
	size_t pos_1;
	size_t pos_2;
	for (;;) {
		pos_1 = data.find("=", pos);
		if (pos_1 == string::npos)
			break;
		pos_2 = data.find("&", pos_1);
		if (pos_2 == string::npos)
			pos_2 = data.length();
		// cout << "pos : " << pos << endl;
		// cout << "pos_1 : " << pos_1 << endl;
		// cout << "pos_2 : " << pos_2 << endl<< endl;
		// cout << "|" << data.substr(pos, pos_1 - pos) <<"|"<< endl;
		// cout << "|" << data.substr(pos_1 + 1, pos_2 - pos_1 - 1) << "|" << endl;
		form_data[data.substr(pos, pos_1 - pos)] = data.substr(pos_1 + 1, pos_2 - pos_1 - 1);
		pos = pos_2 + 1;
		if (pos >= data.length())
			break ;
	}
	
	// cout << "this is the body : (" << data << ")" << endl;
	if (form_data.size() > 0) {
		cout << "DATA passed "<< method << " : (" ;
		for (std::map<string,string>::iterator it = form_data.begin();it != form_data.end();it++) {
			cout << "'" << it->first << "'" << "===" << "'" << it->second << "'" << endl;
		}
	}
}

void	httpRequest::parce_request() {

	string first_line = request.substr(0, request.find("\n"));
	{
		size_t pos_1 = request.find(" ");
		if (pos_1 == string::npos)
			exit (1);
		size_t pos_2 = request.find(" ", pos_1 + 1);
		if (pos_2 == string::npos)
			exit (1);
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
	extract_form_data();
	extract_uri_data();
	checks_();
	size_t sp_pos = uri.find("%20");
	if (sp_pos != string::npos) {
		uri = uri.substr(0, sp_pos) + " " + uri.substr(sp_pos + 3, uri.length());
	}
}

void	httpRequest::extract_uri_data() {
	
	if (uri.find("?") != string::npos) {
		size_t start = uri.find("?");
		if (start == string::npos)
			exit (1);
		start += 1;
		string data = uri.substr(start, uri.length());
		size_t pos = 0;
		size_t pos_1;
		size_t pos_2;
		for (;;) {
			pos_1 = data.find("=", pos);
			if (pos_1 == string::npos)
				break;
			pos_2 = data.find("&", pos_1);
			if (pos_2 == string::npos)
				pos_2 = data.length();
			form_data[data.substr(pos, pos_1 - pos)] = data.substr(pos_1 + 1, pos_2 - pos_1 - 1);
			pos = pos_2 + 1;
			if (pos >= data.length())
				break ;
		}
		
		uri = uri.substr(0, uri.find("?"));
		cout << method <<" DATA : ";
		for (std::map<string,string>::iterator it = form_data.begin();it != form_data.end();it++) {
			cout << "'" << it->first << "'" << "===" << "'" << it->second << "'" << endl;
		}
	}
}

void	httpRequest::generate_response() {
	// cout << request << endl;

	try {
		parce_request();
	}
	catch (std::exception &e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Errorrrrrrrrrrrrrrrr" << endl;
		exit (10);
	}
	// cout << first_line << endl;
	// cout << "method is >> |" << method  << "|" << endl;
	// cout << "uri is >> |" << uri  << "|" << endl;
	// cout << "http_version is >> |" << http_version  << "|" << endl;
	// cout << "hostname is >> |" << hostname  << "|" << endl;
	// cout << "port is >> |" << port  << "|" << endl;
	// cout << "connection is >> |" << connection  << "|" << endl;
	// cout << "***********************************\n";
}
