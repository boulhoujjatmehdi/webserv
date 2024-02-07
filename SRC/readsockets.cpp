/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readsockets.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 11:39:49 by rennatiq          #+#    #+#             */
/*   Updated: 2024/02/06 11:46:30 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"

extern std::map<int, httpRequest> fdMapRead;
extern std::map<int, httpResponse> fdMapWrite;
extern std::vector<int> deleteReadFd;

int  readTheRequest(std::map<int, httpRequest>::iterator& it)
{
	int commSocket;
	char buffer[BUFFER_SIZE + 1];
	int size_readed;
	string request;

	commSocket = it->first;
	bzero(buffer, BUFFER_SIZE + 1);
	size_readed = recv(commSocket, buffer, BUFFER_SIZE, 0);
	if(size_readed <= 0)
	{	
		cout << "socket connection ended " << request << endl;
		close(commSocket);
		// fdMapRead.erase(commSocket);
		deleteReadFd.push_back(commSocket);
		return 0;
	}
	else 
	{
		// cout << buffer << endl;
		it->second.request.append(buffer, size_readed);
			request = it->second.request;//TODO: MAKE THIS REQUEST REFERENCE TO ESCAPE THE LATENCY OF COPPYING A LOT OF DATA
		// cout << "("<< it->second.method <<")"<< endl;
		if(it->second.method.empty())
		{
			size_t pos = request.find(" ");
			if(pos != string::npos)
				it->second.method = request.substr(0, pos);
		}
		size_t posofend;
		if(it->second.method == "POST" && (posofend = request.find("\r\n\r\n")) != string::npos)
		{
			if(it->second.content_length == -1)
			{
				size_t pos = request.find("Content-Length: ");
				if(pos != string::npos)
				{
					pos += 16;
					string st = request.substr(pos, request.find("\n", pos) - pos);
					it->second.content_length = std::atoi(st.c_str());
				}
			}
			if(it->second.content_length != -1 &&  (size_t)it->second.content_length ==  request.length() - (posofend + 4))
			{
				// cout << "full request received POST method"<< endl;
				it->second.generate_response();
				fdMapWrite.insert(std::make_pair(commSocket, httpResponse(it->second)));
				fdMapWrite[commSocket].setData();
				// fdMapRead.erase(commSocket);
				deleteReadFd.push_back(commSocket);
			}
				return 0;
		}
		if(request.size() > 4  && request.substr(request.size() - 4) == "\r\n\r\n")
		{
			// cout << "full request received with GET method"<<endl;
			it->second.generate_response();
			fdMapWrite.insert(std::make_pair(commSocket, httpResponse(it->second)));
			fdMapWrite[commSocket].setData();
			// fdMapRead.erase(commSocket);
			deleteReadFd.push_back(commSocket);
			return 0;
		}
		
	// cout << "("<< request << ")"<< endl;
	
	}
	// cout << "impossible to reach : "<< it->second.content_length << " " << size_readed << endl;
	return 0;
}
