// ===========================================================================
//	CFiltersDlg.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#if !defined(AFX_CFILTERSDLG_H__25ACCDC1_071F_11D1_9F1E_0020AFFA70C1__INCLUDED_)
#define AFX_CFILTERSDLG_H__25ACCDC1_071F_11D1_9F1E_0020AFFA70C1__INCLUDED_

// ---------------------------------------------------------------------------
#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

#include "LDialog.h"

// ---------------------------------------------------------------------------
class CFiltersDlg : public LDialog
{
public:
	CFiltersDlg(CTraceConfigRef aConfig, pmbool afDocFilters, CWnd* pParent = NULL);

	//{{AFX_DATA(CFiltersDlg)
	enum { IDD = IDD_FILTERS };
	//}}AFX_DATA

protected:
	CTraceConfigRef	itsConfig;
	pmbool			itsfDocFilters;

	//{{AFX_VIRTUAL(CFiltersDlg)
	virtual void Start();
	virtual void OnOK();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFiltersDlg)
	afx_msg void OnFiltersAll();
	afx_msg void OnFiltersNone();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

// ---------------------------------------------------------------------------
//{{AFX_INSERT_LOCATION}}

// ---------------------------------------------------------------------------
#endif // !defined(AFX_CFILTERSDLG_H__25ACCDC1_071F_11D1_9F1E_0020AFFA70C1__INCLUDED_)
