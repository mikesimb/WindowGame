/*************************************************************************************
 [Implement File]

 PURPOSE: 
    Define for texture mapped.
*************************************************************************************/

#include "GE_COMMON.h"
#include "TextureAddress.h"

//------------------------------------------------------------------------------------
// Constructor, initialize all pointer with NULL.
//------------------------------------------------------------------------------------
TEXTURE_ADDRESS::TEXTURE_ADDRESS()
{
    _d3d = NULL;
    _d3d_device = NULL;
    _vertex_buffer = NULL;
    _d3d_texture = NULL;
}

//------------------------------------------------------------------------------------
// Destructor, release resource allocated for Direct3D.
//------------------------------------------------------------------------------------
TEXTURE_ADDRESS::~TEXTURE_ADDRESS()
{
    Release_Direct3D();
}

//------------------------------------------------------------------------------------
// Create direct3D interface and direct3D device.
//------------------------------------------------------------------------------------
bool TEXTURE_ADDRESS::Create_D3D_Device(HWND hwnd, bool full_screen)
{
    // Create a IDirect3D9 object and returns an interace to it.
    _d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if(_d3d == NULL)
        return false;

    // retrieve adapter capability
    D3DCAPS9 d3d_caps;    
    _d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3d_caps);
    
    bool hardware_process_enable = (d3d_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ? true : false);

    // Retrieves the current display mode of the adapter.
    D3DDISPLAYMODE display_mode;
    if(FAILED(_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)))
        return false;

    // set present parameter for direct3D device
    D3DPRESENT_PARAMETERS present_param = {0};

    present_param.BackBufferWidth      = WINDOW_WIDTH;
    present_param.BackBufferHeight     = WINDOW_HEIGHT;
    present_param.BackBufferFormat     = display_mode.Format;
    present_param.BackBufferCount      = 1;
    present_param.hDeviceWindow        = hwnd;
    present_param.Windowed             = !full_screen;
    present_param.SwapEffect           = D3DSWAPEFFECT_FLIP;
    present_param.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

    // Creates a device to represent the display adapter.
    DWORD behavior_flags;

    behavior_flags = hardware_process_enable ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;

    if(FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, behavior_flags, 
                                 &present_param, &_d3d_device)))
    {
        return false;
    }
    
    // create successfully
    return true;
}

//------------------------------------------------------------------------------------
// Initialize vertex buffer.
//------------------------------------------------------------------------------------
bool TEXTURE_ADDRESS::Init_Vertex_Buffer()
{
    CUSTOM_VERTEX custom_vertex[] =
	{
		{ 100.0f, 100.0f, 0.0f, 1.0f, 0xFFFFFFFF,0.0f, 0.0f },
		{ 380.0f, 100.0f, 0.0f, 1.0f, 0xFFFFFFFF, 3.0f, 0.0f },
		{ 100.0f, 380.0f, 0.0f, 1.0f, 0xFFFFFFFF, 0.0f, 3.0f },
		{ 380.0f, 380.0f, 0.0f, 1.0f, 0xFFFFFFFF, 3.0f, 3.0f }
    };

    BYTE* vertex_data;

    // create vertex buffer
    if(FAILED(_d3d_device->CreateVertexBuffer(4 * sizeof(CUSTOM_VERTEX), 0, CUSTOM_VERTEX_FVF,
                            D3DPOOL_MANAGED, &_vertex_buffer, NULL)))
    {
        return false;
    }

    // get data pointer to vertex buffer
    if(FAILED(_vertex_buffer->Lock(0, 0, (void **) &vertex_data, 0)))
        return false;

    // copy custom vertex data into vertex buffer
    memcpy(vertex_data, custom_vertex, sizeof(custom_vertex));

    // unlock vertex buffer
    _vertex_buffer->Unlock();

    return true;

}

//------------------------------------------------------------------------------------
// Sets the sampler state value.
//------------------------------------------------------------------------------------
void TEXTURE_ADDRESS::Set_Texture_Address_Mode(int mode)
{
    switch(mode)
    {
    case D3DTADDRESS_WRAP:
        _d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        _d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        break;
    case D3DTADDRESS_MIRROR:
        _d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
        _d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
        break;
    case D3DTADDRESS_CLAMP:
        _d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        _d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        break;
    case D3DTADDRESS_BORDER:
        _d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
        _d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
        break;
    case D3DTADDRESS_MIRRORONCE:
        _d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRRORONCE);
        _d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRRORONCE);
        break;
    }
}

//------------------------------------------------------------------------------------
// Render object.
//------------------------------------------------------------------------------------
void TEXTURE_ADDRESS::Render()
{
    if(_d3d_device == NULL)
        return;

    // clear surface with color white
    _d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0, 0);

    // begin scene
    _d3d_device->BeginScene();

    // Binds a vertex buffer to a device data stream.
    _d3d_device->SetStreamSource(0, _vertex_buffer, 0, sizeof(CUSTOM_VERTEX));

    // Sets the current vertex stream declaration.
    _d3d_device->SetFVF(CUSTOM_VERTEX_FVF);

    // Creates a texture from a file.
    if(FAILED(D3DXCreateTextureFromFile(_d3d_device, _T("E:\\CodeLesson\\Engine\\WindowGame\\Demo2\\Debug\\hum2_D(9000).png"), &_d3d_texture)))
    {
        MessageBox(NULL, "Create texture interface failed.", "ERROR", MB_OK);
        return;
    }

    // Assigns a texture to a stage for a device.
    _d3d_device->SetTexture(0, _d3d_texture);

    // Renders a sequence of nonindexed, geometric primitives of the specified type from the current 
    // set of data input streams.
    _d3d_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

    // end scene
    _d3d_device->EndScene();

    // Presents the contents of the next buffer in the sequence of back buffers owned by the device.
    _d3d_device->Present(NULL, NULL, NULL, NULL);
}

//------------------------------------------------------------------------------------
// Release resource allocated for Direct3D.
//------------------------------------------------------------------------------------
void TEXTURE_ADDRESS::Release_Direct3D()
{
    Safe_Release(_d3d_texture);
    Safe_Release(_vertex_buffer);
    Safe_Release(_d3d_device);
    Safe_Release(_d3d);
}