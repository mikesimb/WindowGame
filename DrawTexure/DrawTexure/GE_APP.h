/*************************************************************************************
 [Include File]

 PURPOSE: 
    Create Game Main Window Framework.
*************************************************************************************/

#ifndef GAME_ENGINE_APP_H
#define GAME_ENGINE_APP_H

#include "GE_COMMON.h"

#define WINDOW_CLASS_NAME       "MY_GAME"

class GE_APP
{
private:
    HWND _wnd;
    WNDCLASSEX _wnd_class;

public:
    GE_APP()    {};
    ~GE_APP()    {};

    WNDCLASSEX Get_Window_Class()       { return _wnd_class; }
    HWND Get_Window_Handle()            { return _wnd; }

    bool Create_Window(LPCTSTR win_title, HINSTANCE instance, int cmd_show);
    static LRESULT CALLBACK Window_Proc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    ATOM _Reg_Window_Class(HINSTANCE instance);
};

#endif

