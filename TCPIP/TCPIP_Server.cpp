#include <iostream>
#include "TCPIP.h"

int main()
{
	int port = 54000;
	TCPIP tcpip;
	SOCKET listening = tcpip.MakeSocket();
	if (!listening) return 0;
	// Bind the ip address and port to a socket
	sockaddr_in hint = tcpip.SetUpAddress_Server(port);
	tcpip.MakeServerListen(listening, hint);

	// Wait for a connection
	SOCKET clientSocket = tcpip.WaitForConnection(listening);
	// Close listening socket
	closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);
		ServerResults serverResults = tcpip.ServerReceived(clientSocket, buf, 4096);

		if (!serverResults.IsSuccessful)
		{
			break;
		}
		// Echo message back to client
		send(clientSocket, buf, serverResults.BytesGiven + 1, 0);
	}
	// Close the socket
	tcpip.CloseSocket(listening);
	return 1;
}
