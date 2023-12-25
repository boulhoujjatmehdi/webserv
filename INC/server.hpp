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

// std::vector<int> fd_vec;
std::map<int, httpRequest> fdMapRead;
std::map<int, httpResponse> fdMapWrite;

int getMaxFd();
void refresh_fd_set(fd_set *fdRead, fd_set *fdWrite);


