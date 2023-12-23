#pragma once

#include "httpRequest.hpp"

class httpResponse: public httpRequest
{
public:
    std::ifstream file;
    string filename;

    const std::ifstream& getFile()const
    {
        return file;
    }
    //orthodox
    httpResponse(const httpResponse& obj)
    {
        socket = obj.socket;
        request = obj.request;
        // file = obj.file;
        filename = obj.filename;
    }
    httpResponse& operator=(const httpResponse& obj)
    {
        socket = obj.socket;
        request = obj.request;
        // file = obj.file;
        filename = obj.filename;
        return *this;
    }
    httpResponse(const httpRequest& obj): httpRequest(obj)
    {
        // file.open(filename.c_str());
    }
    httpResponse()
    {
        filename = "";
    }
    ~httpResponse()
    {
        if(file.is_open())
            file.close();
    }



};
