// ===========================================================================
//	CFiltersDlg.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#include "stdafx.h"
#include "OSReport.h"

#include "CFiltersDlg.h"
#include "CFiltersPP.h"

#include "LTab.h"
#include "LPPage.h"

#include "CTrGroups.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CFiltersDlg, LDialog)
	//{{AFX_MSG_MAP(CFiltersDlg)
	ON_BN_CLICKED(IDC_FILTERS_ALL, OnFiltersAll)
	ON_BN_CLICKED(IDC_FILTERS_NONE, OnFiltersNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
CFiltersDlg::CFiltersDlg(CTraceConfigRef aConfig, pmbool afDocFilters, CWnd* pParent)
	: LDialog(CFiltersDlg::IDD, pParent)
{
	ASSERT(!aConfig.IsNull());

	itsConfig = aConfig;
	itsfDocFilters = afDocFilters;
	//{{AFX_DATA_INIT(CFiltersDlg)
	//}}AFX_DATA_INIT
}

//-----------------------------------------------------------------------
void CFiltersDlg::Start()
{
	LTab* theTab = (LTab*)GetDlgItem(IDC_FILTERS_TAB);
	ASSERT(theTab != 0);

	CTraceGroupRef	theGroup;
	CTraceEntryRef	theEntry;
	CPropertyPage*	thePage;
	CCheckListBox*	theList;
	int				theItem;

	for (size_t i = 0; i < itsConfig->GetGroupCount(); i++)
	{
		theGroup = itsConfig->GetGroup(i);

		thePage = new CFiltersPP;
//		thePage->Create(IDD_FILTER_PAGE, theTab);
		thePage->Create(IDD_FILTER_PAGE, this);

		thePage->SetWindowText(theGroup->GetName());
		theList = (CCheckListBox*)thePage->GetDlgItem(IDC_FILTER_LIST);
		ASSERT(theList != 0);
		for (size_t j = 0; j < theGroup->GetCount(); j++)
		{
			theEntry = theGroup->GetTraceEntry(j);
			if ((theItem = theList->AddString(theEntry->GetName()->c_str())) != LB_ERR)
			{
				theList->SetItemData(theItem, (DWORD)theEntry.Get());
				if (theEntry->IsEnabled())
					theList->SetCheck(theItem, 1);
				else
					theList->SetCheck(theItem, 0);
			}
		}
		theTab->AddPage(thePage);
	}

	theTab->InitPagesSize();
	theTab->RepositionPages();
	theTab->SetCurSel(0);

	ReInitCtlPos(theTab);

	CheckDlgButton(IDC_FILTERS_ALL, itsConfig->GetDisplayAll());
	CheckDlgButton(IDC_FILTERS_NONE, itsConfig->GetDisplayNone());
	CheckDlgButton(IDC_FILTERS_DISPLAY, itsConfig->GetDisplayName());
	CheckDlgButton(IDC_DISPLAY_TS, itsConfig->GetDisplayTS());
	CheckDlgButton(IDC_DISPLAY_SPECIALS, itsConfig->GetDisplaySpecials());
	CheckDlgButton(IDC_DISPLAY_BADDR, itsConfig->GetDisplayBAddr());
	((CComboBox*)GetDlgItem(IDC_FILTERS_TID))->SetCurSel(itsConfig->GetDisplayTID() / 4);

	char theStr[2];
	theStr[0] = itsConfig->GetSeparator();
	theStr[1] = 0;
	SetDlgItemText(IDC_FILTERS_SEPARATOR, theStr);
	((CEdit*)GetDlgItem(IDC_FILTERS_SEPARATOR))->SetLimitText(1);

	if (itsfDocFilters)
		GetDlgItem(IDC_FILTERS_FUTURE)->EnableWindow();

//	RememberMinSizeNow(this, 75, 75);
	RememberMinSizeNow(this, 100, 75);
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);
}

//-----------------------------------------------------------------------
void CFiltersDlg::OnOK()
{
	LTab* theTab = (LTab*)GetDlgItem(IDC_FILTERS_TAB);
	ASSERT(theTab != 0);

	int				theCount = theTab->GetItemCount();
	CCheckListBox*	theList;
	CTraceEntry*	theEntry;

	for (int i = 0; i < theCount; i++)
	{
		theList = (CCheckListBox*)theTab->GetPage(i)->GetDlgItem(IDC_FILTER_LIST);
		for (int j = 0; j < theList->GetCount(); j++)
		{
			theEntry = (CTraceEntry*)theList->GetItemData(j);
			theEntry->Enable(theList->GetCheck(j));
		}
	}

	itsConfig->SetDisplayAll(IsDlgButtonChecked(IDC_FILTERS_ALL));
	itsConfig->SetDisplayNone(IsDlgButtonChecked(IDC_FILTERS_NONE));
	itsConfig->SetDisplayName(IsDlgButtonChecked(IDC_FILTERS_DISPLAY));
	itsConfig->SetDisplayTS(IsDlgButtonChecked(IDC_DISPLAY_TS));
	itsConfig->SetDisplaySpecials(IsDlgButtonChecked(IDC_DISPLAY_SPECIALS));
	itsConfig->SetDisplayBAddr(IsDlgButtonChecked(IDC_DISPLAY_BADDR));
	int theCurSel = ((CComboBox*)GetDlgItem(IDC_FILTERS_TID))->GetCurSel();
	if (theCurSel < 0)
		theCurSel = 0;
	itsConfig->SetDisplayTID(theCurSel * 4);
	char theSep[2];
	if (GetDlgItemText(IDC_FILTERS_SEPARATOR, theSep, 2) < 1)
		itsConfig->SetSeparator(' ');
	else
		itsConfig->SetSeparator(theSep[0]);

	if (itsfDocFilters && IsDlgButtonChecked(IDC_FILTERS_FUTURE))
	{
		COSReportApp::Get()->SetMainConfig(itsConfig);
		COSReportApp::Get()->GetMainConfig()->Save();
	}

	LDialog::OnOK();
}

//-----------------------------------------------------------------------
void CFiltersDlg::OnFiltersAll() 
{
	CheckDlgButton(IDC_FILTERS_NONE, 0);
}

//-----------------------------------------------------------------------
void CFiltersDlg::OnFiltersNone() 
{
	CheckDlgButton(IDC_FILTERS_ALL, 0);
}
