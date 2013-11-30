/*074090 Œ‚±¶¡÷*/
#include "CMyFliter.h"
#include "CMyToolBar.h"
#include "CMyFrameWnd.h"
#include <windows.h>

extern CMyApp g_App;

HDC	CMyFliter::m_dcMem;        //Save mem dc
HDC	CMyFliter::	m_dcBitMap;		//Save the former graph
HBITMAP	CMyFliter::m_bitMap;
RECT CMyFliter::m_rectTrue = {0,0,640,480};

/* Message about current select tool */
CMyToolBarWithList::ToolStyle	CMyFliter::m_tool;
COLORREF CMyFliter::m_FtColor   = RGB(0,0,0);
COLORREF CMyFliter::m_BkColor   = RGB(255,255,255);
int    CMyFliter:: m_XMin = 0;
int      CMyFliter::m_XMax =0;
int      CMyFliter::m_YMin = 0;
int      CMyFliter::m_YMax = 0;


int CMyFliter::m_zoom = 1;
POINT CMyFliter::m_Start = {0,0};



int	CMyFliter::m_SelectItem;

HPEN CMyFliter::m_Pen  = ::CreatePen(PS_SOLID,1,RGB(0,0,0) );  //Pencil default style
HBRUSH CMyFliter::m_Brush = ::CreateSolidBrush( RGB(255,255,255) ); //Pencil default style


HPEN CMyFliter::m_Pen2  = ::CreatePen(PS_SOLID,1,RGB(255,255,255) );  //Pencil default style
HBRUSH CMyFliter::m_Brush2 = ::CreateSolidBrush( RGB(0,0,0) ); //Pencil default style

void CMyFliter::Invert()
{
	RECT rectNow = this->GetRectSel();
	SetRectSel(rectNow,false);
	::InvertRect(CMyFliter::m_dcMem,&rectNow);

	HDC hdc = GetDC(m_pView->GetHWND() );  //pen is alright when it changes its index of each iTem
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	//OnFinish();
	ReleaseDC(m_pView->GetHWND() ,hdc);
	SetRectSel(rectNow,false);


}




void CMyFliter::Clear()
{
	RECT rectNow = this->GetRectSel();
	SetRectSel(rectNow,false);
	FillRect(CMyFliter::m_dcMem,&rectNow,m_Brush);

	HDC hdc = GetDC(m_pView->GetHWND() );  //pen is alright when it changes its index of each iTem
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	OnFinish();
	ReleaseDC(m_pView->GetHWND() ,hdc);
	//SetRectSel(rectNow,false);


}


void CMyFliter:: Flip(int index,const RECT& rect)
{
	/*
	RECT rectNow = {rect.left,rect.top,rect.right,rect.bottom};
	HDC hdcTemp = CreateCompatibleDC(m_dcMem);
	HBITMAP hb = CreateCompatibleBitmap(m_dcMem,rect.right - rect.left ,rect.bottom - rect.top);
	SelectObject(hdcTemp,hb);
	::BitBlt(hdcTemp,0,0,rect.right - rect.left ,rect.bottom - rect.top,m_dcMem,rect.left,rect.top,SRCCOPY);
	::FillRect(m_dcMem,&rectNow,(HBRUSH)GetStockObject(WHITE_BRUSH) );

	HDC hdc = GetDC(m_pView->GetHWND() );
	switch(index)
	{
	case 1:
	case 5:
	::StretchBlt(m_dcMem,rect.right,rect.top,rect.left - rect.right ,rect.bottom - rect.top , hdcTemp,0,0 ,rect.right - rect.left,rect.bottom - rect.top,SRCCOPY);
	break;
	case 2:
	::StretchBlt(m_dcMem,rect.left,rect.bottom,rect.right - rect.left ,rect.top - rect.bottom , hdcTemp,0,0 ,rect.right - rect.left,rect.bottom - rect.top,SRCCOPY);
	break;
	case 4:  //90
	//::StretchBlt(m_dcMem,rect.right,0,rect.left - rect.right,hdcTemp,
	//m_XMax = rect.bottom ;
	//m_YMax = rect.right ;
	break;
	case 6:
	break;
	}

	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	SelectObject(hdc,CreatePen(PS_DOT,1,RGB(0,0,0)) );
	SelectObject(hdc,GetStockObject(NULL_BRUSH) );
	Rectangle(hdc,(m_XMin - CMyFliter::m_Start.x) ,\
	(m_YMin - CMyFliter::m_Start.y) ,\
	(m_XMax - CMyFliter::m_Start.x) ,\
	(m_YMax - CMyFliter::m_Start.y) );
	SetRectSel(rect);
	ReleaseDC(m_pView->GetHWND(),hdc);*/
}


void CMyFliterWord::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = {LOWORD(lParam),HIWORD(lParam) };
	if(m_status == MOVING)
	{
		m_rectWnd.right = pointNow.x;
		m_rectWnd.bottom = pointNow.y;
		m_hEdit = ::CreateWindowEx(NULL,TEXT("EDIT"),NULL,WS_BORDER | WS_VISIBLE |WS_CHILD,m_rectWnd.left,m_rectWnd.top,m_rectWnd.right - m_rectWnd.left,\
			m_rectWnd.bottom - m_rectWnd.top,m_pView->GetHWND(),(HMENU)0x00ff,g_App.GetInst(),NULL);
		m_status = WRITING;
	}

}
void CMyFliterWord::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = {LOWORD(lParam),HIWORD(lParam) };

	if(m_status == NONE)
	{
		m_rectWnd.left = pointNow.x;
		m_rectWnd.top = pointNow.y;
		m_status = MOVING;
	}
	else if(m_status == WRITING)
	{
		if( !PtInRect(&m_rectWnd,pointNow) )
		{
			//Finish
			HDC hdc = GetDC(m_hEdit);
			::SetFocus(m_pView->GetHWND());
			::BitBlt(CMyFliter::m_dcMem,m_rectWnd.left,m_rectWnd.top,m_rectWnd.right ,m_rectWnd.bottom ,hdc,0,0,SRCCOPY);
			
			HBRUSH hb = (HBRUSH)SelectObject(m_dcMem,GetStockObject(NULL_BRUSH) );
			HPEN    hp = (HPEN)SelectObject(m_dcMem,GetStockObject(WHITE_PEN) );
			Rectangle(m_dcMem,m_rectWnd.left,m_rectWnd.top,m_rectWnd.right,m_rectWnd.bottom);

			SelectObject(m_dcMem,hb);
			SelectObject(m_dcMem,hp);

			OnFinish();
			ReleaseDC(m_hEdit,hdc);
			DestroyWindow(m_hEdit);

			hdc = GetDC(m_pView->GetHWND() );
			SetMapMode(hdc,MM_ANISOTROPIC );
			SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
			::BitBlt(hdc,0,0,640,480 ,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);


			ReleaseDC(m_pView->GetHWND(),hdc);

			m_status = NONE;
			m_pDoc->OnFinish(CMyFliter::m_dcMem,m_rectTrue);
			m_dcBitMap = m_pDoc->GetLastDC();
		}
	}

}
void CMyFliterWord::OnMouseMove(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = {LOWORD(lParam),HIWORD(lParam) };

	
	if(wParam & MK_LBUTTON)
	{
		if(m_status == MOVING)
		{
			m_rectWnd.right = pointNow.x;
			m_rectWnd.bottom = pointNow.y;
			HDC hdc = GetDC(m_pView->GetHWND() );
			SetMapMode(hdc,MM_ANISOTROPIC );
			SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

			::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
			SelectObject(CMyFliter::m_dcMem,CreatePen(PS_DOT,1,RGB(0,0,0) ) );
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_BRUSH) );
			Rectangle(CMyFliter::m_dcMem,m_rectWnd.left,m_rectWnd.top,m_rectWnd.right,m_rectWnd.bottom);
			RECT rect ;
			GetClientRect(m_pView->GetHWND(),&rect);
			::BitBlt(hdc,0,0,rect.right - rect.left,rect.bottom - rect.top,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
			ReleaseDC(m_pView->GetHWND() ,hdc);
		}
	}


}


