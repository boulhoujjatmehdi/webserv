/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parceConfFile.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:27:53 by aachfenn          #+#    #+#             */
/*   Updated: 2023/12/15 20:45:39 by aachfenn         ###   ########.fr       */
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
	string path;
	string default_file;
	vector<string> methods;
	string cgi_bin;
	string cgi_extension;
		Location() {}
		~Location() {}
};


class parceConfFile
{
		vector<string> data;
		string listen;
		string server_name;
		vector<string> error_pages;
		double client_body_size;
		string root;
		vector<Location> location;
		vector<string>::iterator it_data;
		int location_nb;
		int server_nb;
		vector<parceConfFile> server;
	public:
		parceConfFile();
		~parceConfFile();
		void read_conf_file();
		void fill_data();
		void my_location(parceConfFile &serv);
		void print_data();;
};
