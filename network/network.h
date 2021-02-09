/* network.h - simple, lightweight, cross platform networking
 *
 * Copyright (c) 2021 Fossbay
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef NETWORK_H
#define NETWORK_H

/*---------------------------------------------------------------------------*/
/*                              Data Structures                              */
/*---------------------------------------------------------------------------*/

enum socket_type
{
	SOCKET_TCP
};

typedef int socket_t;

struct client_info
{
	char address[16];
	unsigned int port;
};

/*---------------------------------------------------------------------------------*/
/*                              Function Declarations                              */
/*---------------------------------------------------------------------------------*/

/**
 * Open a native socket with the specified
 * networking backend.
*/
socket_t open_socket(int type);

/**
 * Connect a socket to a remote host.
*/
int connect_socket(socket_t socket, char *address, unsigned short port);

/**
 * Bind a native socket to the specified port
 * and start listening for connections.
*/
int socket_listen(socket_t socket, unsigned short port);

/**
 * Accept a socket connection.
*/
socket_t accept_socket(socket_t socket, struct client_info *info);

/**
 * Send data to a remote socket.
*/
int socket_send(socket_t socket, char *buf, int size);

/**
 * Receive data from a remote socket.
*/
int socket_recv(socket_t socket, char *buf, int size);

/**
 * Close a native socket.
*/
void close_socket(socket_t socket);

/*----------------------------------------------------------------------------*/
/*                           Windows Implementation                           */
/*----------------------------------------------------------------------------*/

#if defined(_WIN32)

#include <windows.h>



#endif

/*---------------------------------------------------------------------------*/
/*                            Unix Implementation                            */
/*---------------------------------------------------------------------------*/

#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

socket_t open_socket(int type)
{
	socket_t s = -1;
	
	switch (type)
	{
		case SOCKET_TCP:
		{
			s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			break;
		}
	}

	return s;
}

int connect_socket(socket_t socket, char *address, unsigned int port)
{
	struct sockaddr_in addr = {0};

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(address);

	return connect(socket, (struct sockaddr *)&addr, sizeof(addr));
}

int socket_listen(socket_t socket, unsigned int port)
{
	struct sockaddr_in addr = {0};

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) return -1;

	return listen(socket, 1);
}

socket_t accept_socket(socket_t socket, struct client_info *info)
{
	struct sockaddr_in addr = {0};

	int sock = accept(socket, &addr, sizeof addr);

	if(socket != -1)
	{
		if(info != NULL)
		{
			
		}
		
		return sock;
	}
	else return -1;
}

#endif

#endif /* NETWORK_H */
