#include "Application.h"


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

