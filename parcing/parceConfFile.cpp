/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parceConfFile.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:27:37 by aachfenn          #+#    #+#             */
/*   Updated: 2023/12/15 20:53:21 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parceConfFile.hpp"


parceConfFile::parceConfFile() : client_body_size(0),location_nb(0),server_nb(0) {}

parceConfFile::~parceConfFile() {}

void parceConfFile::read_conf_file() {

	std::ifstream file("./default.conf");
    if (!file) {
        std::cout << "error : open file";
        return ;
    }
    std::string input;
    for(int i = 0; std::getline(file, input); i++)
    {
        data.push_back(input);
        // std::cout << data[i] << std::endl;
    }
    file.close();			
}

void	parceConfFile::my_location(parceConfFile __unused &serv) {

	location_nb++;
	string token;
	Location local;
	it_data++;
	// bool check = false;
	for (;it_data != data.end(); it_data++) {

		std::istringstream str(*it_data);
		str >> token;
		if (token == "}") {
			// check = true;
			break ;
		}
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
	}
	// if (!check)
	// 	throw(std::runtime_error("Syntax Error ';'5"));
	serv.location.push_back(local);
}

void parceConfFile::fill_data() {
	vector<string> data = this->data;
	it_data = data.begin();
	
	for (;it_data < data.end(); it_data++) {
		std::istringstream str(*it_data);
		string token;
		str >> token;
		parceConfFile serv;
		if ((token == "server") && (str >> token) && (token == "{")) {
			serv.location_nb = 0;
			server_nb++;
			it_data++;
			for (;it_data < data.end(); it_data++) {
				if (*it_data == "}")
					break ;
				std::istringstream str(*it_data);
				str >> token;
				if (token == "listen" && (str >> token)) {
					serv.listen = token;
					if (!(str >> token) || (token.empty() || token != ";"))
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
					client_body_size = std::strtod(token.c_str(), &end);
					if (!(str >> token) || (token.empty() || token != ";"))
						throw(std::runtime_error("Syntax Error ';'4"));
				}
				else if (token == "root" && (str >> token)) {
					serv.root = token;
					if (!(str >> token) || (token.empty() || token != ";"))
						throw(std::runtime_error("Syntax Error ';'5"));
				}
				else if (token == "location" && (str >> token) && (token == "{")) {
					my_location(serv);
				}
			}
			server.push_back(serv);
		}
	}
	print_data();
}

void parceConfFile::print_data() {

	for (int i = 0;i < server_nb; i++) {
	
		cout << "listen : " << server[i].listen << endl;
		cout << "server_name : " << server[i].server_name << endl;
		cout << "client_body_size : " << server[i].client_body_size << endl;
		cout << "root : " << server[i].root << endl;
		cout << "error_pages : ";
		vector<string>::iterator it = server[i].error_pages.begin();
		for (;it != server[i].error_pages.end();it++)
			cout << *it << ", ";
		cout << endl;
		cout << "----------------------------------" << endl;
		for (int j = 0; (size_t)j < server[i].location.size();j++) {
			cout << "Location :\n";
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
	}
	cout << "nb of locations : " << location_nb << endl;
	cout << "nb of servers : " << server_nb << endl;
}