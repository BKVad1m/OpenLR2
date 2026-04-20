#include <D3dx9tex.h>
#pragma comment(lib, "D3dx9")

#include "ImageLoader.h"


LPDIRECT3DDEVICE9 g_pd3dDevice;
// Simple helper function to load an image into a DX9 texture with common settings
bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
    // Load texture from disk
    PDIRECT3DTEXTURE9 texture;
    HRESULT hr = D3DXCreateTextureFromFileA(g_pd3dDevice, filename, &texture);
    if (hr != S_OK)
        return false;

    // Retrieve description of the texture surface so we can access its size
    D3DSURFACE_DESC my_image_desc;
    texture->GetLevelDesc(0, &my_image_desc);
    *out_texture = texture;
    *out_width = (int)my_image_desc.Width;
    *out_height = (int)my_image_desc.Height;
    return true;
}

bool LoadTextureFromFile(const char* filename, D3Image* d3)
{
    // Load texture from disk
    PDIRECT3DTEXTURE9 texture;
    HRESULT hr = D3DXCreateTextureFromFileA(g_pd3dDevice, filename, &texture);
    if (hr != S_OK)
        return false;

    // Retrieve description of the texture surface so we can access its size
    D3DSURFACE_DESC my_image_desc;
    texture->GetLevelDesc(0, &my_image_desc);
    d3->texture = texture;
    d3->width = (int)my_image_desc.Width;
    d3->height = (int)my_image_desc.Height;
    return true;
}
bool LoadTextureFromMemory(void* data, size_t size, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
    // Load texture from disk
    PDIRECT3DTEXTURE9 texture;
    
    HRESULT hr = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, data, size, &texture);
    if (hr != S_OK)
        return false;

    // Retrieve description of the texture surface so we can access its size
    D3DSURFACE_DESC my_image_desc;
    texture->GetLevelDesc(0, &my_image_desc);
    *out_texture = texture;
    *out_width = (int)my_image_desc.Width;
    *out_height = (int)my_image_desc.Height;
    return true;
}

bool LoadTextureFromRawMemory(void* data, size_t size, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
    PDIRECT3DTEXTURE9 texture;
    D3DPOOL pool;
    HRESULT hr = D3DXCreateTexture(g_pd3dDevice, *out_width, *out_height, 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, pool, &texture);
    
    
    
    //HRESULT hr = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, data, size, &texture);
    if (hr != S_OK)
        return false;

    // Retrieve description of the texture surface so we can access its size
    D3DSURFACE_DESC my_image_desc;
    texture->GetLevelDesc(0, &my_image_desc);
    *out_texture = texture;
    *out_width = (int)my_image_desc.Width;
    *out_height = (int)my_image_desc.Height;
    return true;
}

