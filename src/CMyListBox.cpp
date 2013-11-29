/*074090 Œ‚±¶¡÷*/
#include"CMyListBox.h"
#include<windows.h>

DWORD CMyListBox::ms_ID = 0xE0000000;

void CMyListBox::CreateEx()
{
	CMyWnd::CreateEx();
	m_createStruct.dwExStyle = WS_EX_CLIENTEDGE;
	m_createStruct.style = WS_CHILDWINDOW|LBS_OWNERDRAWFIXED|LBS_NOTIFY|WS_VISIBLE| LBS_MULTICOLUMN;
	m_createStruct.hMenu = (HMENU)ms_ID++;
}