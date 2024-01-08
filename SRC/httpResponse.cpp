/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 09:43:03 by eboulhou          #+#    #+#             */
/*   Updated: 2024/01/06 10:33:20 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"
extern std::map<int, Server> servers_sockets;
extern char *envv;

httpResponse::httpResponse(const httpResponse& obj): httpRequest(obj)
{
	socket = obj.socket;
	header_sent = obj.header_sent;
	header = obj.header;
	request = obj.request;
	filename = obj.filename;
	filePos = obj.filePos;
	fileSize = obj.fileSize;
	content_length = obj.content_length;

	
	// cout << "filename : ("<< filename<< ")"<< endl;
	// open_file:
	// file.open(filename.c_str(), std::ifstream::ate|std::ifstream::binary);

	// if(!file.is_open())
	// {
	// 	status = 404;
	// 	filename = servers_sockets[server_socket].location[0].path + "/" + servers_sockets[server_socket].error_pages[0];
	// 	goto open_file;
	// }

	// filePos = 0;
	// fileSize = file.tellg();
	// file.seekg(0);

	
}

httpResponse::~httpResponse()
{
	if(file.is_open())
		file.close();
}

/* 
send a chunk of data 
return: 0 if still sending the data
return: 1 if the connection closed by peer
return: 2 if the data is sent successfully and the connection is on keep alive mode
*/
int httpResponse::sendChunk()
{
	std::istringstream strm(request);
	string str;
	strm >> str;
	int sent_size, sentData;

	//sending header in one chunk 
	if(header_sent < header.size())
	{
		sent_size = send(socket, header.c_str() + header_sent, header.size() - header_sent , 0);
		if(sent_size == -1)
			return 1;
		header_sent += sent_size;
		return 0;
	}
	
	file.read(buffer, BUFFER_SIZE);
	std::streamsize readedData = file.gcount();
	if(readedData > 0)
	{
		sentData = send(socket, buffer, readedData, 0);
		if(sentData == -1)
		{
			return 1;
		}
		if(sentData < readedData)
			file.seekg(sentData - readedData, std::ios_base::cur);
		return 0;
	}
	else
	{
		//all the data has been sent
		return 2;
	}
}

bool endwith(const std::string& str, const std::string& suffix)
{
	if (str.length() >= suffix.length()) {
		return (0 == str.compare(str.length() - suffix.length(), suffix.length(), suffix));
	} else {
		return false;
	}
}

httpResponse::httpResponse(const httpRequest& obj):httpRequest(obj), header_sent(0)
{
}

void httpResponse::setData()
{
	//setting the uri in case of '/' at uri
	if (uri == "/")
		uri = "/" + servers_sockets[server_socket].location[0].default_file;
	
	//setting file name with the path associated to it in the config file
	filename = servers_sockets[server_socket].location[0].path  + uri;
	
	// cout << "filename : ("<< filename<< ")"<< endl;
	//TODO: remove the algo and set a default 404 page response
	open_file:
	file.open(filename.c_str(), std::ifstream::ate|std::ifstream::binary);

	if(!file.is_open())
	{
		// cout << "here--------------->\n";
		status = 404;
		filename = servers_sockets[server_socket].location[0].path + "/" + servers_sockets[server_socket].error_pages[0];
		goto open_file;
	}

	filePos = 0;
	fileSize = file.tellg();
	if (!file.good()) {
		cout << "::::::::;\n";
	}
	file.seekg(0);
	
	//setting the fileSize to a stream
	std::ostringstream strm;
	strm << fileSize;

	// AYMANE CHANGES
	std::istringstream tmp(status);
	string my_status;
	tmp >> my_status;
	// cout << "my_status ---------------->" << status <<"|"<< endl;
		// header = "HTTP/1.1 " + my_status + " OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	if (endwith(filename, ".html"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".css"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: text/css; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".png"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: image/png; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".jpeg"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".jpg"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: image/jpg; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".js"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: text/javascript; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".js"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: text/javascript; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".cgi"))
		execute_cgi();
	else
		header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
}

void	httpResponse::execute_cgi() {
	
	if(file.is_open())
		file.close();
	cout << "CGI IS DETECTED\n";
	int filefd = 0;
	pid_t pid = fork();
	if (pid == 0) {
		filefd = open("cgi.html", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		dup2(filefd, 1);
		close(filefd);
		char *argv[2];
		argv[0] = (char *)filename.c_str();
		argv[1] = NULL;
		char* envp[5];
		envp[0] = (char *)"QUERY_STRING=query";
		envp[1] = (char *)"REQUEST_METHOD=GET";
		envp[2] = (char *)"CONTENT_TYPE=text/plain";
		envp[3] = (char *)"CONTENT_LENGTH=0";
		envp[4] = NULL;

		if (execve(filename.c_str(), argv, NULL) == -1) {
			std::cerr << "Error execve" << endl;
			exit(1);
		}
	} else if (pid < 0) {
		cout << "Error fork" << endl;
	} else {
		int status;
		waitpid(pid, &status, 0);
	}
	close(filefd);
	filename = "./cgi.html";
		// goto open_file;

	file.open(filename.c_str(), std::ifstream::ate|std::ifstream::binary);

	if(!file.is_open())
	{
		status = 404;
		filename = servers_sockets[server_socket].location[0].path + "/" + servers_sockets[server_socket].error_pages[0];
		file.open(filename.c_str(), std::ifstream::ate|std::ifstream::binary);
		if(!file.is_open()) {
			cout << "Coudn't open the Error Page" << endl;
			exit (1);
		}
	}

	filePos = 0;
	fileSize = file.tellg();
	if (!file.good()) {
		cout << "Coudn't open the Error Page" << endl;
		exit (1);
	}
	file.seekg(0);
	
	//setting the fileSize to a stream
	std::ostringstream strm;
	strm << fileSize;
	header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
}