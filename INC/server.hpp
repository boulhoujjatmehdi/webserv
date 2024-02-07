/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 09:44:55 by eboulhou          #+#    #+#             */
/*   Updated: 2024/02/06 14:20:12 by rennatiq         ###   ########.fr       */
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
bool endwith(const std::string& str, const std::string& suffix);
void init_status_code();
int connectSockets(parceConfFile cf);
void createHtmlFile();
void clear_maps();
void acceptNewConnections(int sockfd);
void writeOnSocket(std::map<int, httpResponse>::iterator& it);
int  readTheRequest(std::map<int, httpRequest>::iterator& it);
void init_status_code();
bool endwith(const std::string& str, const std::string& suffix);
void get_directory(const string& uri, string& rest, string& directory);
bool isDirectory(const std::string& path);



