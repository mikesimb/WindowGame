/*************************************************************************************
 [Include File]

 PURPOSE: 
    Define for texture mapped.
*************************************************************************************/

#ifndef TEXTURE_ADDRESS_H
#define TEXTURE_ADDRESS_H

#define CUSTOM_VERTEX_FVF   (D3DFVF_XYZRHW | D3DFVF_TEX1 |D3DFVF_DIFFUSE)

struct CUSTOM_VERTEX
{
    float x, y, z, rhw;     // vertex coordinate
	DWORD color;
    float u, v;             // texture coordinate
};

class TEXTURE_ADDRESS
{
private:
    IDirect3D9* _d3d;
    IDirect3DDevice9* _d3d_device;
    IDirect3DVertexBuffer9* _vertex_buffer;
    IDirect3DTexture9* _d3d_texture;

public:
    TEXTURE_ADDRESS();
    ~TEXTURE_ADDRESS();
    bool Create_D3D_Device(HWND hwnd, bool full_screen = true);
    bool Init_Vertex_Buffer();
    void Set_Texture_Address_Mode(int mode);
    void Render();
    void Release_Direct3D();
};

#endif

