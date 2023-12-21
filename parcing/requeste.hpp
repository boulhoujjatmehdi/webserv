/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requeste.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:39:58 by rennatiq          #+#    #+#             */
/*   Updated: 2023/12/21 12:49:05 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

class requeste
{
private:
    std::string methode;
    std::string path;
    std::string http_type;
    std::pair<std::string, std::string> host;
    std::string connection;
    std::string referer;
    std::map<std::string, std::string> data;
    std::string content_length;
    std::string origin;
    
    /* data */
public:
    std::vector<std::string> request;

    requeste();
    ~requeste();

    //getters
    std::string get_methode();
    std::string get_path();
    std::string get_http_type();
    std::pair<std::string, std::string> get_host();
    std::string get_connection();
    std::string get_referer();
    std::map<std::string, std::string> get_data();
    std::string get_content_length();
    std::string get_origin();
    
    
//parsing
    void parsing_requeste_get();
    void parsing_requeste_post();
    void parsing_requeste();


    
};


