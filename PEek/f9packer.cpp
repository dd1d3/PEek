#include "f9packer.h"
#include "f1mainmodule.h"
#include <algorithm>
#include "signature_scanner.h"

void DetectPacker(PEResult& result, LPVOID bufferp, IMAGE_NT_HEADERS* nt) {

    auto hasimport = [&](const std::string& pattern) {
        return std::any_of(result.imports.begin(), result.imports.end(),
            [&](const std::string& imp) {
                return imp.find(pattern) != std::string::npos;
            });
        };

    auto hasstring = [&](const std::string& pattern) {
        return std::any_of(result.foundStrings.begin(), result.foundStrings.end(),
            [&](const std::string& s) {
                return s.find(pattern) != std::string::npos;
            });
        };

    auto hassection = [&](const std::string& name) {
        return std::any_of(result.sections.begin(), result.sections.end(),
            [&](const std::string& s) {
                return s == name;
            });


        };

	float ASPACK = 0;

    short aspackPattern1[] = { 0x60, 0xE8, 0x41, 0x06, 0x00, 0x00, 0xEB, 0x41 };
    short aspackPattern2[] = { 0x60, 0xE8, 0x03, 0x00, 0x00, 0x00, 0xE9, 0xEB, 0x04, 0x5D, 0x45, 0x55, 0xC3, 0xE8, 0x01 };

	if (signaturefinder(nt, bufferp, aspackPattern1, sizeof(aspackPattern1) / sizeof(short))) ASPACK += 50;
	if (signaturefinder(nt, bufferp, aspackPattern2, sizeof(aspackPattern2) / sizeof(short))) ASPACK += 50;
	if (ASPACK > 0)
	{
		result.packerHits.push_back({ "ASPack", ASPACK });
	}

    float PECompactgeneric1_4x = 0;

    short pecompactPattern[] = { 0xEB, 0x06, 0x68, -1, -1, -1, -1, 0xC3, 0x9C, 0x60, 0xE8, 0x02, -1, -1, -1, 0x33, 0xC0, 0x8B, 0xC4, 0x83, 0xC0, 0x04, 0x93, 0x8B, 0xE3, 0x8B, 0x5B, 0xFC, 0x81, 0xEB };
	if (signaturefinder(nt, bufferp, pecompactPattern, sizeof(pecompactPattern) / sizeof(short))) PECompactgeneric1_4x += 50;
	if (PECompactgeneric1_4x > 0)
	{
		result.packerHits.push_back({ "PECompact", PECompactgeneric1_4x });
	}  

	float FSG = 0;

    short fsgPattern[] = { 0xBB, 0xD0, 0x01, 0x40, 0x00, 0xBF, 0x00, 0x10, 0x40, 0x00, 0xBE, -1, -1, -1, -1, 0x53, 0xE8, 0x0A, 0x00, 0x00, 0x00, 0x02, 0xD2, 0x75, 0x05, 0x8A, 0x16, 0x46, 0x12, 0xD2, 0xC3 };

	if (signaturefinder(nt, bufferp, fsgPattern, sizeof(fsgPattern) / sizeof(short))) FSG += 50;
	if (FSG > 0)
	{
		result.packerHits.push_back({ "FSG", FSG });
	}

	float NsPack = 0;

    short nspackPattern[] = { 0x9C, 0x60, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x5D, 0xB8, -1, -1, -1, -1, 0x2D, -1, -1, -1, -1 };

	if (signaturefinder(nt, bufferp, nspackPattern, sizeof(nspackPattern) / sizeof(short))) NsPack += 50;

	if (NsPack > 0)
	{
		result.packerHits.push_back({ "NsPack", NsPack });
	}

	float Armadillo = 0;

    short armadilloPattern[] = { 0x55, 0x8B, 0xEC, 0x6A, 0xFF, 0x68, -1, -1, -1, -1, 0x68, -1, -1, -1, -1, 0x64, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x50, 0x64, 0x89, 0x25, 0x00, 0x00, 0x00, 0x00, 0x83, 0xEC, 0x58 };

	if (signaturefinder(nt, bufferp, armadilloPattern, sizeof(armadilloPattern) / sizeof(short))) Armadillo += 50;

	if (Armadillo > 0)
	{
		result.packerHits.push_back({ "Armadillo", Armadillo });
	}

	float MEW11SE = 0;

    short mewPattern[] = { 0xE9, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0C, 0x90, 0xE9 };

	if (signaturefinder(nt, bufferp, mewPattern, sizeof(mewPattern) / sizeof(short))) MEW11SE += 50;

    if (MEW11SE > 0) {
		result.packerHits.push_back({ "MEW11SE", MEW11SE });
    }
     
	float PESpin = 0;

    short pespinPattern[] = { 0xEB, 0x01, 0x68, 0x60, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x1C, 0x24, 0x83, 0xC3, 0x12, 0x81 };

	if (signaturefinder(nt, bufferp, pespinPattern, sizeof(pespinPattern) / sizeof(short))) PESpin += 50;
    
	if (PESpin > 0)
	{
		result.packerHits.push_back({ "PESpin", PESpin });
	}

    float UPXdetect = 0;

    if (hassection("UPX0")) UPXdetect += 25;
    if (hassection("UPX1"))  UPXdetect += 25;
    if (hasstring("UPX!")) UPXdetect += 50;
    if (UPXdetect > 0)
    {
        result.packerHits.push_back({ "UPX", UPXdetect });
    }

    float Enigma = 0;

    short enigma64Pattern[] = { 0x50, 0x51, 0x52, 0x53, 0x55, 0x56, 0x57, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53, 0x41, 0x54, 0x41, 0x55, 0x41, 0x56, 0x41, 0x57, 0x48, 0x9c, 0x48, 0x81, 0xec, 0x08, 0x00, 0x00, 0x00, 0x0f, 0xae, 0x1c, 0x24, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x5d };
    short enigma32Pattern[] = { 0x55, 0x8b, 0xec, 0x83, 0xc4, -1, 0xb8, -1, -1, -1, -1, 0xe8, -1, -1, -1, -1, 0x9a, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0xe9, -1, -1, -1, -1, 0x60, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x5d, -1, 0xed };
    short enigmaOldPattern[] = { 0x60, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x5d, 0x83, -1, -1, 0x81, 0xed };

	if (signaturefinder(nt, bufferp, enigma64Pattern, sizeof(enigma64Pattern) / sizeof(short))) Enigma += 50;
    if (signaturefinder(nt, bufferp, enigma32Pattern, sizeof(enigma32Pattern) / sizeof(short))) Enigma += 50;
    if (signaturefinder(nt, bufferp, enigmaOldPattern, sizeof(enigmaOldPattern) / sizeof(short))) Enigma += 50;
    if (hassection(".enigma")) Enigma += 50;
    if (hasstring("Enigma Protector")) Enigma += 50;

    if (Enigma > 0)
    {
        result.packerHits.push_back({ "Enigma", Enigma });
    }

    float Themida = 0;

    if (hassection(".themida")) Themida += 50;
    if (hassection(".winlice")) Themida += 50;

    bool highEntropyFirstSection = !result.sectionEntropy.empty() && result.sectionEntropy[0] >= 7.5;
    if (highEntropyFirstSection) Themida += 15;

    if (Themida > 0)
    {
        result.packerHits.push_back({ "Themida", Themida });
    }

    float VmProtect = 0;
    short vmpPattern[] = { 0x9c, 0x8d, 0x64 };

    if (signaturefinder(nt, bufferp, vmpPattern, sizeof(vmpPattern) / sizeof(short))) VmProtect += 20;
    if (hassection(".vmp0")) VmProtect += 40;
    if (hassection(".vmp1")) VmProtect += 40;
    if (hassection(".vmp2")) VmProtect += 20;
    if (hassection(".vmp3")) VmProtect += 20;

    if (!result.sectionEntropy.empty() && result.sectionEntropy.back() > 7.6) VmProtect += 15;

    if (VmProtect > 0)
    {
        result.packerHits.push_back({ "VMProtect", VmProtect });
    }
}