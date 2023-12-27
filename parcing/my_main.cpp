/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_main.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eboulhou <eboulhou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:26:32 by aachfenn          #+#    #+#             */
/*   Updated: 2023/12/27 12:51:45 by eboulhou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parceConfFile.hpp"


void parce_conf_file(parceConfFile &parce) {

	try
	{
		// parceConfFile parce;

		parce.read_conf_file();
		parce.fill_data();
		// return parce;
	}
	catch (std::exception &e) {
		cout << e.what() << endl;
		exit(1);
	}
	catch (...) {
		cout << "Error" << endl;
		exit(1);
	}
}