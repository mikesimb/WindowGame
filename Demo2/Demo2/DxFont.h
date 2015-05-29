#pragma once
#include "stdinclud.h"

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
