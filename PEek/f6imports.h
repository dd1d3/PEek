#pragma once
#include <Windows.h>
#include "f1mainmodule.h"

DWORD RvaToOffset(IMAGE_NT_HEADERS* nt, DWORD rva);
void ParseImports(LPVOID bufferp, IMAGE_NT_HEADERS* nt, PEResult& result);