//=======================================================================
// LTab.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LTab.h"

#include "LOVReg.h"
#ifdef OVF_CF_PROPERTYPAGE
	#include "LPPage.h"
#endif
#include "LImgList.h"
#include "LOVUtils.h"
#include "LContain.h"

//-----------------------------------------------------------------------
#define kSMS_TAB_SIZETOPAGE		0x0001
#define kSMS_TAB_CENTERPAGES	0x0002

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_OV_BASECONTROL(LTab, CTabCtrl, WC_TABCONTROL)

BEGIN_MESSAGE_MAP(LTab, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnChangingPropertyPage)
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, OnChangedPropertyPage)
	ON_MESSAGE(TCM_SETCURSEL, OnSetCurSel)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
LTab::LTab() :
	CTabCtrl(),
	LOVControl()
{
	itsInitMaxPageSize = itsMaxPageSize = CSize(0,0);
	itsInitSize = CSize(-1,0);
	itsIL = 0;
	itsTransColor = (COLORREF)-1;
	itsColorBits = (UINT)-1;
}

//-----------------------------------------------------------------------
LTab::~LTab()
{
	delete itsIL;
}

//-----------------------------------------------------------------------
void LTab::DoAttribute(char* anAttribute, char* aValue)
{
	if (::strcmp(anAttribute, "PAGE") == 0)
	{
		CStringArray	theValues;
		UINT			theILIndex = (UINT)-1;
		LPCTSTR			theTitle = 0;
		UINT theCount = LOVUtils::SplitParameterValue(aValue, &theValues);

		ASSERT(theCount >= 1 && theCount <= 3);

		if (theCount >= 2)
			theILIndex = ::atoi(theValues[1]);
		if (theCount == 3)
			theTitle = theValues[2];

		AddPage(::atoi(theValues[0]), theILIndex, theTitle);
	}
	else
	if (::strcmp(anAttribute, "SIZETOPAGE") == 0)
		itsOVStyle |= kSMS_TAB_SIZETOPAGE;
	else
	if (::strcmp(anAttribute, "CENTERPAGES") == 0)
		itsOVStyle |= kSMS_TAB_CENTERPAGES;
	else
	if (::strcmp(anAttribute, "IMAGE") == 0)
	{
		ASSERT(itsIL == 0);
		itsIL = new LImageList;
		if (itsColorBits != (UINT)-1)
			itsIL->SetColorBits(itsColorBits);
		if (itsTransColor != (COLORREF)-1)
			itsIL->SetDefaultMaskColor(itsTransColor);
		UINT theID = ::atoi(aValue);
		UINT theCount = 1;
		char* theString = ::strchr(aValue,',');
		if (theString != 0)
			if ( (theCount = ::atoi(theString+1)) == 0)
				theCount = 1;
		itsIL->Add(theID, theCount, (COLORREF)-1);
		SetImageList(itsIL);
	}
	else
	if (::strcmp(anAttribute, "TRANSCOLOR") == 0)
		itsTransColor = LOVUtils::ParseRGBString(aValue);
	else
	if (::strcmp(anAttribute, "COLORBITS") == 0)
	{
		ASSERT(itsColorBits == (UINT)-1);
		itsColorBits = ::atoi(aValue);
		ASSERT(	itsColorBits == 0 ||
				itsColorBits == 1 ||
				itsColorBits == 4 ||
				itsColorBits == 8 ||
				itsColorBits == 16 ||
				itsColorBits == 24 ||
				itsColorBits == 32);
	}
	else
		LOVControl::DoAttribute(anAttribute, aValue);
}

//-----------------------------------------------------------------------
void LTab::Init()
{
	LOVControl::Init();

	InitPagesSize();
	RepositionPages();

	TC_ITEM theTCI;
	theTCI.mask = TCIF_PARAM;
	for (int i = 0; i < GetItemCount(); i++)
	{
		GetItem(i, &theTCI);
		CWnd* theCWnd = (CWnd*)theTCI.lParam;
		if (theCWnd != 0)
		{
			theCWnd->EnableWindow(TRUE);
			theCWnd->ShowWindow(SW_SHOW);
			break;
		}
	}
}

