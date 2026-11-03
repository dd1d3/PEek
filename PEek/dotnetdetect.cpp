#include <windows.h>
#include "f1mainmodule.h"

void DetectDotNet(LPVOID bufferp, IMAGE_NT_HEADERS* nt, PEResult& result) {
	if (nt->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
		IMAGE_OPTIONAL_HEADER64* opt = (IMAGE_OPTIONAL_HEADER64*)&nt->OptionalHeader;

		if (opt->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress != 0 &&
			opt->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size != 0)
		{
			result.dotNetInfo = true;
		}
		else
		{
			result.dotNetInfo = false;
		}
	}

	if (nt->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
		IMAGE_OPTIONAL_HEADER32* opt = (IMAGE_OPTIONAL_HEADER32*)&nt->OptionalHeader;
		if (opt->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress != 0 &&
			opt->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size != 0)
		{
			result.dotNetInfo = true;
		}
		else
		{
			result.dotNetInfo = false;
		}
	}
}