//=======================================================================
// LButton.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LButton.h"
#include <afxcmn.h>

#include "LOVReg.h"
#include "LOVUtils.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_OV_BASECONTROL(LButton, CButton, "Button")

BEGIN_MESSAGE_MAP(LButton, CButton)
	ON_WM_SIZE()
	ON_WM_ENABLE()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
#define kSMS_BTN_IMGRIGHT	0x0001
#define kSMS_BTN_IMGLEFT	0x0002
#define kSMS_BTN_IMGTOP		0x0004
#define kSMS_BTN_IMGBOTTOM	0x0008
#define kSMS_BTN_SYSTGRAY	0x0010

//-----------------------------------------------------------------------
LButton::LButton() :
	CButton(),
	LOVControl()
{
	itsIL = 0;
	itsImageID = 0;
	itsOVStyle |= kSMS_BTN_SYSTGRAY;
	itsTransClr = RGB(255,0,255);
}

//-----------------------------------------------------------------------
LButton::~LButton()
{
	if (itsIL != 0)
		delete itsIL;
}

//-----------------------------------------------------------------------
void LButton::Init()
{
	if (itsImageID != 0)
	{
		CBitmap theBitmap;
		BITMAP bm;
		if (theBitmap.LoadBitmap(itsImageID))
		{
			if (theBitmap.GetObject(sizeof(bm), &bm) != 0)
			{
				int theWidth = bm.bmWidth;
				if (!(itsOVStyle & kSMS_BTN_SYSTGRAY))
					theWidth /= 2;
				itsIL = new CImageList;
				itsIL->Create(itsImageID,theWidth,1,itsTransClr);

				itsBmpSize = CSize(theWidth, bm.bmHeight);
			}
			theBitmap.DeleteObject();
		}
		ManagePlacements();
	}
	LOVControl::Init();
}

//-----------------------------------------------------------------------
void LButton::DoAttribute(char* anAttribute, char* aValue)
{
	if (::strcmp(anAttribute,"IMAGE") == 0)
	{
		itsImageID = ::atoi(aValue);
		char* theString = ::strchr(aValue,',');
		if (theString != 0 && ::atoi(theString+1) == 2)
			itsOVStyle &= ~kSMS_BTN_SYSTGRAY;

		ModifyStyle(0, BS_OWNERDRAW);
	}
	else
	if (::strcmp(anAttribute,"IMAGERIGHT") == 0)
		itsOVStyle |= kSMS_BTN_IMGRIGHT;
	else
	if (::strcmp(anAttribute,"IMAGELEFT") == 0)
		itsOVStyle |= kSMS_BTN_IMGLEFT;
	else
	if (::strcmp(anAttribute,"IMAGETOP") == 0)
		itsOVStyle |= kSMS_BTN_IMGTOP;
	else
	if (::strcmp(anAttribute,"IMAGEBOTTOM") == 0)
		itsOVStyle |= kSMS_BTN_IMGBOTTOM;
	else
	if (::strcmp(anAttribute,"TRANSCOLOR") == 0)
		itsTransClr = LOVUtils::ParseRGBString(aValue);
	else
		LOVControl::DoAttribute(anAttribute, aValue);
}

//-----------------------------------------------------------------------
// LButton specific code
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

void LButton::ChangeBmp(UINT aNewID)
{
	if (aNewID == 0)
	{
		if (itsIL != 0)
		{
			delete itsIL;
			itsIL = 0;
		}
	}
	else
	{
		CBitmap theBitmap;
		BITMAP bm;
		if (theBitmap.LoadBitmap(aNewID))
		{
			if (theBitmap.GetObject(sizeof(bm), &bm) != 0)
			{
				int theWidth = bm.bmWidth;
				if (!(itsOVStyle & kSMS_BTN_SYSTGRAY))
					theWidth /= 2;

				if (itsIL != 0)
					delete itsIL;

				itsIL = new CImageList;
				itsIL->Create(aNewID,theWidth,1,itsTransClr);

				itsBmpSize = CSize(theWidth, bm.bmHeight);
			}
			theBitmap.DeleteObject();
		}
	}
	
	ManagePlacements();
}

