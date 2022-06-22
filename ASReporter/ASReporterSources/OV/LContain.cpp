//=======================================================================
// LContain.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LContain.h"

#ifdef OVF_CF_TIPS
	#include "LTipInfo.h"
	#include <afxext.h>
#endif
#include "LOVReg.h"

#ifdef OVF_CF_PROPERTYPAGE
	#include "LPPage.h"
#endif

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------
LContainer::LContainer()
{
	itsCWndThis = 0;
}

//-----------------------------------------------------------------------
LContainer::~LContainer()
{
}

//-----------------------------------------------------------------------
BOOL LContainer::DoInit(CWnd* aCWnd)
{
	ASSERT_KINDOF(CWnd, aCWnd);
	itsCWndThis = aCWnd;
	ASSERT(itsCWndThis != 0); // This LContainer is not a CWnd ???

	LOVRegister::CreateInstances(itsCWndThis);

#ifdef OVF_CF_TIPS
	itsCWndThis->EnableToolTips();
#endif

	Start();

	return TRUE;
}

//-----------------------------------------------------------------------
void LContainer::DoDestroy()
{
	ASSERT(itsCWndThis != 0);

	End();
	LOVRegister::EndDialogOrView(itsCWndThis->GetSafeHwnd());

#ifdef OVF_CF_TIPS
	POSITION	thePos;
	void*		theHwnd;
	void*		theTI;

	while (!itsTipsMap.IsEmpty())
	{
		thePos = itsTipsMap.GetStartPosition();
		itsTipsMap.GetNextAssoc(thePos, theHwnd, theTI);
		itsTipsMap.RemoveKey(theHwnd);
		delete (LTipInfo*)theTI;
	}
#endif
	itsCWndThis = 0;
}

#ifdef OVF_CF_TIPS
//-----------------------------------------------------------------------
void LContainer::RegisterTip(HWND aHwnd, LPCSTR aText)
{
	void* theTipInfo = 0;

	if (!itsTipsMap.Lookup((void*)aHwnd, theTipInfo))
	{
		theTipInfo = new LTipInfo;
		itsTipsMap.SetAt((void*)aHwnd, theTipInfo);
	}

	((LTipInfo*)theTipInfo)->SetString(aText);
}

//-----------------------------------------------------------------------
int LContainer::DoToolHitTest(int aHit, CPoint aPoint, TOOLINFO* aTI) const
{
	ASSERT(itsCWndThis != 0);
	CFrameWnd* theFrameWnd = itsCWndThis->GetParentFrame();

	if (aHit == -1)
	{
		if (theFrameWnd != 0)
			theFrameWnd->SetMessageText(AFX_IDS_IDLEMESSAGE);
	}
	else
	{
	    if (aTI->uFlags & TTF_IDISHWND)
		{
			void* theTipInfo;

			if (itsTipsMap.Lookup((void*)aTI->uId, theTipInfo))
			{
				LPCSTR theString = ((LTipInfo*)theTipInfo)->GetMessageString();
				if (theFrameWnd != 0 && theString != 0)
					theFrameWnd->SetMessageText(theString);

				if (((LTipInfo*)theTipInfo)->GetCenter())
					aTI->uFlags |= TTF_CENTERTIP;
				else
					aTI->uFlags &= (~TTF_CENTERTIP);

				return 1;
			}
		}
	}

	return -1;
}

//-----------------------------------------------------------------------
BOOL LContainer::DoTipNeedText(TOOLTIPTEXT* aTTT)
{
    UINT theID = aTTT->hdr.idFrom;

    if (aTTT->uFlags & TTF_IDISHWND)
    {
		void* theTipInfo = 0;
		if (itsTipsMap.Lookup((void*)theID, theTipInfo))
		{
			aTTT->lpszText = (char*)((LTipInfo*)theTipInfo)->GetTipString();
			aTTT->hinst = 0;
		    return TRUE;
        }
    }

    return FALSE;
}

#endif // OVF_CF_TIPS

//-----------------------------------------------------------------------
void LContainer::Start()
{
}

//-----------------------------------------------------------------------
void LContainer::End()
{
}

//-----------------------------------------------------------------------
void LContainer::XCenterCtl(CWnd* aCWnd)
{
	ASSERT(itsCWndThis != 0);
	ASSERT(aCWnd != 0);

	CRect theRect, theDlgRect;
	itsCWndThis->GetClientRect(&theDlgRect);
	aCWnd->GetClientRect(&theRect);
	CPoint thePoint(0,0);
	aCWnd->ClientToScreen(&thePoint);
	itsCWndThis->ScreenToClient(&thePoint);
	aCWnd->SetWindowPos(NULL,(theDlgRect.right-theRect.right) /2,thePoint.y,-1,-1,SWP_NOZORDER | SWP_NOSIZE);
}	

