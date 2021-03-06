#include "DxDevice9.h"
#include <windows.h>
#include "Application.h"

#include <tchar.h>
#include <strsafe.h>



/*
1、设备方面显示模式监测
2、设备能力的监测
3、顶点数据格式的监测
4、设备丢失监测和预警
*/


CDxDevice9::CDxDevice9() : m_pD3D9(NULL), m_pDxDevice9(NULL), m_ParentWindow(NULL)
{
	m_strFPS[50] = { 0 };
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
	
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferWidth = m_ParentWindow->GetWidth();
		d3dpp.BackBufferHeight = m_ParentWindow->GetHeight();
		d3dpp.BackBufferFormat =D3DFMT_UNKNOWN;
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
		//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_ParentWindow->GetWindowHandle(),
		vp,
		&d3dpp, &m_pDxDevice9)))
	{
		return E_FAIL;
	}

	m_font.Init(m_pDxDevice9, L"宋体");

	HRESULT hr = D3DXCreateTextureFromFile(m_pDxDevice9, L"E:\\WindowGame\\Demo2\\Demo2\\hum2_D(9000).png", &m_pTexture);
	if (FAILED(hr))
	{
		OutputDebugString(L"纹理创建出错。");
	}

	// Turn off culling 
	m_pDxDevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
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
		re.top = 100;
		re.right = 500;
		re.bottom = 400;

		DrawRectangle(re, 0xFF00FF00);
		RECT rc;
		rc.left = 100;
		rc.top = 200;
		rc.right = 300;
		rc.bottom =500;
		m_font.DrawTextW(_T("闪闪发光啊"),rc);
		CApplication * app;
		app = CApplication::Instance();

		RECT formatRect;
		GetClientRect(m_ParentWindow->GetWindowHandle(), &formatRect);
		int charCount = swprintf_s(m_strFPS, 20, _T("FPS:%0.3f"),app->Get_FPS());
		m_font.DrawTextW(m_strFPS, formatRect);
		//m_font.DrawText(NULL, m_strFPS, charCount, &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(168, 39, 136));
		int size = 0;
		//DrawRectangleFill(rc, 0xFFFFFFFF);
		DrawTexture(m_pTexture, rc);
		m_pDxDevice9->EndScene();
	}

	m_pDxDevice9->Present(NULL, NULL, NULL, NULL);
}


void CDxDevice9::DrawTexture(LPDIRECT3DTEXTURE9 pTexture, RECT rect, DWORD dwColor)
{
	LPDIRECT3DVERTEXBUFFER9  g_pVB;
	LPDIRECT3DINDEXBUFFER9  pIndexBuffer;
	CUSTOMVERTEX vertices[4];
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			vertices[i].x = (float)rect.left;
			vertices[i].y = (float)rect.top;
			vertices[i].u = 0.0f;
			vertices[i].v = 0.0f;
			break;
		case 1:
			vertices[i].x = (float)rect.right;
			vertices[i].y = (float)rect.top;
			vertices[i].u = 1.0f;
			vertices[i].v = 0.0f;
			break;
		case 2:
			vertices[i].x = (float)rect.left;
			vertices[i].y = (float)rect.bottom;
			vertices[i].u = 0.0f;
			vertices[i].v = 1.0f;
			break;
		case 3:
			vertices[i].x = (float)rect.right;
			vertices[i].y = (float)rect.bottom;
			vertices[i].u = 1.0f;
			vertices[i].v = 1.0f;
			break;
		default:
			break;
		}
		vertices[i].z = 0.0f;
		vertices[i].color = dwColor;
