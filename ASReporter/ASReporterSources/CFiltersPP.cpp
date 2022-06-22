// ===========================================================================
//	CFiltersPP.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#include "stdafx.h"
#include "OSReport.h"
#include "CFiltersPP.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CFiltersPP, LPropertyPage)
	ON_BN_CLICKED(IDC_FP_ALL, OnAll)
	ON_BN_CLICKED(IDC_FP_NONE, OnNone)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
CFiltersPP::CFiltersPP() :
	LPropertyPage(IDD_FILTER_PAGE, TRUE)
{
}

//-----------------------------------------------------------------------
CFiltersPP::~CFiltersPP()
{
}

//-----------------------------------------------------------------------
void CFiltersPP::Start()
{
	CCheckListBox* theList = new CCheckListBox;
	theList->SubclassDlgItem(IDC_FILTER_LIST, this);
}

//-----------------------------------------------------------------------
void CFiltersPP::End()
{
	CCheckListBox* theList = (CCheckListBox*)GetDlgItem(IDC_FILTER_LIST);
	ASSERT(theList != 0);
	// This is very important to allow CCheckListBox to cleanup internal data 
	// structures (AFX_CHECK_STATE). Memory leaks if not done (WinCtrl3.cpp).
	theList->ResetContent();
	theList->UnsubclassWindow();
	delete theList;
}

//-----------------------------------------------------------------------
void CFiltersPP::OnAll()
{
	CCheckListBox* theList = (CCheckListBox*)GetDlgItem(IDC_FILTER_LIST);
	for (int i = 0; i < theList->GetCount(); i++)
		theList->SetCheck(i, 1);
}

//-----------------------------------------------------------------------
void CFiltersPP::OnNone()
{
	CCheckListBox* theList = (CCheckListBox*)GetDlgItem(IDC_FILTER_LIST);
	for (int i = 0; i < theList->GetCount(); i++)
		theList->SetCheck(i, 0);
}
