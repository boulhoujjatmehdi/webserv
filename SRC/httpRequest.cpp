/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 10:06:31 by aachfenn          #+#    #+#             */
/*   Updated: 2023/12/27 14:40:08 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/httpRequest.hpp"

void	httpRequest::calculate_body_size() {
	
	size_t start = request.find("\r\n\r\n");
	start += 4;
	body_size = request.size() - start;
	// i should check the body size with the config file one
		// throw (std::runtime_error("error 10"));
	cout << "body size is : " << body_size << endl;
}

void	httpRequest::parce_request() {
	
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
	// calculate_body_size();
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