#include "windows.h"

#include "ZQWinBase.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	CZQWinBase zq;
	zq.setInstance(hInstance);
	zq.Create(0, 0,
		1024,768,
		biSysMenu | biMaximize | biMinimize, BS_Sizeable, TRUE);
	zq.Show();
	zq.Run();

}