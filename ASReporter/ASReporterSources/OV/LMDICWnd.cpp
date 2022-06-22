//=======================================================================
// LMDICWnd.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LMDICWnd.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(LMDIChildWnd, CMDIChildWnd)

BEGIN_MESSAGE_MAP(LMDIChildWnd, CMDIChildWnd)
	ON_WM_CLOSE()
	ON_WM_SIZE()
#ifdef OVF_CF_SIZABLE
	ON_WM_GETMINMAXINFO()
#endif
	ON_WM_INITMENUPOPUP()
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
LMDIChildWnd::LMDIChildWnd() :
	CMDIChildWnd()
#ifdef OVF_CF_SIZABLE
	, LMinSizeWnd()
#endif
{
	itsfCanClose = itsfCanResize = TRUE;
}

//-----------------------------------------------------------------------------
LMDIChildWnd::~LMDIChildWnd()
{
}

//-----------------------------------------------------------------------------
BOOL LMDIChildWnd::SetfCanClose(BOOL afCanClose)
{
	BOOL thefOld = itsfCanClose;
	itsfCanClose = afCanClose;
	if (!itsfCanClose)
	{
		CMenu* theMenu = GetSystemMenu(FALSE);
		theMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);
	}

	return thefOld;
}

//-----------------------------------------------------------------------------
BOOL LMDIChildWnd::SetfCanResize(BOOL afCanResize)
{
	BOOL thefOld = itsfCanResize;
	itsfCanResize = afCanResize;
	if (!itsfCanResize)
		ModifyStyle(WS_MAXIMIZEBOX, 0);
	
	return thefOld;
}

//-----------------------------------------------------------------------------
void LMDIChildWnd::OnClose()
{
	if (!itsfCanClose)
		return;

	CMDIChildWnd::OnClose();
}

//-----------------------------------------------------------------------------
void LMDIChildWnd::OnInitMenuPopup(CMenu* aPopupMenu, UINT anIndex, BOOL afSysMenu)
{
	CMDIChildWnd::OnInitMenuPopup(aPopupMenu, anIndex, afSysMenu);

	// We must explicitly dims the menu items each time the menu is popped (why ???)
	if (afSysMenu)
	{
		aPopupMenu->EnableMenuItem(SC_SIZE,(itsfCanResize?MF_ENABLED:MF_GRAYED) | MF_BYCOMMAND);
		aPopupMenu->EnableMenuItem(SC_MAXIMIZE,(itsfCanResize?MF_ENABLED:MF_GRAYED) | MF_BYCOMMAND);
	}
}

//-----------------------------------------------------------------------------
void LMDIChildWnd::OnNcLButtonDown(UINT aHTCode, CPoint aPoint)
{
	if (itsfCanResize || 
		(aHTCode != HTBOTTOMLEFT && aHTCode != HTBOTTOMRIGHT && 
		 aHTCode != HTTOPLEFT && aHTCode != HTTOPRIGHT &&
		 aHTCode != HTLEFT && aHTCode != HTRIGHT &&
		 aHTCode != HTTOP && aHTCode != HTBOTTOM))
		CMDIChildWnd::OnNcLButtonDown(aHTCode, aPoint);
}

#ifdef OVF_CF_SIZABLE
//-----------------------------------------------------------------------------
void LMDIChildWnd::OnGetMinMaxInfo(MINMAXINFO* aMMI)
{
	DoGetMinMaxInfo(aMMI);

	CMDIChildWnd::OnGetMinMaxInfo(aMMI);
}
#endif
