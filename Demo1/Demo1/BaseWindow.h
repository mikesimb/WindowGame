#pragma once
#include "BaseWindowMessageControl.h"
#include <string>
#include <assert.h>


#define WINDOW_CLASSNAME L"BASEWINDOWS_CLASSNAME"
#define WINDOW_ATOM      L"BASEWINDOWS_ATOM"

//窗口右上角的按钮
typedef DWORD BorderIcons;
#define biSystemMenu      0x01
#define biMinimize        0x02
#define biMaxmize         0x04




//窗口边框样式
enum BorderStyle
{
	bs_None,
	bs_Single,
	bs_Sizeable
};


enum  WindowState
{
	WS_Normal,
	WS_Minimized,
	WS_Maximized,

};
class CBaseWindow;

struct BaseWindowListener
{
	virtual void OnCreate(CBaseWindow * sender){}
	virtual void OnDestroy(CBaseWindow * sender){}
	virtual void OnShow(CBaseWindow * sender){}
	virtual void OnHide(CBaseWindow * sender){}
	virtual void OnClose(CBaseWindow *sender){}
	virtual void OnPaint(CBaseWindow *sender,HDC dc){}
	virtual void OnSizeChange(CBaseWindow *sender, UINT ntype, int cx, int cy);
	virtual void OnPosChange(CBaseWindow *sender){}
	virtual BOOL OnWndProc(CBaseWindow *sender, UINT msg, WPARAM wParam, LPARAM lParam);
};


class CBaseWindow :
	public CBaseWindowMsgControl
{
public:

	CBaseWindow():
		m_Instance(0), m_hwnd(NULL),
		m_nWidth(0), m_nHeight(0), m_nLeft(0), m_nTop(0)
	{
	}

	virtual ~CBaseWindow()
	{
		if (m_hwnd)
		{
			::DestroyWindow(m_hwnd);
		}
	}

	BOOL Create(int Left = 0,
		int Top = 0,
		int Width = 1024,
		int Height = 600,
		BorderIcons BI = biSystemMenu | biMinimize,
		BorderStyle bs = bs_Single,
		BOOL bResize = FALSE,
		LPCWSTR caption = L"",
		HICON  icon = NULL
		);
	void Destroy();
	HWND GetWindowHandle();
	void ShowWindow();
	void Close();
	void Hide();
	void MoveToTop();
	void MoveToBottom();
	void SetBound(int Left, int top, int width, int height);
	void SetBound(const RECT & rc);
	RECT GetBoundRect();

	void SetWindowState(WindowState state);
	WindowState GetWindowState();

	void SetLeft(int left);
	int GetLeft();

	void SetTop(int Top);
	int GetTop();

	void SetHeight(int Height);
	int GetHeight();

	void SetWidth(int Width);
	int GetWidth();

	SIZE GetClientSize();
	void SetClientSize(SIZE size);

	BOOL Enabled();
	void SetEnabled(BOOL bEnabled);

	BOOL TopMost();
	void SetTopMost(BOOL bTopMost);

	BOOL Visabled();
	void SetVisable(BOOL bVisable);

	BorderIcons GetBorderIcons();
	void SetBorderIcons(BorderIcons bi);

	BorderStyle GetBorderStyle();
	void SetBorderStyle(BorderStyle bs);

	std::wstring GetCaption();
	void SetCaption(const std::wstring& caption);

	void SetIcon(int nIcon);

	//这个是不是放在这里现在还是个疑问？？？
	void SetInstance(HINSTANCE instance);
	void SetWindowListener(BaseWindowListener* listener);

	HRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static HRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


protected:
	virtual BOOL RegisterWndClass();
	DWORD BorderIconsToWindowStyle(DWORD style ,BorderIcons bi);
	DWORD BorderStyleToWindowStyle(DWORD style, BorderStyle bs);

	BorderIcons WindowStyleToBorderIcons(DWORD style);
	BorderStyle WindowStyleToBorderStyle(DWORD style);

	void UpdateBounds();
	void UpdateWindowPos(int x = 50, int y = 50);

	virtual void OnPaint(HDC dc);


	//virtual BOOL Close();
	virtual void OnShow();
	virtual void OnHide();
	virtual void OnClose();

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnSizeChange(UINT nType, INT cx, INT cy);
	virtual void OnPosChange();

	

private:
	HINSTANCE	 m_Instance;
	HWND		 m_hwnd;
	INT          m_nLeft;
	INT          m_nTop;
	INT			 m_nWidth;
	INT			 m_nHeight;
	BorderIcons  m_BorderIcons;
	BorderStyle  m_BorderStyle;
	INT			 m_nBorderWidth;
	//根据下面的实现这个变量是可以不用的
	//std::wstring m_strCaption;
	INT			 m_nClientHeight;
	INT			 m_nClientWidth;
	BOOL		 m_bCtl3D;
	BOOL		 m_bEnabled;
	BOOL		 m_bVisible;
	WindowState  m_WindowState;

	BaseWindowListener* m_Listener;

};

