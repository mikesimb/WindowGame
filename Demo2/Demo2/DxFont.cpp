#include "DxFont.h"
#include "stdinclud.h"

CDxFont::CDxFont()
{
}


CDxFont::~CDxFont()
{
}

void CDxFont::DrawText(LPCTSTR str)
{
	m_gFont->DrawText(0, str, -1,
		&m_FontRect, DT_CENTER,
		D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256));//采用随机RGB值，做出“闪闪惹人爱”的特效  

}


BOOL CDxFont::Init(LPDIRECT3DDEVICE9 device, LPCTSTR lpString)
{
	if (device != NULL)
	{
		if (FAILED(D3DXCreateFont(device, 30, 0, 0, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("宋体"), &m_gFont)))
			return E_FAIL;
	}
	m_FontRect.left = 100;
	m_FontRect.right = 1024;
	m_FontRect.top = 0;
	m_FontRect.bottom = 600;


	
	return TRUE;
}




