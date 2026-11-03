#include "f1mainmodule.h"
#include "timedatestampcheck.h"
#include <time.h>

bool TimeStamp(LPVOID bufferp, IMAGE_NT_HEADERS* nt, PEResult& result) {
	time_t t = (time_t)nt->FileHeader.TimeDateStamp;
	struct tm tmBuf;
	gmtime_s(&tmBuf, &t);
	char buf[64];
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tmBuf);
	result.timeDateStamp = buf;
	return true;
}