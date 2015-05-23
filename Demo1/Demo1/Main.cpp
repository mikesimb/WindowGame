#pragma  once
#include <windows.h>
#include "BaseWindow.h"
#include "Application.h"


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	CApplication m_App;
	if (m_App.Initizlized())
		m_App.Run();
// 	CBaseWindow window;
// 	window.Create();
// 	window.ShowWindow();
// 	window.Run();

	
  
}


