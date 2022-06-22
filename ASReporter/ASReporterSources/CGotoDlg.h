// ===========================================================================
//	CGotoDlg.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef CGotoDlg_h
#define CGotoDlg_h

// ---------------------------------------------------------------------------
#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

#include "LDialog.h"

// ---------------------------------------------------------------------------
class CGotoDlg : public LDialog
{
public:
	CGotoDlg(CWnd* pParent = NULL);

	//{{AFX_DATA(CGotoDlg)
	enum { IDD = IDD_GOTO };
	UINT	itsLineNumber;
	//}}AFX_DATA
	
protected:

	//{{AFX_VIRTUAL(CGotoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGotoDlg)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

// ---------------------------------------------------------------------------
//{{AFX_INSERT_LOCATION}}

// ---------------------------------------------------------------------------
#endif // CGotoDlg_h
