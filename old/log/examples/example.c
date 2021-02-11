#include "log.h"

int main(void) {
	LogRename("MyEpicLogger");
	LogSetAutoFileFlush(0);
	LogSetFatalExit(0);

	LogOpenFile("log.txt");

	Log(SevNone, "This should not have a prefix");
	Log(SevInfo, "Printf-like format ! %d %s", 100, "donuts");
	Log(SevWarn, "This is a warning");
	Log(SevError, "ERRRRRRRRRR");
	Log(SevFatal, "Ouch thats a fatal");
	Log(SevDebug, "You should only see this in debug mode");

	LogRename("Loader");
	Log(sinf, "Loading assets!");
	Log(sinf, "Loading levels!");
	LogRename("Engine");
	Log(sdeb, "Version 1.0");

	int i = 23132;
	LogAssert(serr, i, "i == 0");

	LogFlushFile();
	LogCloseFile();
	LogResetConsoleColor();
	return 0;
}