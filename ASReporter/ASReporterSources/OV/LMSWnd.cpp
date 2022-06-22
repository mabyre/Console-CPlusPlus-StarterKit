//=======================================================================
// LMSWnd.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LMSWnd.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
LMinSizeWnd::LMinSizeWnd()
{
	itsfManageMinSize = FALSE;
	itsMinSize = CSize(-1,0);
}

//-----------------------------------------------------------------------------
LMinSizeWnd::~LMinSizeWnd()
{
}

//-----------------------------------------------------------------------------
BOOL LMinSizeWnd::SetfManageMinSize(BOOL afManageMinSize)
{
	ASSERT(itsMinSize.cx >= 0);
	BOOL thefOld = itsfManageMinSize;
	itsfManageMinSize = afManageMinSize;
	return thefOld;
}

//-----------------------------------------------------------------------------
void LMinSizeWnd::RememberMinSizeNow(CWnd* aWnd, UINT aCx, UINT aCy)
{
	CRect theRect;
	aWnd->GetWindowRect(&theRect);
	itsMinSize = theRect.Size();
	itsMinSize.cx = (itsMinSize.cx * aCx) / 100;
	itsMinSize.cy = (itsMinSize.cy * aCy) / 100;
	itsfManageMinSize = TRUE;
}

//-----------------------------------------------------------------------------
void LMinSizeWnd::DoGetMinMaxInfo(MINMAXINFO* aMMI)
{
	if (itsfManageMinSize)
	{
		aMMI->ptMinTrackSize.x = itsMinSize.cx;
		aMMI->ptMinTrackSize.y = itsMinSize.cy;
	}
}
