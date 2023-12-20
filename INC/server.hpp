#pragma once 

#include <iostream>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <poll.h>


#include "../parcing/parceConfFile.hpp"

#define PORT 8080

using std::cout;
using std::cerr;
using std::endl;
using std::string;
