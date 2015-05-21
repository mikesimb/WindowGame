#pragma  once
#include <windows.h>
#include "BaseWindow.h"


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	CBaseWindow window;
	window.Create();
	window.ShowWindow();
	window.Run();

	
  
}


