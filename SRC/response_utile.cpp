/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_utile.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:18:17 by rennatiq          #+#    #+#             */
/*   Updated: 2024/02/06 14:19:04 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"
extern std::map<int, Server> servers_sockets;
std::map<int, string> status_message;

void init_status_code() {
	status_message[200] = "OK";
	status_message[301] = "Moved Permanently";
	status_message[400] = "Bad Request";
	status_message[401] = "Unauthorized";
	status_message[403] = "Forbidden";
	status_message[404] = "Not Found";
	status_message[408] = "Request Timeout";
	status_message[413] = "Request Entity Too Large";
	status_message[414] = "Request-URI Too Long";
	status_message[500] = "Internal Server Error";
	status_message[501] = "Not Implemented";
}

bool endwith(const std::string& str, const std::string& suffix)
{
	if (str.length() >= suffix.length()) {
		return (0 == str.compare(str.length() - suffix.length(), suffix.length(), suffix));
	} else {
		return false;
	}
}

void get_directory(const string& uri, string& rest, string& directory)
{
	string suri = uri;
	directory = suri.substr(1);
	size_t foundSlash;

	if((foundSlash = directory.find("/")) != string::npos )
	{
		directory = suri.substr(0, foundSlash+1);
		rest = suri.substr(foundSlash+1);
	}
	else
		directory = suri;
}

bool isDirectory(const std::string& path) {
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return false;
	else if (info.st_mode & S_IFDIR)
        return true;
	else
        return false;
}