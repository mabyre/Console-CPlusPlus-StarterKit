//=======================================================================
// LHLine.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LHLine.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_OV_BASECONTROL(LHLine, CWnd, "LHLine")

BEGIN_MESSAGE_MAP(LHLine, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
#define kSMS_HL_3DLOOK			0x0001
#define kSMS_HL_IN				0x0002
#define kSMS_HL_ADJUSTED		0x0004

//-----------------------------------------------------------------------
LHLine::LHLine() :
	CWnd(),
	LOVControl()
{
	itsOVStyle = kSMS_HL_3DLOOK | kSMS_HL_IN | kSMS_HL_ADJUSTED;
}

//----------------------------------------------------------------------
LHLine::~LHLine()
{
}

//-----------------------------------------------------------------------
void LHLine::DoAttribute(char* anAttribute, char* aValue)
{
	if (::strcmp(anAttribute,"NO3D") == 0)
		itsOVStyle &= ~kSMS_HL_3DLOOK;
	else
	if (::strcmp(anAttribute,"OUT") == 0)
		itsOVStyle &= ~kSMS_HL_IN;
	else
	if (::strcmp(anAttribute,"CENTER") == 0)
		itsOVStyle &= ~kSMS_HL_ADJUSTED;
	else
		LOVControl::DoAttribute(anAttribute, aValue);
}

//-----------------------------------------------------------------------
void LHLine::Init()
{
	if (itsOVStyle & kSMS_HL_ADJUSTED)
		Adjust();

	LOVControl::Init();
}

//-----------------------------------------------------------------------
// LHLine specific code
//-----------------------------------------------------------------------
void LHLine::Adjust()
{
	CRect theRect;
	GetParent()->GetClientRect(&theRect);
	UINT theWidth = theRect.right;
	GetClientRect(&theRect);
	ClientToScreen(&theRect);
	GetParent()->ScreenToClient(&theRect);

	SetWindowPos(NULL, 0, theRect.top, theWidth, (itsOVStyle & kSMS_HL_3DLOOK)?2:1, SWP_NOZORDER);
}

//-----------------------------------------------------------------------
void LHLine::OnPaint()
{
	CPaintDC theDC(this);
	CRect theRect;
	GetClientRect(&theRect);
	UINT theWidth = theRect.right;
	if (itsOVStyle & kSMS_HL_3DLOOK)
	{
		UINT theWhite = (itsOVStyle & kSMS_HL_IN)?1:0;
		CPen* theOldPen;
		{
			CPen thePen(PS_SOLID,1,::GetSysColor(COLOR_3DHILIGHT));
			theOldPen = theDC.SelectObject(&thePen);
			theDC.MoveTo(0,theWhite);
			theDC.LineTo(theWidth,theWhite);
		}
		{
			CPen thePen(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));
			theDC.SelectObject(&thePen);
			theDC.MoveTo(0,1-theWhite);
			theDC.LineTo(theWidth,1-theWhite);
		}
		theDC.SelectObject(theOldPen);
	}
	else
	{
		theDC.MoveTo(0,0);
		theDC.LineTo(theWidth,0);
	}
}

//-----------------------------------------------------------------------
BOOL LHLine::OnEraseBkgnd(CDC* pDC)
{
	// Transparent
	return TRUE;
}
