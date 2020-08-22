#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "WS2_32.lib")

using namespace std;


void main()
{
	/*
	* Intiliaze winsock
	* Create a Socket
	* Bind the Ip Addressand port to a Socket
	* Tell Winsock the socket is for for Listening 
	* Wait for a connection 
	* Close listening socket 
	* While Loop: accept and the echo message back to the client 
	* Close the socket 
	* Cleanup Socket
	*/

	//Step 1:Intiliaze winsock
	WSADATA WSData;
	WORD ver = MAKEWORD(2, 2);
	int wsok = WSAStartup(ver, &WSData);
	if (wsok != 0)
	{
		cerr << "Can't intiliaze winsock ! Quitting" << endl;
		return;
	}

	//Step 2:Create a Socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't Create a socket! Quitting" << endl;
		return;
	}

	//Step 3:Bind the Ip Addressand port to a Socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;				// Could Also use inet_pton
	bind(listening, (sockaddr *)&hint, sizeof(hint));

	//Step 4:Tell Winsock the socket is for for Listening
	listen(listening, SOMAXCONN);

	//Step 5:Wait for a connection
	sockaddr_in client;
	int clientsize = sizeof(client);
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsize);
	char host[NI_MAXHOST];			//Client's remote name
	char service[NI_MAXSERV];		//Service (i.e port) the client is connect on
	ZeroMemory(host, NI_MAXHOST);	//same as memser(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV,0) ==  0)
	{
		cout << host << " connected on port  " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}

	//Step 6:Close listening socket
	closesocket(listening);

	//Step 7:While Loop : accept and the echo message back to the client
	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);
		//Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << " Error in recv(). QUitting " << endl;
			break;
		}
		if (bytesReceived == 0)
		{
			cerr << " Client Disconnected " << endl;
			break;
		}
		cout << string(buf, 0, bytesReceived) << endl;
		send(clientSocket, buf, bytesReceived + 1, 0);
		//Echo message back to client
	}

	//Step 8:Close the socket
	closesocket(clientSocket);

	//Step 9:Cleanup Socket
	WSACleanup();

	system("pause");

}