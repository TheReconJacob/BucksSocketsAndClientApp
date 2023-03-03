#include <tchar.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"

int main(int argc, char* argv[]) {
	SOCKADDR_STORAGE from;
	int retval, fromlen, socket_type;
	char servstr[NI_MAXSERV], hoststr[NI_MAXHOST];

	SOCKET serverSocket, acceptSocket;
	int port = 55555;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0) {
		std::cout << "The Winsock dll not found!" << std::endl;
		return 0;
	}
	else {
		std::cout << "The Winsock dll found!" << std::endl;
		std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
	}
	
	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "socket() is OK!" << std::endl;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "bind() is OK!" << std::endl;
	}

	if (listen(serverSocket, 1) == SOCKET_ERROR)
		std::cout << "listen(): Error listening on socket " << WSAGetLastError() << std::endl;
	else
		std::cout << "listen() is OK, I'm waiting for connections..." << std::endl;
	
	fromlen = sizeof(socket_type);
	retval = getsockopt(serverSocket, SOL_SOCKET, SO_TYPE, (char*)&socket_type, &fromlen);
	fromlen = sizeof(from);
	acceptSocket = accept(serverSocket, (SOCKADDR*)&from, &fromlen);
	if (acceptSocket == INVALID_SOCKET) {
		std::cout << "accept failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}
	retval = getnameinfo((SOCKADDR*)&from,
		fromlen,
		hoststr,
		NI_MAXHOST,
		servstr,
		NI_MAXSERV,
		NI_NUMERICHOST | NI_NUMERICSERV);
	if (retval != 0) {
		std::cout << "getnameinfo failed: " << retval << std::endl;
		WSACleanup();
		return -1;
	}
	std::cout << "Accepted connection from host " << hoststr << " and port " << servstr << std::endl;
	system("pause");
	WSACleanup();

	return 0;
}