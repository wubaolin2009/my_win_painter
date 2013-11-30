/*074090 Œ‚±¶¡÷*/
#ifndef _C_MY_COLOR_BAR_H
#define _C_MY_COLOR_BAR_H

#include "CMyWnd.h"
#include "define.h"
#include <windows.h>

class CMyColorBar : public CMyWnd
{
public:
	/* One Application may need more than one tool bar, It's maybe Okay , But I had never try it*/
	CMyColorBar(CMyRect rectWnd) :CMyWnd(true,CMYCOLORBAR_CLASS_NAME,rectWnd)
	{
		m_ftColor = RGB(0,0,0);  //black
		m_bkColor = RGB(255,255,255);  //white
	};

	//This 2 Function is to rewriten
	virtual void PreCreateWindow();  
	virtual void CreateEx(); 
	virtual LRESULT OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //overide
	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnRButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonDBClick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnRButtonDBClick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetFtColor(COLORREF color)
	{
		m_ftColor = color;
		InvalidateRect(m_hWnd,NULL,TRUE);
	}

	void SetBkColor(COLORREF color)
	{
		m_bkColor = color;
		InvalidateRect(m_hWnd,NULL,TRUE);
	}

	virtual ~CMyColorBar()
	{
		DeleteDC(m_hdcMem);
		DeleteObject(m_hbit);
	}	



protected: //some about the bit map and 
	static DWORD ms_ID;
	static COLORREF m_colors[COLOR_NUM_PALLET];
	static bool m_ColorChanged[COLOR_NUM_PALLET];
	HDC m_hdcMem;
	HBITMAP m_hbit;

	COLORREF m_ftColor;
	COLORREF m_bkColor;



private:
	static UINT_PTR CALLBACK CCHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam,  LPARAM lParam);  //ColorDialogHook
};

#endif