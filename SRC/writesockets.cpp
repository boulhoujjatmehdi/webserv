/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writesockets.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rennatiq <rennatiq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 11:39:02 by rennatiq          #+#    #+#             */
/*   Updated: 2024/02/06 11:45:27 by rennatiq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INC/server.hpp"

extern std::map<int, httpRequest> fdMapRead;
extern std::vector<int> deleteWriteFd;

void writeOnSocket(std::map<int, httpResponse>::iterator& it)
{
	int commSocket, returnNumber;

	commSocket = it->first;			


	returnNumber = it->second.sendChunk();


	if (returnNumber == 1)//if the socket is closed by peer or some error occured in send().
	{
		close(commSocket);
		deleteWriteFd.push_back(commSocket);
			}
	else if(returnNumber == 2)//all good in sendChunk().
	{
		if(it->second.connection == true)//if the connection is on keep-alive.
		{
			
		fdMapRead.insert(std::make_pair(commSocket, httpRequest(commSocket, it->second.server_socket)));
				deleteWriteFd.push_back(commSocket);
		
		}
		else //if the connection is on close.
		{
			close(commSocket);
			deleteWriteFd.push_back(commSocket);
		}
	}
}