void CMyFliterCut0::Flip(int index,const RECT& rect)
{
	RECT rectNow = {rect.left,rect.top,rect.right,rect.bottom};
	HDC hdcTemp = CreateCompatibleDC(m_dcMem);
	HBITMAP hb = CreateCompatibleBitmap(m_dcMem,rect.right - rect.left ,rect.bottom - rect.top);
	SelectObject(hdcTemp,hb);
	::BitBlt(hdcTemp,0,0,rect.right - rect.left ,rect.bottom - rect.top,m_dcMem,rect.left,rect.top,SRCCOPY);
	::FillRect(m_dcMem,&rectNow,(HBRUSH)GetStockObject(WHITE_BRUSH) );

	HDC hdc = GetDC(m_pView->GetHWND() );
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	switch(index)
	{
	case 1:
	case 5:
		::StretchBlt(m_dcMem,rect.right,rect.top,rect.left - rect.right ,rect.bottom - rect.top , hdcTemp,0,0 ,rect.right - rect.left,rect.bottom - rect.top,SRCCOPY);
		break;
	case 2:
		::StretchBlt(m_dcMem,rect.left,rect.bottom,rect.right - rect.left ,rect.top - rect.bottom , hdcTemp,0,0 ,rect.right - rect.left,rect.bottom - rect.top,SRCCOPY);
		break;
	case 4:  //90
		//::StretchBlt(m_dcMem,rect.right,0,rect.left - rect.right,hdcTemp,
		//m_XMax = rect.bottom ;
		//m_YMax = rect.right ;
		break;
	case 6:
		break;
	}

	DeleteObject(m_hBitMap);
	m_hBitMap = CreateCompatibleBitmap(m_dcMem,m_XMax - m_XMin,m_YMax -m_YMin);
	SelectObject(m_hBitDc,m_hBitMap);
	::BitBlt( m_hBitDc,0,0,m_XMax - m_XMin,m_YMax -m_YMin,m_dcMem,m_XMin,m_YMin,SRCCOPY);

	::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	SelectObject(hdc,CreatePen(PS_DOT,1,RGB(0,0,0)) );
	SelectObject(hdc,GetStockObject(NULL_BRUSH) );
	Rectangle(hdc,(m_XMin - CMyFliter::m_Start.x) ,\
		(m_YMin - CMyFliter::m_Start.y) ,\
		(m_XMax - CMyFliter::m_Start.x) ,\
		(m_YMax - CMyFliter::m_Start.y) );
	ReleaseDC(m_pView->GetHWND(),hdc);
}


void CMyFliterCut1::Zoom(int xZoom,int yZoom,int lV,int lH,const RECT& rect)
{
	// lV,lH is ignored 
	RECT rectNow = {rect.left,rect.top,rect.right,rect.bottom};
	HDC hdcTemp = CreateCompatibleDC(m_dcMem);
	HBITMAP hb = CreateCompatibleBitmap(m_dcMem,rect.right - rect.left ,rect.bottom - rect.top);
	SelectObject(hdcTemp,hb);

	::BitBlt(hdcTemp,0,0,rect.right - rect.left ,rect.bottom - rect.top,m_dcMem,rect.left,rect.top,SRCCOPY);
	::FillRect(m_dcMem,&rectNow,(HBRUSH)GetStockObject(WHITE_BRUSH) );

	HDC hdc = GetDC(m_pView->GetHWND() );
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);


	StretchBlt(m_dcMem,rect.left,rect.top,(rect.right - rect.left) * xZoom/100.0,(rect.bottom - rect.top) * yZoom/100.0,hdcTemp,0,0,rect.right-rect.left,rect.bottom - rect.top,SRCCOPY);

	BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,CMyFliter::m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	ReleaseDC(m_pView->GetHWND(),hdc);

	DeleteDC(hdcTemp);
	DeleteObject(hb);
	OnFinish();

}


void CMyFliterCut0::Cut()
{
	HDC hdcTemp = CreateCompatibleDC(m_dcMem);
	HBITMAP hb = CreateCompatibleBitmap(m_dcMem, m_XMax - m_XMin, m_YMax - m_YMin );
	SelectObject(hdcTemp,hb);
	BitBlt(hdcTemp,0,0,m_XMax - m_XMin,m_YMax - m_YMin ,m_dcMem,m_XMin,m_YMin,SRCCOPY);

	if(m_XMin != m_XMax)
	{
		::OpenClipboard(NULL);
		EmptyClipboard(); 
		::SetClipboardData(CF_BITMAP,hb);
		::CloseClipboard();
	}
	HBITMAP temp = CreateCompatibleBitmap(m_dcMem,10,10);
	SelectObject(hdcTemp,temp);
	DeleteDC(hdcTemp);
	RECT rect =  {m_XMin,m_YMin,m_XMax,m_YMax};
	FillRect(m_dcMem,&rect,m_Brush);
	OnFinish();
}
void CMyFliterCut0::Copy()
{
	HDC hdcTemp = CreateCompatibleDC(m_dcMem);
	HBITMAP hb = CreateCompatibleBitmap(m_dcMem, m_XMax - m_XMin, m_YMax - m_YMin );
	SelectObject(hdcTemp,hb);
	BitBlt(hdcTemp,0,0,m_XMax - m_XMin,m_YMax - m_YMin ,m_dcMem,m_XMin,m_YMin,SRCCOPY);

	if(m_XMin != m_XMax)
	{
		::OpenClipboard(NULL);
		EmptyClipboard(); 
		::SetClipboardData(CF_BITMAP,hb);
		::CloseClipboard();
	}
	HBITMAP temp = CreateCompatibleBitmap(m_dcMem,10,10);
	SelectObject(hdcTemp,temp);
	DeleteDC(hdcTemp);

}
void CMyFliterCut0::Paste()
{
	::OpenClipboard(NULL);

	HBITMAP hb =  (HBITMAP)::GetClipboardData(CF_BITMAP);

	::CloseClipboard();

	BITMAP bm;
	::GetObject(hb,sizeof(bm), &bm);


	::CloseClipboard();

	HDC hdcTemp = CreateCompatibleDC(m_dcMem);
	SelectObject(hdcTemp,hb);

	if(m_XMin == m_XMax)
	{
		m_XMin = 0;
		m_XMax = bm.bmWidth;
		m_YMax = bm.bmHeight;	
	}

	::StretchBlt(m_dcMem,m_XMin,m_YMin,m_XMax - m_XMin,m_YMax - m_YMin,hdcTemp,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);



	HBITMAP temp = CreateCompatibleBitmap(m_dcMem,10,10);
	SelectObject(hdcTemp,temp);
	DeleteDC(hdcTemp);

	OnFinish();
	HDC hdc = GetDC(m_pView->GetHWND() );
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,CMyFliter::m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	ReleaseDC(m_pView->GetHWND(),hdc);


}


void CMyFliter::Zoom(int xZoom,int yZoom,int lV,int lH,const RECT& rect)
{
	return;

}

