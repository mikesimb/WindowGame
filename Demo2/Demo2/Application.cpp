#include "Application.h"
static CApplication*  m_Application;

CApplication::CApplication() 
{
}


CApplication::~CApplication()
{
}

BOOL CApplication::CreateForm()
{
	m_MainForm.SetMsgControlListener(this);
	m_MainForm.SetWindowListener(this);
	BOOL Result =  m_MainForm.Create();
	if (Result)
	{
		m_MainForm.ShowWindow();
	}

	return Result;

}


float CApplication::Get_FPS()
{

	//�����ĸ���̬����
	static float  fps = 0; //������Ҫ�����FPSֵ
	static int    frameCount = 0;//֡��
	static float  currentTime = 0.0f;//��ǰʱ��
	static float  lastTime = 0.0f;//����ʱ��

	frameCount++;//ÿ����һ��Get_FPS()������֡������1
	currentTime = timeGetTime()*0.001f;//��ȡϵͳʱ�䣬����timeGetTime�������ص����Ժ���Ϊ��λ��ϵͳʱ�䣬������Ҫ����0.001���õ���λΪ���ʱ��

	//�����ǰʱ���ȥ����ʱ�������1���ӣ��ͽ���һ��FPS�ļ���ͳ���ʱ��ĸ��£�����֡��ֵ����
	if (currentTime - lastTime > 1.0f) //��ʱ�������1����
	{
		fps = (float)frameCount / (currentTime - lastTime);//������1���ӵ�FPSֵ
		lastTime = currentTime; //����ǰʱ��currentTime��������ʱ��lastTime����Ϊ��һ��Ļ�׼ʱ��
		frameCount = 0;//������֡��frameCountֵ����
	}

	return fps;
}

BOOL CApplication::OnProcessIdel(CBaseWindowMsgControl* Sender)
{
	

	m_DxDevice.Render();
	return TRUE;
}

BOOL CApplication::OnProcessMsg(CBaseWindowMsgControl* Sender, MSG & Msg)
{
	return FALSE;

}

void CApplication::OnCreate(CBaseWindow * sender)
{

}

void CApplication::OnDestroy(CBaseWindow * sender)
{

}

void CApplication::OnShow(CBaseWindow * sender)
{

}

void CApplication::OnHide(CBaseWindow * sender)
{

}

void CApplication::OnClose(CBaseWindow *sender)
{

}

void CApplication::OnPaint(CBaseWindow *sender, HDC dc)
{

}

void CApplication::OnSizeChange(CBaseWindow *sender, UINT ntype, int cx, int cy)
{

}

void CApplication::OnPosChange(CBaseWindow *sender)
{

}

BOOL CApplication::OnWndProc(CBaseWindow *sender, UINT msg, WPARAM wParam, LPARAM lParam ,HRESULT Ret)
{
	return TRUE;
}

BOOL CApplication::Initizlized()
{
	if (CreateForm())
	{
		if (CreateDevice())
			return true;
		else
		{
			::MessageBox(0, L"�����豸ʧ�ܣ�", L"Warning", MB_OK);
			return FALSE;
		}
		

	}
	else
	{
		::MessageBox(0, L"��������ʧ�ܣ�", L"Warning", MB_OK);
		return FALSE;

	}

}

BOOL CApplication::CreateDevice()
{
	
	return m_DxDevice.InitiD3D(&m_MainForm)==S_OK;
}

void CApplication::Run()
{
		m_MainForm.Run();
}

CApplication * CApplication::Instance()
{
	if (m_Application == NULL)
	{
		m_Application = new CApplication();
		m_Application->Initizlized();
	
	}
	
		return m_Application;
}

