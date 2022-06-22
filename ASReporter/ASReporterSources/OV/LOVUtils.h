//=======================================================================
// LOVUtils.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#ifndef LOVUTILS_H
#define LOVUTILS_H

//-----------------------------------------------------------------------
#include <afxwin.h>
#include "LDefines.h"

//-----------------------------------------------------------------------
class LOVUtils
{
public:

	//-------------------------------------------------------------------
	static void Draw3DFrame(CDC& aDC, CRect& aRect);
	// Draws a 3D Frame for a control. The Frame is drawn inside the Rect and is 2 pixels width

	//-------------------------------------------------------------------
	static void	DestroyCToolTipCtrl(HWND aHwnd);
	// Destroy the CToolTipCtrl attached to a CWnd. Used by LDialog, LFormView
	// And every CMainFrame class.

	//-------------------------------------------------------------------
	// returns COLORREF from 255,255,255 format
	static COLORREF	ParseRGBString(char* aString);

	// Splits a String by ',', returns the number of arguments,
	// CREATES and fills in anArray. 
	// You must use delete[] on the array after use
	static UINT SplitParameterValue(LPCSTR aString, CStringArray* anArray);

	//-------------------------------------------------------------------
	static HBITMAP	LoadPaletteAndBitmap(HINSTANCE hInstance, LPSTR lpString, HPALETTE FAR* lphPalette);
	static HPALETTE	CreateDIBPalette(LPBITMAPINFO lpbmi, LPINT lpiNumColors);

	//-------------------------------------------------------------------
	static BOOL	IsNewComCtlInstalled();
	static void GetComCtlVersion(DWORD* v1, DWORD* v2, DWORD* v3, DWORD* v4);
};

//-----------------------------------------------------------------------
#endif // LOVUTILS_H