void CMyFliter::OnToolChange(CMyToolBarWithList::ToolStyle tool,CMyToolBarWithList::ToolStyle styleFormer)
{
	m_toolFormer = styleFormer;
	m_tool = tool;
	DeleteObject(m_Pen);
	DeleteObject(m_Brush);
	DeleteObject(m_Pen2);
	DeleteObject(m_Brush2);
	switch(tool)
	{
	case CMyToolBarWithList::TOOL_BRUSH:
		m_Pen = ::CreatePen(PS_SOLID,6,m_FtColor);
		m_Pen2 = ::CreatePen(PS_SOLID,6,m_BkColor);
		m_Brush = (HBRUSH)GetStockObject(NULL_BRUSH);
		m_Brush2 = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	case CMyToolBarWithList::TOOL_GET:
	case CMyToolBarWithList::TOOL_ZOOM:
	case CMyToolBarWithList::TOOL_PENCIL:

	case CMyToolBarWithList::TOOL_WORD:
	case CMyToolBarWithList::TOOL_ERASER:
		m_Pen = (HPEN)GetStockObject(NULL_PEN);
		m_Pen2 = ::CreatePen(PS_SOLID,6,m_BkColor);
		m_Brush = (HBRUSH)GetStockObject(NULL_BRUSH);
		m_Brush2 = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	case CMyToolBarWithList::TOOL_LINE:
		m_Pen = CreatePen(PS_SOLID,1,m_FtColor);
		m_Pen2 = CreatePen(PS_SOLID,1,m_BkColor);
		m_Brush = (HBRUSH)GetStockObject(NULL_BRUSH);
		m_Brush2 = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	case CMyToolBarWithList::TOOL_BIN: 
	case CMyToolBarWithList::TOOL_RECT:
	case CMyToolBarWithList::TOOL_POL:
	case CMyToolBarWithList::TOOL_ELLIPSE:
	case CMyToolBarWithList::TOOL_CIRCLERECT:
	case CMyToolBarWithList::TOOL_CURL:
	case CMyToolBarWithList::TOOL_CUT_0:
	case CMyToolBarWithList::TOOL_CUT_1:
	case CMyToolBarWithList::TOOL_GUN:
		m_Pen = ::CreatePen(PS_SOLID,1,m_FtColor);
		m_Pen2 = ::CreatePen(PS_SOLID,1,m_BkColor);
		m_Brush = ::CreateSolidBrush(m_BkColor);
		m_Brush2 = ::CreateSolidBrush(m_FtColor);
		break;
	}

	::DeleteObject( SelectObject(m_dcMem,m_Pen) );
	::DeleteObject( SelectObject(m_dcMem,m_Brush) );
}

void  CMyFliter::OnIndexChangeOfTool(int iTem)  
{
	CMyFliter::m_SelectItem = iTem;
	if( m_tool == CMyToolBarWithList::TOOL_LINE)
	{
		::DeleteObject(m_Pen);
		::DeleteObject(m_Pen2);
		m_Pen = ::CreatePen(PS_SOLID, iTem*2 + 1,m_FtColor);
		m_Pen2 = ::CreatePen(PS_SOLID,iTem*2 + 1,m_BkColor);
		DeleteObject( SelectObject(m_dcMem,m_Pen) );
	}
	else if( m_tool == CMyToolBarWithList::TOOL_ERASER)
	{
		::DeleteObject(m_Pen2);
		m_Pen2 = CreatePen(PS_SOLID,6 + iTem *2,m_BkColor);
		DeleteObject( SelectObject(m_dcMem,m_Pen2) );
	}
	else if( m_tool == CMyToolBarWithList::TOOL_ZOOM)
	{
		int zoom = m_zoom;
		if(iTem == 0)
		{
			zoom = 1;
		}
		if(iTem == 1)
		{
			zoom = 2;
		}
		if(iTem == 2)
		{
			zoom = 6;
		}
		if(iTem == 3)
		{
			zoom = 8;
		}

		m_zoom = zoom;

		//::SetWindowPos(m_pView->GetHWND(),NULL,68,3,640*zoom,480*zoom,SWP_NOZORDER);
		m_pFrameWnd->SetViewToolBar();
		HDC hdc = GetDC(m_pView->GetHWND());
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

		//	SetWindowExtEx(CMyFliter::m_dcMem,1,1,NULL);
		BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,CMyFliter::m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);

		ReleaseDC(m_pView->GetHWND(),hdc);
	}

}


void CMyFliter::SetFtColor( COLORREF color)
{
	m_FtColor = color;
	DeleteObject(m_Pen);
	DeleteObject(m_Brush2);
	int width = 1;   //Default line width
	if(m_tool == CMyToolBarWithList::TOOL_LINE )
	{
		width = 2*m_SelectItem + 1;
	}
	m_Pen = ::CreatePen(PS_SOLID,width,m_FtColor);
	m_Brush2 = ::CreateSolidBrush( m_FtColor);

	DeleteObject( SelectObject(m_dcMem,m_Pen) );

}
void CMyFliter::SetBkColor( COLORREF color)
{
	m_BkColor = color;
	DeleteObject(m_Brush);
	DeleteObject(m_Pen2);
	m_Brush = ::CreateSolidBrush(m_BkColor); 
	int width = 1;   //Default line width
	if(m_tool == CMyToolBarWithList::TOOL_LINE )
	{
		width = 2*m_SelectItem + 1;
	}
	else if(m_tool == CMyToolBarWithList::TOOL_ERASER )
	{
		width = 6 + m_SelectItem*2 ;
	}
	m_Pen2  = ::CreatePen(PS_SOLID,width,m_BkColor);

	DeleteObject( SelectObject(m_dcMem,m_Brush) );
}


void CMyFliter::OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd,&ps);
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,m_zoom,m_zoom,NULL);
	RECT rect = ps.rcPaint;
	BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,CMyFliter::m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	EndPaint(hWnd,&ps);
}


void CMyFliter::OnInit()
{
	RECT  rectClient;
	HDC hdc = ::GetDC(m_pView->GetHWND() );
	::GetClientRect(m_pView->GetHWND(),&rectClient);
	CMyFliter::m_dcMem = CreateCompatibleDC(hdc);
	CMyFliter::m_bitMap = CreateCompatibleBitmap(hdc,rectClient.right - rectClient.left , rectClient.bottom - rectClient.top);
	SelectObject(CMyFliter::m_dcMem,m_bitMap);
	SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_PEN) );
	SelectObject(CMyFliter::m_dcMem,GetStockObject(WHITE_BRUSH) );
	Rectangle(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom);
	SelectObject(CMyFliter::m_dcMem,GetStockObject(BLACK_PEN) );
	SelectObject(CMyFliter::m_dcMem,GetStockObject(WHITE_BRUSH) );
	RECT rectInitialize = {0,0,640,480};
	m_pDoc->OnInit(CMyFliter::m_dcMem,rectInitialize);
	m_dcBitMap = m_pDoc->GetLastDC();
	::ReleaseDC(m_pView->GetHWND(),hdc);

	//Reset all
	CMyFliter::m_tool = CMyToolBarWithList::TOOL_PENCIL;
	CMyFliter::m_FtColor   = RGB(0,0,0);
	CMyFliter::m_BkColor   = RGB(255,255,255);
	CMyFliter::m_SelectItem = 0;

	::DeleteObject(CMyFliter::m_Pen);
	::DeleteObject(CMyFliter::m_Brush);
	::DeleteObject(CMyFliter::m_Pen2);
	::DeleteObject(CMyFliter::m_Brush2);
	CMyFliter::m_Pen  = ::CreatePen(PS_SOLID,1,RGB(0,0,0) );  //Pencil default style
	CMyFliter::m_Brush = ::CreateSolidBrush( RGB(255,255,255) ); //Pencil default style
	CMyFliter::m_Pen2  = ::CreatePen(PS_SOLID,1,RGB(255,255,255) );  //Pencil default style
	CMyFliter::m_Brush2 = ::CreateSolidBrush( RGB(0,0,0) ); //Pencil default style

}



void CMyFliterEndMouseUp::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	//pDoc->OnFinish(m_dcMem)
	if(m_State == DRAWINGL)
	{	
		m_State = NONE;
		m_pDoc->OnFinish(CMyFliter::m_dcMem,m_rectTrue);
		m_dcBitMap = m_pDoc->GetLastDC();

	}
}

void CMyFliterEndMouseUp::OnRButtonUp(WPARAM wParam,LPARAM lParam)
{
	//pDoc->OnFinish(m_dcMem)
	if(m_State == DRAWINGR)
	{
		m_State = NONE;
		m_pDoc->OnFinish(CMyFliter::m_dcMem,m_rectTrue);
		m_dcBitMap = m_pDoc->GetLastDC();
	}
}


void CMyFliterEndMouseUp::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	if(m_State == NONE)
	{
		m_State = DRAWINGL;
		m_point.x = LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x;
		m_point.y =  HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y ;
	}

}

void CMyFliterEndMouseUp::OnRButtonDown(WPARAM wParam,LPARAM lParam)
{
	if(m_State == NONE)
	{
		m_State = DRAWINGR;
		m_point.x = LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x;
		m_point.y = HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y ;
	}
}

