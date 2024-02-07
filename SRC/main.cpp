/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:59:49 by rennatiq          #+#    #+#             */
/*   Updated: 2024/02/06 13:13:59 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"

std::map<int, httpRequest> fdMapRead;
std::map<int, httpResponse> fdMapWrite;
std::map<int, Server> servers_sockets;
std::vector<int> deleteReadFd;
std::vector<int> deleteWriteFd;
fd_set theFdSetRead[NBOFCLIENTS];
fd_set theFdSetWrite[NBOFCLIENTS];
char **envv;

void webserve()
{
	struct timeval timout;
	timout.tv_sec = 5;
	timout.tv_usec = 0;

	while (1)
	{
		refresh_fd_set(theFdSetRead, theFdSetWrite);

		int ret = select(getMaxFd() + 1, theFdSetRead, theFdSetWrite, NULL, &timout);
		if (ret == -1)
		{
			cout << "select failed!!" << endl;
			clear_maps();
			continue;
		}
		else if (ret == 0)
		{
			for (std::map<int, httpRequest>::iterator it = fdMapRead.begin(); it != fdMapRead.end(); it++)
			{
				close(it->first);
				deleteReadFd.push_back(it->first);
			}
			continue;
		}

		for (std::map<int, Server>::iterator it = servers_sockets.begin(); it != servers_sockets.end(); it++)
		{
			if (FD_ISSET(it->first, theFdSetRead))
				acceptNewConnections(it->first);
		}
		for (std::map<int, httpRequest>::iterator it = fdMapRead.begin(); it != fdMapRead.end(); it++)
		{
			if (FD_ISSET(it->first, theFdSetRead))
				readTheRequest(it);
		}

		for (std::map<int, httpResponse>::iterator it = fdMapWrite.begin(); it != fdMapWrite.end(); it++)
		{
			if (FD_ISSET(it->first, theFdSetWrite))
				writeOnSocket(it);
		}
	}
}

int main(int __unused ac, char __unused **av, char **env)
{
	try
	{
		envv = env;

		parceConfFile cf;
		parce_conf_file(cf);
		init_status_code();
		createHtmlFile();

		connectSockets(cf);
		webserve();
		return 0;
	}
	catch (std::exception &e)
	{
		cout << e.what() << endl;
		return 1;
	}
	catch (...)
	{
		cout << "Error" << endl;
		return 1;
	}
}