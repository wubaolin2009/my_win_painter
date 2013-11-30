/*074090 Œ‚±¶¡÷*/
#include"CMyColorBar.h"
#include"CMyApp.h"
#include"CMyWnd.h"
#include "resource.h"
#include "CMyFrameWnd.h"

#include <Commdlg.h>

DWORD CMyColorBar::ms_ID = 0xC0000000;
extern CMyApp g_App;


COLORREF CMyColorBar::m_colors[COLOR_NUM_PALLET] = 
{
	RGB(0,0,0),
	RGB(128,128,128),
	RGB(128,0,0),
	RGB(128,128,0),
	RGB(0,128,0),
	RGB(0,128,128),
	RGB(0,0,128),
	RGB(128,0,128),
	RGB(128,128,0),
	RGB(0,0,0),
	RGB(0,128,128),
	RGB(0,0,128),
	RGB(128,0,128),
	RGB(128,0,0),
	RGB(255,255,255),
	RGB(192,192,192),
	RGB(255,0,0),
	RGB(255,255,0),
	RGB(0,255,0),
	RGB(0,255,255),
	RGB(0,0,255),
	RGB(255,0,255),
	RGB(192,192,192),
	RGB(0,128,128),
	RGB(192,192,192),
	RGB(192,192,192),
	RGB(128,0,128),
	RGB(128,128,0)
};

bool CMyColorBar::m_ColorChanged[COLOR_NUM_PALLET] = {0};


void CMyColorBar::PreCreateWindow()
{
	CMyWnd::PreCreateWindow();
	m_wndClassex.style			= CS_DBLCLKS;
	m_wndClassex.hbrBackground = (HBRUSH)::CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
}

void CMyColorBar::CreateEx()
{
	CMyWnd::CreateEx();
	m_createStruct.style = WS_CHILD | WS_VISIBLE |WS_CLIPSIBLINGS;
	m_createStruct.hMenu = (HMENU) ms_ID++;
}

LRESULT CMyColorBar::OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) //overide
{
	HDC	hdc = GetDC(hWnd);
	m_hbit = LoadBitmap(g_App.GetInst(),MAKEINTRESOURCE(IDB_COLOR));
	m_hdcMem = CreateCompatibleDC(hdc);
	SelectObject(m_hdcMem,m_hbit);
	ReleaseDC(hWnd,hdc);
	return 0;
	
}

LRESULT CMyColorBar::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BEGIN_MY_MESSAGE
		MY_MESSAGE(WM_PAINT,OnPaint)
		MY_MESSAGE(WM_LBUTTONDOWN,OnLButtonDown)
		MY_MESSAGE(WM_RBUTTONDOWN,OnRButtonDown)
		MY_MESSAGE(WM_LBUTTONDBLCLK,OnLButtonDBClick)
		MY_MESSAGE(WM_RBUTTONDBLCLK,OnRButtonDBClick)
	END_MY_MESSAGE
}

LRESULT CMyColorBar::OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = GetDC(hWnd);
	
	for(int i = 0;i < COLOR_NUM_PALLET; i++)
	{
		if(m_ColorChanged[i])
		{
			HBRUSH hbtmp = CreateSolidBrush( m_colors[i] );
			RECT rectToDraw;
			rectToDraw.left = 32+ (i %14) * 16 ;
			rectToDraw.right = rectToDraw.left + 13 ;
			rectToDraw.top = (i>=14)?24:8;
			rectToDraw.bottom = rectToDraw.top + 13;
			FillRect(m_hdcMem,&rectToDraw,hbtmp);
			DeleteObject(hbtmp);
		}
	}
	//DrawCurrent Front color and bk color
	HPEN hp = (HPEN)GetStockObject(NULL_PEN);
	HBRUSH hbf = CreateSolidBrush(m_ftColor);
	HBRUSH hbb = CreateSolidBrush(m_bkColor);
	HPEN hptmp;
	HBRUSH hbtmp;
	hptmp = (HPEN)SelectObject(m_hdcMem,hp);
	hbtmp = (HBRUSH)SelectObject(m_hdcMem,hbf);
	Rectangle(m_hdcMem,6  ,14,18 ,26);

	SelectObject(m_hdcMem,hbb);
	Rectangle(m_hdcMem,19,21,25,28);
	Rectangle(m_hdcMem,13,27,25,33);

	SelectObject(m_hdcMem,hptmp);
	SelectObject(m_hdcMem,hbtmp);
	DeleteObject(hp);
	DeleteObject(hbf);
	DeleteObject(hbb);
	DeleteObject(hptmp);
	DeleteObject(hbtmp);



	BitBlt(hdc,0+10,0,254+10,48,m_hdcMem,0,0,SRCCOPY);
	ReleaseDC(hWnd,hdc);
	return 0;
}

