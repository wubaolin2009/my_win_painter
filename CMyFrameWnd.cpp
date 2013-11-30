/*074090 吴宝林*/
#include"CMyFrameWnd.h"
#include"CMyView.h"
#include"CMyToolBar.h"
#include "CMyWnd.h"
#include "CMyDoc.h"
#include"CMyApp.h"
#include "define.h"
#include"CMyFliter.h"
#include "resource.h"
#include"resource.h"
#include<windows.h>

extern CMyApp g_App;


CMyFrameWnd::CMyFrameWnd() :CMyWnd(true,TEXT("RI"),CMyRect(0,0,1024,768) ) ,\
		m_toolbar(CMyRect(0,1,66,670),BTNS_CHECKGROUP  ,IDB_TOOLBAR,8,2),\
		m_colorbar(CMyRect(0,670,1024,718) )
{
	::ZeroMemory(m_FileName,sizeof(TCHAR) * MAX_FILE_NAME);
	m_toolbar.SetParent(this);
	m_colorbar.SetParent(this);
	m_pView = new CMyView(CMyRect(68,3,708,483));
	m_pView ->SetParent(this);
	m_pDoc = new CMyDoc(m_pView);


	/*Initialize all the fliters*/
	m_Fliters[0] = new CMyFliterCut0(this,m_pView,m_pDoc);
	m_Fliters[1] = new CMyFliterCut1(this,m_pView,m_pDoc);
	m_Fliters[2] = new CMyFliterEraser(this,m_pView,m_pDoc);
	m_Fliters[3] = new CMyFliterBin(this,m_pView,m_pDoc);
	m_Fliters[4] = new CMyFliterGet(this,m_pView,m_pDoc);
	m_Fliters[5] = new CMyFliterZoom(this,m_pView,m_pDoc);
	m_Fliters[6] = new CMyFliterPen(this,m_pView,m_pDoc);
	m_Fliters[7] = new CMyFliterBrush(this,m_pView,m_pDoc);
	m_Fliters[8] = new CMyFliterGun(this,m_pView,m_pDoc);
	m_Fliters[9] = new CMyFliterWord(this,m_pView,m_pDoc);
	m_Fliters[10] = new CMyFliterLine(this,m_pView,m_pDoc);
	m_Fliters[11] = new CMyFliterCurl(this,m_pView , m_pDoc);
	m_Fliters[12] = new CMyFliterRect(this,m_pView,m_pDoc);
	m_Fliters[13] = new CMyFliterPol(this,m_pView,m_pDoc);
	m_Fliters[14] = new CMyFliterEllipse(this,m_pView,m_pDoc);
	m_Fliters[15] = new CMyFliterRoundRect(this,m_pView,m_pDoc);
	m_FliterNow = m_Fliters[6];
	m_pView ->SetFliter(m_FliterNow);		

	m_rectTrue.left = 0;
	m_rectTrue.right = 640;
	m_rectTrue.top = 0;
	m_rectTrue.bottom = 480;
};

void CMyFrameWnd::OnTimer()
{
	if(m_FliterNow)
		m_FliterNow->OnTimer();
}


void CMyFrameWnd::SetViewToolBar()
{



	int zoom = CMyFliter::m_zoom;
	RECT rect	= m_pView->GetRect();
	rect.bottom= zoom * (rect.bottom - rect.top) + rect.top;
	rect.right= zoom * (rect.right - rect.left) + rect.top;

	RECT rectFrame;
    POINT pointNew;
	rectFrame = GetRect();
	pointNew.x = rect.right;
	pointNew.y = rect.bottom;
	
	if( zoom * CMyFliter::m_rectTrue.right <= (rectFrame.right - 68 - 20)   )
	{		
		ShowWindow(m_hScrollH,SW_HIDE);		//m_hScrollV
		pointNew.x = CMyFliter::m_rectTrue.right;
		CMyFliter::m_Start.x = 0;

	}

	if( zoom * CMyFliter::m_rectTrue.right > (rectFrame.right - 68 - 20) )   //ShowRightBar
	{
		SCROLLINFO scr;
		ZeroMemory(&scr,sizeof(scr) );
		scr.cbSize = sizeof(scr);
		scr.nPos = 0;
		scr.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		scr.nMax = 5 *zoom * CMyFliter::m_rectTrue.right /( rectFrame.right - 68);
		scr.nPage = 1;
		SetScrollInfo(m_hScrollH,SB_CTL,&scr,TRUE);
		pointNew.x = rectFrame.right - 68 - 25;
		
		MoveWindow(m_hScrollH,68,m_rectWnd.bottom - 68 - 48,m_rectWnd.right - 20 - 70 ,20,true);
		ShowWindow(m_hScrollH,SW_SHOW);		//m_hScrollV
		UpdateWindow(m_hScrollH);
	}



	if( zoom * CMyFliter::m_rectTrue.bottom < rectFrame.bottom - 100 ) //Hide BottomBar
	{	
		ShowWindow(m_hScrollV,SW_HIDE);		//m_hScrollV
		pointNew.y = CMyFliter::m_rectTrue.bottom;	
		CMyFliter::m_Start.y = 0;
	}
	

	if( zoom * CMyFliter::m_rectTrue.bottom > rectFrame.bottom - 100 ) //Show BottomBar
	{
		SCROLLINFO scr;
		ZeroMemory(&scr,sizeof(scr) );
		scr.cbSize = sizeof(scr);
		scr.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		scr.nMax = 5* zoom * CMyFliter::m_rectTrue.bottom /( rectFrame.bottom - 100);
		
		scr.nPage = 1 ;
		SetScrollInfo(m_hScrollV,SB_CTL,&scr,TRUE);
		pointNew.y = rectFrame.bottom - 70 - 50;
		MoveWindow(m_hScrollV,rectFrame.right - 25,0,20,rectFrame.bottom - 68 - 35 ,true);
		ShowWindow(m_hScrollV,SW_SHOW);		//m_hScrollV
		UpdateWindow(m_hScrollV);
	}


	SetWindowPos(m_pView->GetHWND() , NULL,68,3,pointNew.x,pointNew.y,SWP_NOZORDER);
	RECT rectView = {0,0,pointNew.x,pointNew.y};
	m_pView->SetRect(rectView);
	m_rectTrue.left = 0;
	m_rectTrue.right = pointNew.x;
	m_rectTrue.top = 0;
	m_rectTrue.bottom = pointNew.y;
}


