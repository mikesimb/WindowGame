#pragma once
#include <string>
#include <windows.h>
#include <assert.h>

#include "WinMsgControler.h"

typedef DWORD BorderIcons;
#define biSysMenu		0x01		// 系统菜单
#define biMinimize		0x02		// 最大化按钮
#define biMaximize	0x04		// 最小化按钮

enum BorderStyle
{
	BS_None,					// 无边框
	BS_Single,					// 细边框，不可拉动大小
	BS_Sizeable					// 厚边框，可以拉动大小
};

enum WindowState
{
	WS_Normal,					// 还原
	WS_Minimized,				// 最小化
	WS_Maximized					// 最大化
};

/*
关窗口模式
*/
enum CloseMode
{
	CM_HideWindow,				// 隐藏窗口
	CM_FreeHandle,				// 消毁窗口句柄
	CM_TermApp					// 结束应用程序
};

/*
窗口类名
*/
#define WINDOWS_CLSNAME 	L"ZQWinBase.Frame"
#define WINDOWS_ATOM		L"ZQWinBase.Frame.ATOM"

class CZQWinBase;

struct WNDFrameListener
{
	virtual void OnCreate(CZQWinBase  * Win){}
	virtual void OnDestroy(CZQWinBase  * Win){}
	virtual void OnShow(CZQWinBase  * Win){}
	virtual void OnHide(CZQWinBase  * Win){}
	virtual void OnClose(CZQWinBase  * Win,CloseMode close){}
	virtual void OnPaint(HDC  hdc){}
	virtual void OnSizeChange(CZQWinBase  * Win,UINT nType ,int cx,int cy ){}
	virtual void OnPosChange(CZQWinBase * Win);
	virtual BOOL OnCloseQuery(CZQWinBase * Win){ return TRUE; }
	virtual BOOL OnWndProc(CZQWinBase * win, UINT msg, WPARAM wparam, LPARAM  lparam,HRESULT& ret){ return FALSE; }

};


class CZQWinBase:public CWinMsgControler
{
public:
	CZQWinBase() : m_hWND(NULL), m_WndListener(NULL), m_nleft(0), m_nTop(0), m_nWidth(0), m_nHeight(0), m_hInstance(0)

	{

	}
	virtual ~CZQWinBase()
	{
		if (m_hWND)
			::DestroyWindow(m_hWND);
	}

	BOOL Create(int left =0,
						int top=0,
						int Width=1024,
						int height=768,
						BorderIcons bis= biSysMenu | biMinimize ,
						BorderStyle bs = BS_Single,
						BOOL bChangeSizeable = FALSE,
						LPCWSTR   caption=L"",
						HICON  icon=NULL);

	void Destroy();
	HWND GetHandle();
	void Show(WindowState state = WS_Normal);
	void Close(BOOL bQuery = TRUE);
	void Hide();
	void MoveToTop();
	void MoveToBottom();
	void setBound(int left, int top, int width, int height);
	void setBound(const RECT& rc);
	RECT getBoundRect();
	void setWinPos(int percentX = 50, int percentY = 50);

	int getLeft();
	void setLeft(int left);
	int getTop();
	void setTop(int top);
	
	int getWidth();
	void setWidth(int width);

	int getHeight();
	void setHeight(int Height);

	SIZE getClientSize();
	void SetClientSize(SIZE size);

	BOOL isEnable();
	void setEnable(BOOL enable);

	BOOL isTopMost();
	void setTopMost(BOOL topMost);

	BOOL isVisible();
	void setVisible(BOOL visible);

	BorderIcons getBorderIcons();
	void setBorderIcons(BorderIcons bis);

	BorderStyle getBorderStyle();
	void setBorderStyle(BorderStyle bs);

	WindowState getWindowState();
	void setWindowState(WindowState state);

	std::wstring  getCaption();
	void setCaption(const std::wstring& caption);

