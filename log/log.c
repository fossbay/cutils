/* Log.c - Simple and easy to use console logger
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

#include "log.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
	char * name;
	FILE * stream;
	_Bool	 aflush;
	_Bool  fexit;
} LogInfo;

//Global storage for the log's info
static LogInfo LOG_MAIN_INFO = {
	"Logger", NULL, 1, 0
};

//Parse a severity level into a char *
char * LogParseSeverity(LogSeverity sev) {
	switch (sev) {
		case 0: return "INFO";
		case 1: return "WARN";
		case 2:	return "ERROR";
		case 3: return "FATAL";
		case 4: return "DEBUG";
		default: return "UNKNOWN";
	}
}

//Get the logger's prefix based on the severity
int LogPrefix(LogSeverity sev, char * current) {
	char buffer[100];
	time_t now = time(0);
	strftime(buffer, 100, "%Y-%m-%d %H:%M:%S.000", localtime(&now));
	return sprintf(current, "%s %s %s: ", buffer, LOG_MAIN_INFO.name, LogParseSeverity(sev));
}

void LogPrintConsoleColor(LogSeverity sev) {
	//If LOG_NO_COLORS the colors wont be printed
#if !defined(LOG_NO_COLORS)
	char * color;
	switch (sev) {
		case 1: color = "\033[0;33m"; break;
		case 2:	color = "\033[1;31m"; break;
		case 3:	color = "\033[0;31m"; break;
		case 4:	color = "\033[0;36m"; break;
		default: color = "\033[0m";
	}
	printf(color);
#endif
}

void LogResetConsoleColor(void) {
	printf("\033[0m");
}

void Log(LogSeverity sev, char * fmt, ...) {
#if !defined(_DEBUG) && !defined(DEBUG) && !defined(BUILD_DEBUG)
	//If debug is disabled and the log level == debug return to not print the message
	if (lvl==ldebug)  return;
#endif

	char buffer[1000];
	char * current = buffer;

	//If the severity is >= to SevNone use the prefix
	if (sev>=sinf)
		current += LogPrefix(sev, current);

	//Store the arguments into the buffer
	va_list args;
	va_start(args, fmt);
	current += vsprintf(current, fmt, args);
	va_end(args);

	*current++ = '\n';
	*current = 0;

	size_t lenght = current-buffer;

#ifdef FORCE_SEQUENTIAL
	flockfile(stdout);
#endif

	//Overriding the color
	LogPrintConsoleColor(sev);

	//Writing into stdout
	fwrite(buffer, 1, lenght, stdout);

	//If the file has been opened then write into it
	if (LOG_MAIN_INFO.stream) {
		fwrite(buffer, 1, lenght, LOG_MAIN_INFO.stream);
		//Flush if auto flush is on
		if (LOG_MAIN_INFO.aflush)
			LogFlushFile();
	}

#ifdef FORCE_SEQUENTIAL
	funlockfile(stdout);
#endif

	//If Fatal-Exit is on and the severity == to fatal shutdown
	if (sev==SevFatal&&LOG_MAIN_INFO.fexit) {

		//Clearing because why not
		LogResetConsoleColor();
		exit(8);
	}
}

void LogOpenFile(char * file) {
	LOG_MAIN_INFO.stream = fopen(file, "wb");
	if (LOG_MAIN_INFO.stream==NULL)
		Log(SevError, "Failed to open the file %s, cannot output log inside it", file);
}

void LogCloseFile(void) {
	fclose(LOG_MAIN_INFO.stream);
	LOG_MAIN_INFO.stream = NULL;
}

void LogFlushFile(void) {
	fflush(LOG_MAIN_INFO.stream);
}

void LogRename(char * name) {
	LOG_MAIN_INFO.name = name;
}

void LogSetFatalExit(_Bool fs) {
	LOG_MAIN_INFO.fexit = fs;
}

void LogSetAutoFileFlush(_Bool af) {
	LOG_MAIN_INFO.aflush = af;
}