void CMyFrameWnd::DrawRect(const RECT& rect)
{
	static RECT rectFormer = {0 + 67 + 5,0+50,640 + 67 + 5,480+50};

	//PointStart 68 3
//	HDC hdc = GetDC(m_hWnd);
	HDC hdc = GetWindowDC(m_hWnd);

	RECT rectDraw = {rect.left + 67 + 5,rect.top + 50 ,rect.right + 67 +5,rect.bottom + 50};

	
	//HPEN hp = (HPEN)SelectObject(hdc,CreatePen(PS_DOT,1,RGB(0,0,0) ) );

	SelectObject(hdc,CreatePen(PS_SOLID,1,RGB(128,128,128) )  );
	SelectObject(hdc,GetStockObject(NULL_BRUSH) );

	Rectangle(hdc,rectFormer.left,rectFormer.top,rectFormer.right,rectFormer.bottom);

	SelectObject(hdc,CreatePen(PS_DOT,1,RGB(0,0,0) )  );

	
	Rectangle(hdc,rectDraw.left,rectDraw.top,rectDraw.right,rectDraw.bottom);

	rectFormer.left = rectDraw.left;
	rectFormer.top = rectDraw.top;
	rectFormer.right = rectDraw.right;
	rectFormer.bottom = rectDraw.bottom;


	ReleaseDC(m_hWnd,hdc);
	
	UpdateWindow(m_hWnd);

}




void CMyFrameWnd::PreCreateWindow()
{
	CMyWnd::PreCreateWindow();
	m_wndClassex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
}


LRESULT CMyFrameWnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BEGIN_MY_MESSAGE
		MY_MESSAGE(WM_DESTROY,OnDestroy)
		MY_MESSAGE(WM_SIZE,OnSize)
		MY_MESSAGE(WM_COMMAND,OnCommand)
		MY_MESSAGE(WM_INITMENUPOPUP,OnMenuPop)
		MY_MESSAGE(WM_VSCROLL,OnVScroll)
		MY_MESSAGE(WM_HSCROLL,OnHScroll)

	END_MY_MESSAGE
}


LRESULT CMyFrameWnd::OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_hWnd = hWnd;
	m_pView->Create();
	m_FliterNow->OnInit();
	m_toolbar.Create();
	m_colorbar.Create();

	m_hScrollH = CreateWindowEx(NULL,TEXT("SCROLLBAR"),NULL,WS_CHILD|SBS_HORZ,68,m_rectWnd.bottom - 118,m_rectWnd.right - 98, 20,hWnd,0,g_App.GetInst(),NULL);
	m_hScrollV = CreateWindowEx(NULL,TEXT("SCROLLBAR"),NULL,WS_CHILD|SBS_VERT,m_rectWnd.right - 20,m_rectWnd.top,20,m_rectWnd.bottom - 118,hWnd,(HMENU)0x11,g_App.GetInst(),NULL);

	

	return 0;
}
/*

void CMyFrameWnd::SetSrollBarH(POINT point)
{
	SCROLLINFO scr;
	 ZeroMemory(&scr,   sizeof(SCROLLINFO));   

	scr.cbSize = sizeof(SCROLLINFO);
	scr.fMask = SIF_POS | SIF_RANGE | SIF_TRACKPOS | SIF_PAGE  ;
	GetScrollInfo(m_hScrollH,SB_CTL,&scr);
	RECT rectTrue = CMyFliter::m_rectTrue;

	scr.nPos = point.x / rectTrue.right * scr.nMax;

	scr.fMask = SIF_POS;

	SetScrollInfo(m_hScrollH,SB_CTL,&scr,true);
	CMyFliter::m_Start.x = point.x;



}
void CMyFrameWnd::SetSrollBarV(POINT point)
{
	SCROLLINFO scr;
	ZeroMemory(&scr,   sizeof(SCROLLINFO));   

	scr.cbSize = sizeof(SCROLLINFO);
	scr.fMask = SIF_POS | SIF_RANGE | SIF_TRACKPOS | SIF_PAGE  ;
	GetScrollInfo(m_hScrollV,SB_CTL,&scr);
	RECT rectTrue = CMyFliter::m_rectTrue;
	
	scr.nPos = (double)point.y / rectTrue.bottom * scr.nMax;

	scr.fMask = SIF_POS;

	SetScrollInfo(m_hScrollV,SB_CTL,&scr,true);

	CMyFliter::m_Start.y = point.y;

	
}*/




