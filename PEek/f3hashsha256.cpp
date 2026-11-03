#include <windows.h>
#include <bcrypt.h>
#include <iostream>
#include <iomanip>
#include <sstream>

void f3hash256(HANDLE hFile, std::string& outSha256)
{
    BCRYPT_ALG_HANDLE hAlg = NULL;
    BCRYPT_HASH_HANDLE hHash = NULL;

    BYTE pbOutput[32];

    NTSTATUS Hhash256 = BCryptOpenAlgorithmProvider(
        &hAlg,
        BCRYPT_SHA256_ALGORITHM,
        NULL,
        0);

    if (!BCRYPT_SUCCESS(Hhash256))
    {
        std::cout << "issue" << std::endl;
        return;
    }

    NTSTATUS Hcrhash256 = BCryptCreateHash(
        hAlg,
        &hHash,
        NULL,
        0,
        NULL,
        0,
        0);

    if (!BCRYPT_SUCCESS(Hcrhash256))
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
            std::cout << "issue" << std::endl;
            BCryptDestroyHash(hHash);
            BCryptCloseAlgorithmProvider(hAlg, 0);
            SetFilePointerEx(hFile, pos, NULL, FILE_BEGIN);
            return;
        }
    }

    SetFilePointerEx(hFile, pos, NULL, FILE_BEGIN);

    NTSTATUS HhashFinish = BCryptFinishHash(hHash, pbOutput, 32, 0);
    if (!BCRYPT_SUCCESS(HhashFinish))
    {
        std::cout << "issue" << std::endl;
        BCryptDestroyHash(hHash);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        SetFilePointerEx(hFile, pos, NULL, FILE_BEGIN);
        return;
    }

    std::ostringstream oss;
    for (size_t i = 0; i < 32; i++)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)pbOutput[i];
    outSha256 = oss.str();

    BCryptDestroyHash(hHash);
    BCryptCloseAlgorithmProvider(hAlg, 0);
}