/*074090 Îâ±¦ÁÖ*/
#ifndef _C_MY_DOC_H
#define _C_MY_DOC_H
#include "CMyListHdc.h"
#include "CMyView.h"

class CMyDoc
{
public:
	CMyDoc(CMyView* pView)
	{
	m_pView = pView;
	}
	HDC GetLastDC()
	{
		return m_hdcList.GetLast();
	}

	void OnInit(HDC hdc2push,RECT rect)
	{
		m_hdcList.push(m_pView->GetHWND(),hdc2push,rect);			
	}

	
	void OnFinish(HDC hdc2push,RECT rect)
	{
		m_hdcList.push(m_pView->GetHWND(),hdc2push,rect);
	}
	

	bool bCanUndo()
	{	
		return m_hdcList.bCanUndo();
	}

	bool bCanRedo()
	{
		return m_hdcList.bCanRestore();
	}

	void Undo(RECT& rect)
	{
		m_hdcList.Undo(rect);
	}

	void Redo(RECT& rect)
	{
		m_hdcList.Restore(rect);
	}
	void Clear()
	{
		m_hdcList.Clear();
	}
	~CMyDoc()
	{
		m_hdcList.Clear();
	}


private:
	CMyListHdc m_hdcList;
	CMyView*   m_pView;

};

#endif