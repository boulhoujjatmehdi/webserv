/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorhundeling.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 22:37:49 by rennatiq          #+#    #+#             */
/*   Updated: 2023/12/21 13:22:58 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errorhundeling.hpp"

requeste_error::requeste_error(){}
requeste_error::~requeste_error(){}

int requeste_error::check_request_form(requeste request)
{
     //     if =>
// Transfer-Encoding header
// exist and is different to “chunked"
    // if (???)
    //     return (501);



// if =>
// Transfer-Encoding not exist
// Content-Length not exist
// The method is Post
    if (request.get_methode() == "post" && !request.get_content_length().empty())
        return (400);


// if =>
// Request uri contain
// a character not allowded
    std::string str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=";
    for(int i  = 0; i < request.get_path().length(); i++)
    {
        if (str.find(request.get_path()[i]) == std::string::npos)
            return (400);
    }

// if =>
// Request uri contain
// more that 2048 chars
    if (request.get_path().length() > 2048)
        return (414);

// if =>
// Request body larger
// than client max body size
// in config file
    // if (???)
    //     return (413);
}
int requeste_error::check_request_uri(requeste request){

    // if (locations.count(path) > 0) // likayna f config file
    //     return (404);
}
int requeste_error::check_location_redirection(requeste request){
// if =>
// location have redirection like: 
// return 301 /home/index.html
    // if (???)
    //     return (301);
}

int requeste_error::check_allowded_in_location(requeste request){
//     if =>
// method not allowded

    if (request.get_methode() != "get" || request.get_methode() != "post" || request.get_methode() != "delete")
        return (405);
    // if (request.get_methode() != locations.method) // ila makaynch f config file
    //     return (405);
}
int requeste_error::get_methode(requeste request){
    // get_requested_resource();
    //check if get_path is file or directory 
    
    


    
    if (request.get_path()[request.get_path().size() - 1] == '/')
    {

    }
    else
    {
        
    }
}
int requeste_error::post_methode(){}
int requeste_error::delete_methode(){}