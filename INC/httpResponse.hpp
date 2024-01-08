/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 09:44:52 by eboulhou          #+#    #+#             */
/*   Updated: 2024/01/04 11:20:16 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    //constructor with request as param
    httpResponse(const httpRequest& obj);

    httpResponse():httpRequest(-1, -1)
    {
        filename = "";
    }
    ~httpResponse();

    int sendChunk();
    void setData();
	void execute_cgi();
    // void openFile();
};
