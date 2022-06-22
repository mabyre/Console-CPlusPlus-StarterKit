//=======================================================================
// LFlatBar.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LFlatBar.h"

#include "LOVUtils.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(LFlatToolBar, CToolBar)

BEGIN_MESSAGE_MAP(LFlatToolBar, CToolBar)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
BOOL LFlatToolBar::LoadToolBar(LPCSTR aResource)
{
	if (!CToolBar::LoadToolBar(aResource))
		return FALSE;

	if (LOVUtils::IsNewComCtlInstalled())
		ModifyStyle(0, TBSTYLE_FLAT);
	
	return TRUE;
}

//-----------------------------------------------------------------------
BOOL LFlatToolBar::LoadToolBar(UINT aResourceID)
{
	return LoadToolBar(MAKEINTRESOURCE(aResourceID));
}

//-----------------------------------------------------------------------
void LFlatToolBar::OnWindowPosChanging(LPWINDOWPOS aWP)
{
//	CToolBar::OnWindowPosChanging(aWP);
	if ((GetStyle() & TBSTYLE_FLAT) && (aWP->flags & SWP_NOMOVE) == 0)
	{
		CRect	theRect;
		CWnd*	theParent = GetParent();

		// Ask my parent to repaint my old location
		GetWindowRect(&theRect);
		theParent->ScreenToClient(&theRect);
		theParent->InvalidateRect(&theRect);

		// Repaint my new position background
		RedrawWindow();
	}
}
