#pragma once
#include "stdinclud.h"
#include "BaseWindow.h"
#include "DxDevice9.h"



class CApplication :public BaseWindowListener, public CBaseWindowMsgControlListener
{
public:
	CApplication();
	virtual ~CApplication();

	BOOL Initizlized();
	void Run();

	BOOL CreateForm();
	BOOL CreateDevice();

	static CApplication * Instance();

	virtual void OnCreate(CBaseWindow * sender);
	virtual void OnDestroy(CBaseWindow * sender); 
	virtual void OnShow(CBaseWindow * sender); 
	virtual void OnHide(CBaseWindow * sender); 
	virtual void OnClose(CBaseWindow *sender); 
	virtual void OnPaint(CBaseWindow *sender, HDC dc);
	virtual void OnSizeChange(CBaseWindow *sender, UINT ntype, int cx, int cy);
	virtual void OnPosChange(CBaseWindow *sender);
	virtual BOOL OnWndProc(CBaseWindow *sender, UINT msg, WPARAM wParam, LPARAM lParam, HRESULT Ret);


	virtual BOOL OnProcessIdel(CBaseWindowMsgControl* Sender);
	virtual BOOL OnProcessMsg(CBaseWindowMsgControl* Sender, MSG & Msg);
	float Get_FPS();

private:
	CBaseWindow  m_MainForm;
	CDxDevice9  m_DxDevice;
};

