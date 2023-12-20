#pragma once 

#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#define PORT 8080
#define NBOFCLIENTS 3334


using std::cout;
using std::cerr;
using std::endl;
using std::string;

#include <vector>
std::vector<int> fd_vec;

int getMaxFd()
{
    if(fd_vec.size() < 1)
        return -1;
    int tmp = fd_vec.front();
    std::vector<int>::iterator it = fd_vec.begin();
    while (++it != fd_vec.end())
    {
        if(tmp < *it)
            tmp = *it;
    }
    return tmp;
}

