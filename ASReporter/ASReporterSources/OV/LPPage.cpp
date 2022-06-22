//=======================================================================
// LPPage.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LPPage.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(LPropertyPage, CPropertyPage)

BEGIN_MESSAGE_MAP(LPropertyPage, CPropertyPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
#ifdef OVF_CF_TIPS
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnTipNeedText)
#endif // OVF_CF_TIPS
#ifdef OVF_CF_SIZABLE
	ON_WM_SIZE()
#endif // OVF_CF_SIZABLE
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
LPropertyPage::LPropertyPage() :
	CPropertyPage(),
	LContainer()
#ifdef OVF_CF_SIZABLE
	,LSizable()
#endif
{
	itsfAutoDelete = FALSE;
#ifdef OVF_CF_SIZABLE
	itsfCanManageOnSize = FALSE;
#endif
}

//-----------------------------------------------------------------------
LPropertyPage::LPropertyPage(UINT aDialogID, BOOL afAutoDelete, UINT aCaptionID) :
	CPropertyPage(aDialogID, aCaptionID),
	LContainer()
#ifdef OVF_CF_SIZABLE
	,LSizable()
#endif
{
	itsfAutoDelete = afAutoDelete;
#ifdef OVF_CF_SIZABLE
	itsfCanManageOnSize = FALSE;
#endif
}

//-----------------------------------------------------------------------
LPropertyPage::~LPropertyPage()
{
}

//-----------------------------------------------------------------------
int LPropertyPage::OnCreate(LPCREATESTRUCT aCS)
{
	return CPropertyPage::OnCreate(aCS);
}

//-----------------------------------------------------------------------
BOOL LPropertyPage::OnInitDialog()
{
	BOOL theReturn = CPropertyPage::OnInitDialog();

	DoInit(this);

#ifdef OVF_CF_SIZABLE
	itsfCanManageOnSize = TRUE;
#endif

	return theReturn;
}

//-----------------------------------------------------------------------
void LPropertyPage::OnDestroy()
{
	DoDestroy();
	CPropertyPage::OnDestroy();
}

//-----------------------------------------------------------------------
void LPropertyPage::PostNcDestroy()
{
	if (itsfAutoDelete)
		delete this;
}

#ifdef OVF_CF_TIPS
//-----------------------------------------------------------------------
int	LPropertyPage::OnToolHitTest(CPoint aPoint, TOOLINFO* aTI) const
{
	int theResult = CPropertyPage::OnToolHitTest(aPoint, aTI);

	return DoToolHitTest(theResult, aPoint, aTI);
}

//-----------------------------------------------------------------------
BOOL LPropertyPage::OnTipNeedText(UINT id, NMHDR *aTTTStruct, LRESULT *aResult)
{
	return DoTipNeedText((TOOLTIPTEXT*)aTTTStruct);
}
#endif // OVF_CF_TIPS

#ifdef OVF_CF_SIZABLE	
//-----------------------------------------------------------------------------
void LPropertyPage::OnSize(UINT aType, int aCx, int aCy)
{
	CPropertyPage::OnSize(aType, aCx, aCy);

	if (itsfCanManageOnSize)
		ManageOnSize(aCx, aCy);
}
#endif // OVF_CF_SIZABLE

//-----------------------------------------------------------------------------
void LPropertyPage::OnContextMenu(CWnd* aWnd, CPoint aPoint)
{
	if (!DoContextMenu(aWnd, aPoint, this))
		Default();
}
