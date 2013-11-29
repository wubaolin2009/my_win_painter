/*074090 Œ‚±¶¡÷*/
#ifndef _MY_CWND_H
#define _MY_CWND_H
#include<windows.h>
#include"List.h"
#include"CMyApp.h"



#define MAX_WORD_CLASS 100


class CMyRect
{
public:
	CMyRect()
	{
		SetEmptyRect();
	}
	CMyRect(int left1,int top1,int right1,int bottom1)
	{
		left = left1;
		top = top1;
		right = right1;
		bottom = bottom1;

	}
	int left,top,right,bottom;
	void SetEmptyRect()
	{
		left = top = right = bottom = 0;
	}
	RECT GetRect()
	{
		RECT rect = {left,top,right,bottom};
		return rect;
	}
};

class CMyWnd                 // Just a base class with common data and functions, Every created window must call CMyWnd::Create to Register its message
{
protected:
	CMyRect m_rectWnd;      //The size rect of a window

	HWND    m_hWnd;         //HWND of a window

	WNDCLASSEX m_wndClassex; //A struct used to register a window class

	bool m_bNewWnd;          //Describe that if a window is a user-registed


	TCHAR m_szClassName[MAX_WORD_CLASS];     // class name  

	CREATESTRUCT m_createStruct;         //for createWindows Function

	CMyWnd* m_Parent;

public:

	CMyWnd(bool bIsNewWndClass,LPTSTR szClassName,CMyRect rect)
	{
		m_rectWnd.SetEmptyRect();
		m_hWnd = NULL;
		m_bNewWnd = bIsNewWndClass;
		if(szClassName)
			wcscpy_s(m_szClassName,MAX_WORD_CLASS,szClassName);
		m_Parent = NULL;
		m_rectWnd = rect;
	}

	

	

	virtual void PreCreateWindow();  //for something about class resgister ,if you want to inherit it , It's recommended that
	//call CMyWnd::PreCreateWindow before each inheriment


	virtual void CreateEx();    // for the devired class to setup how createwindow works
	// It's recommned that you call CMyWnd::CreateEx before each inheriment

	//There is no need to inherit it 
	//Create windows  It will be done as folloing ways:
	// Call PreCreateWindow to Register class , thougn some don't need it , It depends on specified implements
	// Then Call RegisterClass if it bNewWnd is true;
	// Then CreateEx to Setup the window Style and CreateWindowEx  ,  make the client rect to m_rectWnd
	// Last but most important if will register messagefunction to the global varible , which you needn't consider about.
	void Create();   //This versionNeedn't the information about class Style if you want just inherit your class from CFrameWnd

	//Every class must inherit this if you want to add some codes on WM_CREATE message, because 
	//when the HWND is available , WM_CREATE is already handled. so WM_CREATE is very different
	//Recomended that call CMyWnd::OnCreate when you inherit it
	virtual LRESULT OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	m_hWnd = hWnd;return 0;};


	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	


	virtual LRESULT OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PostQuitMessage(0);
		return 0;		
	};



	



	//SomeThing about message function

	void SetParent(CMyWnd* hwnParent)
	{
		m_Parent = hwnParent;
	}

	CMyWnd* GetParent()
	{
		return m_Parent;
	}

	void SetRect(const CMyRect* rectSet)             // Set current rect of the window
	{
		m_rectWnd = *rectSet;
		RECT rect = m_rectWnd.GetRect();
		::MoveWindow(m_hWnd,rect.left,rect.top,rect.right-rect.left,rect.bottom - rect.top,true);
	}
	void SetRect(RECT rect)
	{
		m_rectWnd.left  = rect.left;
		m_rectWnd.right = rect.right;
		m_rectWnd.top = rect.top;
		m_rectWnd.bottom = rect.bottom;
		//::MoveWindow(m_hWnd,m_rectWnd.left,m_rectWnd.top,m_rectWnd.right-m_rectWnd.left,m_rectWnd.bottom - m_rectWnd.top,true);
	}

	RECT GetRect()			// Get current rect of the window
	{
		return m_rectWnd.GetRect();
	}

	HWND GetHWND()             // return the handle to the window
	{
		return m_hWnd;
	}

};


#endif