//-----------------------------------------------------------------------
// LTab specific code
//-----------------------------------------------------------------------
void LTab::InitPagesSize()
{
	CRect	theRect;
	int		i, j;

	itsInitPageWidth.RemoveAll();
	itsInitPageHeight.RemoveAll();
	itsMaxPageSize = CSize(0, 0);
	itsInitSize = CSize(-1,0);

	for (i = 0; i < GetItemCount(); i++)
	{
		CWnd* theCWnd = GetPage(i);
		if (theCWnd != 0)
		{
			theCWnd->GetWindowRect(&theRect);

			j = theRect.right - theRect.left;
			if (j < 0)
				j = 0;
			itsInitPageWidth.Add(j);
			if (j > (int)itsMaxPageSize.cx)
				itsMaxPageSize.cx = j;

			j = theRect.bottom - theRect.top;
			if (j < 0)
				j = 0;
			itsInitPageHeight.Add(j);
			if (j > (int)itsMaxPageSize.cy)
				itsMaxPageSize.cy = j;
		}
	}

	if (itsOVStyle & kSMS_TAB_SIZETOPAGE)
	{
		theRect.SetRectEmpty();
		theRect.right = itsMaxPageSize.cx;
		theRect.bottom = itsMaxPageSize.cy;
		AdjustRect(TRUE, &theRect);
		SetWindowPos(NULL, 0, 0, theRect.right - theRect.left, theRect.bottom - theRect.top, SWP_NOZORDER | SWP_NOMOVE);
	}

	GetClientRect(theRect);
	itsInitSize = theRect.Size();
	itsInitMaxPageSize = itsMaxPageSize;
}

