/*
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

#include "cutils.h"

#ifdef _WIN32

#include <windows.h>

socket_t open_socket(int type) {
	socket_t s = -1;

	WSADATA data;
	if(WSAStartup(MAKEWORD(2, 2), &data) != 0)
		return s;

	switch(type) {
		case SOCKET_TCP:
			s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			break;
	}

	return s;
}

int connect_socket(socket_t socket, char *address, unsigned short port) {
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;

	addr.sin_port = port;
	inet_pton(AF_INET, address, &addr.sin_addr);

	return connect(socket, (struct sockaddr *)&addr, sizeof(addr));
}

int socket_listen(socket_t socket, unsigned short port) {
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;

	addr.sin_port = port;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	if(bind(socket, (struct sockaddr *)&addr, sizeof(addr)) != 0)
		return -1;

	return listen(socket, SOMAXCONN);
}

socket_t accept_socket(socket_t socket, struct client_info *info) {
	socket_t client = -1;

	struct sockaddr_in addr;
	int addr_size = sizeof(addr);

	client = accept(socket, (struct sockaddr *)&addr, &addr_size);

	if(client == -1)
		return client;

	if(info != NULL) {
		info->port = addr.sin_port;
		inet_ntop(AF_INET, &addr.sin_addr, info->address, INET_ADDRSTRLEN);
	}

	return client;
}

int socket_send(socket_t socket, char *buf, int size) {
	return send(socket, buf, size, 0);
}

int socket_recv(socket_t socket, char *buf, int size) {
	return recv(socket, buf, size, 0);
}

void close_socket(socket_t socket) {
	closesocket(socket);
}

#else

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

socket_t open_socket(int type) {
	socket_t socket = -1;

	switch(type) {
		case SOCKET_TCP:
			socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			break;
	}

	return socket;
}

int connect_socket(socket_t socket, char *address, unsigned short port) {
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;

	addr.sin_port = port;
	inet_pton(AF_INET, address, &(addr.sin_addr));

	return connect(socket, (struct sockaddr *)&addr, sizeof(addr));
}

int socket_listen(socket_t socket, unsigned short port) {
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;

	addr.sin_port = port;
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(socket, (struct sockaddr *)&addr, sizeof(addr)) != 0)
		return -1;

	return listen(socket, SOMAXCONN);
}

socket_t accept_socket(socket_t socket, struct client_info *info) {
	socket_t client = -1;

	struct sockaddr_in addr;
	int addr_size = sizeof(addr);

	client = accept(socket, (struct sockaddr *)&addr, &addr_size);

	if(client == -1)
		return -1;

	if(info != NULL) {
		info->port = addr.sin_port;
		inet_ntop(AF_INET, &(addr.sin_addr), info->address, INET_ADDRSTRLEN);
	}

	return client;
}

int socket_send(socket_t socket, char *buf, int size) {
	return send(socket, buf, size, 0);
}

int socket_recv(socket_t socket, char *buf, int size) {
	return recv(socket, buf, size, 0);
}

void close_socket(socket_t socket) {
	close(socket);
}

#endif
