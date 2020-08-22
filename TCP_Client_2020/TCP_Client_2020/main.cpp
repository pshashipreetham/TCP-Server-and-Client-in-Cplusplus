#include <iostream>
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
	string ipAddress = "127.0.0.1";			//Ip Address of the Server
	int port = 54000;						//Listening port # on the server

	/*
	Steps for the client Server to Work: 
	* Step 1: Intiliaze WinSock
	* Step 2: Create Socket
	* Step 3: Fill in a hint Structure
	* Step 4: Connect to server
	* Step 5: Do-While loop to send and receive the data
	* Step 6: Gracefully close down Everything
	*/

	//Step 1: Initliaze the WinSock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int WSResult = WSAStartup(ver, &data);
	if (WSResult != 0)
	{
		cerr << "can't start winsock, ERR #" << WSResult << endl;
		return;
	}

	//Step 2: Create Socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create Socket, Err # " << WSAGetLastError<<endl;
		WSACleanup();
		return;
	}

	//Step 3: Fill in a hint Structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//Step 4:  Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't Connect to server, ERR # " << WSAGetLastError << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	//Step 5: Do-While loop to send and receive the data
	char  buf[4096];
	string userInput;
	do
	{
		/*
		*Prompt the user for some text
		* Send the text
		* Wait for response
		* Echo response to console
		*/

		// Step 1: Prompt the user for some text
		cout << "> ";
		getline(cin, userInput);
		
		//Send the text
		if (userInput.size()>0)
		{
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for the Response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0 )
				{
					cout << "SERVER> " <<string(buf,0,bytesReceived)<< endl;
				}
			}
		}
	} while (userInput.size() > 0);
	// Step 6: Gracefully close down Everything
	closesocket(sock);
	WSACleanup();
}