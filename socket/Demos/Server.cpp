#include "socket.h"
#include <iostream>


int main()
{
	socket_t socket = open_socket(SOCKET_TCP);
	socket_listen(socket,80);

 	char buffer[1024]{ 0 };
	socket = accept_socket(socket, NULL);
	std::cout << " == Client Recieved ==\n";

	
	socket_recv(socket, buffer,sizeof(buffer));


	std::cout << buffer << "\n";
	close_socket(socket);
	return 0;
}

