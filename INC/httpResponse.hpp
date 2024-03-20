/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 09:44:52 by eboulhou          #+#    #+#             */
/*   Updated: 2024/02/24 10:56:02 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "httpRequest.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include "../parcing/parceConfFile.hpp"






class httpResponse: public httpRequest
{
public:
    std::ifstream file;
    // string filename;
    string header;
    size_t header_sent;
    char buffer[BUFFER_SIZE];
    int last_send;
    std::streamsize filePos;
    std::streamsize fileSize;
    
    Location* classLocation;
    
    

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
    void openTheAppropriateFile(string& str);
	string fillThePathFile(string& redirection);
	void	delete_files();//rachid

    char **cgi_envatment(char **envv);
};
