#include "TCPIP.h"

TCPIP::TCPIP()
{
}

TCPIP::~TCPIP()
{
}

SOCKET TCPIP::MakeSocket()
{
	//init winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if(wsResult != 0)
	{
		std::cerr << "Can't create socket, ERR #" << WSAGetLastError() << std::endl;
		return SOCKET();
	}
	//creat socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		CloseSocket(sock);
	}
	return sock;
}

void TCPIP::CloseSocket(SOCKET sock)
{
	//gracefully close down everything once we're done
	closesocket(sock);
	WSACleanup();
}

sockaddr_in TCPIP::SetUpAddress_Client(std::string IpAddress, int Port)
{
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(Port);
	inet_pton(AF_INET, IpAddress.c_str(), &hint.sin_addr);
	return hint;
}

sockaddr_in TCPIP::SetUpAddress_Server(int Port)
{
	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(Port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 
	return hint;
}

void TCPIP::MakeServerListen(SOCKET sock, sockaddr_in addr)
{
	bind(sock, (sockaddr*)&addr, sizeof(addr));
	// Tell Winsock the socket is for listening 
	listen(sock, SOMAXCONN);
}

SOCKET TCPIP::WaitForConnection(SOCKET sock)
{
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(sock, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << " connected on port " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
	}
	return clientSocket;
}

ServerResults TCPIP::ServerReceived(SOCKET ClientSock, char* buf, int BufSize)
{
	ServerResults serverResults;
	// Wait for client to send data
	int bytesReceived = recv(ClientSock, buf, BufSize, 0);
	if (bytesReceived == SOCKET_ERROR)
	{
		std::cerr << "Error in recv(). Quitting" << std::endl;
		serverResults.IsSuccessful = false;
		return serverResults;
	}

	if (bytesReceived == 0)
	{
		std::cout << "Client disconnected " << std::endl;
		serverResults.IsSuccessful = false;
		return serverResults;
	}

	std::string received = std::string(buf, 0, bytesReceived);
	std::cout << received << std::endl;
	serverResults.IsSuccessful = true;
	serverResults.BytesGiven = bytesReceived;
	serverResults.MessageRecieved = received;
	return serverResults;
}

bool TCPIP::ConnectToServer(SOCKET sock, sockaddr_in addr)
{
	int connResult = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		CloseSocket(sock);
		return false;
	}
	return true;
}

std::string TCPIP::SendToServer(SOCKET sock, std::string Info)
{
	//Send text
	char buf[4096];
	std::string Response = "";
	int sendResult = send(sock, Info.c_str(), Info.size() + 1, 0);
	if(sendResult != SOCKET_ERROR)
	{
		//Wait for a response
		ZeroMemory(buf, 4096);//garbage collecting
		int bytesReceived = recv(sock, buf, 4096, 0);
		if (bytesReceived > 0)
		{
			//Echo response to console
			Response = std::string(buf, 0, bytesReceived).c_str();
		}
	}
	return Response;
}