inline BOOL CBaseWindow::Create(int Left /* = 0 */, int Top /* = 0 */, int Width /* = 0 */, int Height /* = 0 */, BorderIcons BI /* = biSystemMenu | biMinimize */, BorderStyle bs /* = bs_Single */, BOOL bResize /* = FALSE */, LPCWSTR caption /* = L"" */, HICON icon /* = NULL */)
{
	m_nTop = Top;
	m_nLeft = Left;
	m_nWidth = Width;
	m_nHeight = Height;
	if (RegisterWndClass())
	{
		DWORD dwTag = 0;
		if (bResize)
			dwTag = WS_THICKFRAME;
		m_hwnd = CreateWindow(WINDOW_CLASSNAME,
					caption,
					dwTag | BorderIconsToWindowStyle(0, BI) | BorderStyleToWindowStyle(0, bs),
					Left,
					Top,
					Width,
					Height,
					NULL,
					NULL,
					0,
					this);
		if (m_hwnd)
		{
			::SendMessage(m_hwnd, WM_SETICON, ICON_BIG, LPARAM(icon));
			OnCreate();
		}
		assert(m_hwnd);
		UpdateWindowPos();

	}
	return NULL != m_hwnd;
}

void CBaseWindow::UpdateWindowPos(int x , int y)
{
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x > 100) x = 100;
	if (y > 100) y = 100;

	SIZE szTaskBar = { 0, 0 };
	RECT rc = { 0 };
	if (::SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0))
	{
		szTaskBar.cx = ::GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left);
		szTaskBar.cy = ::GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top);

	}

	int nleft = int((GetSystemMetrics(SM_CXSCREEN) - m_nWidth)* ((double)x / 100)) - szTaskBar.cx / 2;
	int ntop = int((GetSystemMetrics(SM_CYSCREEN) - m_nHeight)* ((double)y / 100)) - szTaskBar.cy / 2;
	SetBound(nleft, ntop, m_nWidth, m_nHeight);
}

inline void CBaseWindow::Destroy()
{
	::DestroyWindow(m_hwnd);
}

inline HWND CBaseWindow::GetWindowHandle()
{
	return m_hwnd;
}

inline void CBaseWindow::ShowWindow()
{
	MoveToTop();
	assert(m_hwnd);
	int cmd = SW_SHOWNORMAL;
	switch (m_WindowState)
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
	::ShowWindow(m_hwnd, cmd);
}

inline void CBaseWindow::Close()
{
	//这里关闭分为很多种是不是需要?可以参考DLEPHI xe7里TForm里的方法。
	//目前这里只放一个方法。
	::DestroyWindow(m_hwnd);
	Quit();
}

inline void CBaseWindow::Hide()
{
	SetVisable(FALSE);
}


inline void CBaseWindow::MoveToTop()
{
	assert(m_hwnd);
	::SetWindowPos(m_hwnd, HWND_TOP,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);


}

