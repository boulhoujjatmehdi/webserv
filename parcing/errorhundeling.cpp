/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorhundeling.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 22:37:49 by rennatiq          #+#    #+#             */
/*   Updated: 2023/12/20 22:39:11 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errorhundeling.hpp"

requeste_error::requeste_error(){}
requeste_error::~requeste_error(){}

int requeste_error::check_request_form()
{
     //     if =>
// Transfer-Encoding header
// exist and is different to “chunked”


// if =>
// Transfer-Encoding not exist
// Content-Length not exist
// The method is Post
    // if (methode == "post" && !content_length.empty())
    //     400 bad request


// if =>
// Request uri contain
// a character not allowded


// if =>
// Request uri contain
// more that 2048 chars


// if =>
// Request body larger
// than client max body size
// in config file
}
int requeste_error::check_request_uri(){}
int requeste_error::check_location_redirection(){}
int requeste_error::check_allowded_in_location(){}
int requeste_error::get_methode(){}
int requeste_error::post_methode(){}
int requeste_error::delete_methode(){}