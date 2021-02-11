/* cutils.h - simple, lightweight, easy to use utils
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

#ifndef CUTILS_H
#define CUTILS_H

/**
 * Environment
*/

/* Enable/disable CLI input echoing */
void setecho(int state);

/**
 * Threading
*/

typedef void * thread_t;
typedef void * mutex_t;

/* Creates a running thread and returns its handle */
thread_t create_thread(void *(*func)(void *), void *arg);

/* Waits until specified thread terminates */
void *join_thread(thread_t thread);

/* Exits current thread */
void exit_thread(void *data);

/* Creates a mutex */
mutex_t create_mutex(void);

/* Destroys a mutex */
int destroy_mutex(mutex_t mutex);

/* Locks a mutex */
int lock_mutex(mutex_t mutex);

/* Unlocks a mutex */
int unlock_mutex(mutex_t mutex);

/**
 * Sockets
*/

enum socket_type {
	SOCKET_TCP
};

typedef int socket_t;

struct client_info {
	char address[64];
	unsigned short port;
};

/* Open a socket using specified backend */
socket_t open_socket(int type);

/* Connect a socket to the specified remote host */
int connect_socket(socket_t socket, char *address, unsigned short port);

/* Bind a socket to the specified port and start listening */
int socket_listen(socket_t socket, unsigned short port);

/* Accept a socket and store info into specified client_info structure */
socket_t accept_socket(socket_t socket, struct client_info *info);

/* Send data through connected socket */
int socket_send(socket_t socket, char *buf, int size);

/* Receive data from connected socket */
int socket_recv(socket_t socket, char *buf, int size);

/* Close a socket */
void close_socket(socket_t socket);

#endif
