/*074090 吴宝林*/
/* This head file is about CMyApp's declaration and the basic frame of message handling */
/* Author 074090            made  on Oct21st ,2008										*/

#ifndef _C_MY_APP_H
#define _C_MY_APP_H
#include<windows.h>
#include"CMyWnd.h"
//#include"CMyFrameWnd.h"
#include"List.h"


/* Some Macros  about message, You should add this Macros to each class in order to register this message to your wndclass 's WndProc Function*/
/* For example:
... ClassName::WndProc(...)
{
	
	BEGIN_MY_MESSAGE
		MY_MESSAGE(WM_PAINT,OnPaint)
		MY_MESSAGE(WM_DESTORY,OnDestroy)
	END_MY_MESSAGE
}
Since it's just a very simple application frame, You can't Inherit the MessageHandler manually, if you want to Inherit it,
just  Declare it more, Sorry. But I don't think It's Necessary to inherit message handler, so You should place your WndProc in 
public area while Each handle to Private Area, It's Recommended
*/
   
#define BEGIN_MY_MESSAGE switch(message){
#define MY_MESSAGE(messagefor,handle)\
case messagefor:\
	return this->handle(hWnd, message, wParam,lParam);\
	break;
#define END_MY_MESSAGE \
default:\
	return ::DefWindowProc(hWnd,message, wParam,lParam);}

class CMyWnd;

struct MessageMap   
{
	HWND hwnd;
	CMyWnd* pWnd;  // This intend to make different fucntion for different class devired from CMyWnd
};


struct FuncMap
{
	int message;
	WNDPROC pfn;
};


class CMyApp
{
public:
	//This constructor Set zero to all the variales in this class
	CMyApp();

	//This intend to prepare something before you run the application
	void InitApplication(HINSTANCE hInst,CMyWnd* wndMain);

	//This could be override in you application to Specify what you application will looks like
	//Its defualt 
	virtual void InitInstance();

	//This is used to transport message handling the last step for your application when it's returns your application is over
	virtual LRESULT Run(); 

	// These Set of Functions is used to get or set the attributes of this class

	HINSTANCE GetInst()
	{
		return m_hInst;
	};

	CMyWnd* GetCWnd()
	{return m_myWnd;}

	//Will be called by CMyWnd::Create after the window is created 
	static void AddMessage(HWND hWnd,CMyWnd* pWnd);
	
	//This Func act as the global function for message handle
	//It send message to the exact window to handle it
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//If a derived class want to settle about Idle time , Override it 这里用作计时器
	virtual void OnIdle();


private:
	HINSTANCE m_hInst;
	// This is the main window of the application
	CMyWnd*   m_myWnd;
	static CMyList<MessageMap> ms_MessageMap;
};




#endif