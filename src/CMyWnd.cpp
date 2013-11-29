/*074090 Œ‚±¶¡÷*/
#include"CMyWnd.h"
#include"Message.h"
#include"CMyApp.h"
#include<windows.h>

extern CMyApp g_App;

void CMyWnd::PreCreateWindow()
{
	ZeroMemory(&m_wndClassex,sizeof(WNDCLASSEX));

	m_wndClassex.cbSize = sizeof(WNDCLASSEX);
	m_wndClassex.style			= CS_HREDRAW | CS_VREDRAW ;
	m_wndClassex.lpfnWndProc	= CMyApp::WndProc;
	m_wndClassex.cbClsExtra		= 0;
	m_wndClassex.cbWndExtra		= 0;
	m_wndClassex.hInstance		= g_App.GetInst();
	m_wndClassex.hIcon			= NULL;
	m_wndClassex.hCursor		= NULL;
	m_wndClassex.hbrBackground	= (HBRUSH)(GetStockObject(GRAY_BRUSH));
	m_wndClassex.lpszMenuName	= NULL;
	m_wndClassex.lpszClassName	= m_szClassName;
	m_wndClassex.hIconSm		= NULL;
}


void CMyWnd::CreateEx()
{
	ZeroMemory(&m_createStruct,sizeof(CREATESTRUCT) );
	m_createStruct.cx = m_rectWnd.right - m_rectWnd.left;
	m_createStruct.cy = m_rectWnd.bottom - m_rectWnd.top;
	m_createStruct.dwExStyle = 0;
	m_createStruct.hInstance = g_App.GetInst();
	m_createStruct.hMenu = NULL;
	m_createStruct.hwndParent = (m_Parent == NULL)?NULL:m_Parent->GetHWND();
	m_createStruct.lpCreateParams = this;
	m_createStruct.lpszClass = m_szClassName;
	m_createStruct.lpszName = NULL;
	m_createStruct.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN;
	m_createStruct.x = m_rectWnd.left;
	m_createStruct.y = m_rectWnd.top;
//	WS
	
}


void CMyWnd::Create()
{
	//Fill in the WNDCLASSEX struct
	PreCreateWindow();
	
	//Resgister Class if you want
	if(m_bNewWnd)
	{
		RegisterClassEx(&m_wndClassex);
		
	}
	//fill in Create Window struct
	CreateEx();

	m_hWnd = ::CreateWindowEx(
		m_createStruct.dwExStyle,
		m_createStruct.lpszClass,
		m_createStruct.lpszName,
		m_createStruct.style,
		m_createStruct.x,
		m_createStruct.y,
		m_createStruct.cx,
		m_createStruct.cy,
		m_createStruct.hwndParent,
		m_createStruct.hMenu,
		m_createStruct.hInstance,
		m_createStruct.lpCreateParams);  // Last as the pointer to create the window
	::UpdateWindow(m_hWnd);

	//Register the message 
	CMyApp::AddMessage(m_hWnd,this);
};


LRESULT CMyWnd::WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	return ::DefWindowProc(hWnd,message,wParam,lParam);
}
