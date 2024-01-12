/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parceConfFile.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:27:37 by aachfenn          #+#    #+#             */
/*   Updated: 2024/01/12 15:36:11 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parceConfFile.hpp"

Server::Server() : client_body_size(-1) {}
Server::~Server() {}
parceConfFile::parceConfFile() : server_nb(0) {}

parceConfFile::~parceConfFile() {}

void parceConfFile::read_conf_file() {

	std::ifstream file("./default.conf");
	if (!file) {
		std::cout << "error : open file";
		return ;
	}
	std::string input;
	for(int i = 0; std::getline(file, input); i++)
		data.push_back(input);
	file.close();			
}

string parceConfFile::my_trim(string str) {

	int first = 0;
	int last = str.length();
	
	while (str[first] == 32 || str[first] == '\t')
		first++;
	while (str[last - 1] == 32 || str[last - 1] == '\t')
		last--;
	return (str.substr(first, last));
}

void	parceConfFile::my_location(Server &serv, string location_name) {

	string token;
	Location local;
	local.name = location_name;
	it_data++;
	for (;it_data != data.end(); it_data++) {

		std::istringstream str(*it_data);
		str >> token;
		if (my_trim(*it_data).empty())
			continue;
		if (token == "}")
			break ;
		if (token == "path" && (str >> token)) {
			local.path = token;
			if (!(str >> token) || (token != ";"))
				throw(std::runtime_error("Syntax Error ';'0"));
			if (str >> token)
				throw(std::runtime_error("Syntax Error 100"));
		}
		else if (token == "default_file" && (str >> token)) {
			local.default_file = token;
			if (!(str >> token) || (token != ";"))
				throw(std::runtime_error("Syntax Error ';'1"));
			if (str >> token)
				throw(std::runtime_error("Syntax Error 100"));
		}
		else if (token == "methods") {
			while (str >> token) {
				if (token == ";")
					break;
				local.methods.push_back(token);
			}
			if (token != ";")
				throw(std::runtime_error("Syntax Error ';'2"));
			if (str >> token)
				throw(std::runtime_error("Syntax Error 100"));
		}
		else if (token == "cgi_extension" && (str >> token)) {
			local.cgi_extension = token;
			if (!(str >> token) || (token != ";"))
				throw(std::runtime_error("Syntax Error ';'4"));
			if (str >> token)
				throw(std::runtime_error("Syntax Error 100"));
		}
		else
			throw(std::runtime_error("Syntax Error brackets"));
	}
	serv.location.push_back(local);
}

void parceConfFile::fill_data() {

	vector<string> data = this->data;
	it_data = data.begin();
	for (;it_data < data.end(); it_data++) {

		std::istringstream str(*it_data);
		string token;
		str >> token;
		Server serv;
		if ((token == "server")) {
			if (!(str >> token) || (token != "{"))
				throw(std::runtime_error("Syntax Error in {}"));
			if (str >> token)
				throw(std::runtime_error("Syntax Error 100"));
			serv.location_nb = 0;
			server_nb++;
			it_data++;
			for (;it_data < data.end(); it_data++) {
				if (*it_data == "}")
					break ;
				std::istringstream str(*it_data);
				str >> token;
				if (my_trim(*it_data).empty())
					continue;
				if (token == "listen") {
					
					while (str >> token) {
						if (token == ";")
							break;
						serv.listen.push_back(token);
					}
					if (token != ";")
						throw(std::runtime_error("Syntax Error ';'1"));
					if (str >> token)
						throw(std::runtime_error("Syntax Error 100"));
				}
				else if (token == "host" && (str >> token)) {
					serv.host = token;
					if (!(str >> token) || (token != ";"))
						throw(std::runtime_error("Syntax Error ';'2"));
					if (str >> token)
						throw(std::runtime_error("Syntax Error 100"));
				}
				else if (token == "directory_listing" && (str >> token)) {
					if (token == "on" || token == "ON")
						serv.directory_listing = true;
					else if (token == "off" || token == "OFF")
						serv.directory_listing = false;
					else
						throw(std::runtime_error("directory listing error"));

					if (!(str >> token) || (token != ";"))
						throw(std::runtime_error("Syntax Error ';'2"));
					if (str >> token)
						throw(std::runtime_error("Syntax Error 100"));
				}
				else if (token == "error_pages") {

					while (str >> token) {
						if (token == ";")
							break;
						serv.error_pages.push_back(token);
					}
					if (token != ";")
						throw(std::runtime_error("Syntax Error ';'3"));
					if (str >> token)
						throw(std::runtime_error("Syntax Error 100"));
				}
				else if (token == "client_body_size" && (str >> token)) {
					char *end;
					serv.client_body_size = std::strtod(token.c_str(), &end);
					if (!(str >> token) || (token != ";"))
						throw(std::runtime_error("Syntax Error ';'4"));
					if (str >> token)
						throw(std::runtime_error("Syntax Error 100"));
				}
				else if (token == "root" && (str >> token)) {
					serv.root = token;
					if (!(str >> token) || (token != ";"))
						throw(std::runtime_error("Syntax Error ';'2"));
					if (str >> token)
						throw(std::runtime_error("Syntax Error 100"));
				}
				else if ((token == "location") && (str >> token)) {
					string location_name = token;
					if (!(str >> token) || (token != "{"))
						throw(std::runtime_error("Syntax Error in {}"));
					if (str >> token)
						throw(std::runtime_error("Syntax Error 100"));
					my_location(serv, location_name);
				}
				else
					throw(std::runtime_error("Syntax Error brackets"));
			}
			serv.location_nb = serv.location.size();
			server.push_back(serv);
		}
	}
	check_ifdata_isnot_empty();
	check_ifdata_is_valid();
	// print_data();
}

void parceConfFile::print_data() {

	for (int i = 0;i < server_nb; i++) {
		
		cout << "listen : ";
		for (size_t k = 0;k < server[i].listen.size() ;k++)
			 cout << server[i].listen[k] << ", ";
		cout << endl;
		cout << "host : " << server[i].host << endl;
		cout << "client_body_size : " << server[i].client_body_size << endl;
		cout << "error_pages : ";
		vector<string>::iterator it = server[i].error_pages.begin();
		for (;it != server[i].error_pages.end();it++)
			cout << *it << ", ";
		cout << endl;
		cout << "directory_listing : " << server[i].directory_listing << endl;
		cout << endl;
		for (int j = 0; (size_t)j < server[i].location.size();j++) {
			cout << "Location : " << server[i].location[j].name << endl;
			cout << "\tpath : " << server[i].location[j].path  << endl;
			cout << "\tdefault_file : " << server[i].location[j].default_file  << endl;
			cout << "\tmethods : ";
			vector<string>::iterator it = server[i].location[j].methods.begin();
			for (;it != server[i].location[j].methods.end();it++)
				cout << *it << ", ";
			cout << endl;
			cout << "\tcgi_extension : " << server[i].location[j].cgi_extension  << endl;
		}
	}
	cout << "nb of servers : " << server_nb << endl;
}

void parceConfFile::check_ifdata_isnot_empty() {

	if (server_nb == 0)
		throw(std::runtime_error("No Server Available"));
	for (int i = 0;i < server_nb; i++) {
	
		if (server[i].listen.size() == 0 || server[i].host.empty() || server[i].error_pages.size() == 0 || 
			server[i].client_body_size == -1 || server[i].location.size() == 0 || server[i].root.empty())
				throw(std::runtime_error("Syntax Error in ONE of the attributes"));
		for (int j = 0; (size_t)j < server[i].location.size();j++) {
			if (server[i].location[j].path.empty() || 
			server[i].location[j].methods.size() == 0 || 
			server[i].location[j].cgi_extension.empty())
				throw(std::runtime_error("Syntax Error in ONE of the location attributes"));				
		}
	}
}

void parceConfFile::check_ifdata_is_valid() {

	char *tmp;
	for (int i = 0;i < server_nb; i++) {
	
		for (size_t k = 0; k < server[i].listen.size();k++) {
			double nb = std::strtod((server[i].listen[k]).c_str(), &tmp);
			if (!string(tmp).empty() || nb < 0)
				throw(std::runtime_error("Syntax Error in listen Port"));
		}
		for (size_t j = 0; j < server[i].location.size();j++) {
			
			int check = 0;
			for (size_t l = 0;l < server[i].location[j].methods.size();l++) {
				if (server[i].location[j].methods[l] == "GET" || server[i].location[j].methods[l] == "POST" || 
				server[i].location[j].methods[l] == "DELETE")
					check++;
			}
			if (check != 3)
				throw(std::runtime_error("Syntax Error in the location METHODS"));
		}
		for (size_t j = 0; j < server[i].location.size(); j++) {

			for (size_t l = j + 1; l < server[i].location.size(); l++) {

				if (server[i].location[j].name == server[i].location[l].name) {
					throw(std::runtime_error("Duplicate Location"));
				}
			}
		}
	}
	 
}

//copy constructor
Server::Server(const Server& obj)
{
	listen = obj.listen;
	host = obj.host;
	root = obj.root;
	error_pages = obj.error_pages;
	client_body_size = obj.client_body_size;
	location = obj.location;
	location_nb = obj.location_nb;
	directory_listing = obj.directory_listing;
}
//copy assignment operator
Server& Server::operator=(const Server& obj)
{
	listen = obj.listen;
	host = obj.host;
	root = obj.root;
	error_pages = obj.error_pages;
	client_body_size = obj.client_body_size;
	location = obj.location;
	location_nb = obj.location_nb;
	directory_listing = obj.directory_listing;
	return *this;
}