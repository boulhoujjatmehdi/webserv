#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <fstream>
#include <sstream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

class httpRequest
{
public:
    int socket;
    string request;
	
	string method;
	// Uniform Resource Identifier
	string uri;
	string http_version;
	string hostname;
	string port;
	bool connection;
	int status;
	int body_size;

    const int& getSocket() const
    {
        return socket;
    }


    httpRequest(const httpRequest& obj)
    {
        *this = obj;
    }
    httpRequest& operator=(const httpRequest& obj)
    {
        socket = obj.socket;
        request = obj.request;
		method = obj.method;
		uri = obj.uri;
		http_version = obj.http_version;
		hostname = obj.hostname;
		port = obj.port;
		connection = obj.connection;
        return *this;
    }
    httpRequest(int socket = -1, string request = ""): socket(socket), request(request), connection(false), body_size(0)
    {

    }
    
    ~httpRequest()
    {

    }

	void	generate_response();
	void	parce_request();
	void	calculate_body_size();

};