//=======================================================================
// LMSWnd.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// This class is designed to be subclassed in a multiple subclass together with a CWnd
// It manages the min sizing. 
// The Subclass must call DoGetMinMaxInfo in OnGetMinMaxInfo.
// You must call RememberMinSizeNow to start managing min size. 
// aCX and aCy are percentage of the current size that is the min size
// It is used by LMDICWnd, LDialog and LSDIFrm
//=======================================================================

#ifndef LMSWND_H
#define LMSWND_H

#include <afxwin.h>
#include "LDefines.h"

//-----------------------------------------------------------------------------
class LMinSizeWnd
{
public:
	LMinSizeWnd();
	virtual ~LMinSizeWnd();

	virtual void	RememberMinSizeNow(CWnd* aWnd, UINT aCx = 100, UINT aCy = 100);
	virtual BOOL	SetfManageMinSize(BOOL afManageMinSize);

protected:
	BOOL	itsfManageMinSize;
	CSize	itsMinSize;

	void	DoGetMinMaxInfo(MINMAXINFO* aMMI);
};

//-----------------------------------------------------------------------------
#endif // LMSWND_H
