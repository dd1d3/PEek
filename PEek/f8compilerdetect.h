#pragma once
#include "f1mainmodule.h"
#include <Windows.h>

void DetectCompiler(LPVOID bufferp, IMAGE_NT_HEADERS* nt, PEResult& result);