//-----------------------------------------------------------------------
void LButton::ManagePlacements()
{
	CRect theRect;
	GetClientRect(&theRect);

	itsTextRect = theRect;
	itsTextRect.DeflateRect(4, 4);

	if (itsIL != 0)
	{
		if (itsOVStyle & kSMS_BTN_IMGLEFT)
		{
			itsImagePos.x = 4;
			itsTextRect.left = itsImagePos.x + itsBmpSize.cx + 2;
		}
		else
			if (itsOVStyle & kSMS_BTN_IMGRIGHT)
			{
				itsImagePos.x = theRect.right - itsBmpSize.cx - 4;
				itsTextRect.right = itsImagePos.x - 2;
			}
			else
				itsImagePos.x = (theRect.right - theRect.left - itsBmpSize.cx) / 2;

		if (itsOVStyle & kSMS_BTN_IMGTOP)
		{
			itsImagePos.y = 4;
			itsTextRect.top = itsImagePos.y + itsBmpSize.cy + 2;
		}
		else
			if (itsOVStyle & kSMS_BTN_IMGBOTTOM)
			{
				itsImagePos.y = theRect.bottom - itsBmpSize.cy - 4;
				itsTextRect.bottom = itsImagePos.y - 2;
			}
			else
				itsImagePos.y = (theRect.bottom - theRect.top - itsBmpSize.cy) / 2;
	}

	itsTextRect.top--;	// text vertical centering ajustment (OLa 970326)
	itsTextRect.bottom--;
}

//-----------------------------------------------------------------------
void LButton::DrawItem(LPDRAWITEMSTRUCT aDIS)
{
	CDC		theDC;
	CRect	theRect(aDIS->rcItem);
	UINT	theState = DFCS_BUTTONPUSH | DFCS_ADJUSTRECT;
	CPoint	thePoint;

	theDC.Attach(aDIS->hDC);
	if (!IsWindowEnabled())
		theState |= DFCS_INACTIVE;
	if (GetState() & 0x0004)
		theState |= DFCS_PUSHED;

/*
	if (GetState() & 0x0008)
	{
		theDC.Rectangle(theRect);
		theRect.DeflateRect(1,1);
	}
*/
	theDC.DrawFrameControl(theRect, DFC_BUTTON, theState);

	if (itsIL != 0)
	{
		thePoint = itsImagePos;
		if (theState & DFCS_PUSHED)
		{
			thePoint.x++;
			thePoint.y++;
		}

		if (itsOVStyle & kSMS_BTN_SYSTGRAY)
		{
			int theDrawStyle = IsWindowEnabled()?ILD_NORMAL:ILD_BLEND50;
			itsIL->Draw(&theDC, 0, thePoint, theDrawStyle);
		}
		else
			itsIL->Draw(&theDC, !IsWindowEnabled(), thePoint, ILD_NORMAL);
	}

	theRect = itsTextRect;
	if (theState & DFCS_PUSHED)
	{
//		theRect.left++;
//		theRect.top++;
		theRect.OffsetRect(1,1);
	}
	thePoint = theRect.TopLeft();
	CString theText;
	GetWindowText(theText);
	if (theState & DFCS_INACTIVE)
	{
		theDC.SetTextColor(::GetSysColor(COLOR_BTNHILIGHT));
		theRect.left += 2;
		theRect.top += 2;
		theDC.DrawText(theText, theRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		theDC.SetTextColor(::GetSysColor(COLOR_BTNSHADOW));
		theDC.SetBkMode(TRANSPARENT);
		theRect.left -= 2;
		theRect.top -= 2;
		theDC.DrawText(theText, theRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}
	else
		theDC.DrawText(theText, theRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	if (GetState() & 0x0008)
		theDC.DrawFocusRect(itsTextRect);

	theDC.Detach();
}

//-----------------------------------------------------------------------
void LButton::OnEnable(BOOL afEnable)
{
	CButton::OnEnable(afEnable);

	CWnd* theFocused = CWnd::GetFocus();

	if (!afEnable && theFocused == 0)
	{
		CWnd* theNext = GetParent()->GetNextDlgTabItem(this, FALSE);
		if (theNext != 0 && theNext != this)
			theNext->SetFocus();
	}
}

//-----------------------------------------------------------------------
void LButton::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);

	ManagePlacements();
}

