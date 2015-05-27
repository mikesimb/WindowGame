#include "DxDevice9.h"
#include <windows.h>
/*
1、设备方面显示模式监测
2、设备能力的监测
3、顶点数据格式的监测
4、设备丢失监测和预警
*/


CDxDevice9::CDxDevice9() : m_pD3D9(NULL), m_pDxDevice9(NULL), m_ParentWindow(NULL)
{
}


CDxDevice9::~CDxDevice9()
{
}

HRESULT CDxDevice9::InitiD3D(CBaseWindow* ParentWindow)
{
	// 这里初始化
	if (ParentWindow == NULL) return E_FAIL;
	m_ParentWindow = ParentWindow;
	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D9 == NULL)
		return E_FAIL;


	//--------------------------------------------------------------------------------------
	// 获取硬件设备信息
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(m_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //支持硬件顶点运算，我们就采用硬件顶点运算，妥妥的
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //不支持硬件顶点运算，无奈只好采用软件顶点运算


	//填充D3DPRESENT_PARAMETERS结构体
	//--------------------------------------------------------------------------------------
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferWidth = m_ParentWindow->GetWidth();
		d3dpp.BackBufferHeight = m_ParentWindow->GetHeight();
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount = 2;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = m_ParentWindow->GetWindowHandle();
		d3dpp.Windowed = true;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.Flags = 0;
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	if (FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_ParentWindow->GetWindowHandle(),
		vp,
		&d3dpp, &m_pDxDevice9)))
	{
		return E_FAIL;
	}

	return S_OK;

}

void CDxDevice9::Clearup()
{
	if (m_pDxDevice9 != NULL)
		m_pDxDevice9->Release();

	if (m_pD3D9 != NULL)
		m_pD3D9->Release();


}

void CDxDevice9::Render()
{
	if (NULL == m_pDxDevice9)
		return;

	m_pDxDevice9->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	if (SUCCEEDED(m_pDxDevice9->BeginScene()))
	{
		
		DrawLine(100, 100, 200, 500, 0xFF00FF00);
		DrawLine(200, 200, 400, 600, 0xFF00FF00);
		DrawLine(300, 300, 400, 600, 0xFF00FF00);
		DrawLine(0, 0, 100, 600, 0xFF00FF00);
		RECT re;
		re.left = 300;
		re.top = 0;
		re.right = 500;
		re.bottom = 400;

		DrawRectangle(re, 0xFF00FF00);

		m_pDxDevice9->EndScene();
	}

	m_pDxDevice9->Present(NULL, NULL, NULL, NULL);
}

void CDxDevice9::DrawLine(int sx, int sy, int dx, int dy, DWORD dwColor)
{
	//首先创建定点缓冲区
	LPDIRECT3DVERTEXBUFFER9  g_pVB;
	CUSTOMVERTEX vertices[2];
	for (int i = 0; i < 2; i++)
	{
		if (i == 1)
		{
			vertices[i].x = (float)sx;
			vertices[i].y = (float)sy;
		}
		else
		{
			vertices[i].x = (float)dx;
			vertices[i].y = (float)dy;

		}
		vertices[i].z = 0.0f;
		vertices[i].color = dwColor;
		vertices[i].rhw = 1.0f;

	}
	
	// Create the vertex buffer. Here we are allocating enough memory
	// (from the default pool) to hold all our 3 custom vertices. We also
	// specify the FVF, so the vertex buffer knows what data it contains.
	if (FAILED(m_pDxDevice9->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return;
	}

	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
	// gain access to the vertices. This mechanism is required becuase vertex
	// buffers may be in device memory.
	VOID* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return;
	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();

	m_pDxDevice9->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pDxDevice9->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pDxDevice9->DrawPrimitive(D3DPT_LINELIST, 0, 1);
}

void CDxDevice9::DrawRectangle(RECT rect, DWORD dwColor)
{
	//首先创建定点缓冲区
	LPDIRECT3DVERTEXBUFFER9  g_pVB;
	CUSTOMVERTEX vertices[4];
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			vertices[i].x = (float)rect.left;
			vertices[i].y = (float)rect.top;
			break;
		case 1:
			vertices[i].x = (float)rect.right;
			vertices[i].y = (float)rect.top;
			break;
		case 2:
			vertices[i].x = (float)rect.right;
			vertices[i].y = (float)rect.bottom;
			break;
		case 3:
			vertices[i].x = (float)rect.left;
			vertices[i].y = (float)rect.bottom;
			break;
		default:
			break;
		}
		vertices[i].z = 0.0f;
		vertices[i].color = dwColor;
		vertices[i].rhw = 1.0f;

	}

	// Create the vertex buffer. Here we are allocating enough memory
	// (from the default pool) to hold all our 3 custom vertices. We also
	// specify the FVF, so the vertex buffer knows what data it contains.
	if (FAILED(m_pDxDevice9->CreateVertexBuffer(sizeof(vertices),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return;
	}

	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
	// gain access to the vertices. This mechanism is required becuase vertex
	// buffers may be in device memory.
	VOID* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return;
	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();

	m_pDxDevice9->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pDxDevice9->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pDxDevice9->DrawPrimitive(D3DPT_TRIANGLELIST, 0,2);
}

