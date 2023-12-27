#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <fstream>
#include <sstream>
#include <map>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

#define BUFFER_SIZE 4096


class httpRequest
{
public:
    int socket;
    string request;
	
	string method;
	string uri;
	string http_version;
	string hostname;
	string port;
	bool connection;
	int status;

    const int& getSocket() const
    {
        return socket;
    }

    httpRequest(const httpRequest& obj)
    {
        *this = obj;
    }

    httpRequest& operator=(const httpRequest& obj);

    httpRequest(int socket = -1): socket(socket), request(""), connection(false){}
    
    ~httpRequest(){}
    
	void	generate_response();

};
