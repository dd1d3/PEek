#include <Windows.h>
#include <bcrypt.h>
#include <iostream>
#include <iomanip>
#include <sstream>


void f10sha1(HANDLE hFile, std::string& outSha1) {

	BCRYPT_ALG_HANDLE hAlg = NULL;
	BCRYPT_HASH_HANDLE hHash = NULL;
	BYTE pbOutput[20];

	NTSTATUS Hhash = BCryptOpenAlgorithmProvider(
		&hAlg,
		BCRYPT_SHA1_ALGORITHM,
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
	bool failed = false;

	while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0)
	{
		NTSTATUS st = BCryptHashData(hHash, buffer, bytesRead, 0);
		if (!BCRYPT_SUCCESS(st)) {
			std::cout << "issue" << std::endl;
			failed = true;
			break;
		}
	}

	SetFilePointerEx(hFile, pos, NULL, FILE_BEGIN);

	if (failed)
	{
		BCryptDestroyHash(hHash);
		BCryptCloseAlgorithmProvider(hAlg, 0);
		return;
	}

	NTSTATUS Hfin = BCryptFinishHash(hHash, pbOutput, sizeof(pbOutput), 0);
	if (!BCRYPT_SUCCESS(Hfin))
	{
		std::cout << "issue" << std::endl;
		BCryptDestroyHash(hHash);
		BCryptCloseAlgorithmProvider(hAlg, 0);
		return;
	}

	std::ostringstream oss;
	for (size_t i = 0; i < 20; i++)
	{
		oss << std::hex << std::setw(2) << std::setfill('0') << (int)pbOutput[i];
	}
	outSha1 = oss.str();

	BCryptDestroyHash(hHash);
	BCryptCloseAlgorithmProvider(hAlg, 0);
}