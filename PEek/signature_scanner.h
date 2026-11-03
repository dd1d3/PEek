#pragma once
#include <Windows.h>

bool signaturefinder(IMAGE_NT_HEADERS* nt, LPVOID bufferp, short* pattern, size_t patternSize);