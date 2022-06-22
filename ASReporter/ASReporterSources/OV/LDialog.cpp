//=======================================================================
// LDialog.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LDialog.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(LDialog, CDialog)

BEGIN_MESSAGE_MAP(LDialog, CDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
#ifdef OVF_CF_TIPS
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnTipNeedText)
#endif
#ifdef OVF_CF_SIZABLE
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
#endif
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
LDialog::LDialog(ePos aPosition) :
	CDialog(),
	LContainer()
#ifdef OVF_CF_SIZABLE
	,LSizable()
	,LMinSizeWnd()
#endif
{
	itsPosition = aPosition;
#ifdef OVF_CF_SIZABLE
	itsfCanManageOnSize = FALSE;
#endif
}

//-----------------------------------------------------------------------
LDialog::LDialog(UINT aDialogID, CWnd* aParent, ePos aPosition) :
	CDialog(aDialogID, aParent),
	LContainer()
#ifdef OVF_CF_SIZABLE
	,LSizable()
	,LMinSizeWnd()
#endif
{
	itsPosition = aPosition;
#ifdef OVF_CF_SIZABLE
	itsfCanManageOnSize = FALSE;
#endif
}

//-----------------------------------------------------------------------
LDialog::~LDialog()
{
}

//-----------------------------------------------------------------------
int LDialog::OnCreate(LPCREATESTRUCT aCS)
{
	if (itsPosition == kCentered)
	{
		CRect	theRect;
		GetWindowRect(&theRect);
		
		int thePosX = (::GetSystemMetrics(SM_CXSCREEN) / 2) - (theRect.Width() / 2);
		int thePosY = (::GetSystemMetrics(SM_CYSCREEN) / 2) - (theRect.Height() / 2);
		
		MoveWindow(thePosX, thePosY, theRect.Width(), theRect.Height(), FALSE); 
	}

	return 	CDialog::OnCreate(aCS);
}

//-----------------------------------------------------------------------
BOOL LDialog::OnInitDialog()
{
	BOOL theReturn = CDialog::OnInitDialog();

	DoInit(this);

#ifdef OVF_CF_SIZABLE
	itsfCanManageOnSize = TRUE;
#endif

	return theReturn;
}

//-----------------------------------------------------------------------
void LDialog::OnDestroy()
{
	DoDestroy();
	CDialog::OnDestroy();
}

#ifdef OVF_CF_TIPS
//-----------------------------------------------------------------------
int	LDialog::OnToolHitTest(CPoint aPoint, TOOLINFO* aTI) const
{
	int theResult = CDialog::OnToolHitTest(aPoint, aTI);

	return DoToolHitTest(theResult, aPoint, aTI);
}

//-----------------------------------------------------------------------
BOOL LDialog::OnTipNeedText(UINT id, NMHDR *aTTTStruct, LRESULT *aResult)
{
	return DoTipNeedText((TOOLTIPTEXT*)aTTTStruct);
}
#endif // OVF_CF_TIPS

#ifdef OVF_CF_SIZABLE	
//-----------------------------------------------------------------------------
void LDialog::OnSize(UINT aType, int aCx, int aCy)
{
	CDialog::OnSize(aType, aCx, aCy);

	if (itsfCanManageOnSize)
		ManageOnSize(aCx, aCy);
}

//-----------------------------------------------------------------------------
void LDialog::OnGetMinMaxInfo(MINMAXINFO* aMMI)
{
	DoGetMinMaxInfo(aMMI);

	CDialog::OnGetMinMaxInfo(aMMI);
}

//-----------------------------------------------------------------------------
#endif // OVF_CF_SIZABLE

//-----------------------------------------------------------------------------
void LDialog::OnContextMenu(CWnd* aWnd, CPoint aPoint)
{
	if (!DoContextMenu(aWnd, aPoint, this))
		Default();
}