LRESULT CMyFrameWnd::OnHScroll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SCROLLINFO scr;
	 ZeroMemory(&scr,   sizeof(SCROLLINFO));   

	scr.cbSize = sizeof(SCROLLINFO);
	scr.fMask = SIF_POS | SIF_RANGE | SIF_TRACKPOS | SIF_PAGE  ;
	GetScrollInfo(m_hScrollH,SB_CTL,&scr);


	RECT rectTrue = CMyFliter::m_rectTrue;
	
	switch(LOWORD(wParam))
	{
	case SB_LINELEFT:
//		scr.nPos -= scr.nPage/2;
//		CMyFliter::m_Start.x -= (rectTrue.right - rectTrue.left) * scr.nPage/2/scr.nMax;
//		break;
	case SB_PAGELEFT:
		scr.nPos -= scr.nPage;
		if(scr.nPos <= 0)
			scr.nPos = 0;
		else
			CMyFliter::m_Start.x= ABS(( (rectTrue.right - rectTrue.left) - (m_pView->GetRect().right - m_pView->GetRect().left) )) * scr.nPos/scr.nMax ;
		break;


	case SB_LINERIGHT:
//		scr.nPos += scr.nPage/2;
//		CMyFliter::m_Start.x += (rectTrue.right - rectTrue.left) * scr.nPage/2/scr.nMax;

	//	scr.nPos = (scr.nPos==scr.nMax)?scr.nMax:scr.nPos+1;
//		break;
	case SB_PAGERIGHT:
		scr.nPos += scr.nPage;
		if(scr.nPos>= scr.nMax)
			scr.nPos = scr.nMax;
		else
			CMyFliter::m_Start.x = ABS(( (rectTrue.right - rectTrue.left) - (m_pView->GetRect().right - m_pView->GetRect().left) ) )* scr.nPos/scr.nMax ;
	
		break;
	
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		
		scr.nPos = scr.nTrackPos;
		CMyFliter::m_Start.x = ABS(( (rectTrue.right - rectTrue.left) - (m_pView->GetRect().right - m_pView->GetRect().left) ) )* scr.nPos/scr.nMax ;
		break;
	}
	scr.fMask = SIF_POS;

	SetScrollInfo(m_hScrollH,SB_CTL,&scr,true);

	HDC hdc = GetDC(m_pView->GetHWND() );
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	BitBlt(hdc,rectTrue.left,rectTrue.top,rectTrue.right,rectTrue.bottom,CMyFliter::m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);

	ReleaseDC(m_pView->GetHWND(),hdc);


	return 1;
}
LRESULT CMyFrameWnd::OnVScroll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	SCROLLINFO scr;
	ZeroMemory(&scr,   sizeof(SCROLLINFO));   

	scr.cbSize = sizeof(SCROLLINFO);
	scr.fMask = SIF_POS | SIF_RANGE | SIF_TRACKPOS | SIF_PAGE  ;
	GetScrollInfo(m_hScrollV,SB_CTL,&scr);
	RECT rectTrue = CMyFliter::m_rectTrue;
	switch(LOWORD(wParam) )
	{
	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		scr.nPos += scr.nPage;
		if(scr.nPos>= scr.nMax)
			scr.nPos = scr.nMax;
		else
			CMyFliter::m_Start.y = ABS((LONG)( (rectTrue.bottom - rectTrue.top) - (m_pView->GetRect().bottom - m_pView->GetRect().top) )) * (double)scr.nPos/(double)scr.nMax ;
		break;


	
	case SB_PAGEUP:
	case SB_LINEUP:
		scr.nPos -= scr.nPage;
		if(scr.nPos <= 0)
			scr.nPos = 0;
		else
			CMyFliter::m_Start.y = ABS(( (rectTrue.bottom - rectTrue.top) - (m_pView->GetRect().bottom - m_pView->GetRect().top) )) * scr.nPos/scr.nMax ;
		break;






	case SB_THUMBPOSITION:

	case SB_THUMBTRACK:
		scr.nPos = scr.nTrackPos;
		CMyFliter::m_Start.y =  ABS(( (rectTrue.bottom - rectTrue.top) - (m_pView->GetRect().bottom - m_pView->GetRect().top) ) )* scr.nPos/scr.nMax ;


		break;
	}

	scr.fMask = SIF_POS;
	SetScrollInfo(m_hScrollV,SB_CTL,&scr,true);
	HDC hdc = GetDC(m_pView->GetHWND() );
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	BitBlt(hdc,rectTrue.left,rectTrue.top,rectTrue.right,rectTrue.bottom,CMyFliter::m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	ReleaseDC(m_pView->GetHWND(),hdc);

	return 0;
}


LRESULT CMyFrameWnd::OnMenuPop(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD wmID = LOWORD(lParam);
	if(wmID == 1) //Edit
	{
	
		::EnableMenuItem((HMENU)wParam,IDM_EDIT_UNDO,(m_pDoc->bCanUndo() && m_FliterNow -> CanUndo())?MF_ENABLED:MF_GRAYED);
		::EnableMenuItem((HMENU)wParam,IDM_EDIT_REDO,m_pDoc->bCanRedo()?MF_ENABLED:MF_GRAYED);
			
		int index = m_toolbar.GetIndex();
		if( index == 0 || index ==1)
		{
			::EnableMenuItem((HMENU)wParam,IDM_EDIT_CUT,MF_ENABLED);
			::EnableMenuItem((HMENU)wParam,IDM_EDIT_COPY,MF_ENABLED);
		}
		else
		{
			::EnableMenuItem((HMENU)wParam,IDM_EDIT_CUT,MF_GRAYED);
			::EnableMenuItem((HMENU)wParam,IDM_EDIT_COPY,MF_GRAYED);
		}

		OpenClipboard(NULL);
		
		if(!IsClipboardFormatAvailable(CF_BITMAP))
			::EnableMenuItem((HMENU)wParam,IDM_EDIT_PASTE,MF_GRAYED);
		else
			::EnableMenuItem((HMENU)wParam,IDM_EDIT_PASTE,MF_ENABLED);

		CloseClipboard();

			
	}

	

	return 0;
}

LRESULT CMyFrameWnd::OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}


LRESULT CMyFrameWnd::OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_rectWnd.left = 0;
	m_rectWnd.top = 0;
	m_rectWnd.right= LOWORD(lParam);
	m_rectWnd.bottom= HIWORD(lParam);
	::SetWindowPos(m_colorbar.GetHWND(),hWnd,0,m_rectWnd.bottom - 48,m_rectWnd.right,48,SWP_NOZORDER);
	::SetWindowPos(m_toolbar.GetHWND(),hWnd,0,1,66,m_rectWnd.bottom - 60,SWP_NOZORDER);

	SetViewToolBar();
	//SetTrueClient(m_rectTrue);
	//::ShowWindow(m_hScrollV,SW_SHOW);


	return 0;
}
/*
LRESULT CMyFrameWnd::OnNCTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}*/


