#pragma  once
#include <windows.h>
#include "BaseWindow.h"
#include "DxDevice9.h"


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	CBaseWindow window;
	window.Create();
	window.ShowWindow();
	window.Run();

	
  
}


