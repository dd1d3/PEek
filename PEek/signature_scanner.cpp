#include <Windows.h>
#include "f6imports.h"

bool signaturefinder(IMAGE_NT_HEADERS* nt, LPVOID bufferp, short* pattern, size_t patternSize) {
    DWORD rva = 0;

    if (nt->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
        IMAGE_NT_HEADERS64* nt64 = (IMAGE_NT_HEADERS64*)nt;
        rva = nt64->OptionalHeader.AddressOfEntryPoint;
    }
    else if (nt->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
        IMAGE_NT_HEADERS32* nt32 = (IMAGE_NT_HEADERS32*)nt;
        rva = nt32->OptionalHeader.AddressOfEntryPoint;
    }

    DWORD offset = RvaToOffset(nt, rva);
    BYTE* entryBytes = (BYTE*)bufferp + offset;


    bool match = true;


    for (size_t i = 0; i < patternSize; i++)
    {
		if (pattern[i] == -1) { 
			continue;
		}
        if (entryBytes[i] != pattern[i]) {
            match = false;
            break;
        }
    }
    return match;
}