/*074090 Œ‚±¶¡÷*/
#ifndef _C_MY_VIEW_H
#define _C_MY_VIEW_H

#include "CMyWnd.h"
#include "define.h"

class CMyFliter;

class CMyView:public CMyWnd
{
public:
	enum Status{DRAWING = 0,MOVINGX,MOVINGY,MOVINGXY};
public:
	CMyView(CMyRect rect) :CMyWnd(true, CMYVIEW_CLASS_NAME, rect)
	{
		m_status = DRAWING;
	};

	void SetFliter(CMyFliter* pFliter)
	{
		m_pFliter = pFliter;
	}

	virtual void PreCreateWindow(); 

	virtual LRESULT CMyView::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnMouseMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnLButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	virtual LRESULT OnRButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnLButtonUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnRButtonUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnLDBCLICK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnRDBCLICK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	



	virtual void CreateEx();   
private:
	static DWORD ms_ID;
	CMyFliter* m_pFliter;
	Status m_status;

};

#endif