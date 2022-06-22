//=======================================================================
// LOVUtils.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LOVUtils.h"

#include <afxcmn.h>

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------
void LOVUtils::Draw3DFrame(CDC& aDC, CRect& aRect)
{
//	aDC.Draw3dRect(aRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHIGHLIGHT));
	CPen theFacePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNFACE));
	CPen theDarkPen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
	CPen theLightPen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHIGHLIGHT));
	CPen theBlackPen;

	theBlackPen.Attach(::GetStockObject(BLACK_PEN));

	CPen* theOldPen = aDC.SelectObject(&theLightPen);

	aDC.MoveTo(0,aRect.bottom-1);
	aDC.LineTo(aRect.right-1, aRect.bottom-1);
	aDC.LineTo(aRect.right-1, 0);

	aDC.SelectObject(&theFacePen);

	aDC.MoveTo(1,aRect.bottom-2);
	aDC.LineTo(aRect.right-2, aRect.bottom-2);
	aDC.LineTo(aRect.right-2, 0);

	aDC.SelectObject(&theDarkPen);

	aDC.MoveTo(0, aRect.bottom-2);
	aDC.LineTo(0, 0);
	aDC.LineTo(aRect.right-1, 0);

	aDC.SelectObject(&theBlackPen);

	aDC.MoveTo(1, aRect.bottom-3);
	aDC.LineTo(1, 1);
	aDC.LineTo(aRect.right-2, 1);

	aDC.SelectObject(theOldPen);
}

//-----------------------------------------------------------------------
void LOVUtils::DestroyCToolTipCtrl(HWND aHwnd)
{
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	CToolTipCtrl* pToolTip = pThreadState->m_pToolTip;
	if (pToolTip != NULL && pToolTip->GetOwner()->GetSafeHwnd() == aHwnd)
	{
		pToolTip->DestroyWindow();
		delete pToolTip;
		pThreadState->m_pToolTip = NULL;
	}
}

//-----------------------------------------------------------------------
COLORREF LOVUtils::ParseRGBString(char* aString)
{
	ASSERT(aString != 0);
	if (aString == 0)
		return RGB(0,0,0);

	UINT r = 0, g = 0, b = 0;
	r =	::atoi(aString);
	char* theString = ::strchr(aString,',');
	if (theString != 0)
	{
		g = ::atoi(theString+1);
		theString = ::strchr(theString+1,',');
		if (theString != 0)
			b = ::atoi(theString+1);
	}
	return RGB(r,g,b);
}

//-----------------------------------------------------------------------
UINT LOVUtils::SplitParameterValue(LPCSTR aString, CStringArray* anArray)
{
	ASSERT(anArray != 0); // anArray can not be 0
	ASSERT(aString != 0); //  aString can not be 0
	anArray->RemoveAll();

	if (*aString == 0)
		return 0; // Empty String

	int thePos;
	CString theString(aString);
	while ((thePos = theString.Find(',')) != -1)
	{
		anArray->Add(theString.Left(thePos));
		theString = theString.Right(theString.GetLength() - thePos - 1);
	}
	if (!theString.IsEmpty())
		anArray->Add(theString);

	return anArray->GetSize();
}

