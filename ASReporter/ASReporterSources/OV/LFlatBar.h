//=======================================================================
// LFlatBar.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// Manages the flat style for toolbars. This is not a CoolBar.
// The modification (ModifyStyle(0, TBSTYLE_FLAT)) is done is LoadToolBar.
// This style cannot be passed to CToolBar::Create. It must be set after.
// If you do not use LoadToolBar to load your bar you will have to change
// the style by yourself.
//=======================================================================

#ifndef LFLATBAR_H
#define LFLATBAR_H

//-----------------------------------------------------------------------
#include <afxext.h>

#ifndef TBSTYLE_FLAT
	// If your commctrl.h defines TBSTYLE_FLAT, it MUST be the same.
	#define TBSTYLE_FLAT	0x0800
#endif

//-----------------------------------------------------------------------
class LFlatToolBar : public CToolBar
{
public:
	BOOL LoadToolBar(LPCSTR aResource);
	BOOL LoadToolBar(UINT aResourceID);

protected:
	afx_msg void OnWindowPosChanging(LPWINDOWPOS aWP);

	DECLARE_DYNAMIC(LFlatToolBar)
	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------
#endif // LFLATBAR_H
