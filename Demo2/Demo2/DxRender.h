#pragma once

#include <windows.h>
#include "DxDevice9.h"

class CDxRender
{
public:

	CDxRender(CDxDevice9 * pDevice9) :m_pDevice9(pDevice9)
	{
		
	}

	virtual ~CDxRender()
	{
	}

	virtual void DrawLine(int x, int y, DWORD dwColor);


private:
	CDxDevice9 * m_pDevice9;
};

inline void DrawLine(int x, int y, DWORD dwColor)
{

}

