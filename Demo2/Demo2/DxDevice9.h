#pragma once
#include "stdinclud.h"
#include "BaseWindow.h"
#include "DxFont.h"
 
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
	BOOL CheckDeviceLosted();
	void OnDeviceLost(LPDIRECT3DDEVICE9 Device);

	void DrawLine(int sx, int sy, int dx, int dy, DWORD dwColor);
	void DrawRectangleFill(RECT rect, DWORD dwColor);
	void DrawRectangle(RECT rect, DWORD dwColor);

	LPDIRECT3DTEXTURE9 TextureLoad(const  WCHAR* FileName, DWORD size, BOOL bMipMap);
	void*  ResourceLoad(const  WCHAR* FileName, DWORD* size);
private:
	CDxFont  m_font;
	LPDIRECT3D9   m_pD3D9;
	LPDIRECT3DDEVICE9  m_pDxDevice9;
	CBaseWindow * m_ParentWindow;
	BOOL    m_bInitialized;  // «∑Ò≥ı ºªØ£∫
	WCHAR   m_strFPS[50];
	D3DPRESENT_PARAMETERS d3dpp;


};

