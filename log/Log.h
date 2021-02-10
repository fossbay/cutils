/* Log.h - Simple and easy to use console logger
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
 * 
*/

#if !defined(_HEADER_LOG)
#define _HEADER_LOG

//Log's severity types
typedef enum {
	SevNone = -1,
	SevInfo = 0,
	SevWarn = 1,
	SevError = 2,
	SevFatal = 3,
	SevDebug = 4
} LogSeverity;

//Short-typing macros for severity types
#define snon SevNone
#define sinf SevInfo
#define swar SevWarn
#define serr SevError
#define sfat SevFatal
#define sdeb SevDebug

//The log function
//Uses printf() format
extern void Log(LogSeverity sev, char * fmt, ...);

//Rename the log's name
//DEFAULT - "Logger"
extern void LogRename(char * name);

//Reset the console's color
//After calling Log() the console's color gets overridden based on the severity level
//If you want to reset it you have to do it MANUALLY due to performance optim
extern void LogResetConsoleColor(void);

//Enable/Disable Fatal-Exit
//If true when calling Log() with fatal severity the app will shutdown with exitcode 8
//Default - FALSE
extern void LogSetFatalExit(_Bool fs);

//Enable/Disable Auto-FileFlush
//If true after calling Log() the file will be flushed
//Default - TRUE
extern void LogSetAutoFileFlush(_Bool af);

//Open a file for the log to print into
extern void LogOpenFile(char * file);

//Close the stream with the log's file when you are done logging
extern void LogCloseFile(void);

//Flush the log's file
extern void LogFlushFile(void);

#include <assert.h>

//If a test fails, log a message with severity
#define LogAssert(sev, test, ...) if(!test) { Log(sev, __VA_ARGS__); assert(0); }

#endif