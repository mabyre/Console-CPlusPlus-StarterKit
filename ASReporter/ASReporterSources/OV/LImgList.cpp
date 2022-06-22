//=======================================================================
// LImgList.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LImgList.h"

#include "LOVUtils.h"

//-----------------------------------------------------------------------
LImageList::LImageList() : 
	CImageList()
{
	itsColorBits = 0;
	itsfUsePalette = itsfUseMask = FALSE;
	itsDefaultMaskColor = RGB(0,0,0);
	itsPalette = 0;
}

//-----------------------------------------------------------------------
LImageList::~LImageList()
{
	if (itsPalette != 0)
		delete itsPalette;
}

//-----------------------------------------------------------------------
int LImageList::Add(UINT anID, UINT aCount, COLORREF aTransColor)
{
	int		theResult = -1;
	HBITMAP	theBitmap;

	if (!itsfUsePalette)
	{
		theBitmap = ::LoadBitmap(::AfxGetResourceHandle(), MAKEINTRESOURCE(anID));
		if (!theBitmap)
		{
			TRACE1("LImageList::Add(): Cannot load bitmap: ID=%d\n", anID);
			return -1;
		}
	}
	else
	{
		HPALETTE	thePalette = NULL;
		theBitmap = LOVUtils::LoadPaletteAndBitmap(::AfxGetResourceHandle(), MAKEINTRESOURCE(anID), &thePalette);
		if (!theBitmap)
		{
			TRACE1("LImageList::Add(): Cannot load bitmap: ID=%d\n", anID);
			return -1;
		}
		if (itsPalette != 0)
			TRACE1("LImageList::Add(): Palette already exists when loading bitmap ID=%d. Using existing.\n", anID);
		else
		{
			itsPalette = new CPalette();
			itsPalette->Attach(thePalette);
		}
	}

	BOOL thefCreated = TRUE;
	if (m_hImageList == 0)
	{
		BITMAP theBM;
		::GetObject(theBitmap, sizeof(theBM), &theBM);
		UINT theWidth = theBM.bmWidth / aCount;
		UINT theHeight = theBM.bmHeight;

		thefCreated = CImageList::Create(theWidth, theHeight, ComputeCreateFlag(), aCount, 1);
	}

	if (thefCreated)
	{
		COLORREF theColor = aTransColor;
		if (aTransColor == (COLORREF)-1)
			theColor = itsDefaultMaskColor;

		theResult = CImageList::Add(CBitmap::FromHandle(theBitmap), theColor);
	}

	::DeleteObject(theBitmap);

	return theResult;
}

//-----------------------------------------------------------------------
int LImageList::Add(HICON anIcon)
{
	if (anIcon == 0)
	{
		TRACE0("LImageList::Add(): Null Icon\n");
		return -1;
	}

	BOOL thefCreated = TRUE;
	if (m_hImageList == 0)
	{
		ICONINFO theII;
		if (!::GetIconInfo(anIcon, &theII))
		{
			TRACE0("LImageList::Add(): Cannot get icon info\n");
			return FALSE;
		}

		BITMAP theBM;
		UINT theHeight;
		if (theII.hbmColor != 0)
		{
			if (!::GetObject(theII.hbmColor, sizeof(theBM), &theBM))
			{
				TRACE0("LImageList::Add(): Cannot get icon info\n");
				return -1;
			}
			theHeight = theBM.bmHeight;
		}
		else
		{
			if (!::GetObject(theII.hbmMask, sizeof(theBM), &theBM))
			{
				TRACE0("LImageList::Add(): Cannot get icon info\n");
				return -1;
			}
			theHeight = theBM.bmHeight / 2;
		}

		thefCreated = CImageList::Create(theBM.bmWidth, theHeight, ComputeCreateFlag(), 1, 1);
	}

	int theResult = -1;
	if (thefCreated)
		theResult = CImageList::Add(anIcon);

	return theResult;
}

//-----------------------------------------------------------------------
UINT LImageList::GetWidth()
{
	IMAGEINFO theII;

	if (!GetImageInfo(0, &theII))
		return 0;

	return theII.rcImage.right - theII.rcImage.left;
}

//-----------------------------------------------------------------------
UINT LImageList::GetHeight()
{
	IMAGEINFO theII;

	if (!GetImageInfo(0, &theII))
		return 0;

	return theII.rcImage.bottom - theII.rcImage.top;
}

//-----------------------------------------------------------------------
void LImageList::SetColorBits(UINT aColorBits)
{
	ASSERT(m_hImageList == NULL); // LImageList::SetColorBits() must be called before creating the ImageList

	itsColorBits = aColorBits;
}

//-----------------------------------------------------------------------
void LImageList::SetUseMask(BOOL afUseMask)
{
	ASSERT(m_hImageList == NULL); // LImageList::SetUseMask() must be called before creating the ImageList

	itsfUseMask = afUseMask;
}

//-----------------------------------------------------------------------
void LImageList::SetUsePalette(BOOL afUsePalette)
{
	ASSERT(m_hImageList == NULL); // LImageList::SetUsePalette() must be called before creating the ImageList

	itsfUsePalette = afUsePalette;
}

//-----------------------------------------------------------------------
void LImageList::SetDefaultMaskColor(COLORREF aColor)
{
	SetUseMask(TRUE);
	itsDefaultMaskColor = aColor;
}

//-----------------------------------------------------------------------
UINT LImageList::ComputeCreateFlag()
{
	UINT theCreateFlag;

	if (itsfUseMask)
		theCreateFlag = ILC_MASK;
	else
		theCreateFlag = 0;

	switch (itsColorBits)
	{
		case 0:
			theCreateFlag |= ILC_COLORDDB;
			break;

		case 1:
			theCreateFlag |= ILC_COLOR;
			break;

		case 4:
			theCreateFlag |= ILC_COLOR4;
			break;

		case 8:
			theCreateFlag |= ILC_COLOR8;
			break;

		case 16:
			theCreateFlag |= ILC_COLOR16;
			break;

		case 24:
			theCreateFlag |= ILC_COLOR24;
			break;

		case 32:
			theCreateFlag |= ILC_COLOR32;
			break;

		default:
			theCreateFlag |= ILC_COLOR;
			TRACE1("LImageList::ComputeCreateFlag(): Invalid color bits: %d, using default\n", itsColorBits);
			break;
	}

	return theCreateFlag;
}

//-----------------------------------------------------------------------
BOOL LImageList::XDraw(CDC* aDC, int anImage, POINT aPoint, UINT aStyle)
{
	CPalette* theOldPalette = 0;
	if (itsPalette != 0)
	{
//		itsPalette->UnrealizeObject();
		theOldPalette = aDC->SelectPalette(itsPalette, TRUE);
		aDC->RealizePalette();
	}

	BOOL theResult = CImageList::Draw(aDC, anImage, aPoint, aStyle);

//	if (itsPalette != 0)
//		aDC->SelectPalette(theOldPalette, FALSE);

	return theResult;
}
