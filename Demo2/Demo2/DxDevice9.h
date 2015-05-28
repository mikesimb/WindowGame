#pragma once
#include <d3d9.h>
#include <strsafe.h>
#pragma  warning (disable : 4996)
#include <vector>
#include "BaseWindow.h"
#include "DxFont.h"
#pragma comment(lib, "d3d9.lib")  
#pragma comment(lib, "d3dx9.lib")  
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW |D3DFVF_DIFFUSE)

class CDxDevice9
{
public:
	CDxDevice9();
	virtual ~CDxDevice9();

	HRESULT InitiD3D(CBaseWindow* ParentWindow);
	void Clearup();

	void Render();

	void DrawLine(int sx, int sy, int dx, int dy, DWORD dwColor);
	void DrawRectangleFill(RECT rect, DWORD dwColor);
	void DrawRectangle(RECT rect, DWORD dwColor);
private:
	CDxFont  m_font;
	LPDIRECT3D9   m_pD3D9;
	LPDIRECT3DDEVICE9  m_pDxDevice9;
	CBaseWindow * m_ParentWindow;
	BOOL    m_bInitialized;  // «∑Ò≥ı ºªØ£∫


};

