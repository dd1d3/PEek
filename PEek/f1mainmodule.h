#pragma once
#include <windows.h>
#include <string>
#include <vector>

struct PackerHit {
    std::string name;
    float score;
};


struct CompilerHit {
    std::string name;
    float score;
};

struct PEResult {
    std::string filePath;
    std::string md5;
    std::string sha256;
    std::string machine;
    std::string sha1;
    std::string timeDateStamp;
    bool is64bit = false;
    bool validPE = false;
	bool dotNetInfo = false;
    std::string error;
    std::vector<std::string> sections;
    std::vector<std::string> imports;
    std::vector<std::string> foundStrings;
    std::vector<double> sectionEntropy;
    std::vector<CompilerHit> compilerHits;
    std::vector<PackerHit> packerHits;
};

void AnalyzePE(const char* fileName, PEResult& result);