void CMyFliterEndMouseUpGraph::OnMouseMove(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = { LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y };
	POINT pointFormer = {m_point.x,m_point.y};
	POINT pointTemp;
	if(  (m_State == DRAWINGL && wParam&MK_LBUTTON ) || ( m_State == DRAWINGR && wParam&MK_RBUTTON ) ) //MouseDown
	{
		//Judge Shift Key
		if(wParam & MK_SHIFT)
		{
			if(ABS(pointNow.x - pointFormer.x) > ABS(pointNow.y - pointFormer.y) )  // x > y
			{
				if(pointNow.x > pointFormer.x )
					pointNow.x = pointFormer.x + ABS(pointNow.y - pointFormer.y);
				else
					pointNow.x = pointFormer.x - ABS(pointNow.y - pointFormer.y);
			}
			else   //x<=y
			{
				if(pointNow.y > pointFormer.y )
					pointNow.y = pointFormer.y + ABS(pointNow.x - pointFormer.x);
				else
					pointNow.y = pointFormer.y - ABS(pointNow.x - pointFormer.x);
			}
		}
		//Make pointNow and pointFormer to left-top to right-bottom form

		if(pointFormer.x > pointNow.x ) 
		{
			pointTemp.x   = pointFormer.x;
			pointFormer.x = pointNow.x;
			pointNow.x = pointTemp.x;
		}
		if(pointFormer.y > pointNow.y)
		{
			pointTemp.y = pointFormer.y;
			pointFormer.y = pointNow.y;
			pointNow.y   = pointTemp.y;
		}
		Draw(pointFormer,pointNow);
	}
}

void CMyFliterLine::OnMouseMove(WPARAM wParam,LPARAM lParam)
{
	if(  (m_State == DRAWINGL && wParam&MK_LBUTTON ) || ( m_State == DRAWINGR && wParam&MK_RBUTTON ) ) //MouseDown
	{
		//if it's right button
		if( wParam& MK_RBUTTON)
			SelectObject(m_dcMem,m_Pen2);
		else
			SelectObject(m_dcMem,m_Pen);

		POINT pointNow = {LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x , HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y };
		Draw( m_point ,pointNow);
	}
}


void CMyFliterLine::Draw(POINT start,POINT end)
{
	HDC hdc = GetDC(m_pView->GetHWND() );  //pen is alright when it changes its index of each iTem
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
	::MoveToEx(CMyFliter::m_dcMem,start.x,start.y ,NULL);
	::LineTo(CMyFliter::m_dcMem,end.x,end.y);
	::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);

	ReleaseDC(m_pView->GetHWND() ,hdc);
}


void CMyFliterRect::Draw(POINT start,POINT end)
{
	HDC hdc = GetDC(m_pView->GetHWND() );
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	switch(CMyFliter::m_SelectItem)
	{
	case 0:
		if( m_State == DRAWINGL)
		{	
			SelectObject(CMyFliter::m_dcMem,m_Pen);
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_BRUSH) );
		}
		else  // right button
		{
			SelectObject(CMyFliter::m_dcMem,m_Pen2);
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_BRUSH) );
		}
		break;

	case 1:
		if( m_State == DRAWINGL)
		{	
			SelectObject(CMyFliter::m_dcMem,m_Pen);
			SelectObject(CMyFliter::m_dcMem,m_Brush);	
		}
		else  // right button
		{
			SelectObject(CMyFliter::m_dcMem,m_Pen2);
			SelectObject(CMyFliter::m_dcMem,m_Brush2);	
		}
		break;

	case 2:
		if( m_State == DRAWINGL)
		{	
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_PEN) );
			SelectObject(CMyFliter::m_dcMem,m_Brush );
		}
		else
		{
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_PEN) );
			SelectObject(CMyFliter::m_dcMem,m_Brush2 );
		}
		break;
	}
	::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
	Rectangle(CMyFliter::m_dcMem,start.x,start.y,end.x,end.y);
	RECT rect ;
	GetClientRect(m_pView->GetHWND(),&rect);
	::BitBlt(hdc,0,0,rect.right - rect.left,rect.bottom - rect.top,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	ReleaseDC(m_pView->GetHWND() ,hdc);
}


void CMyFliterRoundRect::Draw(POINT start,POINT end) 
{
	HDC hdc = GetDC(m_pView->GetHWND() );
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	switch(CMyFliter::m_SelectItem)
	{
	case 0:
		if( m_State == DRAWINGL)
		{	
			SelectObject(CMyFliter::m_dcMem,m_Pen);
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_BRUSH) );
		}
		else  // right button
		{
			SelectObject(CMyFliter::m_dcMem,m_Pen2);
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_BRUSH) );
		}
		break;

	case 1:
		if( m_State == DRAWINGL)
		{	
			SelectObject(CMyFliter::m_dcMem,m_Pen);
			SelectObject(CMyFliter::m_dcMem,m_Brush);	
		}
		else  // right button
		{
			SelectObject(CMyFliter::m_dcMem,m_Pen2);
			SelectObject(CMyFliter::m_dcMem,m_Brush2);	
		}
		break;

	case 2:
		if( m_State == DRAWINGL)
		{	
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_PEN) );
			SelectObject(CMyFliter::m_dcMem,m_Brush );
		}
		else
		{
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_PEN) );
			SelectObject(CMyFliter::m_dcMem,m_Brush2 );
		}
		break;
	}
	::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
	int width = (start.y - end.y )>0? (start.y - end.y ) : (end.y - start.y) /4;
	int height = (start.x - end.x )>0? (start.x - end.x ) : (end.y - start.y) /4;
	RoundRect(CMyFliter::m_dcMem,start.x,start.y,end.x,end.y, width,height );
	::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	ReleaseDC(m_pView->GetHWND() ,hdc);
}

void CMyFliterEllipse::Draw(POINT start,POINT end)
{
	HDC hdc = GetDC(m_pView->GetHWND() );
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	switch(CMyFliter::m_SelectItem)
	{
	case 0:
		if( m_State == DRAWINGL)
		{	
			SelectObject(CMyFliter::m_dcMem,m_Pen);
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_BRUSH) );
		}
		else  // right button
		{
			SelectObject(CMyFliter::m_dcMem,m_Pen2);
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_BRUSH) );
		}
		break;

	case 1:
		if( m_State == DRAWINGL)
		{	
			SelectObject(CMyFliter::m_dcMem,m_Pen);
			SelectObject(CMyFliter::m_dcMem,m_Brush);	
		}
		else  // right button
		{
			SelectObject(CMyFliter::m_dcMem,m_Pen2);
			SelectObject(CMyFliter::m_dcMem,m_Brush2);	
		}
		break;

	case 2:
		if( m_State == DRAWINGL)
		{	
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_PEN) );
			SelectObject(CMyFliter::m_dcMem,m_Brush );
		}
		else
		{
			SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_PEN) );
			SelectObject(CMyFliter::m_dcMem,m_Brush2 );
		}
		break;
	}
	::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);

	Ellipse(CMyFliter::m_dcMem,start.x,start.y,end.x,end.y);

	::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);

	ReleaseDC(m_pView->GetHWND() ,hdc);
}


void CMyFliterGet::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{

	if(m_State == DRAWINGL)
	{
		m_point.x = LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x;
		m_point.y = HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y ;
		COLORREF color;
		color = ::GetPixel(m_dcMem,m_point.x,m_point.y);
		m_pFrameWnd->OnFtColorChange(color);
		m_State = NONE;
		m_pFrameWnd->SetSelect(m_toolFormer);

	}

}


void CMyFliterGet::OnRButtonUp(WPARAM wParam,LPARAM lParam)
{
	if(m_State == DRAWINGR)
	{
		COLORREF color;
		m_point.x = LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x;
		m_point.y = HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y ;
		color = ::GetPixel(m_dcMem,m_point.x,m_point.y);
		m_pFrameWnd->OnBkColorChange(color);
		m_State = NONE;
		m_pFrameWnd->SetSelect(m_toolFormer);
	}
}