LRESULT CMyFrameWnd::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmID = LOWORD(wParam);
	TCHAR szFile[260];       // buffer for file name
	OPENFILENAME ofn;       // common dialog box structure
	RECT rectTo ;          //Rect to operate
	int SelectFlip;         //Select of Flip
	//Select of Zoom
	int xZoom = 1;
	int yZoom = 1;
	int xV    = 0;
	int xH    = 0;
	TCHAR szWindow [] = TEXT("保存");
	TCHAR szMessage[] = TEXT("文件没有保存，是否继续？");

	RECT rectLater;
	
	switch(wmID)	
	{	
	case IDM_FILE_NEW:
		/*if(m_bModified)
		{
			if(MessageBox(m_hWnd,szMessage,szWindow,MB_YESNO) == IDNO)
				break;
		}
		*/
		m_FliterNow->OnFinish();
		m_pDoc->Clear();
		m_FliterNow->OnInit();
		InvalidateRect(m_pView->GetHWND(),NULL,TRUE);
		break;
	case IDM_FILE_OPEN:
		//if(m_bModified)
		//{
		//	if(MessageBox(m_hWnd,szMessage,szWindow,MB_YESNO) == IDNO)
		//		break;
		//}
		m_FliterNow->OnFinish();
     // file handle

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		//
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		//
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = TEXT("Bitmap\0*.bmp\0AnyFiles\0*.*");
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if(::GetOpenFileName(&ofn) )
		{
			::wcsncpy(m_FileName,ofn.lpstrFile,MAX_FILE_NAME);
			LoadBitmapFromFile(m_FileName);
		}
		break;
	case IDM_FILE_SAVE:
	case IDM_FILE_SAVEAS:
		m_FliterNow->OnFinish();
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_pView->GetHWND();
		ofn.lpstrFile = szFile;
		//
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		//
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = TEXT("位图文件(*.bmp)\0 *.bmp\0");
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		
		ofn.Flags   =   OFN_HIDEREADONLY;   
		if(wmID == IDM_FILE_SAVEAS || m_FileName[0] == 0)
		{
			if(::GetSaveFileName(&ofn) )
			{
				::wcsncpy(m_FileName,ofn.lpstrFile,MAX_FILE_NAME);
				::wcscat_s(m_FileName,MAX_FILE_NAME,TEXT(".bmp") );
			}
			HBITMAP hbitmapsave = (HBITMAP)GetCurrentObject(CMyFliter::m_dcMem,OBJ_BITMAP);
			CreateBMPFile(m_pView->GetHWND(),m_FileName,CreateBitmapInfoStruct(m_pView->GetHWND(),hbitmapsave),hbitmapsave,CMyFliter::m_dcMem);
			m_bModified = false;
		}
		break;
	case IDM_FILE_EXIT:
		/*
		if(m_bModified)
		{
			if(MessageBox(m_hWnd,szMessage,szWindow,MB_YESNO) == IDNO)
				break;
		}
		*/
		::PostQuitMessage(0);	
		DeleteDC(CMyFliter::m_dcMem);
		DeleteDC(CMyFliter::m_dcBitMap);
		
		break;
	case IDM_EDIT_UNDO:
		m_pDoc->Undo(rectLater);
		
		if(rectLater.right != CMyFliter::m_rectTrue.right || rectLater.bottom != CMyFliter::m_rectTrue.bottom )
		{
					
			CMyFliter::SetRectTrue(rectLater);
			m_pView->SetRect(rectLater);
			//SetViewToolBar();
			SetTrueClient(rectLater,false);
		}

		CMyFliter::m_dcBitMap = m_pDoc->GetLastDC();
		::BitBlt(CMyFliter::m_dcMem,0,0,rectLater.right,rectLater.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		InvalidateRect(m_pView->GetHWND(),NULL,TRUE);
		
		break;
	case IDM_EDIT_REDO:
		m_pDoc->Redo(rectLater);
		if(rectLater.right != CMyFliter::m_rectTrue.right || rectLater.bottom != CMyFliter::m_rectTrue.bottom )
		{
					
			CMyFliter::SetRectTrue(rectLater);
			m_pView->SetRect(rectLater);
			SetTrueClient(rectLater,false);
		}
		CMyFliter::m_dcBitMap = m_pDoc->GetLastDC();
		::BitBlt(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcBitMap,0,0,SRCCOPY);
		InvalidateRect(m_pView->GetHWND(),NULL,TRUE);
		break;
	case IDM_EDIT_CUT:
		m_FliterNow->Cut();
		break;
	case IDM_EDIT_COPY:
		m_FliterNow->Copy();
		break;
	case IDM_EDIT_PASTE:
		m_FliterNow->Paste();
		break;
	case IDM_EDIT_SELECTALL:
		m_toolbar.SetSelect(1);
		GetClientRect(m_pView->GetHWND(),&rectTo);
		m_FliterNow->SetRectSel(rectTo);
		break;
	case IDM_GRAPH_FLIP:
		m_toolbar.SetSelect(1);
		SelectFlip =(int) ::DialogBox(g_App.GetInst(),MAKEINTRESOURCE(IDD_FLIP),m_pView->GetHWND(),DialogProcFlip);
		rectTo = m_FliterNow->GetRectSel();
		switch(SelectFlip)
		{
		case 0:
			return 0;
		default:
			m_FliterNow->Flip(SelectFlip,rectTo);
		}
		break;
	case IDM_GRAPH_ZOOM:
		if(::DialogBox(g_App.GetInst(),MAKEINTRESOURCE(IDD_ZOOM),m_pView->GetHWND(),DialogProcZoom))
		{
			rectTo = m_FliterNow->GetRectSel();
			m_FliterNow->Zoom(zoom.x,zoom.y,0,0,rectTo);
		}
		break;
	case IDM_GRAPH_INVERTCOLOR:
		m_toolbar.SetSelect(1);
		m_FliterNow->Invert();
		break;
	case IDM_GRAPH_CLEAR:
		m_toolbar.SetSelect(1);
		m_FliterNow->Clear();
		break;
	case IDM_GRAPH_GRAPHATTRIBUTE:
		if(::DialogBox(g_App.GetInst(),MAKEINTRESOURCE(IDD_ATTR),m_pView->GetHWND(),DialogProcAttr))
		{
			SetTrueClient(m_rectAttr);
		}
		break;



	}

	return 0;
}

