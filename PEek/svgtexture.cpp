#include "svgtexture.h"
#include <iostream>

#define NANOSVG_IMPLEMENTATION
#include "imguidepend/nanosvg/nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "imguidepend/nanosvg/nanosvgrast.h"

bool LoadSVGTexture(ID3D11Device* device, const char* path, int targetW, int targetH,
    ID3D11ShaderResourceView** out_srv, int* out_w, int* out_h)
{


    NSVGimage* image = nsvgParseFromFile(path, "px", 96.0f);
    if (!image) {
        return false;
    }

    float scaleX = (float)targetW / image->width;
    float scaleY = (float)targetH / image->height;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    NSVGrasterizer* rast = nsvgCreateRasterizer();
    unsigned char* img_data = (unsigned char*)malloc(targetW * targetH * 4);
    nsvgRasterize(rast, image, 0, 0, scale, img_data, targetW, targetH, targetW * 4);
    nsvgDeleteRasterizer(rast);
    nsvgDelete(image);

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = targetW;
    desc.Height = targetH;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA sub = {};
    sub.pSysMem = img_data;
    sub.SysMemPitch = targetW * 4;

    ID3D11Texture2D* tex = nullptr;
    device->CreateTexture2D(&desc, &sub, &tex);
    free(img_data);
    if (!tex) return false;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    device->CreateShaderResourceView(tex, &srvDesc, out_srv);
    tex->Release();

    *out_w = targetW;
    *out_h = targetH;
    return true;
}