//-----------------------------------------------------------------------
void LTab::RepositionPages()
{
	CRect theRect, theRect2;

	theRect2.SetRectEmpty();
	AdjustRect(TRUE, &theRect2); // Compute left and top offsets inside the tab
	// Compute the pos inside the parent dialog
	theRect.SetRectEmpty();
	MapWindowPoints(GetParent(), &theRect);
	theRect.left -= theRect2.left;
	theRect.top -= theRect2.top;

	GetWindowRect(theRect2);
	AdjustRect(FALSE, theRect2);
	CSize theTotalSize(theRect2.Size());

	int x, y;
	for (int i = 0; i < GetItemCount(); i++)
	{
		CWnd* theCWnd = GetPage(i);
		if (theCWnd != 0)
		{
			x = theRect.left; y = theRect.top;
			if (itsOVStyle & kSMS_TAB_CENTERPAGES)
			{
				theCWnd->GetWindowRect(&theRect2);
				x += (theTotalSize.cx - (theRect2.right - theRect2.left)) / 2;
				y += (theTotalSize.cy - (theRect2.bottom - theRect2.top)) / 2;
				if (x < theRect.left)
					x = theRect.left;
				if (y < theRect.top)
					y = theRect.top;
			}
			theCWnd->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}
}

//-----------------------------------------------------------------------
CPropertyPage* LTab::GetPage(int anIndex)
{
	if (anIndex >= GetItemCount())
		return 0;
	
	TC_ITEM theTCI;
	theTCI.mask = TCIF_PARAM;
	GetItem(anIndex, &theTCI);
	CWnd* theCWnd = (CWnd*)theTCI.lParam;
	return dynamic_cast<CPropertyPage*>((CWnd*)theTCI.lParam);
}

//-----------------------------------------------------------------------
CPropertyPage* LTab::AddPage(CPropertyPage* aPage, UINT anILIndex, LPCTSTR aTitle)
{
	ASSERT(anILIndex == (UINT)-1 || itsIL != 0);

	if (aPage == 0)
		TRACE0("LTab::AddPage(): Warning: Creating a null page\n");

	CString	theTitle;
	if (aPage != 0)
	{
		aPage->GetWindowText(theTitle);
		ASSERT(aPage->GetStyle() & DS_CONTROL);
		aPage->ModifyStyle(WS_CAPTION | WS_SYSMENU, 0);
		aPage->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	}
	if (theTitle.GetLength() == 0 && aTitle != 0)
		theTitle = aTitle;

	TC_ITEM theTCI;

	theTCI.mask = TCIF_PARAM | TCIF_TEXT;
	if (anILIndex != (UINT)-1)
	{
		theTCI.mask |= TCIF_IMAGE;
		theTCI.iImage = anILIndex;
	}
	theTCI.lParam = (LPARAM)aPage;
	theTCI.pszText = (char*)(LPCSTR)theTitle;
	InsertItem(GetItemCount(), &theTCI);

	return aPage;
}

//-----------------------------------------------------------------------
CPropertyPage* LTab::AddPage(UINT anID, UINT anILIndex, LPCTSTR aTitle)
{
	LContainer* theContainer = dynamic_cast<LContainer*>(GetParent());
	CPropertyPage* thePage = 0;

	if (theContainer == 0)
	{
#ifdef OVF_CF_PROPERTYPAGE
		TRACE0("Creating PropertyPages in a Dialog or a View not inherited from LContainer.\n");
		TRACE0("The PropertyPage will inherit from the default LPropertyPage class.\n");
		thePage = new LPropertyPage;
#else
		TRACE0("Creating PropertyPages in a Dialog or a View not inherited from LContainer.\n");
		TRACE0("The PropertyPage will inherit from the default CPropertyPage class.\n");
		thePage = new CPropertyPage;
#endif
	}
#ifdef OVF_CF_PROPERTYPAGE
	else
		if (anID != 0)
			thePage = theContainer->CreatePropertyPage(anID);
#endif

	if (thePage == 0)
	{
		if (anID != 0)
		{
			TRACE1("Failed to create a PropertyPage for ID=%lu.\n", anID);
			return 0;
		}
	}
	else
		if (thePage->GetSafeHwnd() == 0)
			thePage->Create(anID, GetParent());
		// The pages MUST be children of the Parent dialog and not from the tab control
//			thePage->Create(anID, this);

	return AddPage(thePage, anILIndex, aTitle);
}

//-----------------------------------------------------------------------
LRESULT	LTab::OnSetCurSel(WPARAM aWParam, LPARAM aLParam)
{
	LRESULT theResult, theResult2;
	OnChangingPropertyPage(0, &theResult2);

	if (theResult2 == 1)
		return -1;

	theResult = Default();
	if (theResult != -1)
		OnChangedPropertyPage(0, &theResult2);

	return theResult;
}

//-----------------------------------------------------------------------
// Set *aLResult to 1 to disallow changing, to 0 to allow
// This message is not resent to parent
void LTab::OnChangingPropertyPage(NMHDR* aNMHDR, LRESULT* aLResult)
{
	CPropertyPage* thePage = GetPage(GetCurSel());
	// OnKillActive must return TRUE to allow changing
	if (thePage == 0 || thePage->OnKillActive())
	{
		if (thePage != 0)
		{
			thePage->ShowWindow(SW_HIDE);
			thePage->EnableWindow(FALSE);
		}
		*aLResult = 0; // 1 doesn't allow to change
	}
	else
		*aLResult = 1;
}

//-----------------------------------------------------------------------
// In case of a reflected_EX message, return TRUE eats the message so the Parent does not receive it
// *aLResult does not having a meaning here.
BOOL LTab::OnChangedPropertyPage(NMHDR* aNMHDR, LRESULT* aLResult)
{
	CWnd* theCWnd = GetPage(GetCurSel());
	if (theCWnd == 0)
		return FALSE;

	theCWnd->ShowWindow(SW_SHOW);
	theCWnd->EnableWindow(TRUE);

	return TRUE;
}

//-----------------------------------------------------------------------
void LTab::OnSize(UINT aType, int aCx, int aCy)
{
	Default();

	if (itsInitSize.cx < 0)
		return;

	CRect theRect;
	GetClientRect(theRect);
	CSize theDSize = theRect.Size() - itsInitSize, theSize;

	for (int i = 0; i < GetItemCount(); i++)
	{
		CWnd* theCWnd = GetPage(i);
		if (theCWnd != 0)
		{
			theSize.cx = itsInitPageWidth.GetAt(i) + theDSize.cx;
			theSize.cy = itsInitPageHeight.GetAt(i) + theDSize.cy;

			theCWnd->SetWindowPos(NULL, 0, 0, theSize.cx, theSize.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
		}
	}

	itsMaxPageSize = itsInitMaxPageSize + theDSize;

	RepositionPages();
}