void CMyFrameWnd::OnToolChange(CMyToolBarWithList::ToolStyle style,CMyToolBarWithList::ToolStyle styleFormer)
{
	m_Fliters[(int)styleFormer]->OnFinish();
	m_FliterNow = m_Fliters[(int)style];
	m_pView->SetFliter(m_FliterNow);
	m_FliterNow->OnToolChange(style,styleFormer);
}
void CMyFrameWnd::OnIndexChange(int index)
{
	m_FliterNow->OnIndexChangeOfTool(index);
}

void CMyFrameWnd::OnFtColorChange(COLORREF color)
{
	CMyFliter::SetFtColor(color);
	m_colorbar.SetFtColor(color);
	
}
	
void CMyFrameWnd::OnBkColorChange(COLORREF color)
{
	CMyFliter::SetBkColor(color);
	m_colorbar.SetBkColor(color);

}


bool CMyFrameWnd::LoadBitmapFromFile(const TCHAR *pszFileName)
{
	HANDLE handle = INVALID_HANDLE_VALUE;
	bool bret = false;
	DWORD m_nImageSize;
	BITMAPINFO* pDIB ;
	BYTE* m_pBits;
	RECT rectBitmap = {0};

	do
	{
		if (pszFileName == NULL)
			break;
		handle = ::CreateFile(pszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,
			OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if (handle == INVALID_HANDLE_VALUE)
			break;
		BITMAPFILEHEADER bmFileHeader;

		DWORD dwRead;
		if (!ReadFile(handle,&bmFileHeader,sizeof(bmFileHeader),&dwRead,NULL))
		{
			break;
		}
		if (dwRead != sizeof(bmFileHeader))
		{
			break;
		}
		if (bmFileHeader.bfType != 0x4D42)
		{
			break;
		}
		if (bmFileHeader.bfSize != ::GetFileSize(handle,NULL))
		{
			break;
		}
		pDIB = (BITMAPINFO*) new BYTE[bmFileHeader.bfSize - sizeof(bmFileHeader)];
	
		if (pDIB == NULL)
			break;

		if (!ReadFile(handle,pDIB,bmFileHeader.bfSize - sizeof(bmFileHeader),&dwRead,NULL))
			break;
		if (dwRead != (bmFileHeader.bfSize - sizeof(bmFileHeader)))
			break;
		rectBitmap.right = pDIB->bmiHeader.biWidth;
		rectBitmap.bottom = pDIB->bmiHeader.biHeight;

		BITMAPINFO* m_pBMI = pDIB;
		DWORD dwBitmapInfoHeaderSize = *(DWORD*)pDIB;
		if (dwBitmapInfoHeaderSize < sizeof(BITMAPINFOHEADER))
			break;
		BITMAPINFOHEADER* pHeader = &m_pBMI->bmiHeader;
		long m_nWidth = pHeader->biWidth;
		long m_nHeight = pHeader->biHeight;
		WORD m_nPlanes = pHeader->biPlanes;
		WORD m_nBitCount = pHeader->biBitCount;
		m_nImageSize = pHeader->biSizeImage;
		DWORD nCompression = pHeader->biCompression;
		DWORD m_nClrUsed = pHeader->biClrUsed;
		DWORD m_nClrImpt = pHeader->biClrImportant;
		if (m_nBitCount <= 8)
		{
			//如果为0,表示整个颜色表
			if (m_nClrUsed == 0)
			{
				m_nClrUsed = 1 << m_nBitCount;
			}
		}
		m_pBits = (BYTE*)pDIB + bmFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);   //这个是我加的
		RGBQUAD* m_pRGBQUAD;
		DWORD* m_pBitFields;
		//无论任何情况,都可能存在颜色表格,因此如果存在颜色表格
		if (m_nClrUsed)
		{
			//如果为0，表示颜色表格中的所有项目都是重要的
			if (m_nClrImpt == 0)
			{
				m_nClrImpt = m_nClrUsed;
			}
			if (nCompression == BI_BITFIELDS)
			{
				m_pBitFields = (DWORD*)((BYTE*)pDIB + dwBitmapInfoHeaderSize);
				m_pRGBQUAD = (RGBQUAD*)((BYTE*)m_pBitFields + 3 * sizeof(DWORD));
			}
			else
			{
				m_pRGBQUAD = (RGBQUAD*)((BYTE*)pDIB + dwBitmapInfoHeaderSize);
			}
			m_pBits = (BYTE*)&m_pRGBQUAD[m_nClrUsed];
		}
		else
		{
			if (nCompression == BI_BITFIELDS)
			{
				m_pBitFields = (DWORD*)((BYTE*)pDIB + dwBitmapInfoHeaderSize);
				m_pBits = (BYTE*)&m_pBitFields[3];
			}
			else
			{
				m_pBits = (BYTE*)pDIB + dwBitmapInfoHeaderSize;
			}
		}
		DWORD m_nBytesPerRow = (m_nWidth * m_nBitCount + 31) / 32 * 4;
		//m_nHeight > 0:这是由下至上的DIB
		//m_nHeight < 0:这是由上至下的DIB
		if (m_nHeight < 0)
			m_nHeight = -m_nHeight;
		if (m_nImageSize == 0)
		{
			m_nImageSize = m_nBytesPerRow * m_nHeight;
		}
		//DIBFORMAT m_nImageForamt;DIB
		switch (m_nBitCount)
		{
		case 1:
			//m_nImageForamt = DIB_1bpp;
			break;
		case 2:
			//m_nImageForamt = DIB_2bpp;
			break;
		case 4:
			if (nCompression == BI_RLE4)
			{
				//m_nImageForamt = DIB_4bppRLE;
			}
			else
			{
				//m_nImageForamt = DIB_4bpp;
			}
			break;
		case 8:
			if (nCompression == BI_RLE8)
			{
				//m_nImageForamt = DIB_8bppRLE;
			}
			else
			{
				//m_nImageForamt = DIB_8bpp;
			}
			break;
		case 16:
			if (nCompression == BI_BITFIELDS)
			{
				if (m_pBitFields[0] == 0x001F && m_pBitFields[1] == 0x03E0 && m_pBitFields[2] == 0x7C00)
				{
					//m_nImageForamt = DIB_16RGB555;
				}
				else if (m_pBitFields[0] == 0x001F && m_pBitFields[1] == 0x07E0 && m_pBitFields[2] == 0xF800)
				{
					//m_nImageForamt = DIB_16RGB565;
				}
				else
				{
					//m_nImageForamt = DIB_16RGBbitfields;
				}
			}
			else
			{
				//m_nImageForamt = DIB_16RGB555;
			}
			break;
		case 24:
			//m_nImageForamt = DIB_24RGB888;
			break;
		case 32:
			if (nCompression == BI_BITFIELDS)
			{
				if (m_pBitFields[0] == 0x00FF && m_pBitFields[1] == 0xFF00 && m_pBitFields[2] == 0xFF0000)
				{
					//m_nImageForamt = DIB_32RGB888;
				}
				else  
				{
					//m_nImageForamt = DIB_32RGBbitfields;
				}
			}
			else
			{
				//m_nImageForamt = DIB_32RGB888;
			}
			break;
		default:
			break;
		}
		bret = true;
	}while(false);


	//m_pDoc->On
	m_pDoc->Clear();
	

	
	HDC hdc = ::GetDC(m_pView->GetHWND() );
/*	::GetClientRect(m_pView->GetHWND(),&rectClient);
	CMyFliter::m_dcMem = CreateCompatibleDC(hdc);
	CMyFliter::m_bitMap = CreateCompatibleBitmap(hdc,rectClient.right - rectClient.left , rectClient.bottom - rectClient.top);
	SelectObject(CMyFliter::m_dcMem,CMyFliter::m_bitMap);
	SelectObject(CMyFliter::m_dcMem,GetStockObject(NULL_PEN) );
	SelectObject(CMyFliter::m_dcMem,GetStockObject(WHITE_BRUSH) );
	Rectangle(CMyFliter::m_dcMem,0,0,m_rectTrue.right,m_rectTrue.bottom);
	SelectObject(CMyFliter::m_dcMem,GetStockObject(BLACK_PEN) );
	SelectObject(CMyFliter::m_dcMem,GetStockObject(WHITE_BRUSH) );
	//m_pDoc->OnInit(CMyFliter::m_dcMem);*/
	//	DeleteDC(CMyFliter::m_dcMem );
//	::DeleteObject(CMyFliter::m_bitMap);


//	CMyFliter::m_dcMem = CreateCompatibleDC(hdc);
//	CMyFliter::m_bitMap = ::CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
	//SelectObject(CMyFliter::m_dcMem ,CMyFliter::m_bitMap);
	//HBRUSH hbtemp = (HBRUSH)SelectObject( CMyFliter::m_dcMem ,GetStockObject(WHITE_BRUSH) );
	//::Rectangle( CMyFliter::m_dcMem ,0,0,rect.right,rect.bottom);
	//DeleteObject(SelectObject(CMyFliter::m_dcMem ,hbtemp) );

	SetTrueClient(rectBitmap);
	SetDIBitsToDevice(CMyFliter::m_dcMem,0,0,rectBitmap.right,rectBitmap.bottom,0,0,0,rectBitmap.bottom,m_pBits, pDIB,DIB_RGB_COLORS);//这个是我加的
	::BitBlt(hdc,0,0,rectBitmap.right,rectBitmap.bottom,CMyFliter::m_dcMem,0,0,SRCCOPY);
	
	ReleaseDC(m_pView->GetHWND(),hdc);
	
	//m_pDoc->OnFinish(CMyFliter::m_dcMem,m_rectTrue);
	CMyFliter::m_dcBitMap = m_pDoc->GetLastDC();
	
		

	if (handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(handle);
	}
	return bret;
}