// 		vertices[i].u = 1.0f;
// 		vertices[i].v = 1.0f;
		

	}
	// Create the vertex buffer. Here we are allocating enough memory
	// (from the default pool) to hold all our 3 custom vertices. We also
	// specify the FVF, so the vertex buffer knows what data it contains.
	if (FAILED(m_pDxDevice9->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return;
	}


// 	WORD indexbuf[] = { 0, 1, 2, 0, 2, 3 };
// 
// 	if (FAILED(m_pDxDevice9->CreateIndexBuffer(4 * 4 * sizeof(WORD), 0,
// 		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &pIndexBuffer, NULL)))
// 	{
// 		return;
// 
// 	}

	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
	// gain access to the vertices. This mechanism is required becuase vertex
	// buffers may be in device memory.
	VOID* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return;
	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();

	WORD *pindexbuf = NULL;
	//pIndexBuffer->Lock(0, 0, (void**)&pindexbuf, 0);
	//memcpy(pindexbuf, indexbuf, sizeof(indexbuf));
	//pIndexBuffer->Unlock();
	m_pDxDevice9->SetTexture(0, pTexture);
// 	m_pDxDevice9->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
// 	m_pDxDevice9->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

// 	m_pDxDevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
// 	m_pDxDevice9->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
// 	m_pDxDevice9->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
// 	m_pDxDevice9->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pDxDevice9->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pDxDevice9->SetFVF(D3DFVF_CUSTOMVERTEX);
	//m_pDxDevice9->SetIndices(pIndexBuffer);//设置索引缓存  

	//m_pDxDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);//利用索引缓存配合顶点缓存绘制图形  
	m_pDxDevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0,2);
	g_pVB->Release();
//	pIndexBuffer->Release();
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



	}
	


	// Create the vertex buffer. Here we are allocating enough memory
	// (from the default pool) to hold all our 3 custom vertices. We also
	// specify the FVF, so the vertex buffer knows what data it contains.
	if (FAILED(m_pDxDevice9->CreateVertexBuffer(3*sizeof(vertices),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return;
	}

	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
	// gain access to the vertices. This mechanism is required becuase vertex
	// buffers may be in device memory.
	VOID* pVertices;
	if (g_pVB  == NULL) return;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return;
	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();

	m_pDxDevice9->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pDxDevice9->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pDxDevice9->DrawPrimitive(D3DPT_LINELIST, 0, 1);
	//m_pDxDevice9->DrawPrimitiveUP(D3DPT_LINELIST,1, NULL, D3DFMT_INDEX16, NULL, 0);
	g_pVB->Release();

	
}
void CDxDevice9::DrawRectangleFill(RECT rect, DWORD dwColor)
{
	//首先创建定点缓冲区
	LPDIRECT3DVERTEXBUFFER9  g_pVB;
	LPDIRECT3DINDEXBUFFER9  pIndexBuffer;
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


	WORD indexbuf[] = { 0, 1, 2, 0, 2, 3 };

	if (FAILED(m_pDxDevice9->CreateIndexBuffer(3 * 4 * sizeof(WORD), 0,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &pIndexBuffer, NULL)))
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

	WORD *pindexbuf = NULL;
	pIndexBuffer->Lock(0, 0, (void**)&pindexbuf, 0);
	memcpy(pindexbuf, indexbuf, sizeof(indexbuf));
	pIndexBuffer->Unlock();

	m_pDxDevice9->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pDxDevice9->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pDxDevice9->SetIndices(pIndexBuffer);//设置索引缓存  

	m_pDxDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);//利用索引缓存配合顶点缓存绘制图形  
	g_pVB->Release();
	pIndexBuffer->Release();
}




void CDxDevice9::DrawRectangle(RECT rect, DWORD dwColor)
{
	//首先创建定点缓冲区
	LPDIRECT3DVERTEXBUFFER9  g_pVB;
	LPDIRECT3DINDEXBUFFER9  pIndexBuffer;
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


	WORD indexbuf[] = {0,1,1,2,2,3,3,0 };

	if (FAILED(m_pDxDevice9->CreateIndexBuffer(2 * 16 * sizeof(WORD), 0,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &pIndexBuffer, NULL)))
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

	WORD *pindexbuf = NULL;
	pIndexBuffer->Lock(0, 0, (void**)&pindexbuf, 0);
	memcpy(pindexbuf, indexbuf, sizeof(indexbuf));
	pIndexBuffer->Unlock();

	m_pDxDevice9->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pDxDevice9->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pDxDevice9->SetIndices(pIndexBuffer);//设置索引缓存  

	m_pDxDevice9->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, 8, 0, 4);//利用索引缓存配合顶点缓存绘制图形  
	g_pVB->Release();
	pIndexBuffer->Release();
}

