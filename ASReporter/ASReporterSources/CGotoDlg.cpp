// ===========================================================================
//	CGotoDlg.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#include "stdafx.h"
#include "OSReport.h"
#include "CGotoDlg.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CGotoDlg, LDialog)
	//{{AFX_MSG_MAP(CGotoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
CGotoDlg::CGotoDlg(CWnd* pParent)
	: LDialog(CGotoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGotoDlg)
	itsLineNumber = 0;
	//}}AFX_DATA_INIT
}

//-----------------------------------------------------------------------
void CGotoDlg::DoDataExchange(CDataExchange* pDX) 
{
	//{{AFX_DATA_MAP(CGotoDlg)
	DDX_Text(pDX, IDC_GOTO_LINE, itsLineNumber);
	DDV_MinMaxUInt(pDX, itsLineNumber, 0, 0xFFFFFFFF);
	//}}AFX_DATA_MAP
	
	LDialog::DoDataExchange(pDX);
}
