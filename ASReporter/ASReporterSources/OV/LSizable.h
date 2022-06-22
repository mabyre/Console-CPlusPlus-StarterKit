//=======================================================================
// LSizable.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#ifndef LSIZABLE_H
#define LSIZABLE_H

#include <afxwin.h>
#include "LDefines.h"

//-----------------------------------------------------------------------------
class LSizable
{
public:
	struct TSizableCtl
	{
		HWND	itsHWnd;
		CRect	itsInitRect;
		CSize	itsTrans;
		CSize	itsHom;
	};

	LSizable();
	virtual ~LSizable();

	void	AddSizableCtl(const TSizableCtl& aSCtl);

	void	InitAllCtlsPos();
	void	ReInitCtlPos(CWnd* aCtl);

	BOOL	UseMinSize(BOOL afUse = TRUE);

protected:
	CSize		itsInitSize;
	CPtrList	itsSizableCtls;
	BOOL		itsfUseMinSize;

	void	ManageOnSize(int aCx, int aCy);
};

//-----------------------------------------------------------------------------
#endif // LSIZABLE_H
