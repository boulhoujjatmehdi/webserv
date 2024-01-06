/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_main.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:26:32 by aachfenn          #+#    #+#             */
/*   Updated: 2024/01/06 10:39:12 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parceConfFile.hpp"

void parce_conf_file(parceConfFile &parce) {

	try
	{
		// parceConfFile parce;

		parce.read_conf_file();
		parce.fill_data();
	}
	catch (std::exception &e) {
		cout << e.what() << endl;
		exit (1);
	}
	catch (...) {
		cout << "Error" << endl;
		exit (1);
	}
}