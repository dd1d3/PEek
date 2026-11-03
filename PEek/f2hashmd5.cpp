#include <windows.h>
#include <bcrypt.h>
#include <iostream>
#include <iomanip>
#include <sstream>

void fhashfinder(HANDLE hFile, std::string& outMd5) {

    BCRYPT_ALG_HANDLE hAlg = NULL;
    BCRYPT_HASH_HANDLE hHash = NULL;

    BYTE pbOutput[16];

    NTSTATUS Hhash = BCryptOpenAlgorithmProvider(
        &hAlg,
        BCRYPT_MD5_ALGORITHM,
        NULL,
        0);

    if (!BCRYPT_SUCCESS(Hhash))
    {
        std::cout << "issue" << std::endl;
        return;
    }

    NTSTATUS Hcrhash = BCryptCreateHash(
        hAlg,
        &hHash,
        NULL,
        0,
        NULL,
        0,
        0);

    if (!BCRYPT_SUCCESS(Hcrhash))
    {
        std::cout << "issue" << std::endl;
        BCryptCloseAlgorithmProvider(hAlg, 0);
        return;
    }

    LARGE_INTEGER pos = {};
    SetFilePointerEx(hFile, pos, &pos, FILE_CURRENT);

    LARGE_INTEGER zero = {};
    SetFilePointerEx(hFile, zero, NULL, FILE_BEGIN);

    BYTE buffer[4096];
    DWORD bytesRead = 0;

    while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0)
    {
        NTSTATUS st = BCryptHashData(hHash, buffer, bytesRead, 0);
        if (!BCRYPT_SUCCESS(st)) {
            std::cout << "issue BCryptHashData" << std::endl;
            BCryptDestroyHash(hHash);
            BCryptCloseAlgorithmProvider(hAlg, 0);
            SetFilePointerEx(hFile, pos, NULL, FILE_BEGIN);
            return;
        }
    }

    SetFilePointerEx(hFile, pos, NULL, FILE_BEGIN);

    NTSTATUS HhashFinish = BCryptFinishHash(hHash, pbOutput, 16, 0);
    if (!BCRYPT_SUCCESS(HhashFinish))
    {
        std::cout << "issue" << std::endl;
        BCryptDestroyHash(hHash);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        SetFilePointerEx(hFile, pos, NULL, FILE_BEGIN);
        return;
    }

    std::ostringstream oss;
    for (size_t i = 0; i < 16; i++)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)pbOutput[i];
    outMd5 = oss.str();

    BCryptDestroyHash(hHash);
    BCryptCloseAlgorithmProvider(hAlg, 0);
}