void CMyFrameWnd::CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, 
				   HBITMAP hBMP, HDC hDC) 
{ 
	HANDLE hf;                 // file handle 
	BITMAPFILEHEADER hdr;       // bitmap file-header 
	PBITMAPINFOHEADER pbih;     // bitmap info-header 
	LPBYTE lpBits;              // memory pointer 
	DWORD dwTotal;              // total count of bytes 
	DWORD cb;                   // incremental count of bytes 
	BYTE *hp;                   // byte pointer 
	DWORD dwTmp; 

	pbih = (PBITMAPINFOHEADER) pbi; 
	lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);



	// Retrieve the color table (RGBQUAD array) and the bits 
	// (array of palette indices) from the DIB. 
	GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, 
		DIB_RGB_COLORS) ;


	// Create the .BMP file. 
	hf = CreateFile(pszFile, 
		GENERIC_READ | GENERIC_WRITE, 
		(DWORD) 0, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		(HANDLE) NULL); 	
	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
	// Compute the size of the entire file. 
	hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
		pbih->biSize + pbih->biClrUsed 
		* sizeof(RGBQUAD) + pbih->biSizeImage); 
	hdr.bfReserved1 = 0; 
	hdr.bfReserved2 = 0; 

	// Compute the offset to the array of color indices. 
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
		pbih->biSize + pbih->biClrUsed 
		* sizeof (RGBQUAD); 

	// Copy the BITMAPFILEHEADER into the .BMP file. 
	WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
		(LPDWORD) &dwTmp,  NULL) ;
	
	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
	WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
		+ pbih->biClrUsed * sizeof (RGBQUAD), 
		(LPDWORD) &dwTmp, ( NULL)); 
		

	// Copy the array of color indices into the .BMP file. 
	dwTotal = cb = pbih->biSizeImage; 
	hp = lpBits; 
	WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL) ;
		

	// Close the .BMP file. 
	CloseHandle(hf); 


	// Free memory. 
	GlobalFree((HGLOBAL)lpBits);
}


