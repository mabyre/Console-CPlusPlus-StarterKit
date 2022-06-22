//=======================================================================
// LBitmap.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LBitmap.h"

#include "LOVReg.h"
#include "LOVUtils.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_OV_BASECONTROL(LBitmap, CWnd, "LBitmap")

BEGIN_MESSAGE_MAP(LBitmap, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()

	ON_MESSAGE(OVM_ANIM_STARTSTOP, OnStartStop)
	ON_MESSAGE(WM_LBUTTONDOWN,OnSendToParent)
	ON_MESSAGE(WM_LBUTTONUP,OnSendToParent)
	ON_MESSAGE(WM_RBUTTONDOWN,OnSendToParent)
	ON_MESSAGE(WM_RBUTTONUP,OnSendToParent)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
#define kSMS_BM_AUTOSIZE		0x0001
#define kSMS_BM_HCENTERIMAGE	0x0002
#define kSMS_BM_VCENTERIMAGE	0x0004
#define kSMS_BM_TRANSBKGND		0x0008
#define kSMS_BM_ANIMNOERASE		0x0010

//-----------------------------------------------------------------------
LBitmap::LBitmap() : 
	CWnd(),
	LOVControl()
{
	itsFrameType = k3DFrame;
	itsFrameSize = 2;
	itsDrawingStyle = ILD_NORMAL;
	itsCurrentBmp = 0;
	itsDelay = 0;
	itsImageList = new LImageList;
}

//-----------------------------------------------------------------------
LBitmap::~LBitmap()
{
	delete itsImageList;
}

//-----------------------------------------------------------------------
void LBitmap::DoAttribute(char* anAttribute, char* aValue)
{
	if (::strcmp(anAttribute,"IMAGE") == 0)
	{
		CStringArray theValues;
		UINT theNbVal = LOVUtils::SplitParameterValue(aValue, &theValues);
		ASSERT(theNbVal > 0); // IMAGE attribute error

		UINT theNb = 1, theImageID = ::atoi(theValues[0]);
		if (theNbVal > 1)
			if ( (theNb = ::atoi(theValues[1])) == 0)
				theNb = 1;

		COLORREF theTransColor = (COLORREF)-1;
		if (theNbVal > 2)
		{
			ASSERT(theNbVal == 5); // IMAGE attribute error

			theTransColor = RGB(::atoi(theValues[2]), ::atoi(theValues[3]), ::atoi(theValues[4]));
			itsImageList->SetUseMask(TRUE);
			itsDrawingStyle = ILD_TRANSPARENT;
		}

		itsImageList->Add(theImageID, theNb, theTransColor);
	}
	else
	if (::strcmp(anAttribute,"USEPALETTE") == 0)
		itsImageList->SetUsePalette(TRUE);
	else
	if (::strcmp(anAttribute,"NOFRAME") == 0)
	{
		itsFrameType = kNoFrame;
		itsFrameSize = 0;
	}
	else
	if (::strcmp(anAttribute,"FRAME") == 0)
	{
		itsFrameType = kFrame;
		itsFrameSize = 1;
	}
	else
	if (::strcmp(anAttribute,"3DFRAME") == 0)
	{
		itsFrameType = k3DFrame;
		itsFrameSize = 2;
	}
	else
	if (::strcmp(anAttribute,"HCENTERIMAGE") == 0)
		itsOVStyle |= kSMS_BM_HCENTERIMAGE;
	else
	if (::strcmp(anAttribute,"VCENTERIMAGE") == 0)
		itsOVStyle |= kSMS_BM_VCENTERIMAGE;
	else
	if (::strcmp(anAttribute,"AUTOSIZE") == 0)
		itsOVStyle |= kSMS_BM_AUTOSIZE;
	else
	if (::strcmp(anAttribute,"TRANSCOLOR") == 0)
	{
		itsImageList->SetUseMask(TRUE);
		itsImageList->SetDefaultMaskColor(LOVUtils::ParseRGBString(aValue));
		itsDrawingStyle = ILD_TRANSPARENT;
	}
	else
	if (::strcmp(anAttribute,"TRANSBKGND") == 0)
		itsOVStyle |= kSMS_BM_TRANSBKGND;
	else
	if (::strcmp(anAttribute,"ANIMATED") == 0)
		itsDelay = ::atoi(aValue);
	else
	if (::strcmp(anAttribute,"COLORBITS") == 0)
		itsImageList->SetColorBits(::atoi(aValue));
	else
	if (::strcmp(anAttribute,"ANIMNOERASE") == 0)
		itsOVStyle |= kSMS_BM_ANIMNOERASE;
	else
		LOVControl::DoAttribute(anAttribute, aValue);
}

//-----------------------------------------------------------------------
void LBitmap::Init()
{
	ASSERT(itsImageList->GetImageCount() != 0); // No image loaded (IMAGE attribute not found)

	InternalInit();

	LOVControl::Init();
}

//-----------------------------------------------------------------------
// LBitmap specific code
//-----------------------------------------------------------------------
static const UINT kTimerID = 1;

//-----------------------------------------------------------------------
void LBitmap::InternalInit()
{
	if (itsOVStyle & kSMS_BM_AUTOSIZE)
		AdjustSize();

	if (itsDrawingStyle == ILD_TRANSPARENT)
	{
		CDC* theDC = GetParent()->GetDC();
		HBRUSH theBrush = (HBRUSH)GetParent()->SendMessage(WM_CTLCOLORDLG,(WPARAM)theDC->GetSafeHdc(), (LPARAM)GetParent()->GetSafeHwnd());
		LOGBRUSH theLB;
		::GetObject(theBrush, sizeof(LOGBRUSH), &theLB);
		itsImageList->SetBkColor(theLB.lbColor);
		GetParent()->ReleaseDC(theDC);
		itsDrawingStyle = ILD_NORMAL;
	}

	StartAnim(itsDelay);
}

//-----------------------------------------------------------------------
void LBitmap::Reset()
{
	itsImageList->DeleteImageList();
}

//-----------------------------------------------------------------------
BOOL LBitmap::AddBitmap(UINT anID, UINT aNb, COLORREF aMaskColor)
{
	return itsImageList->Add(anID, aNb, aMaskColor);
}

//-----------------------------------------------------------------------
void LBitmap::SetDrawingStyle(int aStyle, BOOL afRedraw)
{
	BOOL thefStyleOK = (aStyle == ILD_NORMAL || aStyle == ILD_TRANSPARENT || aStyle == ILD_BLEND25 || aStyle == ILD_BLEND50);
	ASSERT(thefStyleOK); // Invalid drawing style
	if (!thefStyleOK)
		return;
	
	itsDrawingStyle = aStyle;
	if (afRedraw)
		Invalidate();
}

//-----------------------------------------------------------------------
BOOL LBitmap::SetCurrentBmp(UINT anIndex)
{
	ASSERT(anIndex < (UINT)itsImageList->GetImageCount()); // Too big index
	if (anIndex >= (UINT)itsImageList->GetImageCount())
		return FALSE;

	itsCurrentBmp = anIndex;

	if (itsOVStyle & kSMS_BM_ANIMNOERASE)
		Invalidate(FALSE);
	else
		Invalidate(TRUE);

	return TRUE;
}

//-----------------------------------------------------------------------
void LBitmap::SetNextBmp()
{
	if (++itsCurrentBmp >= (UINT)itsImageList->GetImageCount())
		itsCurrentBmp = 0;

	if (itsOVStyle & kSMS_BM_ANIMNOERASE)
		Invalidate(FALSE);
	else
		Invalidate(TRUE);
}

//-----------------------------------------------------------------------
void LBitmap::SetPrevBmp()
{
	if (itsCurrentBmp == 0)
	{
		if (itsImageList->GetImageCount() != 0)
			itsCurrentBmp = itsImageList->GetImageCount()-1;
	}
	else
		itsCurrentBmp--;

	if (itsOVStyle & kSMS_BM_ANIMNOERASE)
		Invalidate(FALSE);
	else
		Invalidate(TRUE);
}

//-----------------------------------------------------------------------
UINT LBitmap::GetCurrentBmp()
{
	return itsCurrentBmp;
}

//-----------------------------------------------------------------------
void LBitmap::StartAnim(UINT aDelay)
{
	if (aDelay == 0 || itsImageList->GetImageCount() < 2)
		return;

	StopAnim();
	SetTimer(kTimerID, aDelay, NULL);
}

//-----------------------------------------------------------------------
void LBitmap::StopAnim()
{
	KillTimer(kTimerID);
}

//-----------------------------------------------------------------------
void LBitmap::AdjustSize()
{
	SetWindowPos(NULL,-1,-1,itsImageList->GetWidth()+2*itsFrameSize,itsImageList->GetHeight()+2*itsFrameSize,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);

	Invalidate();
}

//-----------------------------------------------------------------------
void LBitmap::DrawFrame(CDC* aDC)
{
    CRect theRect;
    GetClientRect(&theRect);

	CBrush  theBrush;
	theBrush.CreateSolidBrush(RGB(0,0,0));
	aDC->FrameRect(theRect,&theBrush);
}

//-----------------------------------------------------------------------
LImageList*	LBitmap::GetImageList()
{
	return itsImageList;
}

//-----------------------------------------------------------------------
BOOL LBitmap::OnEraseBkgnd(CDC* aDC)
{
	if (itsOVStyle & kSMS_BM_TRANSBKGND)
		return TRUE;

	return CWnd::OnEraseBkgnd(aDC);
}

//-----------------------------------------------------------------------
void LBitmap::OnPaint()
{
	CPaintDC theDC(this);
	CRect theRect;
	GetClientRect(&theRect);
	CPoint thePoint(itsFrameSize, itsFrameSize);

	if (itsOVStyle & kSMS_BM_HCENTERIMAGE)
		thePoint.x = (theRect.right - itsImageList->GetWidth()) / 2;
	if (itsOVStyle & kSMS_BM_VCENTERIMAGE)
		thePoint.y = (theRect.bottom - itsImageList->GetHeight()) / 2;
	
	itsImageList->XDraw(&theDC, itsCurrentBmp, thePoint, itsDrawingStyle);

	if (itsFrameType == kFrame)
		DrawFrame(&theDC);
	else
		if (itsFrameType == k3DFrame)
			LOVUtils::Draw3DFrame(theDC, theRect);
}

//-----------------------------------------------------------------------
void LBitmap::OnTimer(UINT anIDEvent)
{
	if (anIDEvent != kTimerID)
	{
		CWnd::OnTimer(anIDEvent);
		return;
	}

	SetNextBmp();
}

//-----------------------------------------------------------------------
LRESULT	LBitmap::OnStartStop(WPARAM aWParam, LPARAM aLParam)
{
	if ((BOOL)aWParam)
		StartAnim((UINT)aLParam);
	else
		StopAnim();

	return 0;
}

//-----------------------------------------------------------------------
LRESULT	LBitmap::OnSendToParent(WPARAM aWParam, LPARAM aLParam)
{
	return GetParent()->SendMessage(GetCurrentMessage()->message, aWParam, aLParam);
}
