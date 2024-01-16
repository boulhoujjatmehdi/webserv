/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parceConfFile.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:27:53 by aachfenn          #+#    #+#             */
/*   Updated: 2024/01/16 15:41:16 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class Location {

	public:
	string name;
	string path;
	string default_file;
	vector<string> methods;
	string cgi_extension;
	
	Location() : default_file("index.html") {}
	~Location() {}
};

class Server {
	public:
		vector<string> listen;

		string server_name;
		string root;
		vector<string> error_pages;
		double client_body_size;
		vector<Location> location;
		int location_nb;
		// 
		bool directory_listing;
		
		Server();
		~Server();
		//copy construcot 
		Server(const Server& obj);
		//copy assignment operator
		Server& operator=(const Server& obj);
};

class parceConfFile
{
		vector<string> data;
		vector<string>::iterator it_data;
	public:
		int server_nb;
		vector<Server> server;
		
		parceConfFile();
		~parceConfFile();
		void read_conf_file();
		void fill_data();
		void my_location(Server &serv, string location_name);
		void print_data();
		string my_trim(string str);
		void check_ifdata_isnot_empty();
		void check_ifdata_is_valid();
		void add_the_necessary_data();

};

void parce_conf_file(parceConfFile &parce);