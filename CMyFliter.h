/*074090 Œ‚±¶¡÷*/
#ifndef _C_MY_Fliter_H
#define _C_MY_Fliter_H
#include "CMyWnd.h"

#include "CMyView.h"
#include "CMyDoc.h"
#include "CMyToolBar.h"

class CMyFrameWnd;
class CMyView;
class CMyDoc;
//This class intend to draw each Graph with some familiar means


class CMyFliter  
{
public:
	CMyFliter(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc)
	{
		m_pFrameWnd = pFrameWnd;
		m_pDoc		= pDoc;
		m_pView		= pView;
		m_toolFormer = CMyToolBarWithList::TOOL_PENCIL;
	}

//Message Sent by CMyView
	void OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam){};

	virtual void OnFinish(){};

	virtual RECT GetRectSel() { RECT rectClient = {0,0,0,0};return rectClient; };

	virtual void SetRectSel(const RECT& rect,bool bNeedFinish = false){};

	virtual void OnLButtonUp(WPARAM wParam,LPARAM lParam){};
	virtual void OnRButtonDown(WPARAM wParam,LPARAM lParam){};
	virtual void OnRButtonUp(WPARAM wParam,LPARAM lParam){};
	virtual void OnRButtonDBCLICK(WPARAM wParam,LPARAM lParam){}; //Receieved by Poly
	virtual void OnLButtonDBCLICK(WPARAM wParam,LPARAM lParam){}; //Receieved by Poly
	virtual void OnMouseMove(WPARAM wParam,LPARAM lParam) {};
	virtual void OnSize(WPARAM wParam,LPARAM lParam) {};//to write
	virtual void OnTimer(){};


//Message Sent by CMyFrameWnd
	void OnToolChange(CMyToolBarWithList::ToolStyle tool,CMyToolBarWithList::ToolStyle styleFormer);
	void  OnIndexChangeOfTool(int iTem);  
	
	void OnInit();
	//Some Virtual function of settling how the draw of each graph begins and ends

	void Invert();
	virtual bool CanUndo(){return true;}
	static void SetFtColor( COLORREF color);

	static void SetBkColor( COLORREF color);

	virtual void Flip(int index,const RECT& rect);
	virtual void Clear();

	virtual void Cut(){};
	virtual void Copy(){};
	virtual void Paste(){};

	virtual void Zoom(int xZoom,int yZoom,int lV,int lH,const RECT& rect);
protected:
	CMyFrameWnd*	m_pFrameWnd;
	CMyView*	m_pView;
	CMyDoc*		m_pDoc;
	CMyToolBarWithList::ToolStyle m_toolFormer;

	/* Message about current select tool */
	static CMyToolBarWithList::ToolStyle	m_tool;
	static int			m_SelectItem;
public:
	static COLORREF     m_FtColor;
	static COLORREF     m_BkColor;
	static HPEN         m_Pen;
	static HBRUSH       m_Brush;
	static HPEN         m_Pen2;
	static HBRUSH       m_Brush2;
	static	int      m_XMin;
	static  int      m_XMax;
	static  int      m_YMin;
	static  int      m_YMax;

	static void SetRectTrue(RECT rect)
	{
		m_rectTrue.bottom = rect.bottom;
		m_rectTrue.top = rect.top;
		m_rectTrue.left = rect.left;
		m_rectTrue.right = rect.right;
	}



public:
	static HDC& GetDcMem() { return m_dcMem;}
	static HBITMAP& GetBitMap() {return m_bitMap;}

public:
	static HDC			m_dcMem;        //Save mem dc
	static HDC			m_dcBitMap;		//Save the former graph
	static HBITMAP		m_bitMap;
	static int m_zoom;
	static POINT m_Start;
	static RECT m_rectTrue;
	
};



class CMyFliterZoom : public CMyFliter
{
public:
	CMyFliterZoom(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliter(pFrameWnd,pView,pDoc)
	{}
		//virtual void OnMouseMove(WPARAM wParam,LPARAM lParam);
		//virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam);
};

class CMyFliterWord : public CMyFliter
{
public:
	enum Status{NONE,MOVING,WRITING};
	CMyFliterWord(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliter(pFrameWnd,pView,pDoc)
	{
		m_status = NONE;
	}
	virtual void OnLButtonUp(WPARAM wParam,LPARAM lParam);
	virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam);
	virtual void OnMouseMove(WPARAM wParam,LPARAM lParam);
protected:
	HWND m_hEdit;
	Status m_status;
	RECT m_rectWnd;
};


class CMyFliterCut0 : public CMyFliter
{
public:
	enum CutState{NONE = 0,DRAWING,MOVING};
public:
	CMyFliterCut0(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliter(pFrameWnd,pView,pDoc)
	{
		m_State = NONE;
		m_Count = 0;
		m_XMin =  m_XMax =  m_YMin =   m_YMax =0;
	}
	virtual void OnLButtonUp(WPARAM wParam,LPARAM lParam);
	
	virtual void OnRButtonUp(WPARAM wParam,LPARAM lParam);

	virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam);

