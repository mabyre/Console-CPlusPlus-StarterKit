// ===========================================================================
//	CAboutDlg.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#if !defined(AFX_CABOUTDLG_H__EAA45CFA_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_)
#define AFX_CABOUTDLG_H__EAA45CFA_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_

// ---------------------------------------------------------------------------
#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

#include "LDialog.h"

// ---------------------------------------------------------------------------
class CAboutDlg : public LDialog
{
public:
	CAboutDlg(CWnd* pParent = NULL);

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

protected:

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void Start();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

// ---------------------------------------------------------------------------
//{{AFX_INSERT_LOCATION}}

// ---------------------------------------------------------------------------
#endif // !defined(AFX_CABOUTDLG_H__EAA45CFA_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_)
