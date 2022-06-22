//=======================================================================
// LHLine.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// Draw an Horizontal Line in a Dialog. Parameters are:
// - NO3D for non-3D dialogs (should not use IN and OUT)
// - OUT for 3D dialogs
// - CENTER keeps the size of the resource and centers it
//=======================================================================

#ifndef LHLINE_H
#define LHLINE_H

//-----------------------------------------------------------------------
#include "LControl.h"

//-----------------------------------------------------------------------
class LHLine : 
	public CWnd,
	public LOVControl
{
public:
	virtual ~LHLine();

	virtual void Adjust();

protected:
	LHLine();

	virtual void	Init();
	virtual void	DoAttribute(char* anAttribute, char* aValue);

	afx_msg void	OnPaint();
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);

	DECLARE_OV_BASECONTROL(LHLine)
	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------
#endif //LHLINE_H