	virtual void OnRButtonDown(WPARAM wParam,LPARAM lParam);

	virtual void OnMouseMove(WPARAM wParam,LPARAM lParam);

	virtual void OnFinish();

	virtual void Flip(int index,const RECT& rect);
	
	virtual void Cut();
	virtual void Copy();
	virtual void Paste();



	virtual bool CanUndo(){return m_State == MOVING?false:true;}

	virtual RECT GetRectSel()
	{ 
		RECT rect = {m_XMin,m_YMin,m_XMax,m_YMax};
		if(rect.left == rect.right)
		{
			return m_rectTrue;
		}
		return rect;
	};


private:
	int      m_Count;
	POINT    m_Point[1000];
	POINT    m_PointBac[1000];
protected:
	HBITMAP  m_hBitMap;
	HDC		 m_hBitDc;
	CutState m_State;
	HRGN     m_RgnFormer;
	POINT    m_PointStart;

};

class CMyFliterCut1 : public CMyFliterCut0
{
public:
	CMyFliterCut1(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliterCut0(pFrameWnd,pView,pDoc)
	{
		m_State = NONE;
	}


	virtual void OnLButtonUp(WPARAM wParam,LPARAM lParam);
	
	virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam);

	virtual void OnMouseMove(WPARAM wParam,LPARAM lParam);

	virtual void Zoom(int xZoom,int yZoom,int lV,int lH,const RECT& rect);


	virtual void SetRectSel(const RECT& rect,bool bNeedFinish = false);


protected:
	RECT rect;

};

class CMyFliterCurl : public CMyFliter    //When losing focus , it's finished such as 1st of 2nd cut tool of the toolbar
{
public:
	enum CurlState{NONE = 0,DRAWING_LINE,DRAWING_1,DRAWING_2};
public:
	CMyFliterCurl(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliter(pFrameWnd,pView,pDoc)
	{
		m_State = NONE;
	}
	
	
	virtual void OnLButtonUp(WPARAM wParam,LPARAM lParam);
	
	virtual void OnRButtonUp(WPARAM wParam,LPARAM lParam);

	virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam);

	virtual void OnRButtonDown(WPARAM wParam,LPARAM lParam);

	virtual void OnMouseMove(WPARAM wParam,LPARAM lParam);
private:
	CurlState m_State;
	POINT     m_Point[4];
};

class CMyFliterPol : public CMyFliter
{
public:
	enum PolState{NONE = 0,DRAWING_LINE,DRAWING_OTHER};
	CMyFliterPol(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliter(pFrameWnd,pView,pDoc)
	{
		m_State = NONE;
		m_Count =  0;

	}
	
	

	virtual void OnLButtonUp(WPARAM wParam,LPARAM lParam);
	
	virtual void OnRButtonUp(WPARAM wParam,LPARAM lParam);

	virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam);

	virtual void OnRButtonDown(WPARAM wParam,LPARAM lParam);

	virtual void OnRButtonDBCLICK(WPARAM wParam,LPARAM lParam); // Receieved by Poly
	
	virtual void OnLButtonDBCLICK(WPARAM wParam,LPARAM lParam); // Receieved by Poly

	virtual void OnMouseMove(WPARAM wParam,LPARAM lParam);
private:
	PolState m_State;
	POINT     m_Point[100];
	int       m_Count;
};


class CMyFliterEndMouseUp    : public CMyFliter    //When mousebutton up , it's finished such as rect line ellipse and most of all
{
public:
	enum FliterState{ NONE = 0,DRAWINGL,DRAWINGR };
public:
	CMyFliterEndMouseUp(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliter(pFrameWnd,pView,pDoc)
	{
		m_State = NONE;
		m_point.x = m_point.y = 0;	
		
	};
	virtual void OnLButtonUp(WPARAM wParam,LPARAM lParam);

	virtual void OnRButtonUp(WPARAM wParam,LPARAM lParam);

	virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam);

