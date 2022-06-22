//=======================================================================
// LControl.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LControl.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

// OVControl is the Win32 window class name

#define kSMS_HCENTER	0x00010000
#define kSMS_VCENTER	0x00020000
#define kSMS_DELETETHIS	0x80000000

//-----------------------------------------------------------------------
LOVControl::LOVControl()
{
	itsOVStyle = 0;
}

//-----------------------------------------------------------------------
LOVControl::~LOVControl()
{
}

//-----------------------------------------------------------------------
BOOL LOVControl::SetfAutoDelete(BOOL afAutoDelete)
{
	BOOL thefOld = ((itsOVStyle & kSMS_DELETETHIS) != 0);

	if (afAutoDelete)
		itsOVStyle |= kSMS_DELETETHIS;
	else
		itsOVStyle &= ~kSMS_DELETETHIS;

	return thefOld;
}

//-----------------------------------------------------------------------
BOOL LOVControl::GetfAutoDelete()
{
	return (itsOVStyle & kSMS_DELETETHIS) != 0;
}

//-----------------------------------------------------------------------
void LOVControl::DoAttribute(char* anAttribute, char* aValue)
{
	if (::strcmp(anAttribute, "HCENTER") == 0)
		itsOVStyle |= kSMS_HCENTER;
	else
	if (::strcmp(anAttribute, "VCENTER") == 0)
		itsOVStyle |= kSMS_VCENTER;
#ifdef _DEBUG
	else
	{
		CWnd* pThis = dynamic_cast<CWnd*>(this);
		ASSERT(pThis != 0); // LOVControl can not be a base class of an object not derived from CWnd
		TRACE("Unknown attribute [%s]=[%s]for control ID: %d\n", anAttribute, aValue, pThis->GetDlgCtrlID());
	}
#endif // _DEBUG
}

//-----------------------------------------------------------------------
void LOVControl::Init()
{
	CWnd* pThis = dynamic_cast<CWnd*>(this);
	ASSERT(pThis != 0); // LOVControl can not be a base class of an object not derived from CWnd

	if ((itsOVStyle & kSMS_HCENTER) != 0)
	{
		CRect theRect, theParentRect;
		pThis->GetParent()->GetClientRect(theParentRect);
		pThis->GetClientRect(theRect);

		CPoint thePoint(0,0);
		pThis->ClientToScreen(&thePoint);
		pThis->GetParent()->ScreenToClient(&thePoint);

		pThis->SetWindowPos(NULL,(theParentRect.right-theRect.right) /2,thePoint.y,-1,-1,SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	if ((itsOVStyle & kSMS_VCENTER) != 0)
	{
		CRect theRect, theParentRect;

		pThis->GetParent()->GetClientRect(theParentRect);
		pThis->GetClientRect(theRect);

		CPoint thePoint(0,0);
		pThis->ClientToScreen(&thePoint);
		pThis->GetParent()->ScreenToClient(&thePoint);
		pThis->SetWindowPos(NULL,thePoint.x,(theParentRect.bottom-theRect.bottom) /2,-1,-1,SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	}
}

//-----------------------------------------------------------------------
const char* const LOVControl::GetCustomClassName()
{
	return "OVControl";
}
