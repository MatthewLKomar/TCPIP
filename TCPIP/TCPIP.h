#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

struct ServerResults
{
	bool IsSuccessful;
	int BytesGiven; 
	std::string MessageRecieved;

};
//Library the implements the TCPIP for C++ 
class TCPIP
{
public:
	TCPIP(); //do we need a constructor?
	~TCPIP(); //do we need a destructor?

	SOCKET MakeSocket();
	void CloseSocket(SOCKET sock);
	sockaddr_in SetUpAddress_Client(std::string IpAddress, int Port); //Configures connecting socket address
	sockaddr_in SetUpAddress_Server(int Port); //Configures connecting socket address
	void MakeServerListen(SOCKET sock, sockaddr_in addr);
	SOCKET WaitForConnection(SOCKET sock);
	ServerResults ServerReceived(SOCKET ClientSock, char* buf, int BufSize);
	bool ConnectToServer(SOCKET sock, sockaddr_in addr);
	std::string SendToServer(SOCKET sock, std::string Info);
};