LRESULT CMyColorBar::OnLButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int	y = HIWORD(lParam);
	x-=10;
	int colorFill = -1;
	HBRUSH hbtemp;
	HPEN   hptemp;
	RECT rectcolor;  //Judge which rect mouse belongs
	for(int i = 0;i<=27;i++)
	{
		rectcolor.left = 33+ (i %14) * 16 ;
		rectcolor.right = rectcolor.left + 14 ;
		rectcolor.top = (i>=14)?22:8;
		rectcolor.bottom = rectcolor.top + 14;
		if( x >=rectcolor.left && x<=rectcolor.right && y>=rectcolor.top && y<=rectcolor.bottom)
		{
			colorFill = i;
			m_ftColor = m_colors[i];
			break;
		}
	}
	if(colorFill != -1)
	{		
		HDC hdc = GetDC(hWnd);
		hbtemp = (HBRUSH)SelectObject(m_hdcMem,CreateSolidBrush(m_colors[colorFill]) );
		hptemp = (HPEN) SelectObject(m_hdcMem,GetStockObject(NULL_PEN) );
		Rectangle(m_hdcMem,6  ,14,18 ,26);
		DeleteObject( SelectObject(m_hdcMem,hbtemp) );
		DeleteObject( SelectObject(m_hdcMem,hptemp) );
		((CMyFrameWnd*)GetParent() )->OnFtColorChange(m_colors[colorFill]);
		BitBlt(hdc,0+10,0,254 +10,48,m_hdcMem,0,0,SRCCOPY);
		ReleaseDC(hWnd,hdc);
	}
	return 0;
}


LRESULT CMyColorBar::OnRButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int	y = HIWORD(lParam);
	x-=10;
	//y-=5;
	int colorFill = -1;
	HBRUSH hbtemp;
	HPEN   hptemp;
	RECT rectcolor;  //Judge which rect mouse belongs
	for(int i = 0;i<=27;i++)
	{
		rectcolor.left = 33+ (i %14) * 16 ;
		rectcolor.right = rectcolor.left + 14 ;
		rectcolor.top = (i>=14)?22:8;
		rectcolor.bottom = rectcolor.top + 14;
		if( x >=rectcolor.left && x<=rectcolor.right && y>=rectcolor.top && y<=rectcolor.bottom)
		{
			colorFill = i;
			m_bkColor = m_colors[i];
			break;
		}
	}
	if(colorFill != -1)
	{		
		HDC hdc = GetDC(hWnd);
		hbtemp = (HBRUSH)SelectObject(m_hdcMem,CreateSolidBrush(m_colors[colorFill]) );
		hptemp = (HPEN) SelectObject(m_hdcMem,GetStockObject(NULL_PEN) );
		Rectangle(m_hdcMem,19,21,25,28);
		Rectangle(m_hdcMem,13,27,25,33);
		DeleteObject( SelectObject(m_hdcMem,hbtemp) );
		DeleteObject( SelectObject(m_hdcMem,hptemp) );
		BitBlt(hdc,0+10,0,254+10,48,m_hdcMem,0,0,SRCCOPY);
		((CMyFrameWnd*)GetParent() )->OnBkColorChange(m_colors[colorFill]);
		ReleaseDC(hWnd,hdc);
	}
	return 0;
}

