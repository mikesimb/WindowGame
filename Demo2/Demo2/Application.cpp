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

	//定义四个静态变量
	static float  fps = 0; //我们需要计算的FPS值
	static int    frameCount = 0;//帧数
	static float  currentTime = 0.0f;//当前时间
	static float  lastTime = 0.0f;//持续时间

	frameCount++;//每调用一次Get_FPS()函数，帧数自增1
	currentTime = timeGetTime()*0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间，所以需要乘以0.001，得到单位为秒的时间

	//如果当前时间减去持续时间大于了1秒钟，就进行一次FPS的计算和持续时间的更新，并将帧数值清零
	if (currentTime - lastTime > 1.0f) //将时间控制在1秒钟
	{
		fps = (float)frameCount / (currentTime - lastTime);//计算这1秒钟的FPS值
		lastTime = currentTime; //将当前时间currentTime赋给持续时间lastTime，作为下一秒的基准时间
		frameCount = 0;//将本次帧数frameCount值清零
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
			::MessageBox(0, L"创建设备失败！", L"Warning", MB_OK);
			return FALSE;
		}
		

	}
	else
	{
		::MessageBox(0, L"创建窗口失败！", L"Warning", MB_OK);
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

