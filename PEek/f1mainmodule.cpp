#include <Windows.h>
#include <iostream>
#include <fileapi.h>
#include <commdlg.h>
#include <winbase.h>
#include <sstream>
#include "f1mainmodule.h"
#include "f2hashmd5.h"
#include "f3hashsha256.h"
#include "f4enthropy.h"
#include "f5strings.h"
#include "f6imports.h"
#include "f7section.h"
#include "f8compilerdetect.h"
#include "f9packer.h"
#include "F10sha1.h"
#include "signature_scanner.h"
#include "timedatestampcheck.h"
#include "dotnetdetect.h"

void AnalyzePE(const char* fileName, PEResult& result) {
    result = {};
    result.filePath = fileName;

    HANDLE Hcreaproc = CreateFileA(
        fileName,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (Hcreaproc == INVALID_HANDLE_VALUE)
    {
        result.error = "Cannot open file";
        return;
    }

    LARGE_INTEGER fileSizel = {};
    if (!GetFileSizeEx(Hcreaproc, &fileSizel))
    {
        result.error = "Cannot get file size";
        CloseHandle(Hcreaproc);
        return;
    }
    size_t fileSize = (size_t)fileSizel.QuadPart;

    fhashfinder(Hcreaproc, result.md5);
    f3hash256(Hcreaproc, result.sha256);
    f10sha1(Hcreaproc, result.sha1);

    HANDLE hmapfilea = CreateFileMappingA(
        Hcreaproc,
        NULL,
        PAGE_READONLY,
        0,
        0,
        NULL);

    if (hmapfilea == NULL) {
        result.error = "Mapping issue";
        CloseHandle(Hcreaproc);
        return;
    }

    LPVOID bufferp = MapViewOfFile(
        hmapfilea,
        FILE_MAP_READ,
        0,
        0,
        0);

    if (bufferp == NULL)
    {
        result.error = "Map issue";
    }
    else
    {
        if (fileSize < sizeof(IMAGE_DOS_HEADER))
        {
            result.error = "File too small for DOS header";
            UnmapViewOfFile(bufferp);
            CloseHandle(hmapfilea);
            CloseHandle(Hcreaproc);
            return;
        }

        IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)bufferp;
        if (dos->e_magic != IMAGE_DOS_SIGNATURE)
        {
            result.error = "Not a valid PE file";
            UnmapViewOfFile(bufferp);
            CloseHandle(hmapfilea);
            CloseHandle(Hcreaproc);
            return;
        }

        if (dos->e_lfanew < 0 ||
            (size_t)dos->e_lfanew + sizeof(IMAGE_NT_HEADERS) > fileSize)
        {
            result.error = "out of bounds";
            UnmapViewOfFile(bufferp);
            CloseHandle(hmapfilea);
            CloseHandle(Hcreaproc);
            return;
        }

        IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)((BYTE*)bufferp + dos->e_lfanew);
        if (nt->Signature != IMAGE_NT_SIGNATURE)
        {
            result.error = "Invalid signature";
            UnmapViewOfFile(bufferp);
            CloseHandle(hmapfilea);
            CloseHandle(Hcreaproc);
            return;
        }

        result.validPE = true;
        result.is64bit = (nt->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC);

        std::ostringstream oss;
        oss << "0x" << std::hex << nt->FileHeader.Machine;
        result.machine = oss.str();

        TimeStamp(bufferp, nt, result);
        ParseSections(bufferp, nt, result);
        ParseStrings(bufferp, nt, result);
        ParseImports(bufferp, nt, result);
        DetectDotNet(bufferp, nt, result);
        DetectCompiler(bufferp, nt, result);
		DetectPacker(result, bufferp, nt);
        UnmapViewOfFile(bufferp);
    }

    CloseHandle(hmapfilea);
    CloseHandle(Hcreaproc);
}