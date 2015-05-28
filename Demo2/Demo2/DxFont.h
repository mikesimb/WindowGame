#pragma once
#include <d3d9.h>
#include <d3dx9.h>




class CDxFont
{
public:
	CDxFont();
	~CDxFont();
	BOOL  Init(LPDIRECT3DDEVICE9  device, LPCTSTR lpString);
	void DrawText(LPCTSTR str);
	
private:
	ID3DXFont*  m_gFont;
	WCHAR  m_fontname[50];
	RECT   m_FontRect;
	
	
	
};

