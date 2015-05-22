#pragma once
#include <d3d9.h>
#include <vector>
class CDxDevice9
{
public:
	CDxDevice9();
	virtual ~CDxDevice9();
	
private:
	LPDIRECT3D9			m_pDirect3D9;
	D3DCAPS9			m_CapsD9;
	D3DDISPLAYMODE		m_DisplayMode;
	LPDIRECT3DDEVICE9	m_pDevice9;
	D3DVIEWPORT9        m_ViewportD9;

	DWORD               m_Diffuse;
	DWORD               m_BlendMode;
	
	BOOL                m_bVSync;//´¹Ö±Í¬²½

	BOOL                m_bInvalide;

	BOOL                m_bViewportState;
	D3DVIEWPORT9        m_SaveViewport;
	BOOL                m_bSaveViewport;


	BOOL                m_bNextRender;

	



};

