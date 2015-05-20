#pragma once
#include <windows.h>

class CBaseWindowMsgControlListener
{
	virtual BOOL OnProcessMsg(CBaseWindowMsgControl Sender, MSG & Msg) = 0;
	virtual BOOL OnProcessIdel(CBaseWindowMsgControl Sender) = 0;
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
	virtual BOOL ProcessMessage(MSG & msg){ return FALSE; }
	virtual void ProcessIdel(){}

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

