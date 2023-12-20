/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requeste.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:22:22 by rennatiq          #+#    #+#             */
/*   Updated: 2023/12/20 21:36:16 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "requeste.hpp"

requeste::requeste()
{
}

requeste::~requeste()
{
}

void requeste::parsing_requeste_get()
{
    std::cout << ":i am get" << std::endl;
    std::string head;
    std::string tmp_head;
    std::string tmp_url;
    if (!request.empty())
    {
        std::istringstream str(request[0]);
        if (str >> head)
            methode = "get";
        if (str >> head)
            tmp_url = head;
        if (str >> head)
            http_type = head;
    }
    else
        std::cout << "request empty" << std::endl;
    for(int i = 1; i < request.size(); i++)
    {
        std::istringstream str(request[i]);
        if (str >> head)
        {
            if (head == "Host:")
                str >> tmp_head;
            if (head == "Connection:")
                str >> connection;
            if (head == "Referer:")
                str >> referer;
        }
    }
    if(!tmp_head.empty())
    {
        host.first = tmp_head.substr(0, tmp_head.find(':'));
        host.second = tmp_head.substr(tmp_head.find(':') + 1, tmp_head.size());
    }
    if (!tmp_url.empty())
    {
        path = tmp_url.substr(0, tmp_url.find('?'));
        std::string tmp_data = tmp_url.substr(tmp_url.find('?') + 1, tmp_url.size());
        std::string tmp_data2;
        if (!tmp_data.empty())
        {
            int start = 0;
            int end = 0;
            for (;tmp_data[end]; end++)
            {
                if (tmp_data[end] == '&')
                {
                    tmp_data2 = tmp_data.substr(start, end- start);
                    data[tmp_data2.substr(0, tmp_data2.find('='))] = tmp_data2.substr(tmp_data2.find('=') + 1, tmp_data2.size());
                    start = end + 1; 
                }
            }
            tmp_data2 = tmp_data.substr(start, end- start);
            data[tmp_data2.substr(0, tmp_data2.find('='))] = tmp_data2.substr(tmp_data2.find('=') + 1, tmp_data2.size());
        }
        
    }
}
void requeste::parsing_requeste_post()
{
    std::cout << ":i am post" << std::endl;
    std::cout << ":i am get" << std::endl;
    std::string head;
    std::string tmp_head;
    if (!request.empty())
    {
        std::istringstream str(request[0]);
        if (str >> head)
            methode = "get";
        if (str >> head)
            path = head;
        if (str >> head)
            http_type = head;
    }
    else
        std::cout << "request empty" << std::endl;
    for(int i = 1; i < request.size(); i++)
    {
        std::istringstream str(request[i]);
        if (str >> head)
        {
            if (head == "Host:")
                str >> tmp_head;
            if (head == "Connection:")
                str >> connection;
            if (head == "Referer:")
                str >> referer;
            if (head == "Content-Length:")
                str >> content_length;
            if (head == "Origin:")
                str >> origin;
            
        }
    }
    if(!tmp_head.empty())
    {
        host.first = tmp_head.substr(0, tmp_head.find(':'));
        host.second = tmp_head.substr(tmp_head.find(':') + 1, tmp_head.size());
    }
    // 
    // 
    // 
    // hada khas b3da tkon body main orta singh magiz ilan mach ati ghika asat parsa
    // 
    // 
    // 
    // if (!body.empty())
    //     {
        //     std::string tmp_body;
    //         int start = 0;
    //         int end = 0;
    //         for (;body[end]; end++)
    //         {
    //             if (body[end] == '&')
    //             {
    //                 tmp_data2 = body.substr(start, end- start);
    //                 data[tmp_data2.substr(0, tmp_data2.find('='))] = tmp_data2.substr(tmp_data2.find('=') + 1, tmp_data2.size());
    //                 start = end + 1; 
    //             }
    //         }
    //         tmp_data2 = body.substr(start, end- start);
    //         data[tmp_data2.substr(0, tmp_data2.find('='))] = tmp_data2.substr(tmp_data2.find('=') + 1, tmp_data2.size());
    //     }
}

void requeste::parsing_requeste()
{
    std::string head;
    if (!request.empty())
    {
        std::istringstream str(request[0]);
        str >> head;
        if (head == "GET")
            parsing_requeste_get();
        else if (head == "POST")
            parsing_requeste_post();
        else
            std::cout << ":ERROR" << std::endl;
    }
    else
        std::cout << "request empty" << std::endl;
}

int main()
{
    requeste reque;
    std::ifstream my_file("requeste.txt");
    std::string tmp;
    while(std::getline(my_file, tmp))
    {
        reque.request.push_back(tmp);
    }

    reque.parsing_requeste();
    std::cout << "methode: " << reque.methode << std::endl;
    std::cout << "path: " << reque.path << std::endl;
    std::cout << "data: ";
    std::map<std::string, std::string>::iterator it = reque.data.begin();
    for (;it != reque.data.end();it++) {
        std::cout << it->first << ": " << it->second << " || ";
    }
    std::cout  << std::endl;
    std::cout << "http_type: " << reque.http_type << std::endl;
    std::cout << "host: " << reque.host.first << " || " << reque.host.second << std::endl;
    std::cout << "connection: " << reque.connection << std::endl;
    std::cout << "referer: " << reque.referer << std::endl;
    std::cout << "content_length: " << reque.content_length << std::endl;
    std::cout << "origin: " << reque.origin << std::endl;
}