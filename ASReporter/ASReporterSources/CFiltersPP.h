// ===========================================================================
//	CFiltersPP.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#if !defined(AFX_CFILTERSPP_H__25ACCDC2_071F_11D1_9F1E_0020AFFA70C1__INCLUDED_)
#define AFX_CFILTERSPP_H__25ACCDC2_071F_11D1_9F1E_0020AFFA70C1__INCLUDED_

//-----------------------------------------------------------------------
#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

#include "LPPage.h"

//-----------------------------------------------------------------------
class CFiltersPP : public LPropertyPage
{
public:
	CFiltersPP();
	~CFiltersPP();

	//{{AFX_DATA(CFiltersPP)
	enum { IDD = IDD_FILTER_PAGE };
	//}}AFX_DATA


protected:
	//{{AFX_VIRTUAL(CFiltersPP)
	virtual void	Start();
	virtual void	End();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFiltersPP)
	afx_msg void	OnAll();
	afx_msg void	OnNone();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------
//{{AFX_INSERT_LOCATION}}

//-----------------------------------------------------------------------
#endif // !defined(AFX_CFILTERSPP_H__25ACCDC2_071F_11D1_9F1E_0020AFFA70C1__INCLUDED_)