LRESULT CMyColorBar::OnLButtonDBClick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int	y = HIWORD(lParam);
	x-=10;
	//y-=5;
	int colorFill = -1;
	RECT rectcolor;  //Judge which rect mouse belongs
	for(int i = 0;i<=27;i++)
	{
		rectcolor.left = 33+ (i %14) * 16 ;
		rectcolor.right = rectcolor.left + 14 ;
		rectcolor.top = (i>=14)?22:8;
		rectcolor.bottom = rectcolor.top + 14;
		if( x >=rectcolor.left && x<=rectcolor.right && y>=rectcolor.top && y<=rectcolor.bottom)
		{
			colorFill = i;
			break;
		}
	}

	if(colorFill != -1)
	{
		DWORD rgbCurrent = m_colors[colorFill];
		COLORREF acrCustClr[16] = {0}; // array of custom colors 
		ZeroMemory(acrCustClr,sizeof(COLORREF)*16);
		CHOOSECOLOR cc;
		ZeroMemory(&cc,sizeof(cc) );
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = GetParent()->GetHWND();
		cc.lpCustColors = (LPDWORD) acrCustClr;
		cc.rgbResult = rgbCurrent;
		cc.Flags =   CC_RGBINIT | CC_FULLOPEN | CC_ENABLEHOOK;
		cc.lpfnHook = CMyColorBar::CCHookProc;
		::ChooseColor(&cc);
		if(cc.rgbResult != m_colors[colorFill]) //color Changed
		{
			m_ColorChanged[colorFill] = true;
			m_colors[colorFill]  = cc.rgbResult;
			m_ftColor = cc.rgbResult;
			((CMyFrameWnd*)GetParent() )->OnFtColorChange(m_ftColor);
			InvalidateRect(hWnd,NULL,TRUE);
		}
	}
	return 0;
}


LRESULT CMyColorBar::OnRButtonDBClick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int	y = HIWORD(lParam);
	x-=10;
	//y-=5;
	int colorFill = -1;
	RECT rectcolor;  //Judge which rect mouse belongs
	for(int i = 0;i<=27;i++)
	{
		rectcolor.left = 33+ (i %14) * 16 ;
		rectcolor.right = rectcolor.left + 14 ;
		rectcolor.top = (i>=14)?22:8;
		rectcolor.bottom = rectcolor.top + 14;
		if( x >=rectcolor.left && x<=rectcolor.right && y>=rectcolor.top && y<=rectcolor.bottom)
		{
			colorFill = i;
			break;
		}
	}

	if(colorFill != -1)
	{
		DWORD rgbCurrent = m_colors[colorFill];
		COLORREF acrCustClr[16] = {0}; // array of custom colors 
		ZeroMemory(acrCustClr,sizeof(COLORREF)*16);
		CHOOSECOLOR cc;
		ZeroMemory(&cc,sizeof(cc) );
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = GetParent()->GetHWND();
		cc.lpCustColors = (LPDWORD) acrCustClr;
		cc.rgbResult = rgbCurrent;
		cc.Flags =   CC_RGBINIT | CC_FULLOPEN | CC_ENABLEHOOK;
		cc.lpfnHook = CMyColorBar::CCHookProc;
		::ChooseColor(&cc);
		if(cc.rgbResult != m_colors[colorFill]) //color Changed
		{
			m_ColorChanged[colorFill] = true;
			m_colors[colorFill]  = cc.rgbResult;
			m_bkColor = cc.rgbResult;
			((CMyFrameWnd*)GetParent() )->OnBkColorChange(m_bkColor);
			InvalidateRect(hWnd,NULL,TRUE);
		}
	}
	return 0;
}

UINT_PTR CALLBACK CMyColorBar::CCHookProc(HWND hdlg,    UINT uiMsg,    WPARAM wParam,    LPARAM lParam)
{
	if (uiMsg == WM_INITDIALOG)
	{
		::ShowWindow(hdlg, SW_SHOW);
	}
	return 0;
}

