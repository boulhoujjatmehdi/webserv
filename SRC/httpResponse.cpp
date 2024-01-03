/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 09:43:03 by eboulhou          #+#    #+#             */
/*   Updated: 2024/01/03 10:32:32 by aachfenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"
extern std::map<int, Server> servers_sockets;

	httpResponse::httpResponse(const httpResponse& obj): httpRequest(obj)
	{
		socket = obj.socket;
		header_sent = obj.header_sent;
		header = obj.header;
		request = obj.request;
		filename = obj.filename;

		cout << "filename : ("<< filename<< ")"<< endl;
		open_file:
		file.open(filename.c_str(), std::ifstream::ate|std::ifstream::binary);

		if(!file.is_open())
		{
			status = 404;
			filename = servers_sockets[server_socket].location[0].path + "/" + servers_sockets[server_socket].error_pages[0];
			goto open_file;
		}

		filePos = 0;
		fileSize = file.tellg();
		file.seekg(0);

		
	}

	httpResponse::~httpResponse()
	{
		if(file.is_open())
			file.close();
	}

	int httpResponse::sendChunk()
	{
		std::istringstream strm(request);
		string str;
		strm >> str;
		int sent_size, sentData;

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
			return 1;
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

httpResponse::httpResponse(const httpRequest& obj,string Filename): httpRequest(obj),filename(Filename), header_sent(0)
{
	if (uri == "/")
		uri = servers_sockets[server_socket].location[0].default_file;
	filename = servers_sockets[server_socket].location[0].path + "/" + uri;
	if (endwith(filename, ".html"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
	else if (endwith(filename, ".css"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: text/css; charset=UTF-8\r\n\r\n";
	else if (endwith(filename, ".png"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: image/png; charset=UTF-8\r\n\r\n";
	else if (endwith(filename, ".jpeg"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg; charset=UTF-8\r\n\r\n";
	else if (endwith(filename, ".jpg"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: image/jpg; charset=UTF-8\r\n\r\n";
	else if (endwith(filename, ".js"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: text/javascript; charset=UTF-8\r\n\r\n";
	else if (endwith(filename, ".js"))
		header = "HTTP/1.1 200 OK\r\nContent-Type: text/javascript; charset=UTF-8\r\n\r\n";
	else if (endwith(filename, ".cgi")) {

		// header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
		cout << "CGI IS DETECTED\n";
		int fd[2];
		if (pipe(fd) == -1)
			exit (10);
		int filefd = 0;
		pid_t pid = fork();
		if (pid == 0) {
			filefd = open("cgi.html", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
			cout << "CHILD IS BORN\n";
			close(fd[0]);
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

			// cerr << "filename is " << filename << endl;
			if (execve(filename.c_str(), argv, envp) == -1) {
				std::cerr << "Error execve" << endl;
				exit(1);
			}
		} else if (pid < 0) {
			cout << "Error fork" << endl;
		} else {
			// close(fd[1]);
			
			// char buffer[4096];
			// ssize_t n;
			// while ((n = read(filefd, buffer, sizeof(buffer) - 1)) > 0) {
			//     buffer[n] = '\0';  // Null-terminate the string
			//     // Now buffer contains some output from the child process.
			//     // You can write this to the client socket.
			// }
			// cout << "buffer is (" << buffer << ")" << endl;
			// if (n == -1)
			// 	exit(1);
			
			int status;
			waitpid(pid, &status, 0);
		}
		filename = "cgi.html";
	}
	else
		header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";

}