	virtual void OnRButtonDown(WPARAM wParam,LPARAM lParam);
	
protected:
	FliterState m_State;
	POINT m_point;


};


class CMyFliterEndMouseUpGraph : public CMyFliterEndMouseUp   // line rect roundrect and ellipse
{
public:
	CMyFliterEndMouseUpGraph(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliterEndMouseUp(pFrameWnd,pView,pDoc)
	{
			
	}
	virtual void OnMouseMove(WPARAM wParam,LPARAM lParam);
protected:

	virtual void Draw(POINT start,POINT end) {};  //Draw the true Graph 


};

class CMyFliterLine : public CMyFliterEndMouseUp
{
public:
	CMyFliterLine(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc): CMyFliterEndMouseUp(pFrameWnd,pView,pDoc)
	{}
	virtual void OnMouseMove(WPARAM wParam,LPARAM lParam);
protected:
	virtual void Draw(POINT start,POINT end);
};



class CMyFliterRect : public CMyFliterEndMouseUpGraph
{
public:
	CMyFliterRect(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc): CMyFliterEndMouseUpGraph(pFrameWnd,pView,pDoc)
	{}
protected:
	virtual void Draw(POINT start,POINT end);
};

class CMyFliterRoundRect : public CMyFliterEndMouseUpGraph
{
public:
	CMyFliterRoundRect(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliterEndMouseUpGraph(pFrameWnd,pView,pDoc)
	{}
protected:
	virtual void Draw(POINT start,POINT end);
	
};

class CMyFliterEllipse : public CMyFliterEndMouseUpGraph
{
public:
	CMyFliterEllipse(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliterEndMouseUpGraph(pFrameWnd,pView,pDoc)
	{}
protected:
	virtual void Draw(POINT start,POINT end) ;

};



class CMyFliterEndMouseUpPen   : public CMyFliterEndMouseUp   // eraser bin get pen brush Gun
{
public:
	CMyFliterEndMouseUpPen(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliterEndMouseUp(pFrameWnd,pView,pDoc)
	{}

	virtual void OnMouseMove(WPARAM wParam,LPARAM lParam)
	{
		POINT pointNow = {LOWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.x,HIWORD(lParam)/CMyFliter::m_zoom + CMyFliter::m_Start.y  };
		if(  (m_State == DRAWINGL && wParam&MK_LBUTTON ) || ( m_State == DRAWINGR && wParam&MK_RBUTTON ) ) //MouseDown
		{
			Draw(pointNow);
		}
	}
protected:
	virtual void Draw(POINT pointNow){};
};

class CMyFliterEraser : public CMyFliterEndMouseUpPen
{
public:
	CMyFliterEraser(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliterEndMouseUpPen(pFrameWnd,pView,pDoc)
	{};
protected:
	virtual void Draw(POINT pointNow);
};

class CMyFliterBrush: public CMyFliterEndMouseUpPen
{
public:
	CMyFliterBrush(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliterEndMouseUpPen(pFrameWnd,pView,pDoc)
	{}
		//virtual void OnMouseMove(WPARAM wParam,LPARAM lParam);
		//virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam);
protected:
	virtual void Draw(POINT pointNow);
};

class CMyFliterBin : public CMyFliterEndMouseUpPen
{
public:
	CMyFliterBin(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliterEndMouseUpPen(pFrameWnd,pView,pDoc)
	{};
	virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam);
	virtual void OnRButtonDown(WPARAM wParam,LPARAM lParam);
	virtual void CMyFliterBin::OnMouseMove(WPARAM wParam,LPARAM lParam);
protected:
	virtual void Draw(POINT pointNow);
};

class CMyFliterGet : public CMyFliterEndMouseUp
{
public:
	CMyFliterGet(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliterEndMouseUp(pFrameWnd,pView,pDoc)
	{}
	virtual void OnLButtonUp(WPARAM wParam,LPARAM lParam);

	virtual void OnRButtonUp(WPARAM wParam,LPARAM lParam);


};


class CMyFliterPen : public CMyFliterEndMouseUpPen
{
public:
	CMyFliterPen(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliterEndMouseUpPen(pFrameWnd,pView,pDoc)
	{}
protected:
	virtual void Draw(POINT pointNow);
};

class CMyFliterGun : public CMyFliterEndMouseUpPen
{
public:
	CMyFliterGun(CMyFrameWnd* pFrameWnd,CMyView* pView,CMyDoc* pDoc) : CMyFliterEndMouseUpPen(pFrameWnd,pView,pDoc)
	{
		bIsTimer = false;
	}
	virtual void OnLButtonUp(WPARAM wParam,LPARAM lParam);
	
	

	virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam);


	virtual void OnMouseMove(WPARAM wParam,LPARAM lParam);
	virtual void OnTimer();



private:

	static VOID CALLBACK CMyFliterGun::TimerProc(HWND hwnd,  UINT uMsg, UINT_PTR idEvent,  DWORD dwTime);
	bool bIsTimer;
	POINT pointStart;
	static void  Air_Brush(HDC hdc, POINT start, int diameter, int density, COLORREF color);
};











#endif