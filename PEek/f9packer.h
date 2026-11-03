#pragma once
#include "f1mainmodule.h"
#include <Windows.h>

void DetectPacker(PEResult& result, LPVOID bufferp, IMAGE_NT_HEADERS* nt);