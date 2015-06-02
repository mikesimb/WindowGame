#include "DxDevice9.h"
#include <windows.h>
#include "Application.h"

#include <tchar.h>
#include <strsafe.h>



/*
1���豸������ʾģʽ���
2���豸�����ļ��
3���������ݸ�ʽ�ļ��
4���豸��ʧ����Ԥ��
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
	// �����ʼ��
	if (ParentWindow == NULL) return E_FAIL;
	m_ParentWindow = ParentWindow;
	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D9 == NULL)
		return E_FAIL;


	//--------------------------------------------------------------------------------------
	// ��ȡӲ���豸��Ϣ
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(m_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //֧��Ӳ���������㣬���ǾͲ���Ӳ���������㣬���׵�
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //��֧��Ӳ���������㣬����ֻ�ò��������������


	//���D3DPRESENT_PARAMETERS�ṹ��
	//--------------------------------------------------------------------------------------
	
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

	m_font.Init(m_pDxDevice9, L"����");

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
		m_font.DrawTextW(_T("�������Ⱑ"),rc);
		CApplication * app;
		app = CApplication::Instance();

		RECT formatRect;
		GetClientRect(m_ParentWindow->GetWindowHandle(), &formatRect);
		int charCount = swprintf_s(m_strFPS, 20, _T("FPS:%0.3f"),app->Get_FPS());
		m_font.DrawTextW(m_strFPS, formatRect);
		//m_font.DrawText(NULL, m_strFPS, charCount, &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(168, 39, 136));


	
		m_pDxDevice9->EndScene();
	}

	m_pDxDevice9->Present(NULL, NULL, NULL, NULL);
}

void CDxDevice9::DrawLine(int sx, int sy, int dx, int dy, DWORD dwColor)
{
	//���ȴ������㻺����
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
	//���ȴ������㻺����
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
	m_pDxDevice9->SetIndices(pIndexBuffer);//������������  

	m_pDxDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);//��������������϶��㻺�����ͼ��  
	g_pVB->Release();
	pIndexBuffer->Release();
}




void CDxDevice9::DrawRectangle(RECT rect, DWORD dwColor)
{
	//���ȴ������㻺����
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
	m_pDxDevice9->SetIndices(pIndexBuffer);//������������  

	m_pDxDevice9->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, 8, 0, 4);//��������������϶��㻺�����ͼ��  
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
	//�豸��ʧ��ʱ����Ҫ��������m_pDxDevcie9���������е����ݡ�
	m_font.Finalize();
	m_pDxDevice9->Reset(&d3dpp);
	m_font.Init(m_pDxDevice9, _T("����"));
}

void*  CDxDevice9::ResourceLoad(const char* FileName, DWORD* size)
{

}

void CDxDevice9::TextureLoad(const char * FileName, DWORD size, BOOL bMipMap)
{
		void *data;
		DWORD _size;
		D3DFORMAT fmt1, fmt2;
		LPDIRECT3DTEXTURE9 pTex;
		D3DXIMAGE_INFO info;
//		CTextureList *texItem;

		if (size) { data = (void *)FileName; _size = size; }
		else
		{
		//	data = pHGE->Resource_Load(FileName, &_size);
			data = ResourceLoad(FileName, &_size);
			if (!data) return ;
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
			bMipmap ? 0 : 1,		// Mip levels
			0,					// Usage
			fmt1,				// Format
			D3DPOOL_MANAGED,	// Memory pool
			D3DX_FILTER_NONE,	// Filter
			D3DX_DEFAULT,		// Mip filter
			0,					// Color key
			&info, NULL,
			&pTex)))

		if (FAILED(D3DXCreateTextureFromFileInMemoryEx(m_pDxDevice9, data, _size,
			D3DX_DEFAULT, D3DX_DEFAULT,
			bMipmap ? 0 : 1,		// Mip levels
			0,					// Usage
			fmt2,				// Format
			D3DPOOL_MANAGED,	// Memory pool
			D3DX_FILTER_NONE,	// Filter
			D3DX_DEFAULT,		// Mip filter
			0,					// Color key
			&info, NULL,
			&pTex)))

		{
			_PostError("Can't create texture");
			if (!size) Resource_Free(data);
			return NULL;
		}

		if (!size) Resource_Free(data);

// 		texItem = new CTextureList;
// 		texItem->tex = (HTEXTURE)pTex;
// 		texItem->width = info.Width;
// 		texItem->height = info.Height;
// 		texItem->next = textures;
// 		textures = texItem;

		return (HTEXTURE)pTex;
	}

}

