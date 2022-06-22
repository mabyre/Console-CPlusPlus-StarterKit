//=======================================================================
// LImgList.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#ifndef LImgList_H
#define LImgList_H

//-----------------------------------------------------------------------
#include <afxcmn.h>

//-----------------------------------------------------------------------
class LImageList : 
	public CImageList
{
public:
	LImageList();
	virtual ~LImageList();

	int		Add(UINT anID, UINT aCount = 1, COLORREF aTransColor = (COLORREF)-1);
	int		Add(HICON anIcon);

	UINT	GetWidth();
	UINT	GetHeight();

	// 0,1,4,8,16,24,32: 0 = ILC_DDB, 1 = ILC_COLOR, n = ILC_COLORn. See ImageList_Create
	void	SetColorBits(UINT aColorBits);
	void	SetUseMask(BOOL afUseMask);
	void	SetDefaultMaskColor(COLORREF aColor);
	void	SetUsePalette(BOOL afUsePalette);

	virtual BOOL	XDraw(CDC* aDC, int anImage, POINT aPoint, UINT aStyle);

protected:
	UINT		itsColorBits;
	BOOL		itsfUseMask, itsfUsePalette;
	COLORREF	itsDefaultMaskColor;
	CPalette*	itsPalette;

	UINT		ComputeCreateFlag();

private:
	LImageList(const LImageList&);
	LImageList& operator=(const LImageList&);
};

//-----------------------------------------------------------------------
#endif // LImgList_H
