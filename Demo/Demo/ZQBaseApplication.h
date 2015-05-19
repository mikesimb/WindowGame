// ------------------------------------------------------------------------- 
//	�ļ���	:  ZQBaseApplication.h
//	��������: ��Ҫ���صĹ��ܣ�
// 1�� ���������Ŀͻ��˴���
// 2�����ؼ��̺������¼�
// 3��Ӧ����Ҫ�õ��Ļ�����Ϣ
//
// -------------------------------------------------------------------------

#pragma once

#ifndef  __ZQBASEAPPLICATION__
#define  __ZQBASEAPPLICATION__

#include <windows.h>
#include "WinMsgControler.h"
#include "ZQWinBase.h"

class CZQBaseApplication :public CWinMsgControler, public WNDFrameListener
{
public:
	CZQBaseApplication()
	{
	};
	virtual ~CZQBaseApplication();

	virtual BOOL Init();
	virtual void Finalize();


	void SetFPS(DWORD dwFPS);
	DWORD getFPS();
	CZQWinBase * getMainWindow();

	SIZE getDisplaySize()const;
	void getClientMousePos(POINT & pt);
	virtual DWORD GetCurTime()const { return 0; }

	void setEraseBK(BOOL Value){ m_bEraseBK = Value; }
	BOOL getEraseBK(){ return m_bEraseBK; }

public:  //WNDFrameListener
	virtual void OnSizeChange(CZQWinBase * Win, UINT nType, int cx, int cy);
	virtual void OnDestroy(CZQWinBase * Win);
	virtual void OnSetCursour();
	virtual BOOL OnWndProc(CZQWinBase * win, UINT msg, WPARAM wparam, LPARAM lparam, HRESULT& ret);

	//mouse
	//virtual void OnMouseWheel(ShiftStates shift, const POINT & pt, SHORT nRow);
	//virtual void OnMouseMove(ShiftStates shift, const POINT & pt);



private:
	CZQWinBase  m_MainWin;
	DWORD   m_dwLastTick;                             //��һ����Ⱦ��ʱ��
	DWORD   m_dwFrameDelayTime;                //֡���
	DWORD   m_dwFPS;                                   //ÿ����Ⱦ��֡��
	DWORD   m_dwLastSecTick;                        ///��һ�ε�Tick��
	DWORD   m_dwFPSCount;                           // FPS������
	BOOL      m_bRunAlways;                            //�������У����ܴ����Ƿ��ڼ���״̬��


	int      m_nClientWidth;
	int     m_nClientHeight;
	float   m_nMouseSX;
	float   m_nMouseSY;
	UINT   m_uPeriod;

	BYTE  m_bCurrRawInput;


private:
	BOOL m_bEraseBK;

};

inline CZQBaseApplication::CZQBaseApplication() 
:m_dwLastTick(0),
m_dwFrameDelayTime(0),
m_dwFPS(0), 
m_dwLastSecTick(0),
m_dwFPSCount(0), 
m_bRunAlways(FALSE),
m_bEraseBK(TRUE)
{

}

inline CZQBaseApplication::~CZQBaseApplication()
{

}

inline BOOL CZQBaseApplication::Init()
{
	m_MainWin.setListener(this);
	m_dwLastTick = timeGetTime();
	m_dwLastSecTick = m_dwLastTick;

	m_uPeriod = 1;
	m_

}



#endif