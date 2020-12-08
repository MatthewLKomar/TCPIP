// TCPIP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "TCPIP.h"

using namespace std; 

int main1()
{
	TCPIP tcpip; 
	string ipAddress = "127.0.0.1";
	int port = 54000;

	SOCKET sock = tcpip.MakeSocket();


	//fill in a hint structure
	sockaddr_in hint = tcpip.SetUpAddress_Client(ipAddress, port);
	
	if (!tcpip.ConnectToServer(sock, hint))
	{
		std::cerr << "Couldn't connect to the server" << std::endl;
	}

	//do while loop to send and recieve data
	char buf[4096];
	string userInput; 
	do {
		// Prompt user for text
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0) //Make sure the user has typed in something
		{
			std::cout << "Server> " << tcpip.SendToServer(sock, userInput) << std::endl;
		}
		
	} while (userInput.size() > 0);
	//gracefully close down everything once we're done
	tcpip.CloseSocket(sock);
	return 1;
}

