#pragma  once
#include "stdinclud.h"
#include "BaseWindow.h"
#include "Application.h"


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	CApplication* App = CApplication::Instance();
	App->Run();
// 	CBaseWindow window;
// 	window.Create();
// 	window.ShowWindow();
// 	window.Run();

	
  
}


