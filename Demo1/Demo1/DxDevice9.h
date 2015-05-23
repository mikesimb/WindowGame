#pragma once
#include <d3d9.h>
#include <strsafe.h>
#pragma  warning (disable : 4996)
#include <vector>
#include "BaseWindow.h"

#pragma comment(lib, "d3d9.lib")  
#pragma comment(lib, "d3dx9.lib")  


class CDxDevice9
{
public:
	CDxDevice9();
	virtual ~CDxDevice9();

	HRESULT InitiD3D(CBaseWindow* ParentWindow);
	void Clearup();

	void Render();
private:

	LPDIRECT3D9   m_pD3D9;
	LPDIRECT3DDEVICE9  m_pDxDevice9;
	CBaseWindow * m_ParentWindow;
	BOOL    m_bInitialized;  // «∑Ò≥ı ºªØ£∫
};

