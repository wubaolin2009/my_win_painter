/*074090 Œ‚±¶¡÷*/
#include "CMyToolBar.h"
#include "CMyApp.h"
#include "CMyFliter.h"
#include "CMyFrameWnd.h"
#include<windows.h>
#include <commctrl.h>

DWORD CMyToolBar::ms_ID = 0x80000000;
extern CMyApp g_App;

void CMyToolBar::PreCreateWindow()
{
	CMyWnd::PreCreateWindow();
	m_wndClassex.hbrBackground = (HBRUSH)::CreateSolidBrush(GetSysColor(COLOR_BTNFACE));

}

void CMyToolBar::CreateEx()
{
	CMyWnd::CreateEx();
	m_createStruct.style = WS_CHILDWINDOW | WS_VISIBLE |WS_BORDER | WS_CLIPSIBLINGS;
	m_createStruct.hMenu = (HMENU)ms_ID;
	
}

LRESULT CMyToolBar::OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) //overide
{
	//Prepare for the m_buttonInfo
	CMyWnd::OnCreate(hWnd,message,wParam,lParam);
	int iButtonNum = m_Line*m_Col;
	for(int i = 0;i<iButtonNum;i++)
	{
		m_buttonInfo[i].iBitmap=i;
		m_buttonInfo[i].idCommand=i;   
		m_buttonInfo[i].fsState=TBSTATE_ENABLED;   
		m_buttonInfo[i].fsStyle=  m_style ;  
		if(i % m_Col == 1)
			m_buttonInfo[i].fsState |= TBSTATE_WRAP;
		m_buttonInfo[i].dwData=0;   
		m_buttonInfo[i].iString=0;  
	}
	
	m_wndTrue = CreateToolBar(g_App.GetInst(),m_hWnd,m_bmpId,m_Line*m_Col,m_buttonInfo);
	return 0;
}


HWND  CMyToolBar::CreateToolBar(HINSTANCE   hIns,  HWND   hParent,   int   BmpID,   int   nBtn,TBBUTTON*   Btn)   
{   
	HWND   hToolBar;   
	BITMAP   Bmp;   
	HBITMAP   hBitmap;   
	TBADDBITMAP   tbab;   
	 
	//InitCommonControls();   
	hToolBar   =   ::CreateWindowEx(WS_EX_TOOLWINDOW,TOOLBARCLASSNAMEW,
		NULL, WS_CHILD| WS_VISIBLE | TBSTYLE_FLAT ,m_rectWnd.left,   
		m_rectWnd.top,m_rectWnd.right - m_rectWnd.left , m_rectWnd.bottom - m_rectWnd.top, hParent,  (HMENU)ms_ID++,   hIns,   NULL); 
	
	SendMessage(hToolBar,   TB_BUTTONSTRUCTSIZE,   
		(WPARAM)   sizeof(TBBUTTON),   0);   
	hBitmap   =   LoadBitmap(hIns,   MAKEINTRESOURCE(BmpID));   
	GetObject(hBitmap,   sizeof(BITMAP),   &Bmp);   
	SendMessage(hToolBar,   TB_SETBITMAPSIZE,   0,   
		MAKELONG(26,26));   
	tbab.hInst   =   NULL;   
	tbab.nID       =   (UINT)hBitmap;   
	SendMessage(hToolBar,   TB_ADDBITMAP,   nBtn,   
		(LPARAM)&tbab);   

	
	SendMessage(hToolBar,   TB_ADDBUTTONS,   nBtn,   
		(LPARAM)Btn);   
	SendMessage(hToolBar,   TB_AUTOSIZE,   0,   0);   

	ShowWindow(hToolBar,   SW_SHOW);  
	SendMessage(hToolBar,WM_SIZE,0,0);

	return   hToolBar;   
}




LRESULT CMyToolBarWithList::OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) //overide
{	
	CMyToolBar::OnCreate(hWnd,message,wParam,lParam);  //Create ToolBar first
	//Create a combobox
	m_Listbox.Create();
	m_wndList = m_Listbox.GetHWND();
	SendMessage(m_wndTrue,TB_CHECKBUTTON,(int)TOOL_PENCIL,TRUE);

	return 0;
}

LRESULT CMyToolBarWithList::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BEGIN_MY_MESSAGE
		MY_MESSAGE(WM_DRAWITEM,OnDrawItem)
		MY_MESSAGE(WM_COMMAND, OnCommand)
	END_MY_MESSAGE
}


void  CMyToolBarWithList:: SetSelect(int tool)
{
	if(m_toolSelect == tool)
		return ;
	
	::SendMessage(m_wndTrue,TB_PRESSBUTTON,tool,MAKELONG(TRUE,0));
	bool t = ::SendMessage(m_wndTrue,TB_PRESSBUTTON,m_toolSelect,MAKELONG(FALSE,0));
	OnCommand(m_hWnd,WM_COMMAND,(WPARAM)tool,(LPARAM)m_wndTrue);
}