PBITMAPINFO CMyFrameWnd::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{ 
	BITMAP bmp; 
	PBITMAPINFO pbmi; 
	WORD    cClrBits; 

	// Retrieve the bitmap color format, width, and height. 
GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp);
 

	// Convert the color format to a count of bits. 
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
	if (cClrBits == 1) 
		cClrBits = 1; 
	else if (cClrBits <= 4) 
		cClrBits = 4; 
	else if (cClrBits <= 8) 
		cClrBits = 8; 
	else if (cClrBits <= 16) 
		cClrBits = 16; 
	else if (cClrBits <= 24) 
		cClrBits = 24; 
	else cClrBits = 32; 

	// Allocate memory for the BITMAPINFO structure. (This structure 
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
	// data structures.) 

	if (cClrBits != 24) 
		pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
		sizeof(BITMAPINFOHEADER) + 
		sizeof(RGBQUAD) * (1<< cClrBits)); 

	// There is no RGBQUAD array for the 24-bit-per-pixel format. 

	else 
		pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
		sizeof(BITMAPINFOHEADER)); 

	// Initialize the fields in the BITMAPINFO structure. 

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	pbmi->bmiHeader.biWidth = bmp.bmWidth; 
	pbmi->bmiHeader.biHeight = bmp.bmHeight; 
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
	if (cClrBits < 24) 
		pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

	// If the bitmap is not compressed, set the BI_RGB flag. 
	pbmi->bmiHeader.biCompression = BI_RGB; 

	// Compute the number of bytes in the array of color 
	// indices and store the result in biSizeImage. 
	// For Windows NT, the width must be DWORD aligned unless 
	// the bitmap is RLE compressed. This example shows this. 
	// For Windows 95/98/Me, the width must be WORD aligned unless the 
	// bitmap is RLE compressed.
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
		* pbmi->bmiHeader.biHeight; 
	// Set biClrImportant to 0, indicating that all of the 
	// device colors are important. 
	pbmi->bmiHeader.biClrImportant = 0; 
	return pbmi; 
} 


INT_PTR CALLBACK  CMyFrameWnd::DialogProcFlip( HWND hwndDlg,  UINT uMsg,   WPARAM wParam, LPARAM lParam)
{
	static int Selected = 1;
	DWORD wmID;
	switch( uMsg)
	{
	case WM_INITDIALOG:
		ShowWindow(hwndDlg,SW_SHOW);
		::EnableWindow( GetDlgItem(hwndDlg,IDC_RADIO4),FALSE);
		::EnableWindow( GetDlgItem(hwndDlg,IDC_RADIO5),FALSE);
		::EnableWindow( GetDlgItem(hwndDlg,IDC_RADIO6),FALSE);
		::SendMessage( GetDlgItem(hwndDlg,IDC_RADIO1),BM_SETCHECK,1,0);
		break;
	case WM_COMMAND:
		wmID = LOWORD(wParam);
		switch(wmID)
		{
		case IDOK:
			EndDialog(hwndDlg,Selected);
			break;
		case IDCANCEL:
			EndDialog(hwndDlg,0);
			break;
		case IDC_RADIO1:
			Selected = 1;
			::EnableWindow( GetDlgItem(hwndDlg,IDC_RADIO4),FALSE);
			::EnableWindow( GetDlgItem(hwndDlg,IDC_RADIO5),FALSE);
			::EnableWindow( GetDlgItem(hwndDlg,IDC_RADIO6),FALSE);
			break;
		case IDC_RADIO2:
			Selected = 2;
			::EnableWindow( GetDlgItem(hwndDlg,IDC_RADIO4),FALSE);
			::EnableWindow( GetDlgItem(hwndDlg,IDC_RADIO5),FALSE);
			::EnableWindow( GetDlgItem(hwndDlg,IDC_RADIO6),FALSE);
			break;
		case IDC_RADIO3:
			::EnableWindow( GetDlgItem(hwndDlg,IDC_RADIO4),TRUE);
			::EnableWindow( GetDlgItem(hwndDlg,IDC_RADIO5),TRUE);
			::EnableWindow( GetDlgItem(hwndDlg,IDC_RADIO6),TRUE);
			Selected = 4;
			::SendMessage( GetDlgItem(hwndDlg,IDC_RADIO4),BM_SETCHECK,1,0 );
			break;
		case IDC_RADIO4:
			Selected = 4;
			::SendMessage( GetDlgItem(hwndDlg,IDC_RADIO3),BM_SETCHECK,1,0 );
			break;
		case IDC_RADIO5:
			Selected = 5;
			::SendMessage( GetDlgItem(hwndDlg,IDC_RADIO3),BM_SETCHECK,1,0 );
			break;
		case IDC_RADIO6:
			Selected = 6;
			::SendMessage( GetDlgItem(hwndDlg,IDC_RADIO3),BM_SETCHECK,1,0 );
			break;
		}
		break;
	//case WM_DESTROY:
	//	EndDialog(hwndDlg,0);
	//	break;
	}
	return 0;
}