//-----------------------------------------------------------------------
HBITMAP LOVUtils::LoadPaletteAndBitmap(HINSTANCE hInstance, LPSTR lpString, HPALETTE FAR* lphPalette)
{
	HRSRC				hRsrc;
	HGLOBAL				hGlobal;
	HBITMAP				hBitmapFinal = NULL;
	LPBITMAPINFOHEADER	lpbi;
	HDC					hdc;
	int					iNumColors;
 
	if (hRsrc = ::FindResource(hInstance, lpString, RT_BITMAP))
	{
		hGlobal = ::LoadResource(hInstance, hRsrc);
		lpbi = (LPBITMAPINFOHEADER)::LockResource(hGlobal);
 
		hdc = ::GetDC(NULL);
		*lphPalette =  CreateDIBPalette ((LPBITMAPINFO)lpbi, &iNumColors);
		if (*lphPalette)
		{
			::SelectPalette(hdc,*lphPalette,FALSE);
			::RealizePalette(hdc);
		}
 
		hBitmapFinal = ::CreateDIBitmap(hdc, (LPBITMAPINFOHEADER)lpbi, (LONG)CBM_INIT, 
			(LPSTR)lpbi + lpbi->biSize + iNumColors * sizeof(RGBQUAD), (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
 
		::ReleaseDC(NULL,hdc);
		::UnlockResource(hGlobal);
		::FreeResource(hGlobal);
	}
	return (hBitmapFinal);
}
 
//-----------------------------------------------------------------------
HPALETTE LOVUtils::CreateDIBPalette(LPBITMAPINFO lpbmi, LPINT lpiNumColors)
{
	LPBITMAPINFOHEADER	lpbi;
	LPLOGPALETTE			lpPal;
	HANDLE				hLogPal;
	HPALETTE				hPal = NULL;
	int					i;
 
	lpbi = (LPBITMAPINFOHEADER)lpbmi;
	if (lpbi->biBitCount <= 8)
		*lpiNumColors = (1 << lpbi->biBitCount);
	else
		*lpiNumColors = 0;  // No palette needed for 24 BPP DIB
 
	if (*lpiNumColors)
	{
		hLogPal = ::GlobalAlloc(GHND, sizeof (LOGPALETTE) + sizeof (PALETTEENTRY) * (*lpiNumColors));
		lpPal = (LPLOGPALETTE)::GlobalLock (hLogPal);
		lpPal->palVersion = 0x300;
		lpPal->palNumEntries = *lpiNumColors;
 
		for (i = 0;  i < *lpiNumColors;  i++)
		{
			lpPal->palPalEntry[i].peRed   = lpbmi->bmiColors[i].rgbRed;
			lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue  = lpbmi->bmiColors[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}
		hPal = ::CreatePalette(lpPal);
		::GlobalUnlock(hLogPal);
		::GlobalFree(hLogPal);
	}
	return hPal;
}

//-----------------------------------------------------------------------
struct _TComCtlVersion
{
	DWORD	dwSize;
	DWORD	dwV1, dwV2, dwV3, dwV4;
};

//-----------------------------------------------------------------------
BOOL LOVUtils::IsNewComCtlInstalled()
{
	// -1 when not yet tested, 0 or 1 after
	static int sReturnValue = -1;

	if (sReturnValue < 0)
	{
		sReturnValue = 0;
		HMODULE theModule = ::GetModuleHandle("COMCTL32");
		if (theModule != 0)
		{
			FARPROC theProc = ::GetProcAddress(theModule, "InitCommonControlsEx");
			if (theProc != 0)
				sReturnValue = 1;
		}
	}

	return (BOOL)sReturnValue;
}

//-----------------------------------------------------------------------
void LOVUtils::GetComCtlVersion(DWORD* v1, DWORD* v2, DWORD* v3, DWORD* v4)
{
	static _TComCtlVersion cVersion = { 0, 0, 0, 0, 0 };

	if (cVersion.dwSize == 0)
	{
		HMODULE theModule = ::GetModuleHandle("COMCTL32");
		if (theModule != 0)
		{
			FARPROC theProc = ::GetProcAddress(theModule, "DllGetVersion");
			if (theProc != 0)
			{
				cVersion.dwSize = sizeof(_TComCtlVersion);
				((DWORD (__stdcall *)(_TComCtlVersion*))theProc)(&cVersion);
			}
		}
	}
	if (v1 != 0)
		*v1 = cVersion.dwV1;
	if (v2 != 0)
		*v2 = cVersion.dwV2;
	if (v3 != 0)
		*v3 = cVersion.dwV3;
	if (v4 != 0)
		*v4 = cVersion.dwV4;
}
