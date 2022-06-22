// ===========================================================================
//	CChildFrame.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#include "stdafx.h"
#include "OSReport.h"
#include "CChildFrame.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
BOOL CChildFrame::sfMDIMaximized = TRUE;

//-----------------------------------------------------------------------
CChildFrame::CChildFrame()
{
	itsfManageMaximized = FALSE;
}

//-----------------------------------------------------------------------
CChildFrame::~CChildFrame()
{
}

//-----------------------------------------------------------------------
void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);

	if (itsfManageMaximized)
	{
		BOOL thefMaximized = sfMDIMaximized;
		sfMDIMaximized = (nType == SIZE_MAXIMIZED);
		if (thefMaximized != sfMDIMaximized)
			::AfxGetApp()->WriteProfileInt(COSReportApp::sSettingsSection, COSReportApp::sMDIMaximizedEntry, sfMDIMaximized);
	}
}

//-----------------------------------------------------------------------
BOOL CChildFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext))
		return FALSE;

	if (sfMDIMaximized)
		MDIMaximize();

	itsfManageMaximized = TRUE;
	return TRUE;
}
