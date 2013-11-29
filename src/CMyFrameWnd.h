/*074090 Œ‚±¶¡÷*/
#ifndef _C_MY_FRAME_WND_H
#define _C_MY_FRAME_WND_H

#include"CMyWnd.h"
#include"CMyToolBar.h"
#include "CMyColorBar.h"
#include"resource.h"
#include"define.h"

#include "CMyView.h"
#include "CMyDoc.h"


#include <commctrl.h>
#include<windows.h>

class CMyFliter;
class CMyFrameWnd : public CMyWnd{
public:
	
	CMyFrameWnd();

	virtual void PreCreateWindow();

	virtual LRESULT OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	 

	virtual LRESULT OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	/* Message Handler */
	virtual LRESULT OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnMenuPop(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnVScroll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnHScroll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnTimer();


	virtual void SetViewToolBar();

/*	void SetSrollBarH(POINT point);

	void SetSrollBarV(POINT point);*/

	void OnToolChange(CMyToolBarWithList::ToolStyle style,CMyToolBarWithList::ToolStyle styleFormer);

	void SetSelect(CMyToolBarWithList::ToolStyle style)
	{
		m_toolbar.SetSelect(style);
	}
	
	void OnIndexChange(int index);

	void OnFtColorChange(COLORREF color);
	
	void OnBkColorChange(COLORREF color);

	bool LoadBitmapFromFile(const TCHAR *pszFileName);

	void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,  HBITMAP hBMP, HDC hDC) ;
	PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);

	CMyView* GetView()
	{
		return m_pView;
	}

	void DrawRect(const RECT& rect);
	void SetTrueClient(RECT rect,bool bNeedFinish = true);

	RECT GetViewRect(){return m_rectTrue;}

	void SetAttrRect(int width,int height)
	{
		m_rectAttr.left = m_rectAttr.top = 0;
		m_rectAttr.right = width;
		m_rectAttr.bottom = height;
	}

	void SetZoom(int x,int y)
	{
		zoom.x = x;
		zoom.y = y;
	}

	void SetModified(bool b)
	{
		m_bModified = b;
	}
	
protected:
	static INT_PTR CALLBACK DialogProcFlip( HWND hwndDlg,  UINT uMsg,   WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK DialogProcZoom( HWND hwndDlg,  UINT uMsg,   WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK DialogProcAttr( HWND hwndDlg,  UINT uMsg,   WPARAM wParam, LPARAM lParam);
private:
	CMyFliter* m_FliterNow;
	(CMyFliter*) m_Fliters[TOOL_NUM];
	CMyToolBarWithList m_toolbar;
	CMyColorBar m_colorbar;
	CMyView*     m_pView;
	CMyDoc*      m_pDoc;
	TCHAR        m_FileName[MAX_FILE_NAME];
	HWND         m_hScrollV;
	HWND         m_hScrollH;
	RECT		m_rectTrue;

	bool m_bModified;

	


	RECT m_rectAttr;   //Attr Dialog
	POINT zoom;

};


#endif