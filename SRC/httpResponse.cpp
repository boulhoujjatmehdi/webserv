/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eboulhou <eboulhou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 09:43:03 by eboulhou          #+#    #+#             */
/*   Updated: 2024/01/08 17:54:46 by eboulhou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"
extern std::map<int, Server> servers_sockets;
extern char *envv;

//STATUS CODE
std::map<int, string> status_message;

void init_status_code() {
    status_message[200] = "OK";
    status_message[400] = "Bad Request";
    status_message[401] = "Unauthorized";
    status_message[403] = "Forbidden";
    status_message[404] = "Not Found";
    status_message[413] = "Request Entity Too Large";
    status_message[414] = "Request-URI Too Long";
    status_message[500] = "Internal Server Error";
	
}


httpResponse::httpResponse(const httpResponse& obj): httpRequest(obj)
{
	classLocation = obj.classLocation;
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

void httpResponse::openTheAppropriateFile()
{
	string pathToFile;
	classLocation = NULL;
	// cout << "empty : " << location.size() << "    " << location << endl;
	if(location.empty() && uri == "/")
		location = "/";
	for(size_t i = 0 ; (i < servers_sockets[server_socket].location.size()); i++)
	{
		// cout << servers_sockets[server_socket].location[i].name << " ********** "<<  this->location << endl;
		if(servers_sockets[server_socket].location[i].name == this->location)
		{
			classLocation = &servers_sockets[server_socket].location[i];
			pathToFile = servers_sockets[server_socket].location[i].path;
			pathToFile += (location == "/")? '/' + servers_sockets[server_socket].location[i].default_file: simple_uri;
			// if (simple_uri == "/" || (location == "/" && simple_uri.empty()))
			// 	pathToFile += classLocation->default_file;//TODO: tomorowland 
		}
	}
	if(pathToFile.empty())
	{
		for(size_t i = 0 ; (i < servers_sockets[server_socket].location.size()); i++) {
			if (servers_sockets[server_socket].location[i].name == "/")
			{
				classLocation = &servers_sockets[server_socket].location[i];
				pathToFile = servers_sockets[server_socket].location[i].path + uri;
				cout << "hello" << endl;
				break ;
			}
		}	
	}
	if(pathToFile.empty())
	{
		status = 404;
		pathToFile = "./404Error.html";
	}
	cout << "path to file :: "<< pathToFile << endl;
	

	
	//setting the uri in case of '/' at uri
	// if (uri == "/")
	// 	uri = "/" + servers_sockets[server_socket].location[0].default_file;
	
	//setting file name with the path associated to it in the config file
	// filename = servers_sockets[server_socket].location[0].path  + uri;
	filename = pathToFile;
	
	open_file:
	file.open(filename.c_str(), std::ifstream::ate|std::ifstream::binary);

	if(!file.is_open())
	{
		// cout << "here--------------->\n";
		status = 404;
		filename = servers_sockets[server_socket].error_pages[0];
		goto open_file;
	}

	filePos = 0;
	fileSize = file.tellg();
	if (!file.good()) {
		cout << "::::::::;\n";
	}
	file.seekg(0);
}



void httpResponse::setData()
{

	openTheAppropriateFile();
	
	//setting the fileSize to a stream
	std::ostringstream strm;
	strm << fileSize;
	
	std::ostringstream tmp;
	string my_status;
	tmp << status;
	my_status = tmp.str();
	if (endwith(filename, ".html"))
		header = "HTTP/1.1 " + my_status + " " + status_message[status] + "\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".css"))
		header = "HTTP/1.1 " + my_status + " " + status_message[status] + "\r\nContent-Type: text/css; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".scss"))
		header = "HTTP/1.1 " + my_status + " " + status_message[status] + "\r\nContent-Type: text/scss; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".png"))
		header = "HTTP/1.1 " + my_status + " " + status_message[status] + "\r\nContent-Type: image/png; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".jpeg"))
		header = "HTTP/1.1 " + my_status + " " + status_message[status] + "\r\nContent-Type: image/jpeg; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".jpg"))
		header = "HTTP/1.1 " + my_status + " " + status_message[status] + "\r\nContent-Type: image/jpg; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, ".js"))
		header = "HTTP/1.1 " + my_status + " " + status_message[status] + "\r\nContent-Type: text/javascript; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
	else if (endwith(filename, classLocation->cgi_extension))
		execute_cgi();
	else
		header = "HTTP/1.1 " + my_status + " " + status_message[status] + "\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
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
		// status = 404;
		filename = servers_sockets[server_socket].error_pages[0];
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
	std::ostringstream tmp;
	string my_status;
	tmp << status;
	my_status = tmp.str();
	header = "HTTP/1.1 " + my_status + " " + status_message[status] + "\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
}