// ===========================================================================
//	CTipDlg.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#include "stdafx.h"
#include "resource.h" 

#include "CTipDlg.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CTipDlg, LDialog)
	ON_BN_CLICKED(IDC_TIP_NEXT, OnNext)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
static TCHAR sTipSection[] = _T("TipOfTheDay");
static TCHAR sStartupEntry[] = _T("Startup");
static TCHAR sIndexEntry[] = _T("Index");
UINT CTipDlg::cCount = (UINT)-1;

//-----------------------------------------------------------------------
CTipDlg::CTipDlg(CWnd* aParent) : 
	LDialog(IDD_TIP, aParent)
{
	itsfOnStartup = ::AfxGetApp()->GetProfileInt(sTipSection, sStartupEntry, 1);
	itsIndex = ::AfxGetApp()->GetProfileInt(sTipSection, sIndexEntry, 0);
	if (cCount == (UINT)-1)
	{
		CString theString;
		if (!theString.LoadString(IDS_TIP_COUNT))
			cCount = 0;
		else
			cCount = ::atoi(theString);
	}
}

//-----------------------------------------------------------------------
CTipDlg::~CTipDlg()
{
}

//-----------------------------------------------------------------------
BOOL CTipDlg::HasTips()
{
	return (cCount != 0);
}

//-----------------------------------------------------------------------
BOOL CTipDlg::DisplayOnStartup()
{
	return itsfOnStartup;
}

//-----------------------------------------------------------------------
void CTipDlg::Start()
{
	CheckDlgButton(IDC_TIP_STARTUP, itsfOnStartup);
	OnNext();
}

//-----------------------------------------------------------------------
void CTipDlg::End()
{
	itsfOnStartup = IsDlgButtonChecked(IDC_TIP_STARTUP);
	::AfxGetApp()->WriteProfileInt(sTipSection, sStartupEntry, itsfOnStartup);
	::AfxGetApp()->WriteProfileInt(sTipSection, sIndexEntry, itsIndex);
}

//-----------------------------------------------------------------------
void CTipDlg::OnNext()
{
	CString theString;
	if (!theString.LoadString(IDS_TIP_STRING0 + itsIndex))
		theString = "Sorry: Tip of the day not available...";

	itsIndex++;
	if (itsIndex >= cCount)
		itsIndex = 0;
	SetDlgItemText(IDC_TIP_TEXT, theString);
}