INT_PTR CALLBACK  CMyFrameWnd::DialogProcZoom( HWND hwndDlg,  UINT uMsg,   WPARAM wParam, LPARAM lParam)
{
	static int Selected = 1;
	TCHAR buffer[50];
	DWORD wmID;
	CMyFrameWnd* pWnd = NULL;
	BOOL bt;
	int x,y;
	switch( uMsg)
	{
	case WM_INITDIALOG:
		ShowWindow(hwndDlg,SW_SHOW);
		wsprintf(buffer,TEXT("%d"),100);
		ShowWindow(hwndDlg,SW_SHOW);
		::SetDlgItemText(hwndDlg,IDC_EDIT1,buffer);
		::SetDlgItemText(hwndDlg,IDC_EDIT2,buffer);
		wsprintf(buffer,TEXT("%d"),0);
		::SetDlgItemText(hwndDlg,IDC_EDIT3,buffer);
		::SetDlgItemText(hwndDlg,IDC_EDIT4,buffer);
		::SetFocus(GetDlgItem(hwndDlg,IDC_EDIT1) );
		::SendMessage( GetDlgItem(hwndDlg,IDC_EDIT1),EM_SETSEL,0,3);
		
		break;
	case WM_COMMAND:
		wmID = LOWORD(wParam);
		switch(wmID)
		{
		case IDOK:
			x = GetDlgItemInt(hwndDlg,IDC_EDIT1,&bt,true);
			y = GetDlgItemInt(hwndDlg,IDC_EDIT2,&bt,true);

			pWnd = (CMyFrameWnd*)g_App.GetCWnd();
			pWnd->SetZoom(x,y);
			EndDialog(hwndDlg,Selected);
			break;
		case IDCANCEL:
			EndDialog(hwndDlg,0);
			break;
		}
		break;
	}
	return 0;
}

INT_PTR CALLBACK  CMyFrameWnd::DialogProcAttr( HWND hwndDlg,  UINT uMsg,   WPARAM wParam, LPARAM lParam)
{
	//TCHAR buffer[50];
	DWORD wmID;
	int x,y;
	BOOL bo;
	CMyFrameWnd* pWnd;
	switch( uMsg)
	{
	case WM_INITDIALOG:
		ShowWindow(hwndDlg,SW_SHOW);
		::SetDlgItemInt(hwndDlg,IDC_Width,640,true);
		::SetDlgItemInt(hwndDlg,IDC_HEIGHT,480,true);
		break;
	case WM_COMMAND:
		wmID = LOWORD(wParam);
		switch(wmID)
		{
		case IDC_OK:
			x = GetDlgItemInt(hwndDlg,IDC_Width,&bo,true);
			y = GetDlgItemInt(hwndDlg,IDC_HEIGHT,&bo,true);
			pWnd = (CMyFrameWnd*)g_App.GetCWnd();
			pWnd->SetAttrRect(x,y);
			EndDialog(hwndDlg,1);
			break;
		case IDC_CANCLE:
			EndDialog(hwndDlg,0);
			break;
		}
		break;

	}
	return 0;
}

void CMyFrameWnd::SetTrueClient(RECT rect,bool bNeedFinish)
{
	
	HDC hdcTemp = ::CreateCompatibleDC(CMyFliter::m_dcMem);
	HBITMAP hbTemp = ::CreateCompatibleBitmap(CMyFliter::m_dcMem,rect.right-rect.left,rect.bottom-rect.top);
	SelectObject(hdcTemp,hbTemp);
	SelectObject(hdcTemp,GetStockObject(NULL_PEN) );
	SelectObject(hdcTemp,CMyFliter::m_Brush);
	Rectangle(hdcTemp,rect.left,rect.top,rect.right,rect.bottom);
	BitBlt(hdcTemp,0,0,m_rectTrue.right,m_rectTrue.bottom,CMyFliter::m_dcMem,0,0,SRCCOPY);
	
	DeleteDC(CMyFliter::m_dcMem);
	DeleteObject(CMyFliter::m_bitMap);
	/*CMyFliter::m_dcMem = ::CreateCompatibleDC(hdcTemp);
	CMyFliter::m_bitMap = ::CreateCompatibleBitmap(hdcTemp,rect.right-rect.left,rect.bottom-rect.top);
	SelectObject(CMyFliter::m_dcMem ,CMyFliter::m_bitMap);
	SelectObject(CMyFliter::m_dcMem,CMyFliter::m_Brush);
	SelectObject(CMyFliter::m_dcMem ,CMyFliter::m_Pen);
	BitBlt(CMyFliter::m_dcMem,0,0,rect.right,rect.bottom,hdcTemp,0,0,SRCCOPY);


	DeleteDC(hdcTemp);
	DeleteObject(hbTemp);*/
	CMyFliter::m_dcMem = hdcTemp;
	CMyFliter::m_bitMap = hbTemp;

	if(bNeedFinish)
	{

	m_pDoc->OnFinish(CMyFliter::m_dcMem,rect);
	CMyFliter::m_dcBitMap = m_pDoc->GetLastDC();
	}
	

	CMyFliter::SetRectTrue(rect);
	m_pView->SetRect(rect);
	SetViewToolBar();

	HDC hdc = GetDC(m_pView->GetHWND() );
	SetMapMode(hdc,MM_ANISOTROPIC );
	SetViewportExtEx(hdc,CMyFliter::m_zoom,CMyFliter::m_zoom,NULL);
	BitBlt(hdc,0,0,m_rectTrue.right  ,m_rectTrue.bottom ,CMyFliter::m_dcMem,CMyFliter::m_Start.x,CMyFliter::m_Start.y,SRCCOPY);
	ReleaseDC(m_pView->GetHWND(),hdc);

}