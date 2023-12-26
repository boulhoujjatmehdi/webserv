#pragma once

#include "httpRequest.hpp"
#include <sys/socket.h>
#include <sys/types.h>

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
    int x;

    const std::ifstream& getFile()const
    {
        return file;
    }
    //copy constructor
    httpResponse(const httpResponse& obj)
    {
        x = obj.x;
        socket = obj.socket;
        header_sent = obj.header_sent;
        header = obj.header;
        request = obj.request;
        filename = obj.filename;
        file.open(filename.c_str(), std::ifstream::ate|std::ifstream::binary);
        filePos = 0;
        fileSize = file.tellg();
        file.seekg(0);

        std::stringstream ss;
        ss << fileSize;
        // header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
        header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Encoding: UTF-8\r\nContent-Length: " + ss.str() +  "\r\n\r\n";

    }

    httpResponse(const httpRequest& obj, string filename): httpRequest(obj), filename(filename), header_sent(0), x(0)
    {
        // header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\nContent-Lenght: "
        header = "";
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
        if(!file.is_open())
            return 1; //TODO: SET A SPECIFIC ERROR IN FUTURE
        // cout << "header_sent : " << header_sent<< endl << " header.size(): " << header.size() << endl;
        if(header_sent < header.size())
        {
            int sent_size = send(socket, header.c_str() + header_sent, header.size() - header_sent , 0);

            if(sent_size == -1)
            {
                cout << "send header errno = " << errno << endl;
                cerr << "/////////Send header Error" << endl;
            }
            header_sent += sent_size;
            return 0;
        }
        file.read(buffer, BUFFER_SIZE);
        std::streamsize readedData = file.gcount();
        if(readedData > 0 )
        {
            int sentData = send(socket, buffer, readedData, 0);
            if(sentData == -1)
            {
                if(errno == EPIPE)
                {
                    file.seekg(-BUFFER_SIZE, std::ios_base::cur);
                    cout << "mehdi" << endl;
                    file.close();
                    return 1;
                }
                return 1;
            }
            filePos += sentData;
			if(sentData < readedData)
                file.seekg(sentData - readedData, std::ios_base::cur);
            return 0;
        }
        else
        {
            return 2;
        }
        


    }


};
