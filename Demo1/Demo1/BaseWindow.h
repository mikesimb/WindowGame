#pragma once
#include "BaseWindowMessageControl.h"
class CBaseWindow :
	public CBaseWindowMsgControl
{
public:

	CBaseWindow()
	{
	}

	virtual ~CBaseWindow()
	{
	}
private:
	HINSTANCE m_Instance;
	HWND   m_hwnd;
	INT     nWidth;
	INT    nHeight;
	
};

