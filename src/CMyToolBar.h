/*074090 Œ‚±¶¡÷*/
#ifndef _C_MY_TOOL_BAR_H
#define _C_MY_TOOL_BAR_H

#include"CMyApp.h"
#include"CMyWnd.h"
#include"CMyListBox.h"


#include"define.h"
#include<windows.h>
#include <commctrl.h>

class CMyToolBar : public CMyWnd
{


public:
	/* One Application may need more than one tool bar, It's maybe Okay , But I had never try it*/
	CMyToolBar(CMyRect rectWnd,DWORD Style,int BmpID,int line,int col) :CMyWnd(true,CMYTOOLBAR_CLASS_NAME,rectWnd)
	{
		m_style = Style;
		m_bmpId = BmpID;
		m_Line = line;
		m_Col = col;		
	}

	//This 2 Function is to rewriten
	virtual void PreCreateWindow();  
	virtual void CreateEx(); 
	virtual LRESULT OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //overide

protected: //some about the bit map and 

	TBBUTTON m_buttonInfo[MAX_NUM_ITEM_TOOLBAR];   //StoreButton
	DWORD m_style;                   //Button Style
	int m_bmpId;					// Resource ID 
	int m_Line     ;
	int m_Col;

	HWND m_wndTrue;               //True ToolBar

	static DWORD ms_ID;
private:
	HWND  CreateToolBar(HINSTANCE   hIns,  HWND   hParent,int  BmpID,int  nBtn,TBBUTTON*  Btn);	
};


class CMyToolBarWithList:public CMyToolBar
{
public:
	enum ToolStyle{TOOL_CUT_0 = 0,TOOL_CUT_1,TOOL_ERASER,TOOL_BIN,TOOL_GET,TOOL_ZOOM,TOOL_PENCIL,TOOL_BRUSH,TOOL_GUN,TOOL_WORD,TOOL_LINE,TOOL_CURL,TOOL_RECT,TOOL_POL,TOOL_ELLIPSE,TOOL_CIRCLERECT};
public:
	CMyToolBarWithList(CMyRect rectWnd,DWORD Style,int BmpID,int line,int col) :CMyToolBar(rectWnd,Style,BmpID,line,col),m_Listbox(CMyRect(2,290,62,370) )
	{
		m_toolSelect = TOOL_PENCIL;
		m_wndList = NULL;
		m_Listbox.SetParent(this);
		m_current = 0;
	};

	//This FUnctions are to handle messages
	virtual LRESULT OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //overide
	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	
	virtual LRESULT OnDrawItem(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);  //draw the listbox
	
	virtual LRESULT OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);    //Select change of the toolbar
	void    SetSelect(int tool);
	ToolStyle GetSelect() 
	{
		return m_toolSelect;
	};

	int GetIndex()
	{
		return (int)m_toolSelect;
	}
	
private:
	HWND m_wndList;           //Handle to the Listbox
	CMyListBox m_Listbox;


	ToolStyle m_toolSelect;
	int       m_current;   //current select of ListBox

private:     //Draw the listbox
	void DrawEraser(HDC hdc);
	void DrawZoom(HDC hdc);
	void DrawGun(HDC hdc);
	void DrawBrush(HDC hdc);
	void DrawLine(HDC hdc);
	void DrawRect(HDC hdc);

};
#endif
