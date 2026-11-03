#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>

void tlhelp10() {
    int inputuser;

    HANDLE hlistproc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hlistproc == INVALID_HANDLE_VALUE) {
        std::cout << "issue: " << GetLastError() << std::endl;
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hlistproc, &pe32)) {
        std::cout << "issue: " << GetLastError() << std::endl;
        CloseHandle(hlistproc);
        return;
    }

    do {
        std::wcout << pe32.szExeFile
            << L" PID: "
            << pe32.th32ProcessID
            << std::endl;

    } while (Process32Next(hlistproc, &pe32));

    CloseHandle(hlistproc);

}