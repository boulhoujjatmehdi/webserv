/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 11:36:55 by rennatiq          #+#    #+#             */
/*   Updated: 2024/02/06 12:24:21 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"

extern std::map<int, httpRequest> fdMapRead;
extern std::map<int, httpResponse> fdMapWrite;
extern std::map<int, Server> servers_sockets;
extern std::vector<int> deleteReadFd;
extern std::vector<int> deleteWriteFd;
extern std::map<int, string> status_message;

void createHtmlFile()
{
	std::ofstream file;
	for (std::map<int, string>::iterator it = status_message.begin() ; it != status_message.end(); it++)
	{
		if(it->first == 200)
			continue;
		std::ostringstream strstream;
		strstream << it->first;
		strstream << "Error.html";
		file.open(strstream.str().c_str());
	if(!file.is_open())
	{
		std::cerr << "default pages failed to open"<< endl;
		exit(12);
	}
	file << "<!DOCTYPE html>\n"
		 << "<html>\n"
		 << "<head>\n"
		 << "<title>"<< it->first <<" "<< it->second <<"</title>\n"
		 << "<style>\n"
		 << "body {\n"
		 << "    font-family: Arial, sans-serif;\n"
		 << "    background-color: #f4f4f4;\n"
		 << "    text-align: center;\n"
		 << "}\n"
		 << "h1 {\n"
		 << "    color: #333;\n"
		 << "}\n"
		 << "p {\n"
		 << "    color: #777;\n"
		 << "}\n"
		 << "</style>\n"
		 << "</head>\n"
		 << "<body>\n"
		 << "<h1>"<< it->first <<" "<< it->second <<"</h1>\n"
		 << "<p>The requested page could not be found.</p>\n"
		 << "</body>\n"
		 << "</html>\n";

	file.close();
}
}

int getMaxFd()
{
	int tmp = -1;
    if(fdMapRead.size() >= 1)
	{
		//PARSE: TRY TO USE RBEGIN() INSTEAD OF END
		std::map<int, httpRequest>::iterator it = fdMapRead.end();
		it--;
		if( it->first > tmp)
			tmp = it->first;
	}
	if(fdMapWrite.size() >= 1)
	{
		std::map<int, httpResponse>::iterator it = fdMapWrite.end();
		it--;
		if( it->first > tmp)
			tmp = it->first;
	}
	if(servers_sockets.size() >= 1)
	{
		if (servers_sockets.rbegin()->first > tmp)
		{
			tmp = servers_sockets.rbegin()->first;
		}
	}

    return tmp;
}

void clear_maps()
{
	cout << "clear" << endl;
	exit(112);
	for (std::__1::map<int, httpRequest>::iterator it = fdMapRead.begin(); it != fdMapRead.end(); it++)
	{
		close(it->first);
		deleteReadFd.push_back(it->first);
	}
	for (std::__1::map<int, httpResponse>::iterator it = fdMapWrite.begin(); it != fdMapWrite.end(); it++)
	{
		close(it->first);
		deleteWriteFd.push_back(it->first);
	}
}