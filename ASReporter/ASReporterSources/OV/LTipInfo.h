//=======================================================================
// LTipInfo.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// If the Tip String begins with character ] then it is centered under the control
// The tip may be followed by ~ and a text for the status bar.
// If you want to have a ~ in your tip text escape it (~~)
//=======================================================================

#ifndef LTIPINFO_H
#define LTIPINFO_H

//-----------------------------------------------------------------------
#include <afxwin.h>
#include "LDefines.h"

//-----------------------------------------------------------------------
class LTipInfo
{
public:
	LTipInfo();
	virtual ~LTipInfo();

	virtual void	SetCenter(BOOL afCentered = TRUE);
	virtual void	SetString(LPCSTR aTipString = 0);

	virtual LPCSTR	GetTipString();
	virtual LPCSTR	GetMessageString();
	virtual BOOL	GetCenter();

protected:
	char*	itsTipString;
	char*	itsMessageString;
	BOOL	itsfCentered;

private:
	LTipInfo(const LTipInfo&);
	LTipInfo& operator=(const LTipInfo&);
};

//-----------------------------------------------------------------------
#endif // LTIPINFO_H
