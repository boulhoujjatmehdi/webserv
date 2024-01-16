/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 09:43:03 by eboulhou          #+#    #+#             */
/*   Updated: 2024/01/16 15:39:33 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"
extern std::map<int, Server> servers_sockets;
extern char **envv;

//STATUS_CODE
std::map<int, string> status_message;

void init_status_code() {
	status_message[200] = "OK";
	status_message[301] = "Moved Permanently";
	status_message[400] = "Bad Request";
	status_message[401] = "Unauthorized";
	status_message[403] = "Forbidden";
	status_message[404] = "Not Found";
	status_message[405] = "Method Not Allowed";
	status_message[408] = "Request Timeout";
	status_message[413] = "Request Entity Too Large";
	status_message[414] = "Request-URI Too Long";
	status_message[500] = "Internal Server Error";
	status_message[501] = "Not Implemented";
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
return: 2 if the data is sent successfully
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

void get_directory(const string& uri, string& rest, string& directory)
{
	string suri = uri;
	// cout << "uri			:("<< uri << ")"<< endl;
	directory = suri.substr(1);
	size_t foundSlash;

	if((foundSlash = directory.find("/")) != string::npos )
	{
		directory = suri.substr(0, foundSlash+1);
		rest = suri.substr(foundSlash+1);
	}
	else
		directory = suri;
	
	// cout << "rest			:("<< rest << ")"<< endl;
	// cout << "directory		:("<< directory << ")"<< endl << endl;
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

void listDirectoriesAsHtml(string path) {
	DIR* dir = opendir(path.c_str());

	if (dir == NULL) {
		throw std::runtime_error("Could not open directory: " + path);
	}
	string name = "tmp.html";
	std::ofstream my_file(name);
	if (!my_file) {
		throw std::runtime_error("Could not open file: " + name);
	}

	my_file << "<html>\n<head>\n<title>Index of " << path <<"</title>\n";
	my_file << "<style>\n";
	my_file << "body { font-family: Arial, sans-serif; }\n";
	my_file << "h1 { color: #333; }\n";
	my_file << "ul { list-style-type: none; }\n";
	my_file << "li { margin-bottom: 5px; }\n";
	my_file << "a { text-decoration: none; color: #007bff; }\n";
	my_file << "a:hover { text-decoration: underline; }\n";
	my_file << "</style>\n";
	my_file << "</head>\n<body>\n<h1>Index of " << path <<"</h1>\n<ul>\n";

	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {
		my_file << "<li><a href=\"" << entry->d_name << "\">" << entry->d_name << "</a></li>\n";
	}
	my_file << "</ul>\n</body>\n</html>";
	closedir(dir);
}


string httpResponse::fillThePathFile(string& __unused redirection)
{
	string pathToFile;
	classLocation = NULL;
	// cout << "empty : " << location.size() << "    " << location << endl;

	location = '/' + location;

	// string rest, directory;
	
	get_directory(uri, simple_uri, location);
	// cout << "uri 			:("<< uri << ")"<< endl;
	// cout << "name			:("<< servers_sockets[server_socket].location[0].name << ")"<< endl;
	// cout << "simple uri		:("<< simple_uri << ")"<< endl;
	// cout << "location		:("<< location << ")"<< endl ;
	if (location == "/" && simple_uri.empty())
		simple_uri = "/";

	for(size_t i = 0 ; (i < servers_sockets[server_socket].location.size()); i++)
	{
		
		if((servers_sockets[server_socket].location[i].name  == this->location))
		{
			classLocation = &servers_sockets[server_socket].location[i];
			pathToFile = servers_sockets[server_socket].location[i].path + uri;
			if (simple_uri == "")
			{
				redirect:
				// cout << "_-------redirected_-------\n";
				if (access(uri.c_str(), F_OK) == -1) {
					status = 301;
					redirection = "Location: " +  uri + "/\r\n";
					pathToFile = "./404Error.html";
					// cout << "THIRD" << endl;
					goto endd;
				}
			}
			if(simple_uri == "/") {
				// cout << "--------\n";
				if (access((pathToFile + classLocation->default_file).c_str(), F_OK) == -1 && servers_sockets[server_socket].directory_listing == true) {
					// cout << "---auto index-----\n";
					listDirectoriesAsHtml(pathToFile);
					pathToFile = "./tmp.html";
				}
				else
					pathToFile += classLocation->default_file;
			}
			else {
				// cout << "pathToFile -----------> " << pathToFile << endl;
				if ((access(uri.c_str(), F_OK) == -1 && !endwith(uri, "/") && isDirectory(pathToFile)))
					goto redirect;
				if (isDirectory(pathToFile) && servers_sockets[server_socket].directory_listing == true) {
					listDirectoriesAsHtml(pathToFile);
					pathToFile = "./tmp.html";
				}
				/////////
				goto endd;
			}
			// cout << "FIRST" << endl;
			break;
		}
	}

	if(pathToFile.empty())
	{
		for(size_t i = 0 ; (i < servers_sockets[server_socket].location.size()); i++) {
			if (servers_sockets[server_socket].location[i].name == "/")
			{
				classLocation = &servers_sockets[server_socket].location[i];
				pathToFile = classLocation->path + uri;
				if(location == "/")
					pathToFile +=  "/" + classLocation->default_file;

				if ((access(uri.c_str(), F_OK) == -1 && !endwith(uri, "/") && isDirectory(pathToFile)))
				{
					if (access(uri.c_str(), F_OK) == -1) {
						status = 301;
						redirection = "Location: " +  uri + "/\r\n";
						pathToFile = "./404Error.html";
						// cout << "THIRD" << endl;
						goto endd;
					}
				}
				if (isDirectory(pathToFile)) {
					listDirectoriesAsHtml(pathToFile);
					pathToFile = "./tmp.html";
				}
					
				// cout << "SECOND" << endl;
				break ;
			}
		}
	}
	if(pathToFile.empty())
	{
		// error_404:
		
		status = 404;
		pathToFile = "./404Error.html";
		cout << "FOURTH" << endl;
	}
	endd:
	cout << "path to file :: "<< pathToFile << endl;
	return pathToFile;
}


void httpResponse::openTheAppropriateFile(string& redirection)
{
	string pathToFile;

	if (this->status == 200) {
		pathToFile = fillThePathFile(redirection);
		filename = pathToFile;
	}
	
	// cout << "filename is : " << filename << " and status is : " << status << endl;
	
	open_file:
	file.open(filename.c_str(), std::ifstream::ate|std::ifstream::binary);
	if(!file.is_open() || isDirectory(filename))
	{
		status = 404;
		filename = servers_sockets[server_socket].error_pages[0];
		if(file.is_open())
			file.close();
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
	string redirectLocation = "";
	openTheAppropriateFile(redirectLocation);
	
	//setting the fileSize to a stream
	std::ostringstream strm;
	strm << fileSize;
	
	std::ostringstream tmp;
	string my_status;
	tmp << status;
	my_status = tmp.str();
	if (endwith(filename, ".html"))
		header = "HTTP/1.1 " + my_status + " " + status_message[status] + "\r\n"+redirectLocation+"Content-Type: text/html; charset=UTF-8\r\nContent-Length: "+ strm.str() + "\r\n\r\n";
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
#include "signal.h"//TODO: MINOR
void	httpResponse::execute_cgi() {
	
	if(file.is_open())
		file.close();
	cout << "CGI IS DETECTED\n";
	int filefd = 0;
	int pid = fork();
	if (pid == 0) {
		filefd = open("cgi.html", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		dup2(filefd, 1);
		close(filefd);
		char *argv[2];
		argv[0] = (char *)filename.c_str();
		argv[1] = NULL;
		if (execve(filename.c_str(), argv, envv) == -1) {
			kill(getpid(), SIGKILL);
		}
	} else if (pid < 0) {
		cout << "Error fork" << endl;
	} else {
		int monitor_process_id = fork();
		if (monitor_process_id == 0) {
			sleep(5);
			// cout << "---------------()---------------\n";
			kill(pid, SIGTERM);
			kill(getpid(), SIGKILL);
		}
		else if (monitor_process_id < 0)
			cout << "Error fork" << endl;
		else {
			int status;
			waitpid(pid, &status, 0);
			kill(monitor_process_id, SIGKILL);
			if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGKILL)) {
				// cout << "---------------501 error---------------\n";
				this->status = 501;
				filename = "./501.html";
			}
			else if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGTERM)) {
				// cout << "---------------408 error---------------\n";
				this->status = 408;
				filename = "./408.html";
			}
			else
				filename = "./cgi.html";
		}
	}
	cout << "CGI filename is : " << filename << " and status is : " << status << endl;
	file.open(filename.c_str(), std::ifstream::ate|std::ifstream::binary);
	if(!file.is_open())
	{
		filename = servers_sockets[server_socket].error_pages[0];
		file.open(filename.c_str(), std::ifstream::ate|std::ifstream::binary);
		if(!file.is_open())
			throw (std::runtime_error("Coudn't open the Error Page"));
	}

	filePos = 0;
	fileSize = file.tellg();
	if (!file.good())
		throw (std::runtime_error("Coudn't open the Error Page"));
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