BOOL CDxDevice9::CheckDeviceLosted()
{
	HRESULT hr = m_pDxDevice9->TestCooperativeLevel();
	switch (hr)
	{
	case D3DERR_DEVICELOST:
		Sleep(25);
		return true;
		break;
	case D3DERR_DEVICENOTRESET:
		OnDeviceLost(m_pDxDevice9);
		return true;
	case S_OK:
		return false;
	default:
		break;
	}

	return true;

}

void CDxDevice9::OnDeviceLost(LPDIRECT3DDEVICE9 Device)
{
	//设备丢失的时候需要处理利用m_pDxDevcie9创建的所有的内容。
	m_font.Finalize();
	m_pDxDevice9->Reset(&d3dpp);
	m_font.Init(m_pDxDevice9, _T("宋体"));
}

void*  CDxDevice9::ResourceLoad(const WCHAR* FileName, DWORD* size)
{
	
	void *ptr;
	DWORD _size;
	HANDLE hF;

	hF = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
	if (hF == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	_size = GetFileSize(hF, NULL);
	ptr = malloc(_size);
	if (!ptr)
	{
		CloseHandle(hF);

		return 0;
	}
	if (ReadFile(hF, ptr, _size, &_size, NULL) == 0)
	{
		CloseHandle(hF);
		free(ptr);
		return 0;
	}

	CloseHandle(hF);
	if (size) *size = _size;
	return ptr;
}

LPDIRECT3DTEXTURE9 CDxDevice9::TextureLoad(const WCHAR * FileName, DWORD size, BOOL bMipMap)
{
		void *data;
		DWORD _size;
		D3DFORMAT fmt1, fmt2;
		LPDIRECT3DTEXTURE9 pTex =NULL;
		D3DXIMAGE_INFO info;
//		CTextureList *texItem;

		if (size) { data = (void *)FileName; _size = size; }
		else
		{
		//	data = pHGE->Resource_Load(FileName, &_size);
			data = ResourceLoad(FileName, &_size);
			if (!data) return NULL ;
		}

		if (*(DWORD*)data == 0x20534444) // Compressed DDS format magic number
		{
			fmt1 = D3DFMT_UNKNOWN;
			fmt2 = D3DFMT_A8R8G8B8;
		}
		else
		{
			fmt1 = D3DFMT_A8R8G8B8;
			fmt2 = D3DFMT_UNKNOWN;
		}

		//	if( FAILED( D3DXCreateTextureFromFileInMemory( pD3DDevice, data, _size, &pTex ) ) ) pTex=NULL;
		if (FAILED(D3DXCreateTextureFromFileInMemoryEx(m_pDxDevice9, data, _size,
			D3DX_DEFAULT, D3DX_DEFAULT,
			bMipMap ? 0 : 1,		// Mip levels
			0,					// Usage
			fmt1,				// Format
			D3DPOOL_MANAGED,	// Memory pool
			D3DX_FILTER_NONE,	// Filter
			D3DX_DEFAULT,		// Mip filter
			0,					// Color key
			&info, NULL,
			&pTex)))

		{
// 			_PostError("Can't create texture");
// 			if (!size) Resource_Free(data);
			return NULL;
		}

		//if (!size) Resource_Free(data);

// 		texItem = new CTextureList;
// 		texItem->tex = (HTEXTURE)pTex;
// 		texItem->width = info.Width;
// 		texItem->height = info.Height;
// 		texItem->next = textures;
// 		textures = texItem;

		return pTex;

}

