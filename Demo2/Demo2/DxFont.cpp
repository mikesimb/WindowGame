#include "DxFont.h"
#include "stdinclud.h"

CDxFont::CDxFont()
{
}


CDxFont::~CDxFont()
{
}

void CDxFont::DrawText(LPCTSTR str,RECT rc)
{
	m_gFont->DrawText(0, str, -1,
		&rc, DT_CENTER,
		D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256));//采用随机RGB值，做出“闪闪惹人爱”的特效  

}


BOOL CDxFont::Init(LPDIRECT3DDEVICE9 device, LPCTSTR lpString)
{
	if (device != NULL)
	{
		if (FAILED(D3DXCreateFont(device, 30, 0, 0, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, lpString, &m_gFont)))
			return E_FAIL;
	}
	
	return TRUE;
}

void CDxFont::Finalize()
{
	m_gFont->Release();
	m_gFont = NULL;

}




