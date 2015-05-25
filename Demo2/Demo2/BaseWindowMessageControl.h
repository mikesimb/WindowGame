#pragma once
#include <windows.h>
#include<Mmsystem.h>

#pragma comment(lib,"Winmm.lib")

class CBaseWindowMsgControl;
class CBaseWindowMsgControlListener
{
public:
	virtual BOOL OnProcessMsg(CBaseWindowMsgControl* Sender, MSG & Msg) { return TRUE; };
	virtual BOOL OnProcessIdel(CBaseWindowMsgControl* Sender) { return TRUE; };
};

class CBaseWindowMsgControl
{
public:

	CBaseWindowMsgControl() :m_bRunning(FALSE), m_listener(NULL)
	{
	}

	virtual ~CBaseWindowMsgControl()
	{
	}
	virtual void Run();
	virtual void Quit();
	virtual BOOL IsRunning();
	void SetMsgControlListener(CBaseWindowMsgControlListener * listener);
	CBaseWindowMsgControlListener * GetMsgControlListener();

protected:
	virtual BOOL ProcessMessage(MSG & msg){
		if (m_listener)  return m_listener->OnProcessMsg(this, msg); else return FALSE;
	}
	virtual void ProcessIdel(){ if (m_listener) m_listener->OnProcessIdel(this); }

private: 
	BOOL m_bRunning;
	CBaseWindowMsgControlListener* m_listener;

};

inline void CBaseWindowMsgControl::Run()
{
	m_bRunning = TRUE;
	timeBeginPeriod(1);
	do 
	{
		MSG msg;
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message != WM_QUIT)
			{
				if (!ProcessMessage(msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{ 
				m_bRunning = FALSE;
			}
		}
		else
		{
			ProcessIdel();
		}
	} while (m_bRunning);
	timeEndPeriod(1);
}

inline void CBaseWindowMsgControl::Quit()
{
	m_bRunning = FALSE;
}

inline BOOL CBaseWindowMsgControl::IsRunning()
{
	return m_bRunning;
}

inline void CBaseWindowMsgControl::SetMsgControlListener(CBaseWindowMsgControlListener* listener)
{
	m_listener = listener;
}

inline CBaseWindowMsgControlListener* CBaseWindowMsgControl::GetMsgControlListener()
{
	return m_listener;
}