void CMyFliterBin::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	if(m_State == NONE)
	{	m_State = DRAWINGL;
	POINT point = { LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
	Draw(point);
	}
}

void CMyFliterBin::OnRButtonDown(WPARAM wParam,LPARAM lParam)
{
	if(m_State == NONE)
	{
		m_State = DRAWINGR;
		POINT point = { LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
		Draw(point);
	}


}

void CMyFliterBin::OnMouseMove(WPARAM wParam,LPARAM lParam)
{
	return ;
}

void CMyFliterBin::Draw(POINT pointNow)
{

	if(m_State == DRAWINGL)
		::SelectObject(m_dcMem,m_Brush2);
	else
		::SelectObject(m_dcMem,m_Brush);

	::ExtFloodFill(m_dcMem,pointNow.x,pointNow.y,::GetPixel(m_dcMem,pointNow.x,pointNow.y),FLOODFILLSURFACE);
	HDC hdc = GetDC( m_pView->GetHWND() );
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	ReleaseDC( m_pView->GetHWND(),hdc);

}

void CMyFliterEraser::Draw(POINT pointNow)
{
	if(m_State == DRAWINGL)
	{
		::SelectObject(m_dcMem,m_Pen2);
		::MoveToEx(m_dcMem,m_point.x,m_point.y,NULL);
		::LineTo(m_dcMem,pointNow.x,pointNow.y);
		m_point.x = pointNow.x;
		m_point.y = pointNow.y;
		HDC hdc = GetDC( m_pView->GetHWND() );
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		ReleaseDC( m_pView->GetHWND(),hdc);
	}

};

void CMyFliterBrush::Draw(POINT pointNow)
{	
		::SelectObject(m_dcMem,m_Pen);
		::MoveToEx(m_dcMem,m_point.x,m_point.y,NULL);
		::LineTo(m_dcMem,pointNow.x,pointNow.y);
		m_point.x = pointNow.x;
		m_point.y = pointNow.y;
		HDC hdc = GetDC( m_pView->GetHWND() );
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		ReleaseDC( m_pView->GetHWND(),hdc);


}

void CMyFliterPen::Draw(POINT pointNow)
{
	if(m_State == DRAWINGL)
		::SelectObject(m_dcMem,m_Pen);
	else
		::SelectObject(m_dcMem,m_Pen2);

	::MoveToEx(m_dcMem,m_point.x,m_point.y,NULL);
	::LineTo(m_dcMem,pointNow.x,pointNow.y);
	m_point.x = pointNow.x;
	m_point.y = pointNow.y;
	HDC hdc = GetDC( m_pView->GetHWND() );
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	ReleaseDC( m_pView->GetHWND(),hdc);
}


void CMyFliterCurl::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = {LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y };
	if( m_State == DRAWING_LINE)
	{
		m_State = DRAWING_1;
		m_Point[2].x = m_Point[3].x = pointNow.x;
		m_Point[2].y = m_Point[3].y = pointNow.y;
	}
	else if(m_State == DRAWING_1)
	{
		m_State = DRAWING_2;
	}
	else if(m_State == DRAWING_2)
	{
		m_pDoc->OnFinish(CMyFliter::m_dcMem,m_rectTrue);
		m_dcBitMap = m_pDoc->GetLastDC();
		m_State = NONE;
	}

}

void CMyFliterCurl::OnRButtonUp(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = {LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
	if( m_State == DRAWING_LINE)
	{
		m_State = DRAWING_1;
		m_Point[2].x = m_Point[3].x = pointNow.x;
		m_Point[2].y = m_Point[3].y = pointNow.y;
	}
	else if(m_State == DRAWING_1)
	{
		m_State = DRAWING_2;
	}
	else if(m_State == DRAWING_2)
	{
		m_pDoc->OnFinish(CMyFliter::m_dcMem,m_rectTrue);
		m_dcBitMap = m_pDoc->GetLastDC();
		m_State = NONE;
	}
}

void CMyFliterCurl::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	SelectObject(m_dcMem,m_Pen);
	POINT pointNow = {LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y };
	if( m_State == NONE)
	{
		SelectObject(m_dcMem,m_Pen);
		m_Point[1].x = m_Point[0].x = pointNow.x;
		m_Point[1].y = m_Point[0].y = pointNow.y;
		m_State = DRAWING_LINE;
	}
	else if (m_State == DRAWING_1)
	{
		HDC hdc = ::GetDC(m_pView->GetHWND() );		
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

		::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		m_Point[1].x = pointNow.x;
		m_Point[1].y = pointNow.y;
		::PolyBezier(CMyFliter::m_dcMem,m_Point,4);
		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		::ReleaseDC( m_pView->GetHWND(),hdc);
	}
	else if (m_State == DRAWING_2)
	{
		HDC hdc = ::GetDC(m_pView->GetHWND() );		
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

		::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		m_Point[2].x = pointNow.x;
		m_Point[2].y = pointNow.y;
		::PolyBezier(CMyFliter::m_dcMem,m_Point,4);
		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		::ReleaseDC( m_pView->GetHWND(),hdc);
	}


}

void CMyFliterCurl::OnRButtonDown(WPARAM wParam,LPARAM lParam)
{
	SelectObject(m_dcMem,m_Pen2);

	POINT pointNow = {LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
	if( m_State == NONE)
	{
		SelectObject(m_dcMem,m_Pen2);
		m_Point[0].x = pointNow.x;
		m_Point[0].y = pointNow.y;
		m_State = DRAWING_LINE;
	}
	else if (m_State == DRAWING_1) 
	{
		HDC hdc = ::GetDC(m_pView->GetHWND() );	
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

		::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		m_Point[1].x = pointNow.x;
		m_Point[1].y = pointNow.y;
		::PolyBezier(CMyFliter::m_dcMem,m_Point,4);
		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		::ReleaseDC( m_pView->GetHWND(),hdc);
	}
	else if (m_State == DRAWING_2)
	{
		HDC hdc = ::GetDC(m_pView->GetHWND() );	
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

		::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		m_Point[2].x = pointNow.x;
		m_Point[2].y = pointNow.y;
		::PolyBezier(CMyFliter::m_dcMem,m_Point,4);
		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		::ReleaseDC( m_pView->GetHWND(),hdc);
	}

}

void CMyFliterCurl::OnMouseMove(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = { LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y };
	if( wParam & MK_LBUTTON)
		SelectObject(m_dcMem,m_Pen);
	else if(wParam & MK_RBUTTON)
		SelectObject(m_dcMem,m_Pen2);

	if( m_State == DRAWING_LINE &&  ( (wParam & MK_LBUTTON) || (wParam & MK_RBUTTON )  ) )
	{
		HDC hdc = ::GetDC(m_pView->GetHWND() );		
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

		::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		::MoveToEx(CMyFliter::m_dcMem,m_Point[0].x,m_Point[0].y,NULL);
		::LineTo(CMyFliter::m_dcMem,pointNow.x,pointNow.y);
		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		::ReleaseDC( m_pView->GetHWND(),hdc);

	}
	else if( m_State == DRAWING_1 &&  ( (wParam & MK_LBUTTON) || (wParam & MK_RBUTTON )  ) )
	{
		HDC hdc = ::GetDC(m_pView->GetHWND() );		
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

		::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		m_Point[1].x = pointNow.x;
		m_Point[1].y = pointNow.y;
		::PolyBezier(CMyFliter::m_dcMem,m_Point,4);
		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		::ReleaseDC( m_pView->GetHWND(),hdc);

	}
	else if( m_State == DRAWING_2 &&  ( (wParam & MK_LBUTTON) || (wParam & MK_RBUTTON )  ) )
	{
		HDC hdc = ::GetDC(m_pView->GetHWND() );	
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

		::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		m_Point[2].x = pointNow.x;
		m_Point[2].y = pointNow.y;
		::PolyBezier(CMyFliter::m_dcMem,m_Point,4);
		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		::ReleaseDC( m_pView->GetHWND(),hdc);

	}
}


void CMyFliterPol::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = {LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y };
	if(m_State == DRAWING_LINE)
	{
		m_State = DRAWING_OTHER;
		m_Point[1].x = pointNow.x;
		m_Point[1].y = pointNow.y;
		m_Count++;
	}
	else if(m_State == DRAWING_OTHER)
	{
		m_Point[m_Count].x = pointNow.x;
		m_Point[m_Count++].y = pointNow.y;
	}

}

