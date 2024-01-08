/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:38:13 by aachfenn          #+#    #+#             */
/*   Updated: 2024/01/08 10:25:02 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <fstream>
#include <sstream>
#include <map>
#include "../parcing/parceConfFile.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

#define BUFFER_SIZE 4096

class httpRequest
{
public:
    int socket;
    int server_socket;
    string request;
	string method;
	string uri;
	string http_version;
	string hostname;
	string port;
	bool connection;
	int body_size;
	int content_length;
	int status;

	std::map<string, string> form_data;

    const int& getSocket() const
    {
        return socket;
    }

    httpRequest(const httpRequest& obj)
    {
        *this = obj;
    }

    httpRequest& operator=(const httpRequest& obj);


    httpRequest(int socket ): socket(socket), server_socket(-1), request(""), connection(false), content_length(-1) , status(200)
    {
    }
    httpRequest(int socket , int serverSocket): socket(socket), server_socket(serverSocket), request(""), connection(false), content_length(-1), status(200)
    {
    }
    // httpRequest(): request(""), connection(false)
    // {

    //     cout << "server_socket3 : "<< server_socket << endl;
    // }
    
    ~httpRequest(){}
    
	void	generate_response();
	void	parce_request();
	void	extract_form_data();
	void	checks_();
	void	extract_uri_data();
	void	upload_files();
};
