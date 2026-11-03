#include "f6imports.h"

DWORD RvaToOffset(IMAGE_NT_HEADERS* nt, DWORD rva) {
    IMAGE_SECTION_HEADER* sec = IMAGE_FIRST_SECTION(nt);
    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        if (rva >= sec[i].VirtualAddress && rva < (sec[i].VirtualAddress + sec[i].Misc.VirtualSize)) {
            return (rva - sec[i].VirtualAddress) + sec[i].PointerToRawData;
        }
    }
    return 0;
}

void ParseImports(LPVOID bufferp, IMAGE_NT_HEADERS* nt, PEResult& result) {
    IMAGE_DATA_DIRECTORY* dataDir = nullptr;

    if (nt->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
        IMAGE_NT_HEADERS64* nt64 = (IMAGE_NT_HEADERS64*)nt;
        dataDir = nt64->OptionalHeader.DataDirectory;
    }
    else if (nt->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
    {
        IMAGE_NT_HEADERS32* nt32 = (IMAGE_NT_HEADERS32*)nt;
        dataDir = nt32->OptionalHeader.DataDirectory;
    }

    if (dataDir != nullptr) {
        DWORD importRva = dataDir[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
        if (importRva != 0)
        {
            DWORD importRaw = RvaToOffset(nt, importRva);
            if (importRaw != 0) {
                IMAGE_IMPORT_DESCRIPTOR* importDesc = (IMAGE_IMPORT_DESCRIPTOR*)((BYTE*)bufferp + importRaw);

                while (importDesc->Name != 0) {
                    DWORD nameRaw = RvaToOffset(nt, importDesc->Name);
                    if (nameRaw != 0) {
                        char* dllName = (char*)((BYTE*)bufferp + nameRaw);
                        result.imports.push_back(dllName);
                    }
                    importDesc++;
                }
            }
        }
    }
}