void CMyFliterPol::OnRButtonUp(WPARAM wParam,LPARAM lParam)
{
	OnLButtonUp(wParam,lParam);

}

void CMyFliterPol::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = { LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
	::SelectObject(m_dcMem,m_Pen);
	if( m_State == NONE)
	{

		m_Point[0].x = pointNow.x;
		m_Point[0].y = pointNow.y;
		m_Count++;
		m_State = DRAWING_LINE;
	}
	else if( m_State == DRAWING_OTHER)
	{
		HDC hdc = ::GetDC(m_pView->GetHWND() );	
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
		::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		::MoveToEx(CMyFliter::m_dcMem,m_Point[0].x,m_Point[0].y,NULL);
		for(int i = 1;i<m_Count;i++)
		{
			::LineTo(CMyFliter::m_dcMem,m_Point[i].x,m_Point[i].y);
			::MoveToEx(CMyFliter::m_dcMem,m_Point[i].x,m_Point[i].y,NULL);
		}
		::LineTo(CMyFliter::m_dcMem , pointNow.x,pointNow.y);
		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		::ReleaseDC( m_pView->GetHWND(),hdc);
	}
}

void CMyFliterPol::OnRButtonDown(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = { LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
	::SelectObject(m_dcMem,m_Pen2);
	if( m_State == NONE)
	{

		m_Point[0].x = pointNow.x;
		m_Point[0].y = pointNow.y;
		m_Count++;
		m_State = DRAWING_LINE;
	}
	else if( m_State == DRAWING_OTHER)
	{
		HDC hdc = ::GetDC(m_pView->GetHWND() );		
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
		::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		::MoveToEx(CMyFliter::m_dcMem,m_Point[0].x,m_Point[0].y,NULL);
		for(int i = 1;i<m_Count;i++)
		{
			::LineTo(CMyFliter::m_dcMem,m_Point[i].x,m_Point[i].y);
			::MoveToEx(CMyFliter::m_dcMem,m_Point[i].x,m_Point[i].y,NULL);
		}
		::LineTo(CMyFliter::m_dcMem , pointNow.x,pointNow.y);

		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		::ReleaseDC( m_pView->GetHWND(),hdc);
	}
}

void CMyFliterPol::OnMouseMove(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = {LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
	if( wParam & MK_LBUTTON)
		SelectObject(m_dcMem,m_Pen);
	else if(wParam & MK_RBUTTON)
		SelectObject(m_dcMem,m_Pen2);
	if( m_State == DRAWING_LINE &&  ( (wParam & MK_LBUTTON) || (wParam & MK_RBUTTON )  ) )
	{
		HDC hdc = ::GetDC(m_pView->GetHWND() );		
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
		::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		::MoveToEx(CMyFliter::m_dcMem,m_Point[0].x,m_Point[0].y,NULL);
		::LineTo(CMyFliter::m_dcMem,pointNow.x,pointNow.y);
		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		::ReleaseDC( m_pView->GetHWND(),hdc);
	}
	else if( m_State == DRAWING_OTHER &&  ( (wParam & MK_LBUTTON) || (wParam & MK_RBUTTON ) ))
	{
		HDC hdc = ::GetDC(m_pView->GetHWND() );		
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
		::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		::MoveToEx(CMyFliter::m_dcMem,m_Point[0].x,m_Point[0].y,NULL);
		for(int i = 1;i< m_Count;i++)
		{
			::LineTo(CMyFliter::m_dcMem,m_Point[i].x,m_Point[i].y);
			::MoveToEx(CMyFliter::m_dcMem,m_Point[i].x,m_Point[i].y,NULL);
		}
		::LineTo(CMyFliter::m_dcMem,pointNow.x,pointNow.y);		
		::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		::ReleaseDC( m_pView->GetHWND(),hdc);
	}
}

void CMyFliterPol::OnRButtonDBCLICK(WPARAM wParam,LPARAM lParam)
{
	if( m_SelectItem == 0)
	{
		SelectObject(m_dcMem,m_Pen2);
		SelectObject(m_dcMem,GetStockObject(NULL_BRUSH) );
	}
	else if( m_SelectItem == 1)
	{
		SelectObject(m_dcMem,m_Pen2);
		SelectObject(m_dcMem,m_Brush2);
	}
	else if( m_SelectItem == 2)
	{
		SelectObject(m_dcMem,GetStockObject(NULL_PEN) );
		SelectObject(m_dcMem,m_Brush2);
	}
	HDC hdc = ::GetDC(m_pView->GetHWND() );	
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
	::Polygon(CMyFliter::m_dcMem,m_Point,m_Count);
	::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	::ReleaseDC( m_pView->GetHWND(),hdc);
	m_pDoc->OnFinish(CMyFliter::m_dcMem,m_rectTrue);
	m_dcBitMap = m_pDoc->GetLastDC();
	m_State = NONE;
	m_Count = 0;
}
void CMyFliterPol::OnLButtonDBCLICK(WPARAM wParam,LPARAM lParam)
{
	if( m_SelectItem == 0)
	{
		SelectObject(m_dcMem,m_Pen);
		SelectObject(m_dcMem,GetStockObject(NULL_BRUSH) );
	}
	else if( m_SelectItem == 1)
	{
		SelectObject(m_dcMem,m_Pen);
		SelectObject(m_dcMem,m_Brush);
	}
	else if( m_SelectItem == 2)
	{
		SelectObject(m_dcMem,GetStockObject(NULL_PEN) );
		SelectObject(m_dcMem,m_Brush);
	}


	HDC hdc = ::GetDC(m_pView->GetHWND() );	
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
	::Polygon(CMyFliter::m_dcMem,m_Point,m_Count);
	::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	::ReleaseDC( m_pView->GetHWND(),hdc);
	m_pDoc->OnFinish(CMyFliter::m_dcMem,m_rectTrue);
	m_dcBitMap = m_pDoc->GetLastDC();
	m_State = NONE;
	m_Count = 0;

}

void CMyFliterCut0::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = { LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y };
	if( m_State == DRAWING)
	{
		m_State = MOVING;
		HDC hdc = ::GetDC(m_pView->GetHWND() );	
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

		SelectObject(hdc, GetStockObject(NULL_BRUSH) );
		HPEN pen = ::CreatePen(PS_DOT,1,RGB(0,0,0) );
		SelectObject(hdc,pen);

		m_RgnFormer = ::CreatePolygonRgn(m_Point,m_Count,WINDING);
		m_hBitDc = ::CreateCompatibleDC(hdc);
		m_hBitMap = ::CreateCompatibleBitmap(hdc,m_XMax - m_XMin , m_YMax - m_YMin);
		SelectObject(m_hBitDc,m_hBitMap);
		BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		BitBlt(m_hBitDc,0,0,m_XMax - m_XMin , m_YMax - m_YMin,m_dcMem,m_XMin,m_YMin,SRCCOPY);
		Rectangle(hdc,(m_XMin - CMyFliter::m_Start.x) ,\
			(m_YMin - CMyFliter::m_Start.y) ,\
			(m_XMax - CMyFliter::m_Start.x) ,\
			(m_YMax - CMyFliter::m_Start.y) );
		::ReleaseDC( m_pView->GetHWND(),hdc);

	}
	else if(m_State == MOVING)
	{
		int offX = pointNow.x - m_PointStart.x;
		int offY = pointNow.y - m_PointStart.y;
		for(int i = 0;i<m_Count ;i++)
		{
			m_Point[i].x = m_Point[i].x + offX;
			m_Point[i].y = m_Point[i].y + offY;
		}
		m_XMax += offX;
		m_XMin += offX;
		m_YMax += offY;
		m_YMin += offY;
		HPEN hp = CreatePen(PS_DOT,1,RGB(0,0,0) );

		HDC hdc = ::GetDC(m_pView->GetHWND() );	
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
		HPEN hpt = (HPEN)SelectObject(hdc,hp);
		HBRUSH hbt = (HBRUSH)SelectObject(hdc,GetStockObject(NULL_BRUSH) );

		Rectangle(hdc,(m_XMin - CMyFliter::m_Start.x) ,\
			(m_YMin - CMyFliter::m_Start.y) ,\
			(m_XMax - CMyFliter::m_Start.x) ,\
			(m_YMax - CMyFliter::m_Start.y) );
		DeleteObject( SelectObject(hdc,hpt) );
		DeleteObject( SelectObject(hdc,hbt) );
		::ReleaseDC( m_pView->GetHWND(),hdc);



		//HRGN hr = ::CreatePolygonRgn(m_PointBac,m_Count,WINDING);

	}
}

void CMyFliterCut0::OnRButtonUp(WPARAM wParam,LPARAM lParam)
{
	OnLButtonUp(wParam,lParam);
}

void CMyFliterCut0::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = {LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
	if( m_State == NONE)
	{
		m_State = DRAWING;
		m_Point[0].x = pointNow.x;
		m_Point[0].y = pointNow.y;
		m_XMin =  m_XMax = pointNow.x;
		m_YMin=  m_YMax = pointNow.y;
		m_Count ++;
	}
	else if ( m_State == MOVING )
	{
		if(pointNow.x < m_XMin || pointNow.x >m_XMax || pointNow.y < m_YMin || pointNow.y > m_YMax )
		{
			OnFinish();
			return;

		}



		//if not in the rgn 
		//m_State = NONE
		//OnFinish
		m_PointStart .x = pointNow.x;
		m_PointStart .y = pointNow.y;

	}

}

void CMyFliterCut0::OnRButtonDown(WPARAM wParam,LPARAM lParam)
{
	OnLButtonDown(wParam,lParam);
}

void CMyFliterCut0::OnMouseMove(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = { LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
	if(m_State == DRAWING)
	{
		if(  (wParam & MK_LBUTTON) || (wParam & MK_RBUTTON) )
		{
			HDC hdc = ::GetDC(m_pView->GetHWND() );		
			SetMapMode(hdc,MM_ANISOTROPIC );
			SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
			//::BitBlt(CMyFliter::m_dcMem,0,0,640,480,CMyFliter::m_dcBitMap,0,0,SRCCOPY);

			::MoveToEx(hdc, ( m_Point[m_Count - 1].x - CMyFliter::m_Start.x ) , (m_Point[m_Count-1].y - CMyFliter::m_Start.y),NULL );
			::LineTo(hdc,(pointNow.x - CMyFliter::m_Start.x ) , (pointNow.y - CMyFliter::m_Start.y));
			//::BitBlt(hdc,0,0,640,480,m_dcMem,CMyFliter::m_Start.x,m_Start.y,SRCCOPY);
			::ReleaseDC( m_pView->GetHWND(),hdc);
			if(pointNow.x < m_XMin)
				m_XMin = pointNow.x;
			if(pointNow.x > m_XMax)
				m_XMax = pointNow.x;
			if(pointNow.y < m_YMin)
				m_YMin = pointNow.y;
			if(pointNow.y > m_YMax)
				m_YMax = pointNow.y;

			m_Point[m_Count].x = pointNow.x;
			m_Point[m_Count++].y = pointNow.y;
		}
	}
	else if(m_State == MOVING)
	{
		if(  (wParam & MK_LBUTTON) || (wParam & MK_RBUTTON) )
		{

			int offX = pointNow.x - m_PointStart.x;
			int offY = pointNow.y - m_PointStart.y;
			for(int i = 0;i<m_Count ;i++)
			{
				m_PointBac[i].x = m_Point[i].x + offX;
				m_PointBac[i].y = m_Point[i].y + offY;
			}

			HRGN hr = ::CreatePolygonRgn(m_PointBac,m_Count,WINDING);
			BitBlt(m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,m_pDoc->GetLastDC(),0,0,SRCCOPY);

			HDC hdc = ::GetDC(m_pView->GetHWND() );	
			SetMapMode(hdc,MM_ANISOTROPIC );
			SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

			FillRgn(m_dcMem,m_RgnFormer,m_Brush);

			HRGN hrTemp = ::CreateRectRgn(0,0,m_rectTrue.right,m_rectTrue.bottom);
			SelectObject(m_dcMem,hr);

			BitBlt(m_dcMem, m_XMin + offX ,m_YMin + offY,m_XMax-m_XMin,m_YMax-m_YMin,m_hBitDc,0,0,SRCCOPY);


			BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
			::ReleaseDC( m_pView->GetHWND(),hdc);
			DeleteObject(SelectObject(m_dcMem,hrTemp) );
			DeleteObject(hr);

		}
	}

}
void CMyFliterCut0::OnFinish()
{
	if( m_State == MOVING)
	{
		HDC hdc = ::GetDC(m_pView->GetHWND() );	
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
		BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		::ReleaseDC( m_pView->GetHWND(),hdc);
		m_pDoc->OnFinish(CMyFliter::m_dcMem,m_rectTrue);
		m_dcBitMap = m_pDoc->GetLastDC();
		m_State = NONE;
		m_Count = 0;
		m_XMin =  m_XMax =  m_YMin =   m_YMax =0;
		DeleteDC(m_hBitDc);
		DeleteObject(m_hBitMap);

	}
}


void CMyFliterCut1::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = { LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
	if( m_State == DRAWING)
	{
		m_State = MOVING;
		HDC hdc = ::GetDC(m_pView->GetHWND() );	
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
		SelectObject(hdc, GetStockObject(NULL_BRUSH) );
		HPEN pen = ::CreatePen(PS_DOT,1,RGB(0,0,0) );
		SelectObject(hdc,pen);

		m_RgnFormer = ::CreateRectRgn(m_XMin,m_YMin,m_XMax,m_YMax);
		m_hBitDc = ::CreateCompatibleDC(hdc);
		m_hBitMap = ::CreateCompatibleBitmap(hdc,m_XMax - m_XMin , m_YMax - m_YMin);
		SelectObject(m_hBitDc,m_hBitMap);
		BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		BitBlt(m_hBitDc,0,0,m_XMax - m_XMin , m_YMax - m_YMin,m_dcMem,m_XMin,m_YMin,SRCCOPY);
		Rectangle(hdc,(m_XMin - CMyFliter::m_Start.x) ,\
			(m_YMin - CMyFliter::m_Start.y) ,\
			(m_XMax - CMyFliter::m_Start.x) ,\
			(m_YMax - CMyFliter::m_Start.y) );
		::ReleaseDC( m_pView->GetHWND(),hdc);

	}
	else if(m_State == MOVING)
	{
		int offX = pointNow.x - m_PointStart.x;
		int offY = pointNow.y - m_PointStart.y;
		m_XMax += offX;
		m_XMin += offX;
		m_YMax += offY;
		m_YMin += offY;
		HPEN hp = CreatePen(PS_DOT,1,RGB(0,0,0) );

		HDC hdc = ::GetDC(m_pView->GetHWND() );	
		SetMapMode(hdc,MM_ANISOTROPIC );
		SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
		HPEN hpt = (HPEN)SelectObject(hdc,hp);
		HBRUSH hbt = (HBRUSH)SelectObject(hdc,GetStockObject(NULL_BRUSH) );

		Rectangle(hdc,(m_XMin - CMyFliter::m_Start.x) ,\
			(m_YMin - CMyFliter::m_Start.y) ,\
			(m_XMax - CMyFliter::m_Start.x) ,\
			(m_YMax - CMyFliter::m_Start.y) );
		DeleteObject( SelectObject(hdc,hpt) );
		DeleteObject( SelectObject(hdc,hbt) );
		::ReleaseDC( m_pView->GetHWND(),hdc);

	}
}



void CMyFliterCut1::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = { LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y };
	if( m_State == NONE)
	{
		m_State = DRAWING;
		m_XMin =  m_XMax = pointNow.x;
		m_YMin=  m_YMax = pointNow.y;
	}
	else if ( m_State == MOVING )
	{
		if(pointNow.x < m_XMin || pointNow.x >m_XMax || pointNow.y < m_YMin || pointNow.y > m_YMax )
		{
			OnFinish();
			return;

		}
		m_PointStart .x = pointNow.x;
		m_PointStart .y = pointNow.y;

	}
}

void CMyFliterCut1::SetRectSel(const RECT& rect,bool bNeedFinish)
{
	if(m_State != NONE && bNeedFinish)
		OnFinish();
	m_State = MOVING;
	m_YMin = rect.top;
	m_YMax = rect.bottom;
	m_XMax = rect.right;
	m_XMin = rect.left;
	m_PointStart.x = rect.left;
	m_PointStart.y = rect.right;
	HDC hdc = ::GetDC(m_pView->GetHWND() );	
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

	m_RgnFormer = ::CreateRectRgn(m_XMin,m_YMin,m_XMax,m_YMax);
	m_hBitDc = ::CreateCompatibleDC(hdc);
	m_hBitMap = ::CreateCompatibleBitmap(hdc,m_XMax - m_XMin , m_YMax - m_YMin);
	SelectObject(m_hBitDc,m_hBitMap);
	BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	BitBlt(m_hBitDc,0,0,m_XMax - m_XMin , m_YMax - m_YMin,m_dcMem,m_XMin,m_YMin,SRCCOPY);


	HPEN hptemp = (HPEN)SelectObject(hdc,CreatePen(PS_DOT,1,RGB(0,0,0)) );
	SelectObject(hdc,GetStockObject(NULL_BRUSH) );
	Rectangle(hdc,(m_XMin - CMyFliter::m_Start.x) ,\
		(m_YMin - CMyFliter::m_Start.y) ,\
		(m_XMax - CMyFliter::m_Start.x) ,\
		(m_YMax - CMyFliter::m_Start.y) );
	DeleteObject(SelectObject(hdc,hptemp) );
	::ReleaseDC( m_pView->GetHWND(),hdc);
}



void CMyFliterCut1::OnMouseMove(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = {LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
	if(m_State == DRAWING)
	{
		if(  (wParam & MK_LBUTTON) || (wParam & MK_RBUTTON) )
		{			

			m_XMax = pointNow.x;
			m_YMax = pointNow.y;
			BitBlt(m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,m_pDoc->GetLastDC(),0,0,SRCCOPY);
			HPEN hp = CreatePen(PS_DOT,1,RGB(0,0,0) );

			HDC hdc = ::GetDC(m_pView->GetHWND() );	
			SetMapMode(hdc,MM_ANISOTROPIC );
			SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);

			::BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
			HPEN hptemp = (HPEN)SelectObject(hdc,hp );
			SelectObject(hdc,GetStockObject(NULL_BRUSH) );
			Rectangle(hdc,(m_XMin - CMyFliter::m_Start.x) ,\
				(m_YMin - CMyFliter::m_Start.y) ,\
				(m_XMax - CMyFliter::m_Start.x) ,\
				(m_YMax - CMyFliter::m_Start.y) );
			DeleteObject(SelectObject(hdc,hptemp) );
			::ReleaseDC( m_pView->GetHWND(),hdc);


			DeleteObject(hp);

		}
	}
	else if(m_State == MOVING)
	{
		if(  (wParam & MK_LBUTTON) || (wParam & MK_RBUTTON) )
		{

			int offX = pointNow.x - m_PointStart.x;
			int offY = pointNow.y - m_PointStart.y;


			HRGN hr = ::CreateRectRgn(m_XMin + offX,m_YMin + offY,m_XMax + offX,m_YMax + offY);

			HRGN hrTemp = ::CreateRectRgn(0,0,m_rectTrue.right,m_rectTrue.bottom);

			BitBlt(m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,m_pDoc->GetLastDC(),0,0,SRCCOPY);
			FillRgn(m_dcMem,m_RgnFormer,m_Brush );
			SelectObject(m_dcMem,hr);
			BitBlt(m_dcMem, m_XMin + offX ,m_YMin + offY,m_XMax-m_XMin,m_YMax-m_YMin,m_hBitDc,0,0,SRCCOPY);
			SelectObject(m_dcMem,hrTemp);

			HDC hdc = ::GetDC(m_pView->GetHWND() );	
			SetMapMode(hdc,MM_ANISOTROPIC );
			SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
			BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
			::ReleaseDC( m_pView->GetHWND(),hdc);
			DeleteObject(hr);


		}
	}

}
/*
void CMyFliterZoom::OnMouseMove(WPARAM wParam,LPARAM lParam) 
{
if(m_SelectItem != 0)
return ;
POINT pointNow = {LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
BitBlt(m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,m_pDoc->GetLastDC(),CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
HPEN hptemp = (HPEN)SelectObject(m_dcMem,CreatePen(PS_SOLID,2,RGB(0,0,0) )  );

if(pointNow.x <=100 || pointNow.y <=60)
return ;

Rectangle(m_dcMem,pointNow.x - 100,pointNow.y - 60,pointNow.x + 100,pointNow.y + 60);
HDC hdc = ::GetDC(m_pView->GetHWND() );	
BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
::ReleaseDC( m_pView->GetHWND(),hdc);

DeleteObject( SelectObject(m_dcMem,hptemp) );



};

void CMyFliterZoom::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
POINT pointNow = {LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };

if(pointNow.x <=100 )
pointNow.x  = 100;
if( pointNow.y <=60)
pointNow.y = 60;
POINT point = {pointNow.x - 100,pointNow.y -60};
OnIndexChangeOfTool(1);

BitBlt(m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,m_pDoc->GetLastDC(),0,0,SRCCOPY);

m_pFrameWnd->SetSrollBarH(point);
m_pFrameWnd->SetSrollBarV(point);

HDC hdc = ::GetDC(m_pView->GetHWND() );	
SetMapMode(hdc,MM_ANISOTROPIC );
SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
ReleaseDC(m_pView->GetHWND(),hdc);



}*/

void CMyFliterGun::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	bIsTimer =false;
	
}



