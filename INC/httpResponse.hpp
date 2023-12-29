#pragma once

#include "httpRequest.hpp"
#include <sys/socket.h>
#include <sys/types.h>






class httpResponse: public httpRequest
{
public:
    std::ifstream file;
    string filename;
    string header;
    size_t header_sent;
    char buffer[BUFFER_SIZE];
    int last_send;
    std::streamsize filePos;
    std::streamsize fileSize;

    const std::ifstream& getFile()const
    {
        return file;
    }
    //copy constructor
    httpResponse(const httpResponse& obj);

    httpResponse(const httpRequest& obj);

    httpResponse():httpRequest(-1, -1)
    {
        filename = "";
    }
    ~httpResponse();

    int sendChunk();
};
