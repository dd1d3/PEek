#include "f7section.h"
#include "f4enthropy.h"

void ParseSections(LPVOID bufferp, IMAGE_NT_HEADERS* nt, PEResult& result) {
    IMAGE_SECTION_HEADER* sec = IMAGE_FIRST_SECTION(nt);
    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++)
    {
        char name[9] = { 0 };
        memcpy(name, sec[i].Name, 8);
        result.sections.push_back(name);

        BYTE* sectionData = (BYTE*)bufferp + sec[i].PointerToRawData;
        double ent = shEntropy(sectionData, sec[i].SizeOfRawData);
        result.sectionEntropy.push_back(ent);
    }
}