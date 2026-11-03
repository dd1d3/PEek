#include "f4enthropy.h"

double shEntropy(BYTE* data, DWORD size) {
    if (size == 0) return 0.0;

    int freq[256] = {};
    for (DWORD i = 0; i < size; i++)
        freq[data[i]]++;

    double entropy = 0.0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] == 0) continue;
        double p = (double)freq[i] / size;
        entropy -= p * log2(p);
    }

    return entropy;
}