inline void CBaseWindow::MoveToBottom()
{
	assert(m_hwnd);
	::SetWindowPos(m_hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

inline void CBaseWindow::SetBound(int Left, int top, int width, int height)
{
	assert(m_hwnd);
	if (Left != m_nLeft || top != m_nTop || width != m_nWidth || height != m_nHeight)
	{
		if (!IsIconic(m_hwnd))
		{
			::SetWindowPos(m_hwnd, 0, Left, top, width, height, SWP_NOMOVE | SWP_NOACTIVATE);
		}
	}
}

inline void CBaseWindow::SetBound(const RECT & rc)
{
	SetBound(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

inline std::wstring CBaseWindow::GetCaption()
{
	assert(m_hwnd);
	std::wstring str;
	int len = (int)::SendMessage(m_hwnd, WM_GETTEXTLENGTH, 0, 0);
	if (len > 0)
	{
		WCHAR* text = new WCHAR[len + 1];
		ZeroMemory(text, sizeof(WCHAR)*(len + 1));
		::GetWindowText(m_hwnd, text, len + 1);
		str = text;
		delete[]text;
	}
	return str;

}

inline void CBaseWindow::SetCaption(const std::wstring & caption)
{
	assert(m_hwnd);
	::SetWindowText(m_hwnd, caption.c_str());
}

inline RECT CBaseWindow::GetBoundRect()
{
	RECT rc;
	::SetRect(&rc, m_nLeft, m_nTop, m_nWidth, m_nHeight);
	return rc;
}

inline void CBaseWindow::SetWindowState(WindowState state)
{
	m_WindowState = state;
	ShowWindow();

}

inline WindowState CBaseWindow::GetWindowState()
{
	return m_WindowState;

}

inline void CBaseWindow::SetLeft(int left)
{
	m_nLeft = left;
	SetBound(m_nLeft, m_nTop, m_nWidth, m_nHeight);
}
inline int CBaseWindow::GetLeft()
{
	return m_nLeft;
}

inline void CBaseWindow::SetTop(int Top)
{
	m_nTop = Top;
	SetBound(m_nLeft, m_nTop, m_nWidth, m_nHeight);
}

inline int CBaseWindow::GetTop()
{
	return m_nTop;
}

inline void CBaseWindow::SetWidth(int Width)
{
	m_nWidth = Width;
	SetBound(m_nLeft, m_nTop, m_nWidth, m_nHeight);
}

inline INT CBaseWindow::GetWidth()
{
	return m_nWidth;

}

inline void CBaseWindow::SetHeight(int Height)
{
	m_nHeight = Height;
	SetBound(m_nLeft, m_nTop, m_nWidth, m_nHeight);
}

inline SIZE CBaseWindow::GetClientSize()
{
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	SIZE sz;
	sz.cx = rc.right;
	sz.cy = rc.bottom;
	return sz;
}

inline void CBaseWindow::SetClientSize(SIZE sz)
{
	SIZE s = GetClientSize();
	SetBound(m_nLeft, m_nTop, m_nWidth - s.cx+sz.cx, m_nHeight - s.cy+sz.cy);

}

inline BOOL CBaseWindow::Enabled()
{
	m_bEnabled = ::IsWindowEnabled(m_hwnd);
	return m_bEnabled;
}

inline void CBaseWindow::SetEnabled(BOOL bEnabled)
{
	assert(m_hwnd);
	m_bEnabled = bEnabled;
	::EnableWindow(m_hwnd, bEnabled);

}

inline BOOL CBaseWindow::TopMost()
{
	LONG Style = ::GetWindowLong(m_hwnd, GWL_EXSTYLE);
	return (Style & WS_EX_TOPMOST) != 0;
}

inline void CBaseWindow::SetTopMost(BOOL bTopMost)
{
	::SetWindowPos(m_hwnd, bTopMost ? HWND_TOPMOST : HWND_NOTOPMOST,
		0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

inline BOOL CBaseWindow::Visabled()
{
	assert(m_hwnd);
	m_bVisible = ::IsWindowVisible(m_hwnd);
	return m_bVisible;

}

inline void CBaseWindow::SetVisable(BOOL bVisable)
{
	m_bVisible = bVisable;
	if (m_bVisible)
	{
		if (::IsIconic(m_hwnd))
		{
			::ShowWindow(m_hwnd, SW_SHOWMINNOACTIVE);
		}else
		if (::IsIconic(m_hwnd))
			::ShowWindow(m_hwnd, SW_SHOWMAXIMIZED);
		else
			::ShowWindow(m_hwnd, SW_SHOWNORMAL);
		OnShow();
	}
	else
	{
		OnHide();
		::ShowWindow(m_hwnd, SW_HIDE);
	}

}

inline BorderIcons CBaseWindow::GetBorderIcons()
{
	assert(m_hwnd);
	return WindowStyleToBorderStyle((LONG)::GetWindowLong(m_hwnd, GWL_STYLE));
}

inline void CBaseWindow::SetBorderIcons(BorderIcons bi)
{
	assert(m_hwnd);
	::SetWindowLong(m_hwnd, GWL_STYLE, BorderIconsToWindowStyle(::GetWindowLong(m_hwnd, GWL_STYLE), bi));

	::SetWindowPos(m_hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

}

inline BorderStyle CBaseWindow::GetBorderStyle()
{
	assert(m_hwnd);
	return WindowStyleToBorderStyle((LONG)::GetWindowLong(m_hwnd, GWL_STYLE));
}

inline void CBaseWindow::SetBorderStyle(BorderStyle bs)
{
	assert(m_hwnd);
	::SetWindowLong(m_hwnd, GWL_STYLE, BorderStyleToWindowStyle(::GetWindowLong(m_hwnd, GWL_STYLE), bs));
	::SetWindowPos(m_hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

inline void CBaseWindow::SetInstance(HINSTANCE instance)
{
	m_Instance = instance;
}

inline void CBaseWindow::SetIcon(int nIcon)
{
	LONG lRet = ::GetWindowLong(m_hwnd, GWL_HINSTANCE);
	HICON icon = LoadIcon((HINSTANCE)lRet, MAKEINTRESOURCE(nIcon));
	::SendMessage(m_hwnd, WM_SETICON, ICON_SMALL, LPARAM(nIcon));
	::SendMessage(m_hwnd, WM_SETICON, ICON_BIG, LPARAM(nIcon));
}

inline BOOL CBaseWindow::RegisterWndClass()
{
	
	WNDCLASSEX wcex;
	BOOL bReg = ::GetClassInfoEx(m_Instance, WINDOW_CLASSNAME, &wcex);
	if (bReg)
	{
		UnregisterClass(WINDOW_CLASSNAME, m_Instance);
		bReg = false;
	}
	if (!bReg)
	{
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_Instance;
		wcex.hIcon = 0;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = WINDOW_CLASSNAME;
		wcex.hIconSm = 0;
		return RegisterClassEx(&wcex);
	}	
}

inline DWORD CBaseWindow::BorderIconsToWindowStyle(DWORD style, BorderIcons bi)
{
	DWORD oldstyle = style;
	oldstyle &= ~(WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
	if ((oldstyle & WS_SYSMENU) == 0)
	{
		oldstyle |= WS_SYSMENU;
	}

	if ((oldstyle & WS_SYSMENU) != 0)
	{
		if ((bi & biMaxmize) != 0)
			oldstyle |= WS_MAXIMIZEBOX;
		if ((bi & biMinimize) != 0)
			oldstyle |= WS_MINIMIZEBOX;
	}
	return oldstyle;
}

inline BorderStyle CBaseWindow::WindowStyleToBorderStyle(DWORD style)
{
	BorderStyle bs = bs_None;
	if ((style |= WS_CAPTION) != 0)
	{
		if ((style |= WS_THICKFRAME) != 0)
			bs = bs_None;
		else
			bs = bs_Single;
	}
	return bs;
}

inline DWORD CBaseWindow::BorderStyleToWindowStyle(DWORD style, BorderStyle bs)
{
	DWORD oldstyle = style;
	style &= ~(WS_THICKFRAME | WS_BORDER | WS_CAPTION | WS_POPUP);
	if (bs == bs_None)
		oldstyle |= WS_POPUP;
	if (bs == bs_Single)
		oldstyle |= WS_CAPTION | WS_BORDER;
	if (bs == bs_Sizeable)
		oldstyle |= WS_CAPTION | WS_THICKFRAME;
	return oldstyle;
}

inline BorderIcons CBaseWindow::WindowStyleToBorderIcons(DWORD style)
{
	BorderIcons bi = 0;
	if ((style & WS_SYSMENU) != 0)
	{
		bi |= biSystemMenu;
		if ((style & WS_MINIMIZEBOX) != 0)
			bi = biMinimize;
		if ((style & WS_MAXIMIZEBOX) != 0)
			bi = biMaxmize;
	}
	return bi;
}

inline void CBaseWindow::UpdateBounds()
{
	assert(m_hwnd);
	RECT rc;
	if (::IsIconic(m_hwnd))
	{
		WINDOWPLACEMENT wp;
		wp.length = sizeof(wp);
		::GetWindowPlacement(m_hwnd, &wp);
		if ((wp.flags != WPF_RESTORETOMAXIMIZED) != 0)
		{
			rc.left = wp.ptMaxPosition.x;
			rc.top = wp.ptMaxPosition.y;
			rc.right = rc.left + m_nWidth;
			rc.bottom = rc.top + m_nHeight;
		}
		else
			rc = wp.rcNormalPosition;
	}
	else
		::GetWindowRect(m_hwnd, &rc);

	m_nLeft = rc.left;
	m_nTop = rc.top;

	INT width = rc.right - rc.left;
	INT height = rc.bottom - rc.top;

	if (width != m_nWidth || height != m_nHeight)
	{
		m_nWidth = width;
		m_nHeight = height;
		OnSizeChange(0, 0, 0);

	}

}

inline void CBaseWindow::OnPaint(HDC dc)
{
	if (m_Listener)
		m_Listener->OnPaint(this,dc);
	
}

inline void CBaseWindow::SetWindowListener(BaseWindowListener* listener)
{
	m_Listener = listener;
}

inline void CBaseWindow::OnShow()
{
	if (m_Listener)
		m_Listener->OnShow(this);

}

inline void CBaseWindow::OnHide()
{
	if (m_Listener)
		m_Listener->OnHide(this);
}

inline void CBaseWindow::OnClose()
{
	if (m_Listener)
		m_Listener->OnClose(this);
}

inline  void CBaseWindow::OnCreate()
{
	if (m_Listener)
		m_Listener->OnCreate(this);
}

inline void CBaseWindow::OnDestroy()
{
	if (m_Listener)
		m_Listener->OnDestroy(this);
}

inline void CBaseWindow::OnSizeChange(UINT nType, INT cx, INT cy)
{
	if (m_Listener && WS_Minimized != GetWindowState())
		m_Listener->OnSizeChange(this, nType, cx, cy);
}

void CBaseWindow::OnPosChange()
{
	if (m_Listener)
		m_Listener->OnPosChange(this);
}

inline HRESULT CALLBACK CBaseWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_CREATE)
	{
		assert(lparam);
		LPCREATESTRUCTW pcs = (LPCREATESTRUCTW)lparam;

		assert(pcs->lpCreateParams);
		CBaseWindow* Window = (CBaseWindow*)pcs->lpCreateParams;
		Window->m_hwnd = hwnd;
		//assert(::GetPropW(hWnd, WINDOWS_ATOM) == NULL);
		//::SetWindowLongW(hWnd, GWL_WNDPROC, (LONG)StdWndProc);
		::SetPropW(hwnd, WINDOW_ATOM, (HANDLE)pcs->lpCreateParams);	
		LRESULT ret;
		ret = Window->WindowProc(hwnd, msg, wparam, lparam);
		return ret;
	}

		return ::CallWindowProcW(DefWindowProcW, hwnd, msg, wparam, lparam);

}

inline HRESULT CALLBACK CBaseWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (m_hwnd == NULL)
		return FALSE;
  
	if (m_Listener)
		HRESULT Result = m_Listener->OnWndProc(this, msg, wparam, lparam);

	switch (msg)
	{
	case WM_PAINT:
	{
					 PAINTSTRUCT ps;
					 HDC dc = ::BeginPaint(hwnd, &ps);
					 OnPaint(dc);
					 EndPaint(hwnd, &ps);
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
						 
						  return 1;
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
		m_hwnd = NULL;
		break;
	default:
		break;
	}
	return FALSE;
}



















