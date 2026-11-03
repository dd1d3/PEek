#pragma once
#include <d3d11.h>

bool LoadSVGTexture(ID3D11Device* device, const char* path, int targetW, int targetH,
    ID3D11ShaderResourceView** out_srv, int* out_w, int* out_h);