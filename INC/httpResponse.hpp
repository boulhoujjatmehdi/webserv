#pragma once

#include "httpRequest.hpp"

#define BUFFER_SIZE 1024

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
    httpResponse(const httpResponse& obj)
    {
        socket = obj.socket;
        header = obj.header;
        request = obj.request;
        filename = obj.filename;
        file.open(filename.c_str(), std::ifstream::ate|std::ifstream::binary);
        filePos = 0;
        fileSize = file.tellg();
        file.seekg(0);
    }

    // httpResponse& operator=(const httpResponse& obj)
    // {
    //     // cout << "COPY ass CALLED !!"<< endl;
    //     socket = obj.socket;
    //     request = obj.request;
    //     filename = obj.filename;
    //     file.open(filename.c_str(), std::ifstream::ate|std::ifstream::binary);
    //     filePos = 0;
    //     fileSize = file.tellg();
    //     file.seekg(0);
    //     return *this;
    // }

    httpResponse(const httpRequest& obj, string filename): httpRequest(obj), filename(filename), header_sent(0)
    {
        // cout << "was here1" << endl;
        header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
        // cout << "was here2" << endl;
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

    int sendChunk()
    {
        std::istringstream strm(request);
        string str;
        strm >> str;
        sleep(1);
        if(!file.is_open())
            return 1; //TODO: SET A SPECIFIC ERROR IN FUTURE
        if(header_sent < header.size())
        {
            header_sent += send(socket, header.c_str(), header.size(), 0);
            cout << "send HEADER to "<< socket << endl;
            
            return 0;
        }
        file.read(buffer, BUFFER_SIZE);
        std::streamsize readedData = file.gcount();
        if(readedData > 0)
        {
            int sentData = send(socket, buffer, readedData, 0);
			if(sentData < readedData)
                file.seekg(sentData - readedData, std::ios_base::cur);
            cout << "send Chunk to "<< socket << endl;
            return 0;
        }
        else
        {
            cout << "clear"<< endl;
            return 1;   
        }
        


    }


};
