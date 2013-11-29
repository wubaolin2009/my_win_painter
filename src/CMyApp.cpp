/*074090 Œ‚±¶¡÷*/
#include"CMyapp.h"
#include "CMyFrameWnd.h"
#include<windows.h>
#include <commctrl.h>

CMyList<MessageMap> CMyApp::ms_MessageMap;


LRESULT CALLBACK CMyApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPCREATESTRUCT lpCreate = (LPCREATESTRUCT)lParam;
	if( message == WM_CREATE)
	{
		return ( (CMyWnd*) (lpCreate->lpCreateParams) )->OnCreate(hWnd,message,wParam,lParam);
	}

	for(int i = 0;i<ms_MessageMap.size();i++)
	{
		if( ms_MessageMap[i].hwnd == hWnd)
		{
			return ms_MessageMap[i].pWnd->WndProc(hWnd,message,wParam,lParam);
		}
	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}


CMyApp::CMyApp()
{
	m_hInst = NULL;
	m_myWnd = NULL;
}

//This intend to prepare something before you run the application
void CMyApp::InitApplication(HINSTANCE hInst,CMyWnd* pwndMain)
{
	m_hInst = hInst;
	m_myWnd = pwndMain;
	INITCOMMONCONTROLSEX ss = {sizeof(ss),ICC_BAR_CLASSES};
	InitCommonControlsEx(&ss);
	//m_myWnd->SetApp(this);
}

//This could be override in you application to Specify what you application will looks like
//Its defualt behavior is call m_myWnd.Create()
void CMyApp::InitInstance()
{
	if(m_myWnd)
		m_myWnd->Create();
}
	
//This is used to transport message handling the last step for your application when it's returns your application is over
LRESULT  CMyApp::Run()
{
	MSG msg;
	while (true)
	{
		if(::PeekMessage(&msg, NULL, 0, 0,PM_REMOVE) )
		{
			if(msg.message == WM_QUIT)
				break;	
			if(msg.message == WM_TIMER)
			{

			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);		
		}
		else
			OnIdle();
	}
	return msg.lParam;
}

//Will be called by CMyWnd::Create after the window is created 
void CMyApp::AddMessage(HWND hWnd,CMyWnd* pWnd)
{
	MessageMap messageMap = {hWnd,pWnd};
	ms_MessageMap.Add(messageMap);
}

void CMyApp::OnIdle(){ 
		static int time = 0;
		time++;
		if(time == 100)
		{
			time = 0;
			((CMyFrameWnd*)m_myWnd)->OnTimer();
		}
	}
	