//-----------------------------------------------------------------------
void LContainer::YCenterCtl(CWnd* aCWnd)
{
	ASSERT(itsCWndThis != 0);
	ASSERT(aCWnd != 0);
	
	CRect theRect, theDlgRect;
	itsCWndThis->GetClientRect(&theDlgRect);
	aCWnd->GetClientRect(&theRect);
	CPoint thePoint(0,0);
	aCWnd->ClientToScreen(&thePoint);
	itsCWndThis->ScreenToClient(&thePoint);
	aCWnd->SetWindowPos(NULL,thePoint.x,(theDlgRect.bottom-theRect.bottom) /2,-1,-1,SWP_NOZORDER | SWP_NOSIZE);
}	

//-----------------------------------------------------------------------
void LContainer::CenterCtl(CWnd* aCWnd)
{
	ASSERT(itsCWndThis != 0);
	ASSERT(aCWnd != 0);

	CRect theRect, theDlgRect;
	itsCWndThis->GetClientRect(&theDlgRect);
	aCWnd->GetClientRect(&theRect);
	aCWnd->SetWindowPos(NULL,(theDlgRect.right-theRect.right) /2,(theDlgRect.bottom-theRect.bottom) /2,-1,-1,SWP_NOZORDER | SWP_NOSIZE);
}	

//-----------------------------------------------------------------------
void LContainer::XCenterCtl(UINT anID)
{
	ASSERT(itsCWndThis != 0);
	XCenterCtl(itsCWndThis->GetDlgItem(anID));
}

//-----------------------------------------------------------------------
void LContainer::YCenterCtl(UINT anID)
{
	ASSERT(itsCWndThis != 0);
	YCenterCtl(itsCWndThis->GetDlgItem(anID));
}

//-----------------------------------------------------------------------
void LContainer::CenterCtl(UINT anID)
{
	ASSERT(itsCWndThis != 0);
	CenterCtl(itsCWndThis->GetDlgItem(anID));
}

//-----------------------------------------------------------------------
void LContainer::GetCtlRect(UINT anID, RECT* aRect)
{
	ASSERT(itsCWndThis != 0);
	GetCtlRect(itsCWndThis->GetDlgItem(anID), aRect);
}

//-----------------------------------------------------------------------
void LContainer::GetCtlRect(CWnd* aCWnd, RECT* aRect)
{
	ASSERT(itsCWndThis != 0);
	ASSERT(aCWnd != 0);
	ASSERT(aRect != 0);

	aCWnd->GetClientRect(aRect);
	CPoint thePoint(0,0);
	aCWnd->ClientToScreen(&thePoint);
	itsCWndThis->ScreenToClient(&thePoint);
	aRect->top += thePoint.y;
	aRect->bottom += thePoint.y;
	aRect->left += thePoint.x;
	aRect->right += thePoint.x;
}

#ifdef OVF_CF_PROPERTYPAGE
//-----------------------------------------------------------------------
LPropertyPage* LContainer::CreatePropertyPage(UINT anID)
{
	TRACE0("Creating LPropertyPage in LContainer. CreatePropertyPage should be subclassed to do it.\n");
	
	return new LPropertyPage(anID);
}
#endif // OVF_CF_PROPERTYPAGE

//-----------------------------------------------------------------------------
UINT LContainer::GetContextMenuID(CWnd* aWnd, const CPoint& aPoint, int* aDefaultItem, CWnd** aCommandHandler)
{
	return 0;
}

//-----------------------------------------------------------------------------
BOOL LContainer::DoContextMenu(CWnd* aWnd, CPoint aPoint, CWnd* aDefaultCommandHandler)
{
	CWnd*	theCommandHandler = aDefaultCommandHandler;
	int		theDefaultItem = -1;
	UINT	theID = GetContextMenuID(aWnd, aPoint, &theDefaultItem, &theCommandHandler);
	if (theID == 0)
		return FALSE;

	CMenu theMenu;
	if (theMenu.LoadMenu(theID))
	{
		CMenu* thePopup = theMenu.GetSubMenu(0);
		ASSERT(thePopup != NULL);

		CPoint thePoint;
		if (!::GetCursorPos(&thePoint))
			thePoint = aPoint;

		if (theDefaultItem >= 0)
			::SetMenuDefaultItem(thePopup->GetSafeHmenu(), theDefaultItem, TRUE);
		thePopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, thePoint.x, thePoint.y, theCommandHandler);
	}

	return TRUE;
}