	void setIcon(int nIconID);
	void setBigIcon(int nIconID);
	void setSmallIcon(int nIonID);

	WNDFrameListener * getListener();
	void setListener(WNDFrameListener * listener);

	void setInstance(HINSTANCE hInstance);
protected: 
	virtual BOOL RegWndClass();
	DWORD BorderIconsToStyle(DWORD oldStyle, BorderIcons bis);
	DWORD BorderStyleToStyle(DWORD oldStyle, BorderStyle bs);
	BorderIcons StyleToBorderIcons(DWORD style);
	BorderStyle StyleToBorderStyle(DWORD style);

	void UpdateBounds();

	virtual void OnPaint(HDC dc);
	virtual BOOL CloseQuery();
	virtual void OnClose(CloseMode& mode);
	virtual void OnShow();
	virtual void OnHide();
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnSizeChange(UINT nType, INT cx, INT cy);

	virtual void OnPosChange();

	static LRESULT CALLBACK InitWndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM  lparam);
	static LRESULT CALLBACK StdWndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	virtual BOOL WndPro(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& ret);



private:
	HWND m_hWND;
	int m_nleft;
	int m_nTop;
	int m_nWidth;
	int m_nHeight;
	WNDFrameListener*	m_WndListener;
	HINSTANCE m_hInstance;
	
};

inline BOOL CZQWinBase::Create(int left,
	int top,
	int Width,
	int height ,
	BorderIcons bis,
	BorderStyle bs ,
	BOOL bChangeSizeable ,
	LPCWSTR   caption ,
	HICON  icon )
{
	m_nleft = left;
	m_nTop = top;
	m_nWidth = Width;
	m_nHeight = height;

	if (RegWndClass())
	{
		DWORD  dwTag = 0; 
		if (bChangeSizeable)
			dwTag = WS_THICKFRAME;

		m_hWND = CreateWindow(WINDOWS_CLSNAME,
												caption,
												dwTag | BorderIconsToStyle(0, bis) | BorderStyleToStyle(0, bs),
												left,
												top,
												Width,
												height,
												NULL,
												NULL,
												0,
												this);
		if (m_hWND)
		{
			::SendMessage(m_hWND, WM_SETICON, ICON_BIG, LPARAM(icon));
			OnCreate();
		}
		assert(m_hWND);
		setWinPos();
	
	}
	return NULL != m_hWND;
}

inline void CZQWinBase::Destroy()
{
	::DestroyWindow(m_hWND);
}

inline HWND CZQWinBase::GetHandle()
{
	return m_hWND;
}

inline void CZQWinBase::Show(WindowState state)
{
	setWindowState(state);
	MoveToTop();
	OnShow();
}

inline void CZQWinBase::Close(BOOL bQuery)
{
	assert(m_hWND);
	if (!bQuery || CloseQuery())
	{
		CloseMode closemode = CM_TermApp;

		OnClose(closemode);

		switch (closemode)
		{
		case CM_HideWindow:
			Hide();
			break;
		case CM_FreeHandle:
			::DestroyWindow(m_hWND);
			break;
		case CM_TermApp:
			::DestroyWindow(m_hWND);
			break;
		}
	}

}

inline void CZQWinBase::Hide()
{
	setVisible(FALSE);
}

