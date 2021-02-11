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

void setecho(int state) {
	HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);

	DWORD mode;
	GetConsoleMode(handle, &mode);

	if(!state)
		mode &= ~ENABLE_ECHO_INPUT;
	else
		mode |= ENABLE_ECHO_INPUT;

	SetConsoleMode(handle, mode);
}

#else

#include <unistd.h>
#include <termios.h>

void setecho(int state) {
	struct termios tty;

	tcgetattr(STDIN_FILENO, &tty);

	if(!state)
		tty.c_lflag &= ~(0u | ECHO);
	else
		tty.c_lflag |= (0u | ECHO);

	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

#endif
