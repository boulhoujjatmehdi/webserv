/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorhundeling.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 22:36:37 by rennatiq          #+#    #+#             */
/*   Updated: 2023/12/21 13:18:23 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "requeste.hpp"


class requeste_error
{
private:
    
public:

    requeste_error();
    ~requeste_error();

// les cas
    int check_request_form(requeste request);
    int check_request_uri(requeste request);
    int check_location_redirection(requeste request);
    int check_allowded_in_location(requeste request);
    int get_methode(requeste request);
    int post_methode();
    int delete_methode();
    
};


