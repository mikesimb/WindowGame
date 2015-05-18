#pragma  once
#include <windows.h>
#include <mmsystem.h>


class CWinMsgControler;
struct CWinMsgControlerListener
{
	virtual BOOL  OnProcessMsg(CWinMsgControler sender, MSG &msg) = 0;
	virtual BOOL   OnProcessIdel(CWinMsgControler sender) = 0;
};
class CWinMsgControler
{
public:
	CWinMsgControler() :m_bRunning(FALSE), m_listener(NULL)
	{
	};
	virtual~CWinMsgControler()
	{
	};
	virtual void Run();
	virtual void Quit();
	virtual BOOL IsRunning();
	void setMsgControlerListener(CWinMsgControlerListener * listener);
	CWinMsgControlerListener* getMsgControlerListener();

protected:
	virtual BOOL ProcessMessage(MSG& msg){ return FALSE; }
	virtual void ProcessIdel(){};

private:
	CWinMsgControlerListener * m_listener;
	BOOL  m_bRunning;


private:

};

inline void CWinMsgControler::Run()
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
				m_bRunning = false;
			}
		}
		else
		{
			ProcessIdel();
		}

	} while (m_bRunning);
	timeEndPeriod(1);
}

inline void CWinMsgControler::Quit()
{
	m_bRunning = FALSE;

}

inline BOOL CWinMsgControler::IsRunning()
{
	return m_bRunning;
}

