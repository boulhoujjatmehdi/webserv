/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorhundeling.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 22:36:37 by rennatiq          #+#    #+#             */
/*   Updated: 2023/12/20 22:37:17 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "requeste.hpp"


class requeste_error
{
private:
    /* data */
public:

    requeste_error();
    ~requeste_error();

// les cas
    int check_request_form();
    int check_request_uri();
    int check_location_redirection();
    int check_allowded_in_location();
    int get_methode();
    int post_methode();
    int delete_methode();
    
};


