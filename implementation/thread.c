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
#include <stdlib.h>

#ifdef _WIN32

#include <windows.h>

thread_t create_thread(void *(*func)(void *), void *arg) {
	return (thread_t)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, arg, 0, NULL);
}

void *join_thread(thread_t thread) {
	void *data = (void *)WaitForSingleObject((HANDLE)thread, INFINITE);
	CloseHandle((HANDLE)thread);

	return data;
}

void exit_thread(void *data) {
	ExitThread((DWORD)data);
}

mutex_t create_mutex(void) {
	return (mutex_t)CreateMutex(NULL, FALSE, NULL);
}

int destroy_mutex(mutex_t mutex) {
	return CloseHandle((HANDLE)mutex);
}

int lock_mutex(mutex_t mutex) {
	return (int)WaitForSingleObject((HANDLE)mutex, INFINITE);
}

int unlock_mutex(mutex_t mutex) {
	return (int)ReleaseMutex((HANDLE)mutex);
}

#else

#include <pthread.h>

thread_t create_thread(void *(*func)(void *), void *arg) {
	pthread_t thread;
	pthread_reate(&thread, NULL, func, arg);

	return (thread_t)thread;
}

void *join_thread(thread_t thread) {
	void *data;
	pthread_join((pthread_t)thread, &data);

	return data;
}

void exit_thread(void *data) {
	pthread_exit(data);
}

mutex_t create_mutex(void) {
	pthread_mutex_t *mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex, NULL);

	return (mutex_t)mutex;
}

int destroy_mutex(mutex_t mutex) {
	int result = pthread_mutex_destroy((pthread_mutex_t *)mutex);
	free(mutex);

	return result;
}

int lock_mutex(mutex_t mutex) {
	return pthread_mutex_lock((pthread_mutex_t *)mutex);
}

int unlock_mutex(mutex_t mutex) {
	return pthread_mutex_unlock((pthread_mutex_t *)mutex);
}

#endif
