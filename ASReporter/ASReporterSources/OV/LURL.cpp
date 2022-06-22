//=======================================================================
// LURL.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LURL.h"

#include "LOVUtils.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_OV_SUBCONTROL(LURL, LStatic, "LURL")

BEGIN_MESSAGE_MAP(LURL, LStatic)
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONUP()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
#define kSMS_URL_THREADED		0x0010

//-----------------------------------------------------------------------
struct TLaunchParams
{
	CString itsURL;
	HWND	itsHwnd;
};

//-----------------------------------------------------------------------
LURL::LURL() : 
	LStatic()
{
	itsCursor = 0;
	itsCursorID = 0;
	itsColor = RGB(0,0,192);
	itsClickedColor = RGB(192,0,0);
}

//-----------------------------------------------------------------------
LURL::~LURL()
{
}

//-----------------------------------------------------------------------
void LURL::DoAttribute(char* anAttribute, char* aValue)
{
	if (::strcmp(anAttribute,"CURSOR") == 0)
		itsCursorID = (UINT)::atoi(aValue);
	else
	if (::strcmp(anAttribute,"COLOR") == 0)
		itsColor = LOVUtils::ParseRGBString(aValue);
	else
	if (::strcmp(anAttribute,"CLICKEDCOLOR") == 0)
		itsClickedColor = LOVUtils::ParseRGBString(aValue);
	else
	if (::strcmp(anAttribute,"THREADED") == 0)
		itsOVStyle |= kSMS_URL_THREADED;
	else
	if (::strcmp(anAttribute,"URL") == 0)
		itsURL = aValue;
	else
		LStatic::DoAttribute(anAttribute, aValue);
}

//-----------------------------------------------------------------------
void LURL::Init()
{
	if (itsCursorID != 0)
		itsCursor = AfxGetApp()->LoadCursor(itsCursorID);

	if (itsURL.IsEmpty())
		GetWindowText(itsURL);

	LStatic::Init();
}

//-----------------------------------------------------------------------
UINT LURL::LaunchBrowser(LPVOID aParam)
{
	TLaunchParams* theParams = (TLaunchParams*)aParam;

	::ShellExecute(theParams->itsHwnd, "open", (LPCSTR)theParams->itsURL, 0, 0, SW_SHOW);
	delete theParams;

	return 0;
}

//-----------------------------------------------------------------------
void LURL::OnLButtonUp(UINT aFlags, CPoint aPoint)
{
	if (GetWindowTextLength() == 0)
		return;

	TLaunchParams* theParams = new TLaunchParams;
	theParams->itsURL = itsURL;
	theParams->itsHwnd = GetSafeHwnd();

	OSVERSIONINFO	theVersion;
	theVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (!::GetVersionEx(&theVersion))
		theVersion.dwPlatformId = VER_PLATFORM_WIN32s;

	if ((itsOVStyle & kSMS_URL_THREADED) && theVersion.dwPlatformId != VER_PLATFORM_WIN32s)
		::AfxBeginThread(LaunchBrowser, theParams);
	else
		LaunchBrowser(theParams);

	itsColor = itsClickedColor;
	Invalidate();
}

//-----------------------------------------------------------------------
BOOL LURL::OnSetCursor(CWnd* aWnd, UINT aHitTest, UINT aMessage)
{
	if (aHitTest == HTCLIENT && itsCursor != 0)
	{
		::SetCursor(itsCursor);
		return TRUE;
	}

	return LStatic::OnSetCursor(aWnd, aHitTest, aMessage);
}

//-----------------------------------------------------------------------
HBRUSH LURL::CtlColor(CDC* aDC, UINT aCtlColor) 
{
	HBRUSH theBrush = (HBRUSH)GetParent()->SendMessage(WM_CTLCOLORDLG,(WPARAM)aDC->GetSafeHdc(), (LPARAM)GetParent()->GetSafeHwnd());
	aDC->SetTextColor(itsColor);

	return theBrush;
}

//-----------------------------------------------------------------------
// LStatic::OnNcHitTest always replies HTTRANSPARENT that 
// prevents control to receive mouse messages
UINT LURL::OnNcHitTest(CPoint)
{
	return HTCLIENT;
}