LRESULT CMyToolBarWithList::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool bIsZoom = false;
	if(m_wndTrue == (HWND)lParam)  //Message From ToolBar
	{
	CMyToolBarWithList::ToolStyle styleFormer = m_toolSelect;
	if(m_toolSelect == CMyToolBarWithList::ToolStyle::TOOL_ZOOM )
		bIsZoom = true;
	m_toolSelect = (ToolStyle)wParam;  //Get CurrentSelected
	//SendMessage(m_wndList,WM_LBUTTONDOWN,0,0);  //Draw the listbox
	
	SendMessage(m_wndList,LB_RESETCONTENT,0,0);

	int    select;
	if(CMyFliter::m_zoom == 1)
		select = 0;
	else if(CMyFliter::m_zoom == 2)
		select = 1;
	else if(CMyFliter::m_zoom == 6)
		select = 4;
	else
		select = 3;
	
	switch(m_toolSelect)
	{	
	case TOOL_GUN:
		SendMessage(m_wndList,LB_ADDSTRING,0,0);
		SendMessage(m_wndList,LB_ADDSTRING,0,1);
		SendMessage(m_wndList,LB_ADDSTRING,0,2);
		SendMessage(m_wndList,LB_SETITEMHEIGHT, 0,16);
		SendMessage(m_wndList,LB_SETCURSEL,select,0);
		break;

	case TOOL_ERASER:
	case TOOL_ZOOM: 
		SendMessage(m_wndList,LB_ADDSTRING,0,0);
		SendMessage(m_wndList,LB_ADDSTRING,0,1);
		SendMessage(m_wndList,LB_ADDSTRING,0,2);
		SendMessage(m_wndList,LB_ADDSTRING,0,3);
		if(!bIsZoom)
			SendMessage(m_wndList,LB_SETCURSEL,0,0);
		if( m_toolSelect == TOOL_ZOOM)
			SendMessage(m_wndList,LB_SETCURSEL,select,0);

		SendMessage(m_wndList,LB_SETITEMHEIGHT, 0,16);
		break;

	case TOOL_CURL:
	case TOOL_BIN:
	case TOOL_GET:
	case TOOL_PENCIL: 
	case TOOL_BRUSH:
	
	case TOOL_WORD:
	case TOOL_CUT_1:
	case TOOL_CUT_0:

		break;
	
	case TOOL_LINE:
		SendMessage(m_wndList,LB_ADDSTRING,0,0);
		SendMessage(m_wndList,LB_ADDSTRING,0,1);
		SendMessage(m_wndList,LB_ADDSTRING,0,2);
		SendMessage(m_wndList,LB_ADDSTRING,0,3);
		SendMessage(m_wndList,LB_ADDSTRING,0,4);
		if(!bIsZoom)
		SendMessage(m_wndList,LB_SETCURSEL,0,0);
		SendMessage(m_wndList,LB_SETITEMHEIGHT, 0,12);
		break;

			
	case TOOL_RECT:
	case TOOL_POL:
	case TOOL_ELLIPSE:
	case TOOL_CIRCLERECT:

		SendMessage(m_wndList,LB_ADDSTRING,0,0);
		SendMessage(m_wndList,LB_ADDSTRING,0,1);
		SendMessage(m_wndList,LB_ADDSTRING,0,2);
		if(!bIsZoom)
		SendMessage(m_wndList,LB_SETCURSEL,0,0);
		SendMessage(m_wndList,LB_SETITEMHEIGHT, 0,18);

		break;
	}

	//tell the cFliter that has changed
	( (CMyFrameWnd*)m_Parent )->OnToolChange(m_toolSelect,styleFormer);
	if( m_toolSelect != TOOL_ZOOM)
		( (CMyFrameWnd*)m_Parent )->OnIndexChange(0);
	if(bIsZoom)
	{
		SendMessage(m_wndList,LB_SETCURSEL,0,0);
	}
	}
	
	return 0;
}