inline void CZQWinBase::MoveToTop()
{
	assert(m_hWND);
	::SetWindowPos(m_hWND, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

inline void CZQWinBase::setBound(int left, int top, int width, int Height)
{
	assert(m_hWND);
	if (left != m_nleft || top != m_nTop || width != m_nWidth || Height != m_nHeight)
	{
		if (!IsIconic(m_hWND))
		{
			::SetWindowPos(m_hWND, 0, left, top, width, Height, SWP_NOZORDER + SWP_NOACTIVATE);
		}
	}
}

inline void CZQWinBase::setBound(const RECT&rc)
{
	setBound(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

inline RECT CZQWinBase::getBoundRect()
{
	RECT rc;
	::SetRect(&rc, m_nleft, m_nTop, m_nWidth, m_nHeight);
	return rc;
}

inline void CZQWinBase::setWinPos(int percentX, int percentY)
{
	assert(m_hWND);
	if (percentX < 0) percentX = 0;
	if (percentY < 0) percentY = 0;
	if (percentX  > 100) percentX = 100;
	if (percentY > 100) percentY = 100;

	SIZE szTaskBar = { 0, 0 };
	RECT rc = { 0 };
	if (::SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0))
	{
		szTaskBar.cx = ::GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left);
		szTaskBar.cy = ::GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top);

	}

	setBound(int((GetSystemMetrics(SM_CXSCREEN) - m_nWidth)*((double)percentX / 100)) - szTaskBar.cx / 2,
		int((GetSystemMetrics(SM_CYSCREEN) - m_nHeight)*((double)percentY / 100)) - szTaskBar.cy / 2,
		m_nWidth,
		m_nHeight);


}
inline int CZQWinBase::getLeft()
{
	return m_nleft;
}

inline void  CZQWinBase::setLeft(int left)
{
	setBound(left, m_nTop, m_nWidth, m_nHeight);
}

inline int CZQWinBase::getTop()
{
	return m_nTop;
}

inline void CZQWinBase::setTop(int top)
{
	setBound(m_nleft, top, m_nWidth, m_nHeight);
}

inline int CZQWinBase::getWidth()
{
	return m_nWidth;
}

inline void CZQWinBase::setWidth(int width)
{
	setBound(m_nleft, m_nTop, width, m_nHeight);
}

inline int CZQWinBase::getHeight()
{
	return m_nHeight;
}

inline void CZQWinBase::setHeight(int height)
{
	setBound(m_nleft, m_nTop, m_nWidth, height);
}

inline SIZE CZQWinBase::getClientSize()
{
	assert(m_hWND);
	RECT rc;
	::GetClientRect(m_hWND, &rc);
	SIZE sz;
	sz.cx = rc.right;
	sz.cy = rc.bottom;

	return sz;
}

inline void CZQWinBase::SetClientSize(SIZE size)
{
	SIZE sz = getClientSize();
	setBound(m_nleft, m_nTop, m_nWidth - sz.cx + size.cx,
		m_nHeight - sz.cy + size.cy);
}

inline BOOL CZQWinBase::isEnable()
{
	assert(m_hWND);
	return ::IsWindowEnabled(m_hWND);
}

inline void CZQWinBase::setEnable(BOOL enable)
{
	assert(m_hWND);
    ::EnableWindow(m_hWND, enable);
}

inline BOOL  CZQWinBase::isTopMost()
{
	assert(m_hWND);
	LONG style = ::GetWindowLong(m_hWND, GWL_EXSTYLE);
	return (style & WS_EX_TOPMOST) != 0;
}

inline void CZQWinBase::setTopMost(BOOL topMost)
{
	assert(m_hWND);
	::SetWindowPos(m_hWND, topMost ? HWND_TOPMOST : HWND_NOTOPMOST,
		0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

inline BOOL CZQWinBase::isVisible()
{
	assert(m_hWND);
	return ::IsWindowVisible(m_hWND);
}

inline void CZQWinBase::setVisible(BOOL visible)
{
	assert(m_hWND);
	if (visible)
	{
		if (::IsIconic(m_hWND))
			::ShowWindow(m_hWND, SW_SHOWMINNOACTIVE);
		else if (::IsZoomed(m_hWND))
			::ShowWindow(m_hWND, SW_SHOWMAXIMIZED);
		else
			::ShowWindow(m_hWND, SW_SHOWNORMAL);
		OnShow();

	}
	else
	{
		OnHide();
		::ShowWindow(m_hWND, SW_HIDE);
	}
}

inline BorderIcons CZQWinBase::getBorderIcons()
{
	assert(m_hWND);
	return StyleToBorderIcons(GetWindowLong(m_hWND, GWL_STYLE));

}

inline void CZQWinBase::setBorderIcons(BorderIcons bis)
{
	assert(m_hWND);
	::SetWindowLong(m_hWND, GWL_STYLE, BorderIconsToStyle(::GetWindowLong(m_hWND, GWL_STYLE), bis));

	::SetWindowPos(m_hWND, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}


inline BorderStyle CZQWinBase::getBorderStyle()
{
	assert(m_hWND);
	return StyleToBorderStyle(::GetWindowLong(m_hWND, GWL_STYLE));
}

inline void CZQWinBase::setBorderStyle(BorderStyle bs)
{
	assert(m_hWND);
	::SetWindowLong(m_hWND, GWL_STYLE, BorderStyleToStyle(::GetWindowLong(m_hWND, GWL_STYLE), bs));
	::SetWindowPos(m_hWND, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

inline WindowState CZQWinBase::getWindowState()
{
	assert(m_hWND);
	if (::IsIconic(m_hWND))
		return WS_Minimized;
	else
	if (::IsZoomed(m_hWND))
		return WS_Maximized;
	else
		return WS_Normal;

}

inline void CZQWinBase::setWindowState(WindowState state)
{
	assert(m_hWND);
	int cmd = SW_SHOWNORMAL;
	switch (state)
	{
	case WS_Maximized:
		cmd = SW_SHOWNORMAL;
		break;
	case WS_Minimized:
		cmd = SW_MINIMIZE;
		break;
	default:
		cmd = SW_SHOWNORMAL;
		break;
	}
	::ShowWindow(m_hWND, cmd);
}

inline std::wstring CZQWinBase::getCaption()
{
	assert(m_hWND);
	std::wstring str ;
	int len = (int)::SendMessage(m_hWND, WM_GETTEXTLENGTH, 0, 0);
	if (len > 0)
	{
		WCHAR * text = new WCHAR[len + 1];
		ZeroMemory(text, sizeof(WCHAR)*(len + 1));
		::GetWindowText(m_hWND, text, len + 1);
		str = text;
		delete[]text;
	}
	return str;
}

inline void CZQWinBase::setCaption(const std::wstring& caption)
{
	assert(m_hWND);
	::SetWindowText(m_hWND, caption.c_str());
}

inline void CZQWinBase::setIcon(int nIconID)
{
	LONG lRet = ::GetWindowLong(m_hWND, GWL_HINSTANCE);
	HICON hIcon = LoadIcon((HINSTANCE)lRet, MAKEINTRESOURCE(nIconID));
	::SendMessage(m_hWND, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	::SendMessageW(m_hWND, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}


inline void CZQWinBase::setSmallIcon(int nIconID)
{
	LONG lRet = ::GetWindowLong(m_hWND, GWL_HINSTANCE);
	HICON hIcon = LoadIcon((HINSTANCE)lRet, MAKEINTRESOURCE(nIconID));
	::SendMessage(m_hWND, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}

inline void CZQWinBase::setBigIcon(int nIconID)
{
	LONG lRet = ::GetWindowLong(m_hWND, GWL_HINSTANCE);
	HICON hIcon = LoadIcon((HINSTANCE)lRet, MAKEINTRESOURCE(nIconID));
	::SendMessage(m_hWND, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}

inline WNDFrameListener * CZQWinBase::getListener()
{
	return m_WndListener;
}

inline void CZQWinBase::setListener(WNDFrameListener * listener)
{
	m_WndListener = listener;
}

inline BOOL CZQWinBase::RegWndClass()
{
	WNDCLASS wc;
	BOOL bReg = ::GetClassInfoW(m_hInstance, WINDOWS_CLSNAME, &wc);
	if (bReg && (wc.lpfnWndProc != &InitWndProc))
	{
		UnregisterClass(WINDOWS_CLSNAME, m_hInstance);
		bReg = false;
	}
	if (!bReg)
	{
		wc.style = CS_DBLCLKS;
		wc.lpfnWndProc = &InitWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = 0;
		wc.hCursor = ::LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		wc.lpszClassName = WINDOWS_CLSNAME;
		wc.lpszMenuName = NULL;
		return (RegisterClassW(&wc) != 0);
	}
	return TRUE;
}




inline DWORD CZQWinBase::BorderIconsToStyle(DWORD oldStyle, BorderIcons bis)
{
	DWORD style = oldStyle;
	style &= ~(WS_SYSMENU | WS_MAXIMIZEBOX | WS_MAXIMIZEBOX);

	if ((style & WS_SYSMENU) == 0)	{
		style |= WS_SYSMENU;
	}
	
	if ((style & WS_SYSMENU )!= 0)
	{
	if ((bis & biMaximize) != 0)
		style |=WS_MAXIMIZEBOX;
	if ((bis & biMinimize) != 0)
		style |= WS_MINIMIZEBOX;

	}
	return style;
}

inline DWORD CZQWinBase::BorderStyleToStyle(DWORD oldStyle, BorderStyle bs)
{
	DWORD style = oldStyle;
	style &= ~( WS_THICKFRAME | WS_BORDER | WS_CAPTION | WS_POPUP);
	if (bs == BS_None)
		style |= WS_POPUP;
	if (bs == BS_Single)
		style |= WS_CAPTION | WS_BORDER;
	if (bs == BS_Sizeable)
		style |= WS_CAPTION | WS_THICKFRAME;

	return style;
}

inline BorderIcons CZQWinBase::StyleToBorderIcons(DWORD style)
{
	BorderIcons bis = 0;
	if ((style & WS_SYSMENU) != 0)
	{
		bis |= biSysMenu;
		if ((style & WS_MAXIMIZEBOX) != 0)
			bis |= biMaximize;
		if ((style & WS_MINIMIZEBOX) != 0)
			bis |= biMinimize;
	}
	return bis;
}

inline BorderStyle CZQWinBase::StyleToBorderStyle(DWORD style)
{
	BorderStyle bi = BS_None;
	if ((style |= WS_CAPTION) != 0)
	{
		if ((style |= WS_THICKFRAME) != 0)
			bi = BS_Sizeable;
		else
			bi = BS_Single;
	}
	return bi;
}

inline void CZQWinBase::UpdateBounds()
{
	assert(m_hWND);
	RECT rc;
	if (::IsIconic(m_hWND))
	{
		WINDOWPLACEMENT wp;
		wp.length = sizeof(wp);
		::GetWindowPlacement(m_hWND, &wp);
		if ((wp.flags |= WPF_RESTORETOMAXIMIZED)!=0)
		{
			rc.left = wp.ptMaxPosition.x;
			rc.top = wp.ptMaxPosition.y;
			rc.right = rc.left + m_nWidth;
			rc.bottom = rc.top + m_nHeight;
		}
		else
		{
			rc = wp.rcNormalPosition;
		}
	}
	else
	{
		::GetWindowRect(m_hWND, &rc);
	}


	m_nleft = rc.left;
	m_nTop = rc.top;

	INT nWidth = rc.right - rc.left;
	INT nHeight = rc.bottom - rc.top;

	//检查窗口大小是否改变
	if (nWidth != m_nWidth || nHeight != m_nHeight)
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		OnSizeChange(0, 0, 0);
	}
}

inline void CZQWinBase::OnPaint(HDC dc)
 {
 	if (m_WndListener)
 		m_WndListener->OnPaint(dc);
 }

inline BOOL CZQWinBase::CloseQuery()
{
	if (m_WndListener)
		return m_WndListener->OnCloseQuery(this);
	return TRUE;
}

inline void CZQWinBase::OnClose(CloseMode& mode)
{
	if (m_WndListener)
		m_WndListener->OnClose(this, mode);
}

inline void CZQWinBase::OnShow()
{
	if (m_WndListener)
		m_WndListener->OnShow(this);
}

inline void CZQWinBase::OnHide()
{
	if (m_WndListener)
		m_WndListener->OnHide(this);
}

inline void CZQWinBase::OnCreate()
{
	if (m_WndListener)
		m_WndListener->OnCreate(this);
}

inline void CZQWinBase::OnDestroy()
{
	if (m_WndListener)
		m_WndListener->OnDestroy(this);
}

inline void CZQWinBase::OnSizeChange(UINT nType, INT cx, INT cy)
{
	if (m_WndListener && WS_Minimized != getWindowState())
		m_WndListener->OnSizeChange(this, nType, cx, cy);
}

inline void CZQWinBase::OnPosChange()
{
	if (m_WndListener)
		m_WndListener->OnPosChange(this);
}

inline LRESULT CALLBACK	CZQWinBase::InitWndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_CREATE)
	{
		assert(lparam);
		LPCREATESTRUCTW pcs = (LPCREATESTRUCTW)lparam;

		assert(pcs->lpCreateParams);
		CZQWinBase* wndFrame = (CZQWinBase*)pcs->lpCreateParams;
		wndFrame->m_hWND = hWnd;

		assert(::GetPropW(hWnd, WINDOWS_ATOM) == NULL);
		::SetWindowLongW(hWnd, GWL_WNDPROC, (LONG)StdWndProc);
		::SetPropW(hWnd, WINDOWS_ATOM, (HANDLE)pcs->lpCreateParams);

		LRESULT ret = TRUE;
		wndFrame->WndPro(hWnd, msg, wparam, lparam, ret);
		return ret;
	}
	return ::CallWindowProcW(DefWindowProcW, hWnd, msg, wparam, lparam);
}

inline LRESULT CALLBACK CZQWinBase::StdWndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	CZQWinBase* wndFrame = (CZQWinBase*)GetPropW(hWnd, WINDOWS_ATOM);
	assert(wndFrame);

	LRESULT ret = 0;
	BOOL isDone = wndFrame->WndPro(hWnd, msg, wparam, lparam, ret);
	if (isDone)
		return ret;
	else
		return ::CallWindowProcW(DefWindowProcW, hWnd, msg, wparam, lparam);
}

/*
类的窗口过程
ret 为标准窗口过程的返回值
return 如果返回TRUE，标准窗口过程将返回ret; 如果返回FALSE，交给默认处理过程去处理
*/
inline BOOL CZQWinBase::WndPro(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& ret)
{
	if (m_hWND == NULL)
		return FALSE;

	if (m_WndListener && m_WndListener->OnWndProc(this, msg, wparam, lparam, ret))
		return TRUE;

	switch (msg)
	{
	case WM_PAINT:
	{
					 PAINTSTRUCT ps;
					 HDC dc = ::BeginPaint(hWnd, &ps);
					 OnPaint(dc);
					 EndPaint(hWnd, &ps);
					 return TRUE;
	}
	case WM_SYSKEYDOWN:
	{
						  if (wparam == VK_F10 || wparam == VK_MENU)
							  return TRUE;
						  break;
	}
	case WM_ERASEBKGND:
	{
						  ret = 1;
						  return TRUE;
	}
	case WM_CLOSE:
		Close();
		return TRUE;
	case WM_SIZE:
		UpdateBounds();
		OnSizeChange(wparam, lparam & 0xffff, (lparam & 0xffff0000) >> 16);
		break;
	case WM_MOVE:
		UpdateBounds();
		OnPosChange();
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		OnDestroy();
		break;
	case WM_NCDESTROY:
		m_hWND = NULL;
		break;
	default:
		break;
	}
	return FALSE;
}

inline void CZQWinBase::setInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
}



