/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 10:06:31 by aachfenn          #+#    #+#             */
/*   Updated: 2024/01/13 14:21:34 by aachfenn         ###   ########.fr       */
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
	location  = obj.location;
	simple_uri = obj.simple_uri;
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
	filename = obj.filename;
	return *this;
}

void	httpRequest::checks_() {

	size_t start = request.find("\r\n\r\n");
	start += 4;
	body_size = request.size() - start;
	if (method != "GET" && method != "POST" && method != "DELETE")
		throw (std::runtime_error("error 9"));
		
	if (body_size > servers_sockets[this->server_socket].client_body_size) {
		status = 413;
		filename = "./413.html";
	}
	if (uri.length() > 2048) {
		status = 414;
		filename = "./413.html";
	}
}

void	httpRequest::extract_form_data() {
	
	size_t start = request.find("\r\n\r\n");
	if (start == string::npos)
		throw (std::runtime_error("find failed"));
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
		form_data[data.substr(pos, pos_1 - pos)] = data.substr(pos_1 + 1, pos_2 - pos_1 - 1);
		pos = pos_2 + 1;
		if (pos >= data.length())
			break ;
	}
	
	// cout << "this is the body : (" << data << ")" << endl;
	// if (form_data.size() > 0) {
	// 	cout << "DATA passed "<< method << " : (" ;
	// 	for (std::map<string,string>::iterator it = form_data.begin();it != form_data.end();it++) {
	// 		cout << "'" << it->first << "'" << "===" << "'" << it->second << "'" << endl;
	// 	}
	// }
}

void	httpRequest::parce_request() {

	string first_line = request.substr(0, request.find("\n"));
	{
		size_t pos_1 = request.find(" ");
		if (pos_1 == string::npos)
			throw (std::runtime_error("find failed"));
		size_t pos_2 = request.find(" ", pos_1 + 1);
		if (pos_2 == string::npos)
			throw (std::runtime_error("find failed"));
		method = request.substr(0, pos_1);
		uri = request.substr(pos_1 + 1, pos_2 - pos_1 - 1);
		http_version = request.substr(pos_2 + 1, request.find("\n") - pos_2 - 2);
	}
	{
		size_t pos_1 = request.find("Host");
		if (pos_1 == string::npos)
			throw (std::runtime_error("find failed"));
		pos_1 += 6;
		size_t pos_2 = request.find(":", pos_1);
		if (pos_2 == string::npos)
			throw (std::runtime_error("find failed"));
		hostname = request.substr(pos_1, pos_2 - pos_1);
		port = request.substr(pos_2 + 1, request.find("\r", pos_2) - pos_2 - 1);
	}
	{
		size_t pos_1 = request.find("Connection");
		if (pos_1 == string::npos)
			throw (std::runtime_error("find failed"));
		pos_1 += 12;
		size_t pos_2 = request.find("\r", pos_1);
		if (pos_2 == string::npos)
			throw (std::runtime_error("find failed"));

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
	// extract location from uri
	size_t pos = uri.find("/", 1);
	if (pos != string::npos) {
		location = uri.substr(1, pos - 1);
		simple_uri = uri.substr(pos, uri.length());
	}
}

void	httpRequest::extract_uri_data() {
	
	if (uri.find("?") != string::npos) {
		size_t start = uri.find("?");
		if (start == string::npos)
			throw (std::runtime_error("find failed"));
		uri = uri.substr(0, start);
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
		
		// cout << method <<" DATA : ";
		// for (std::map<string,string>::iterator it = form_data.begin();it != form_data.end();it++) {
		// 	cout << "'" << it->first << "'" << "===" << "'" << it->second << "'" << endl;
		// }
	}
}

void httpRequest::upload_files()
{
	if (method == "POST")
	{
		size_t start = request.find("\r\n\r\n");
		// cout << request.substr(0, start + 100)<< endl;
		if (start == std::string::npos)
			return;
		string sup;
		std::string body;
		start += 4;
		body = request.substr(start, request.size());
		size_t end = body.find("\r\n");
		if (end == std::string::npos)
			return;
		sup = body.substr(0, end);
		body = body.substr(end + 1, body.size());
		size_t t = body.find(sup);
		if (t == std::string::npos)
			return;
		body = body.substr(0, t);

		string name;
		string filename;
		string Content_Type;
		std::istringstream iss(body);
		string line1;
		string line2;
		bool upload = false;
		while (std::getline(iss, line1))
		{
			std::istringstream iss2(line1);
			string tmp;
			if (iss2 >> tmp && tmp == "Content-Disposition:" && iss2 >> tmp && tmp == "form-data;")
			{
				std::getline(iss, line2);
				upload = true;
				break;
			}
		}
		if (upload)
		{
			std::istringstream iss3(line2);
			string tmp;
			if (!(iss3 >> tmp && tmp == "Content-Type:"))
				return;
			iss3 >> Content_Type;
			std::istringstream iss4(line1);
			vector<string> vec;
			while (std::getline(iss4, tmp, ';'))
			{
				vec.push_back(tmp);
			}
			if (vec[1].substr(0, vec[1].find('=')) == " name")
			{
				name = vec[1].substr(vec[1].find('=') + 2, vec[1].size() - vec[1].find('"') - 2);
			}
			if (vec[2].substr(0, vec[2].find('=')) == " filename")
				filename = vec[2].substr(vec[2].find('=') + 2, vec[2].size() - vec[2].find('"') - 3);
			if (filename.empty())
				return;
			std::ofstream file;
			file.open("./upload/" + filename);
			start = 0;
			for (int i = 0; i < 4; ++i)
			{
				while (body[start] != '\n' && start < body.size())
					start += 1;
				start++;
			}
			string data = body.substr(start, body.size());
			file << data;
			file.close();
		}
	}
}

void httpRequest::delete_files()
{
	if (method == "DELETE")
	{
		string tmp = uri.substr(1, uri.length());
		if (remove(tmp.c_str()) == -1)
			throw (std::runtime_error("not found ola kra"));
		
		cout << "file deleted (" << tmp << ")\n";
	}
}

void	httpRequest::generate_response() {
	// cout << request << endl;

	try {
		parce_request();
		upload_files();//TODO: RETURN TO THROW
		delete_files();
	}
	catch (std::exception &e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Errorrrrrrrrrrrrrrrr" << endl;
		// just to see if something unexpected happens :: should be removed 
		exit (10);
	}
	// cout << "uri is >> |" << uri  << "|" << endl;

	// cout << first_line << endl;
	// cout << "method is >> |" << method  << "|" << endl;
	// cout << "http_version is >> |" << http_version  << "|" << endl;
	// cout << "hostname is >> |" << hostname  << "|" << endl;
	// cout << "port is >> |" << port  << "|" << endl;
	// cout << "connection is >> |" << connection  << "|" << endl;
}
