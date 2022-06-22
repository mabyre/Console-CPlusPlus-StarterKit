//=======================================================================
// LButton.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// Description:
//   Attributes:
//      IMAGE=id[,2 : gray img]
//      IMAGERIGHT
//      IMAGELEFT
//      IMAGETOP
//      IMAGEBOTTOM
//      TRANSCOLOR=r,g,b
//
// Revision history :
// OLa 970326 : added ChangeBmp function
//
//=======================================================================

#ifndef LBUTTON_H
#define LBUTTON_H

//-----------------------------------------------------------------------
#include "LControl.h"
class CImageList;

//-----------------------------------------------------------------------
class LButton : 
	public CButton,
	public LOVControl
{
public:
	virtual ~LButton();

	// dynamicaly changes the bitmap of the button.
	// set <aNewID> to 0 to remove the bitmap
	virtual void ChangeBmp(UINT aNewID);

protected:
	LButton();

	CImageList*	itsIL;
	UINT		itsImageID;
	CPoint		itsImagePos;
	COLORREF	itsTransClr;
	CRect		itsTextRect;
	CSize		itsBmpSize;

	virtual void	Init();
	virtual void	DoAttribute(char* anAttribute, char* aValue);

	virtual void	ManagePlacements();

	virtual void	DrawItem(LPDRAWITEMSTRUCT aDIS);
	afx_msg	void	OnSize(UINT nType, int cx, int cy);
	afx_msg	void	OnEnable(BOOL afEnable);

	DECLARE_MESSAGE_MAP()
	DECLARE_OV_BASECONTROL(LButton)
};

//-----------------------------------------------------------------------
#endif // LBUTTON_H
