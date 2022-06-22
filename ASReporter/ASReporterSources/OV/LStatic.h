//=======================================================================
// LStatic.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// Attributes:
//	BOLD
//	UNDERLINE
//	ITALIC
//	FONTSIZE
//	AUTOSIZE
//=======================================================================

#ifndef LSTATIC_H
#define LSTATIC_H

//-----------------------------------------------------------------------
#include "LControl.h"

//-----------------------------------------------------------------------
class LStatic : 
	public CStatic,
	public LOVControl
{
public:
	virtual ~LStatic();

protected:
	CFont*	itsFont;
	int		itsFontSize;

	LStatic();

	virtual void 	Init();
	virtual void	DoAttribute(char* anAttribute, char* aValue);

	DECLARE_OV_BASECONTROL(LStatic)
	DECLARE_MESSAGE_MAP()

private:
	LStatic(const LStatic&);	
	LStatic& operator=(const LStatic&);	
};

//-----------------------------------------------------------------------
#endif // LSTATIC_H
