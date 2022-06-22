// ===========================================================================
//	CAboutDlg.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#include "stdafx.h"
#include "OSReport.h"
#include "CAboutDlg.h"

#include "PMBuffer.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CAboutDlg, LDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
CAboutDlg::CAboutDlg(CWnd* pParent)
	: LDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

//-----------------------------------------------------------------------
void CAboutDlg::Start()
{
	char thePath[MAX_PATH+1];
	if (::GetModuleFileName(0, thePath, MAX_PATH+1) == 0)
		return;

	DWORD theSize, theDummy;
	theSize = ::GetFileVersionInfoSize(thePath, &theDummy);
	if (theSize == 0)
		return;

	PMBuffer theBuffer(theSize);
	void* thePtr = theBuffer.buffer();
	if (!::GetFileVersionInfo(thePath, 0, theSize, thePtr))
		return;

	UINT				theLen;
	VS_FIXEDFILEINFO*	theInfo;
	if (!::VerQueryValue(thePtr, "\\", (LPVOID*)&theInfo, &theLen))
		return;

    // In case off creation of a Build DlgBox doesnt exist anymore
	char theString[64];
	::sprintf
    (
        theString, "%d.%d.%d.%d", 
        HIWORD(theInfo->dwFileVersionMS), 
        LOWORD(theInfo->dwFileVersionMS), 
        HIWORD(theInfo->dwFileVersionLS), 
        LOWORD(theInfo->dwFileVersionLS)
    );
	SetDlgItemText(IDC_ABOUT_BUILD, theString);

    // Adding the management of dwProductVersionMS
	char theStringProductVersion[64];
	::sprintf
    (
        theStringProductVersion, "%d.%d.%d.%d", 
        HIWORD(theInfo->dwProductVersionMS), 
        LOWORD(theInfo->dwProductVersionMS), 
        HIWORD(theInfo->dwProductVersionLS), 
        LOWORD(theInfo->dwProductVersionLS)
    );
	SetDlgItemText(IDC_ABOUT_BUILD, theStringProductVersion);

	CString theCString, theCString2, theCString3;
	if (!theCString3.LoadString(AFX_IDS_APP_TITLE))
		theCString3 = "Reporter";

    // Use the first tree digit to get the product version as the following format : x.x.x
	GetDlgItemText(IDC_ABOUT_TEXT, theCString);
	theCString2.Format
    (
        theCString, 
        (LPCSTR)theCString3, 
        HIWORD(theInfo->dwProductVersionMS), 
        LOWORD(theInfo->dwProductVersionMS), 
        HIWORD(theInfo->dwProductVersionLS)
    );
	SetDlgItemText(IDC_ABOUT_TEXT, theCString2);

	GetWindowText(theCString);
	theCString2.Format(theCString, (LPCSTR)theCString3);
	SetWindowText(theCString2);
}

//-----------------------------------------------------------------------
BOOL CAboutDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (wParam == MAKELONG(IDC_ABOUT_ICON, STN_DBLCLK) && 
		(GetKeyState(VK_SHIFT) & 0x8000) && 
		(GetKeyState(VK_CONTROL) & 0x8000) && 
		(GetKeyState(VK_MENU) & 0x8000) )
		::AfxMessageBox("Written by AbyreSoft", MB_OK | MB_ICONINFORMATION);

	return LDialog::OnCommand(wParam, lParam);
}
