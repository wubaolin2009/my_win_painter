/*074090 Îâ±¦ÁÖ*/
#include"CMyView.h"
#include "CMyFliter.h"
#include"CMyFrameWnd.h"

DWORD CMyView::ms_ID = 0xF0000000;



void CMyView::PreCreateWindow()
{
	CMyWnd::PreCreateWindow();
	m_wndClassex.style			= CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW ;
//	m_wndClassex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	m_wndClassex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
}
	
void CMyView::CreateEx()
{
	CMyWnd::CreateEx();
	m_createStruct.dwExStyle= WS_EX_LEFT | WS_EX_LTRREADING;
	m_createStruct.style = WS_CHILD | WS_VISIBLE   ;
	m_createStruct.hMenu = (HMENU) ms_ID++;
}

LRESULT CMyView::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BEGIN_MY_MESSAGE
		MY_MESSAGE(WM_MOUSEMOVE,OnMouseMove)
		MY_MESSAGE(WM_LBUTTONDOWN,OnLButtonDown)
		MY_MESSAGE(WM_RBUTTONDOWN,OnRButtonDown)
		MY_MESSAGE(WM_LBUTTONUP,OnLButtonUp)
		MY_MESSAGE(WM_RBUTTONUP,OnRButtonUp)
		MY_MESSAGE(WM_PAINT,OnPaint)
		MY_MESSAGE(WM_LBUTTONDBLCLK,OnLDBCLICK)
		MY_MESSAGE(WM_RBUTTONDBLCLK,OnRDBCLICK)
	END_MY_MESSAGE
}



LRESULT CMyView::OnMouseMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	//Set the cursor when you want to resize the window
	/*if( !(wParam & MK_LBUTTON) && !(wParam & MK_LBUTTON) )
	{
	//	RECT rect = m_rectWnd.GetRect();
		RECT rect = {0,0,m_rectTrue.right,m_rectTrue.bottom};

		RECT rectBottom = {rect.right/2 - 3,rect.bottom-3,rect.right/2+3,rect.bottom+3 };

		RECT rectRight = {rect.right -3,rect.bottom/2 -3,rect.right + 3,rect.bottom/2 + 3};

		RECT rectRB   = {rect.right -3,rect.bottom -3,rect.right+3,rect.bottom+3};

		POINT pointNow = {LOWORD(lParam),HIWORD(lParam) };

		if(::PtInRect(&rectBottom,pointNow) )
		{
			m_status = MOVINGY;
			SetCursor( LoadCursor(NULL,IDC_SIZENS) );
		}
		else if(::PtInRect(&rectRight,pointNow) )
		{
			m_status = MOVINGX;
			SetCursor( LoadCursor(NULL,IDC_SIZEWE) );
		}
		else if(::PtInRect(&rectRB,pointNow) )
		{
			m_status = MOVINGXY;
			SetCursor( LoadCursor(NULL,IDC_SIZENWSE) );

		}
		else
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW) );
			m_status = DRAWING;
		}

	}

	if( (wParam & MK_LBUTTON)&m_status!= DRAWING)
	{
		RECT rect;
		if(m_status == MOVINGXY)
		{
			 rect.left = 0;
			 rect.top = 0;
			 rect.right = LOWORD(lParam);
			 rect.bottom = HIWORD(lParam);			
		}
		else if(m_status == MOVINGX)
		{
			GetClientRect(hWnd,&rect);
			rect.right = LOWORD(lParam);

		}
		else if(m_status == MOVINGY)
		{
			GetClientRect(hWnd,&rect);
			rect.bottom = HIWORD(lParam);
		}

		((CMyFrameWnd*)m_Parent)->DrawRect(rect);
	}*/

	//if( m_status == DRAWING)
	m_pFliter->OnMouseMove(wParam,lParam);
	return 0;
}

LRESULT CMyView::OnLDBCLICK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pFliter->OnLButtonDBCLICK(wParam,lParam);
	return 0;
}

LRESULT CMyView::OnRDBCLICK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pFliter->OnRButtonDBCLICK(wParam,lParam);
	return 0;
}







LRESULT CMyView::OnLButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	::SetCapture(hWnd);
	m_pFliter->OnLButtonDown(wParam,lParam);
	((CMyFrameWnd*)m_Parent )->SetModified(true);
	return 0;
}
	
LRESULT CMyView::OnRButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	::SetCapture(hWnd);
	m_pFliter->OnRButtonDown(wParam,lParam);
	return 0;
}

	
LRESULT CMyView::OnLButtonUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	if( m_status!= DRAWING)
	{
		m_status = DRAWING;
		SetCursor(LoadCursor(NULL,IDC_ARROW) );
	}
	m_pFliter->OnLButtonUp(wParam,lParam);
	return 0;
}

	
LRESULT CMyView::OnRButtonUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	m_pFliter->OnRButtonUp(wParam,lParam);
	return 0;
}	

LRESULT CMyView::OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	m_pFliter->OnPaint(hWnd,message,wParam,lParam);
	//Draw the Rectangle of drawing
	HDC hdc = 	BeginPaint(hWnd,&ps);
	//RECT rect = m_rectWnd.GetRect();
	
	EndPaint(hWnd,&ps);
	return 0;
}