/*074090 Œ‚±¶¡÷*/
#ifndef _C_MY_LIST_BOX_H
#define _C_MY_LIST_BOX_H
#include "define.h"
#include "CMyWnd.h"



class CMyListBox : public CMyWnd
{
public:
	/* One Application may need more than one tool bar, It's maybe Okay , But I had never try it*/
	CMyListBox(CMyRect rectWnd) :CMyWnd(false,TEXT("LISTBOX"),rectWnd)
	{			
	};

	//This  Function is to rewriten
	virtual void CreateEx(); 
	//virtual LRESULT OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //overide
	//virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


protected: //some about the bit map and 
	static DWORD ms_ID;
};

#endif