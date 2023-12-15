/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parceConfFile.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:27:37 by aachfenn          #+#    #+#             */
/*   Updated: 2023/12/15 13:36:33 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parceConfFile.hpp"


parceConfFile::parceConfFile() : client_body_size(0) {}

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

void	parceConfFile::my_location() {
	string token;
	Location local;
	location.push_back(local);
	it_data++;
	for (;it_data < data.end(); it_data++) {
		std::istringstream str(*it_data);
		str >> token;
		if (token == "}")
			break ;
		if (token == "path" && (str >> token)) {
			location.back().path = token;
			// cout << token ;
			if (!(str >> token) || (token.empty() || token != ";"))
				throw(std::runtime_error("Syntax Error ';'"));
		}
		else if (token == "default_file" && (str >> token)) {
			location.back().default_file = token;
			// cout << token ;
			if (!(str >> token) || (token.empty() || token != ";"))
				throw(std::runtime_error("Syntax Error ';'"));
		}
		else if (token == "methods") {
			while (str >> token) {
				if (token.empty() || token == ";")
					break;
				location.back().methods.push_back(token);
			}
			if (token != ";")
				throw(std::runtime_error("Syntax Error ';'"));
		}
		else if (token == "cgi_bin" && (str >> token)) {
			location.back().cgi_bin = token;
			if (!(str >> token) || (token.empty() || token != ";"))
				throw(std::runtime_error("Syntax Error ';'"));
		}
		else if (token == "cgi_extension" && (str >> token)) {
			location.back().cgi_extension = token;
			if (!(str >> token) || (token.empty() || token != ";"))
				throw(std::runtime_error("Syntax Error ';'"));
		}
	}
}

void parceConfFile::fill_data() {
	vector<string> data = this->data;
	it_data = data.begin();
	
	for (;it_data < data.end(); it_data++) {
		std::istringstream str(*it_data);
		string token;
		str >> token;
		if ((token == "server") && (str >> token) && (token == "{")) {
			it_data++;
			for (;it_data < data.end(); it_data++) {
				if (*it_data == "}")
					break ;
				std::istringstream str(*it_data);
				str >> token;
				if (token == "listen" && (str >> token)) {
					listen = token;
					// cout << token ;
					if (!(str >> token) || (token.empty() || token != ";"))
						throw(std::runtime_error("Syntax Error ';'"));
				}
				else if (token == "server_name" && (str >> token)) {
					server_name = token;
					if (!(str >> token) || (token.empty() || token != ";"))
						throw(std::runtime_error("Syntax Error ';'"));
				}
				else if (token == "error_pages") {

					while (str >> token) {
						if (token.empty() || token == ";")
							break;
						error_pages.push_back(token);
					}
					if (token != ";")
						throw(std::runtime_error("Syntax Error ';'"));
				}
				else if (token == "client_body_size" && (str >> token)) {
					char *end;
					client_body_size = std::strtod(token.c_str(), &end);
					if (!(str >> token) || (token.empty() || token != ";"))
						throw(std::runtime_error("Syntax Error ';'"));
				}
				else if (token == "root" && (str >> token)) {
					root = token;
					if (!(str >> token) || (token.empty() || token != ";"))
						throw(std::runtime_error("Syntax Error ';'"));
				}
				else if (token == "location" && (str >> token) && (token == "{"))
					my_location();
			}
			cout << "listen : " << listen << endl;
			cout << "server_name : " << server_name << endl;
			cout << "client_body_size : " << client_body_size << endl;
			cout << "root : " << root << endl;
			cout << "error_pages : ";
			vector<string>::iterator it = error_pages.begin();
			for (;it != error_pages.end();it++)
				cout << *it << ", ";
			cout << endl;
			cout << "Location :\n";
			cout << "\tpath : " << location.back().path  << endl;
			cout << "\tdefault_file : " << location.back().default_file  << endl;
			cout << "\tmethods : ";
			it = location.back().methods.begin();
			for (;it != location.back().methods.end();it++)
				cout << *it << ", ";
			cout << endl;
			cout << "\tcgi_bin : " << location.back().cgi_bin  << endl;
			cout << "\tcgi_extension : " << location.back().cgi_extension  << endl;
			cout << "Location :\n";
			cout << "\tpath : " << location.front().path  << endl;
			cout << "\tdefault_file : " << location.front().default_file  << endl;
			cout << "\tmethods : ";
			it = location.front().methods.begin();
			for (;it != location.front().methods.end();it++)
				cout << *it << ", ";
			cout << endl;
			cout << "\tcgi_bin : " << location.front().cgi_bin  << endl;
			cout << "\tcgi_extension : " << location.front().cgi_extension  << endl;
		}
	}
}
// throw(std::runtime_error("Syntax Error ';'"));