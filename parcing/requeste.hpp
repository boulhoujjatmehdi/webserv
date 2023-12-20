/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requeste.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:39:58 by rennatiq          #+#    #+#             */
/*   Updated: 2023/12/20 22:37:23 by rennatiq         ###   ########.fr       */
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
    /* data */
public:
    std::string methode;
    std::string path;
    std::string http_type;
    std::pair<std::string, std::string> host;
    std::string connection;
    std::string referer;
    std::map<std::string, std::string> data;
    std::string content_length;
    std::string origin;
    
    std::vector<std::string> request;

    requeste();
    ~requeste();
//parsing
    void parsing_requeste_get();
    void parsing_requeste_post();
    void parsing_requeste();


    
};


