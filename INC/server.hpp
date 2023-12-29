/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eboulhou <eboulhou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 09:44:55 by eboulhou          #+#    #+#             */
/*   Updated: 2023/12/29 09:44:56 by eboulhou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>

#define PORT1 8080

#define NBOFCLIENTS 3334


using std::cout;
using std::cerr;
using std::endl;
using std::string;

#include <vector>
#include <map>

#include "httpResponse.hpp"




int getMaxFd();
void refresh_fd_set(fd_set *fdRead, fd_set *fdWrite);



