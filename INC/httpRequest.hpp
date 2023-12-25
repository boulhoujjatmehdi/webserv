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

    const int& getSocket() const
    {
        return socket;
    }


    httpRequest(const httpRequest& obj)
    {
        socket = obj.socket;
        request = obj.request;
    }
    httpRequest& operator=(const httpRequest& obj)
    {
        socket = obj.socket;
        request = obj.request;
        return *this;
    }
    httpRequest(int socket = -1, string request = ""): socket(socket), request(request)
    {

    }
    
    ~httpRequest()
    {

    }
    

};