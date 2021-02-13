#include "socket.h"
#include <iostream>

int main()
{
	socket_t socket;
	socket = open_socket(SOCKET_TCP);
	connect_socket(socket, (char*)"127.0.0.1", 80);


	std::string a = "Hello, World!";
	socket_send(socket, (char*)a.c_str(), a.length());
	close_socket(socket);
	return 0;
}