LRESULT CMyToolBarWithList::OnDrawItem(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  //draw the listbox
{
	static HBRUSH hbGray = ::CreateSolidBrush( GetSysColor(COLOR_BTNFACE) );
	static RECT   rect   =  {0,0,60,80};
	LPDRAWITEMSTRUCT drawItem = NULL;   //DrawItem
	drawItem = (DRAWITEMSTRUCT*)lParam;
	int current = SendMessage(m_wndList,LB_GETCURSEL,0,0);
	if(current != m_current)
		((CMyFrameWnd*)m_Parent )->OnIndexChange(current);
	m_current = current;
	FillRect(drawItem->hDC,&rect,hbGray);

	TCHAR ttt[100];
	wsprintf(ttt,TEXT("%d"),m_current);
	
	switch(m_toolSelect)
	{
	case TOOL_GUN:
		DrawGun(drawItem->hDC);
		break;
	case TOOL_ERASER:
		DrawEraser(drawItem->hDC);
		break;
	
	case TOOL_ZOOM: 
		DrawZoom(drawItem->hDC);
		break;

	case TOOL_BRUSH:
		DrawBrush(drawItem->hDC);
		break;
	
	case TOOL_LINE:
		DrawLine(drawItem->hDC);
		break;
	

	case TOOL_RECT:
	case TOOL_POL:
	case TOOL_ELLIPSE:
	case TOOL_CIRCLERECT:
		DrawRect(drawItem->hDC);
		break;

	}	
	return 0;
}


void CMyToolBarWithList::DrawEraser(HDC hdc)
{
	//There'are 4 erasers

	int top;  //top of each Rect
	int left = 24;
	int right = 28 ;
	int bottom;
	for(int i = 0;i<4;i++)
	{
		top = 4*(i+1) +12*i + (4 -i);
		bottom = top + 12 - (4-i);
		left --;
		right ++;
		Rectangle(hdc,left,top,right,bottom);
	}

	//restore the biggest rect
	top = 4*(m_current+1) +12*m_current;
	bottom = top + 12;
	left = 20;
	bottom = top + 12;
	
/*Selected Rect */
	RECT rectFocus = {left,top,right,bottom};
	InvertRect(hdc,&rectFocus);
	

}


void CMyToolBarWithList::DrawZoom(HDC hdc)
{
	TCHAR sz1[] = TEXT("1x");
	TCHAR sz2[] = TEXT("2x");
	TCHAR sz3[] = TEXT("6x");
	TCHAR sz4[] = TEXT("8x");

	RECT rectFocus = {0,m_current*16,60,m_current*16 + 16};

	TextOut(hdc,20,0,sz1,2);
	TextOut(hdc,20,16,sz2,2);
	TextOut(hdc,20,32,sz3,2);
	TextOut(hdc,20,48,sz4,2);

	/*Selected Rect*/

	InvertRect(hdc,&rectFocus);
}


void CMyToolBarWithList::DrawGun(HDC hdc)
{
	TCHAR sz1[] = TEXT("1x");
	TCHAR sz2[] = TEXT("2x");
	TCHAR sz3[] = TEXT("3x");


	RECT rectFocus = {0,m_current*16,60,m_current*16 + 16};

	TextOut(hdc,20,0,sz1,2);
	TextOut(hdc,20,16,sz2,2);
	TextOut(hdc,20,32,sz3,2);

	/*Selected Rect*/

	InvertRect(hdc,&rectFocus);
}

void CMyToolBarWithList::DrawBrush(HDC hdc)
{
	
}


void CMyToolBarWithList::DrawLine(HDC hdc)
{
	int iLineWide = m_current*2 + 2;   //LineWidth		
	HBRUSH hrush;
   
	RECT rectFocus = {0,m_current*12,60,m_current*12 + 12};

	int pos;  // Calculate the position of each line

	hrush = (HBRUSH) SelectObject(hdc,GetStockObject(BLACK_BRUSH));
	for(int i = 0;i<5;i++)
	{
		pos = 12*i + (12 - (i+1))/2 ;
	  	::Rectangle(hdc,5,pos ,53,pos + i +1);
	}	

	/*Selected Rect*/

	InvertRect(hdc,&rectFocus);

	SelectObject(hdc,hrush);
}

void CMyToolBarWithList::DrawRect(HDC hdc)
{
		//There'are 3 rect
	HBRUSH hb;
	HPEN   hp;  //backup 
	int top;  //top of each Rect
	int left = 10;
	int right = 10 + 38;
	int bottom;
	RECT rectFocus = {0,m_current*18,60,m_current*18 + 18};
	
/*1st Rect*/
	top = 4*1 ;
	bottom = top + 12;
	hb = (HBRUSH)SelectObject(hdc,::GetStockObject(NULL_BRUSH) );
	Rectangle(hdc,left,top,right,bottom);

/*2nd Rect*/
	 SelectObject(hdc,GetStockObject(GRAY_BRUSH) );
	top = 4*2 +12*1 ;
	bottom = top + 12;
	Rectangle(hdc,left,top,right,bottom);


/*3rd Rect*/
	hp = (HPEN)SelectObject(hdc,GetStockObject(NULL_PEN) );
	top = 4*3 +12*2 +2;
	bottom = top + 12;
	Rectangle(hdc,left,top,right,bottom);

/*Selected Rect*/

	InvertRect(hdc,&rectFocus);
	
/*Restore dc status*/
	SelectObject(hdc,hp);
	SelectObject(hdc,hb);
}







