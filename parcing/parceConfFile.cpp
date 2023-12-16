/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parceConfFile.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:27:37 by aachfenn          #+#    #+#             */
/*   Updated: 2023/12/16 15:23:55 by aachfenn         ###   ########.fr       */
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

void	parceConfFile::my_location(Server &serv) {

	string token;
	Location local;
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
			if (!(str >> token) || (token.empty() || token != ";"))
				throw(std::runtime_error("Syntax Error ';'0"));
		}
		else if (token == "default_file" && (str >> token)) {
			local.default_file = token;
			if (!(str >> token) || (token.empty() || token != ";"))
				throw(std::runtime_error("Syntax Error ';'1"));
		}
		else if (token == "methods") {
			while (str >> token) {
				if (token.empty() || token == ";")
					break;
				local.methods.push_back(token);
			}
			if (token != ";")
				throw(std::runtime_error("Syntax Error ';'2"));
		}
		else if (token == "cgi_bin" && (str >> token)) {
			local.cgi_bin = token;
			if (!(str >> token) || (token.empty() || token != ";"))
				throw(std::runtime_error("Syntax Error ';'3"));
		}
		else if (token == "cgi_extension" && (str >> token)) {
			local.cgi_extension = token;
			if (!(str >> token) || (token.empty() || token != ";"))
				throw(std::runtime_error("Syntax Error ';'4"));
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
						if (token.empty() || token == ";")
							break;
						serv.listen.push_back(token);
					}
					if (token != ";")
						throw(std::runtime_error("Syntax Error ';'1"));
				}
				else if (token == "server_name" && (str >> token)) {
					serv.server_name = token;
					if (!(str >> token) || (token.empty() || token != ";"))
						throw(std::runtime_error("Syntax Error ';'2"));
				}
				else if (token == "error_pages") {

					while (str >> token) {
						if (token.empty() || token == ";")
							break;
						serv.error_pages.push_back(token);
					}
					if (token != ";")
						throw(std::runtime_error("Syntax Error ';'3"));
				}
				else if (token == "client_body_size" && (str >> token)) {
					char *end;
					serv.client_body_size = std::strtod(token.c_str(), &end);
					if (!(str >> token) || (token.empty() || token != ";"))
						throw(std::runtime_error("Syntax Error ';'4"));
				}
				else if (token == "root" && (str >> token)) {
					serv.root = token;
					if (!(str >> token) || (token.empty() || token != ";"))
						throw(std::runtime_error("Syntax Error ';'5"));
				}
				else if ((token == "location")) {
					if (!(str >> token) || (token != "{"))
						throw(std::runtime_error("Syntax Error in {}"));
					my_location(serv);
				}
				else
					throw(std::runtime_error("Syntax Error brackets"));
			}
			serv.location_nb = serv.location.size();
			server.push_back(serv);
		}
	}
	check_ifdata_isnot_empty();
	print_data();
}

void parceConfFile::print_data() {

	for (int i = 0;i < server_nb; i++) {
		
		cout << "listen : ";
		for (size_t k = 0;k < server[i].listen.size() ;k++)
			 cout << server[i].listen[k] << ", ";
		cout << endl;
		cout << "server_name : " << server[i].server_name << endl;
		cout << "client_body_size : " << server[i].client_body_size << endl;
		cout << "root : " << server[i].root << endl;
		cout << "error_pages : ";
		vector<string>::iterator it = server[i].error_pages.begin();
		for (;it != server[i].error_pages.end();it++)
			cout << *it << ", ";
		cout << endl;
		for (int j = 0; (size_t)j < server[i].location.size();j++) {
			cout << "Location : \n";
			cout << "\tpath : " << server[i].location[j].path  << endl;
			cout << "\tdefault_file : " << server[i].location[j].default_file  << endl;
			cout << "\tmethods : ";
			vector<string>::iterator it = server[i].location[j].methods.begin();
			for (;it != server[i].location[j].methods.end();it++)
				cout << *it << ", ";
			cout << endl;
			cout << "\tcgi_bin : " << server[i].location[j].cgi_bin  << endl;
			cout << "\tcgi_extension : " << server[i].location[j].cgi_extension  << endl;
		}
		cout << "\t<  nb of locations  >>> " << server[i].location_nb << endl;
		cout << "----------------------------------" << endl;
	}
	cout << "nb of servers : " << server_nb << endl;
}

void parceConfFile::check_ifdata_isnot_empty() {

	for (int i = 0;i < server_nb; i++) {
	
		if (server[i].listen.size() == 0 || server[i].server_name.empty() || server[i].error_pages.size() == 0 || 
			server[i].client_body_size == -1 || server[i].root.empty() || server[i].location.size() == 0)
				throw(std::runtime_error("Syntax Error in ONE of the attributes"));
		for (int j = 0; (size_t)j < server[i].location.size();j++) {
			if (server[i].location[j].path.empty() || server[i].location[j].default_file.empty() || 
			server[i].location[j].methods.size() == 0 || server[i].location[j].cgi_bin.empty() || 
			server[i].location[j].cgi_extension.empty())
				throw(std::runtime_error("Syntax Error in ONE of the location attributes"));
				
		}
	}
}