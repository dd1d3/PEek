#include "f5strings.h"
#include <unordered_set>

void ParseStrings(LPVOID bufferp, IMAGE_NT_HEADERS* nt, PEResult& result) {
    std::unordered_set<std::string> seen;
    IMAGE_SECTION_HEADER* sec = IMAGE_FIRST_SECTION(nt);

    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++)
    {
        char name[9] = { 0 };
        memcpy(name, sec[i].Name, 8);

        if (strcmp(name, ".rdata") == 0)
        {
            BYTE* fileBase = (BYTE*)bufferp;
            BYTE* sectionData = fileBase + sec[i].PointerToRawData;
            std::string tempStr;

            for (DWORD a = 0; a < sec[i].SizeOfRawData; a++) {
                BYTE b = sectionData[a];

                if (b >= 0x20 && b <= 0x7E) {
                    tempStr += (char)b;
                }
                else
                {
                    if (tempStr.size() >= 10) {
                        bool hasLetter = false;
                        for (char c : tempStr) {
                            if (isalpha((unsigned char)c)) { hasLetter = true; break; }
                        }
                        bool onlyDotDigit = true;
                        for (char c : tempStr) {
                            if (!isdigit((unsigned char)c) && c != '.') { onlyDotDigit = false; break; }
                        }
                        if (hasLetter && !onlyDotDigit && seen.find(tempStr) == seen.end()) {
                            seen.insert(tempStr);
                            result.foundStrings.push_back(tempStr);
                        }
                    }
                    tempStr.clear();
                }
            }

            if (tempStr.size() >= 10) {
                bool hasLetter = false;
                for (char c : tempStr) {
                    if (isalpha((unsigned char)c)) { hasLetter = true; break; }
                }
                bool onlyDotDigit = true;
                for (char c : tempStr) {
                    if (!isdigit((unsigned char)c) && c != '.') { onlyDotDigit = false; break; }
                }
                if (hasLetter && !onlyDotDigit && seen.find(tempStr) == seen.end()) {
                    seen.insert(tempStr);
                    result.foundStrings.push_back(tempStr);
                }
            }
        }
    }
}