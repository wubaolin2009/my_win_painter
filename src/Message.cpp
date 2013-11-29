/*074090 Îâ±¦ÁÖ*/
#include "Message.h"
#include "CWnd.h"

LRESULT CALLBACK WndProcGlobal(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPCREATESTRUCT lpCreate = (LPCREATESTRUCT)lParam;
	if( message == WM_CREATE)
	{
		return ( (CMyWnd*) (lpCreate->lpCreateParams) )->OnCreate(hWnd,message,wParam,lParam);
	}

	for(int i = 0;i<g_MessageMap.size();i++)
	{
		if( g_MessageMap[i].hwnd == hWnd)
		{
			return g_MessageMap[i].pWnd->WndProc(hWnd,message,wParam,lParam);
		}
	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}

