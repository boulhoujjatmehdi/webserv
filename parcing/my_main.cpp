/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_main.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:26:32 by aachfenn          #+#    #+#             */
/*   Updated: 2024/01/12 14:57:49 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parceConfFile.hpp"

void parce_conf_file(parceConfFile &parce) {

	parce.read_conf_file();
	parce.fill_data();
}