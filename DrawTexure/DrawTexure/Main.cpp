/*************************************************************************************
 [Implement File]

 PURPOSE: 
    Test for texture render.
*************************************************************************************/

#define DIRECTINPUT_VERSION 0x0900

#include "GE_APP.h"
#include "TextureAddress.h"

#pragma warning(disable : 4305 4996)

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR cmd_line, int cmd_show)
{
    GE_APP ge_app;
    //GE_INPUT ge_input;    
    TEXTURE_ADDRESS texture_address;

    MSG msg = {0};

    // create window
    if(! ge_app.Create_Window("Material and light test", instance, cmd_show))
        return false;

    HWND hwnd = ge_app.Get_Window_Handle();
    HDC hdc = GetDC(hwnd);

    // create directinput
    //ge_input.Create_Input(instance, hwnd);

    SetWindowPos(hwnd, 0, 0,0,0,0, SWP_NOSIZE);
    SetCursorPos(0, 0);

    // Create direct3D interface and direct3D device.
    if(! texture_address.Create_D3D_Device(hwnd, false))
        return false;

    // Initialize vertex buffer with curstom vertex structure.
    if(! texture_address.Init_Vertex_Buffer())
        return false;

    while(msg.message != WM_QUIT)
    {
        if(PeekMessage(&msg, NULL, 0,0 , PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // read data from keyboard buffer
           
			bool key_w_pressed = true;//ge_input.Is_Key_Pressed(DIK_W);
			bool key_m_pressed = false; //ge_input.Is_Key_Pressed(DIK_M);
			bool key_c_pressed = false; //ge_input.Is_Key_Pressed(DIK_C);
			bool key_b_pressed = false; //ge_input.Is_Key_Pressed(DIK_B);
			bool key_o_pressed = false; //ge_input.Is_Key_Pressed(DIK_O);

                const char* text = NULL;
                
                // set texture address mode
                if(key_w_pressed || key_m_pressed || key_c_pressed || key_b_pressed || key_o_pressed)
                {
                    if(key_w_pressed)
                    {
                        texture_address.Set_Texture_Address_Mode(D3DTADDRESS_WRAP);
                        text = "wrap mode";
                    }

                    if(key_m_pressed)
                    {
                        texture_address.Set_Texture_Address_Mode(D3DTADDRESS_MIRROR);
                        text = "morror mode";
                    }

                    if(key_c_pressed)
                    {
                        texture_address.Set_Texture_Address_Mode(D3DTADDRESS_CLAMP);
                        text = "clamp mode";
                    }

                    if(key_b_pressed)
                    {
                        texture_address.Set_Texture_Address_Mode(D3DTADDRESS_BORDER);
                        text = "address border mode";
                    }

                    if(key_o_pressed)
                    {
                        texture_address.Set_Texture_Address_Mode(D3DTADDRESS_MIRRORONCE);
                        text = "address mirror once mode";
                    }
                    
                    // render object
                    texture_address.Render();
                    
                    // print hint information
                    TextOut(hdc, WINDOW_WIDTH - 200, WINDOW_HEIGHT - 100, text, (int) strlen(text));

                // press "ESC", close window.
//                 if(ge_input.Is_Key_Pressed(DIK_ESCAPE))
//                     PostQuitMessage(0);
            }                         
        }
    }    

    UnregisterClass(WINDOW_CLASS_NAME, instance);

    return true;
}


