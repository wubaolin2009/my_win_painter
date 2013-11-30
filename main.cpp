/*074090 Œ‚±¶¡÷*/
#include <windows.h>
#include "CMyWnd.h"
#include "List.h"
#include "Message.h"
#include "CMyFrameWnd.h"

CMyApp g_App;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
        
                   PSTR szCmdLine, int iCmdShow)
{
	
	CMyFrameWnd me;
	g_App.InitApplication(hInstance,&me);
	g_App.InitInstance();
	return (int)g_App.Run();	
}