void CMyFliterGun::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = { LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y };
	pointStart.x = pointNow.x;
	pointStart.y = pointNow.y;
	bIsTimer = true;
}



void CMyFliterGun::OnMouseMove(WPARAM wParam,LPARAM lParam)
{
	POINT pointNow = { LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y };
	pointStart.x = pointNow.x;
	pointStart.y = pointNow.y;
	if(wParam & MK_LBUTTON)
	{
		HDC hdc = GetDC(m_pView->GetHWND() );
		BitBlt(hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
		ReleaseDC(m_pView->GetHWND(),hdc);

	}

}

void CMyFliterGun::OnTimer()
{
	HWND wndView = ((CMyFrameWnd*)g_App.GetCWnd())->GetView() ->GetHWND();
	HDC hdc = GetDC(wndView);
	POINT start = {10,10};
	Air_Brush(m_dcMem,pointStart,10,10,RGB(0,0,0) );
	
		BitBlt( hdc,0,0,m_pView->GetRect().right,m_pView->GetRect().bottom,m_dcMem,0,0,SRCCOPY);
	ReleaseDC(wndView,hdc);
}

void CMyFliterGun::Air_Brush(HDC hdc, POINT start, int diameter, int density, COLORREF color)
{
	POINT random[100];
	POINT plus_minus;
	for(int i = 0; i < density; i++)
	{
		random[i].x = rand() % diameter;
		random[i].y = rand() % diameter;
		plus_minus.x = rand() % 2;
		plus_minus.y = rand() % 2;
		if(plus_minus.x == 0)
			random[i].x = -random[i].x;
		if(plus_minus.y == 0)
			random[i].y = -random[i].y;
		if (random[i].x*random[i].x + random[i].y*random[i].y < diameter*diameter)
		{
			SetPixel (hdc, start.x + random[i].x, start.y + random[i].y, color) ;
		}
	}
}