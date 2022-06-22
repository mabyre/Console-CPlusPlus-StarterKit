//=======================================================================
// LMDICWnd.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//
// Author  : RNi
//
// Class       :	LMDIChildWnd
// Superclass  :	CMDIChildWnd
//					LMinSizeWnd (if OVF_CF_SIZABLE defined)
//
// Description:
//
// Revision history :
//	22/04/1997 : Derived from LMinSizeWnd
//
//=======================================================================

#ifndef LMDICWND_H
#define LMDICWND_H

#include <afxwin.h>
#include "LDefines.h"
#ifdef OVF_CF_SIZABLE
	#include "LMSWnd.h"
#endif

//-----------------------------------------------------------------------------
class LMDIChildWnd : public CMDIChildWnd
#ifdef OVF_CF_SIZABLE
	, public LMinSizeWnd
#endif
{
public:
	LMDIChildWnd();
	virtual ~LMDIChildWnd();

	virtual BOOL	SetfCanClose(BOOL afCanClose);
	virtual BOOL	SetfCanResize(BOOL afCanResize);

protected:
	BOOL	itsfCanClose, itsfCanResize;

	afx_msg void	OnClose();
	afx_msg void	OnInitMenuPopup(CMenu* aPopupMenu, UINT anIndex, BOOL afSysMenu);
	afx_msg void	OnNcLButtonDown(UINT aHTCode, CPoint aPoint);
#ifdef OVF_CF_SIZABLE
	afx_msg void	OnGetMinMaxInfo(MINMAXINFO* aMMI);
#endif

	DECLARE_DYNCREATE(LMDIChildWnd)
	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------------
#endif // LMDICWND_H
