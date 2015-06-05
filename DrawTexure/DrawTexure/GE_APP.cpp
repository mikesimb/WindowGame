/*************************************************************************************
 [Implement File]

 PURPOSE: 
    Create Game Main Window Framework.
*************************************************************************************/

#include "GE_COMMON.h"
#include "GE_APP.h"

//------------------------------------------------------------------------------------
// Create a window by specified title and show type.
//------------------------------------------------------------------------------------
bool GE_APP::Create_Window(LPCTSTR win_title, HINSTANCE instance, int cmd_show)
{
    // regiter window class
    _Reg_Window_Class(instance);

    // create a window
    _wnd = CreateWindow(WINDOW_CLASS_NAME, win_title, WS_SYSMENU | WS_CAPTION | WS_VISIBLE,
                        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, instance, NULL);
    
    if(_wnd == NULL)
        // create failed
        return false;

    // sets the specified window's show state 
    ShowWindow(_wnd, cmd_show);
    // updates the client area of the specified window
    UpdateWindow(_wnd);

    return true;
}

//------------------------------------------------------------------------------------
// Registers a window class for subsequent use in calls to the CreateWindow 
// or CreateWindowEx function. 
//
// If the function succeeds, the return value is a class atom that uniquely 
// identifies the class being registered.
// If the function fails, the return value is zero. 
//------------------------------------------------------------------------------------
ATOM GE_APP::_Reg_Window_Class(HINSTANCE instance)
{
    _wnd_class.cbSize          = sizeof(WNDCLASSEX);
    _wnd_class.style           = CS_HREDRAW | CS_VREDRAW;
    _wnd_class.lpfnWndProc     = (WNDPROC) Window_Proc;
    _wnd_class.cbClsExtra      = 0;
    _wnd_class.cbWndExtra      = 0;
    _wnd_class.hInstance       = instance;
    _wnd_class.hIcon           = 0;
    _wnd_class.hCursor         = LoadCursor(NULL, IDC_ARROW);
    _wnd_class.hbrBackground   = (HBRUSH) GetStockObject(BLACK_BRUSH);
    _wnd_class.lpszMenuName    = 0;
    _wnd_class.lpszClassName   = WINDOW_CLASS_NAME;
    _wnd_class.hIconSm         = NULL;

    return RegisterClassEx(&_wnd_class);  
}

//--------------------------------------------------------------------------
// Callback function, handle message return by windows.
//--------------------------------------------------------------------------
LRESULT CALLBACK GE_APP::Window_Proc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(wnd, msg, wParam, lParam);
}

