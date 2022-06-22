//=======================================================================
// LStatic.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LStatic.h"

#include "LOVUtils.h"

//-----------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_OV_BASECONTROL(LStatic, CStatic, "Static")

BEGIN_MESSAGE_MAP(LStatic, CStatic)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------
#define kSMS_ST_BOLD		0x0001
#define kSMS_ST_UNDERLINE	0x0002
#define kSMS_ST_ITALIC		0x0004
#define kSMS_ST_AUTOSIZE	0x0008

//-----------------------------------------------------------------------
LStatic::LStatic() : 
	CStatic(),
	LOVControl()
{
	itsFont = 0;
	itsFontSize = 0;
}

//-----------------------------------------------------------------------
LStatic::~LStatic()
{
	if (itsFont != 0)
		delete itsFont;
}

//-----------------------------------------------------------------------
void LStatic::DoAttribute(char* anAttribute, char* aValue)
{
	if (::strcmp(anAttribute,"UNDERLINE") == 0)
		itsOVStyle |= kSMS_ST_UNDERLINE;
	else
	if (::strcmp(anAttribute,"ITALIC") == 0)
		itsOVStyle |= kSMS_ST_ITALIC;
	else
	if (::strcmp(anAttribute,"BOLD") == 0)
		itsOVStyle |= kSMS_ST_BOLD;
	else
	if (::strcmp(anAttribute,"FONTSIZE") == 0)
		itsFontSize = ::atol(aValue);
	else
	if (::strcmp(anAttribute,"AUTOSIZE") == 0)
		itsOVStyle |= kSMS_ST_AUTOSIZE;
	else
		LOVControl::DoAttribute(anAttribute, aValue);
}

//-----------------------------------------------------------------------
void LStatic::Init()
{
	if ( (itsOVStyle & (kSMS_ST_BOLD | kSMS_ST_UNDERLINE | kSMS_ST_ITALIC)) || 
		itsFontSize != 0)
	{
		LOGFONT theLF;
		GetFont()->GetObject(sizeof(LOGFONT), &theLF);

		if (itsOVStyle & kSMS_ST_BOLD)
			theLF.lfWeight = FW_BOLD;
		if (itsOVStyle & kSMS_ST_UNDERLINE)
			theLF.lfUnderline = TRUE;
		if (itsOVStyle & kSMS_ST_ITALIC)
			theLF.lfItalic = TRUE;
		if (itsFontSize != 0)
		{
			CDC* theDC = GetDC();
			if (theDC != 0)
			{
				theLF.lfHeight = -::MulDiv(itsFontSize, theDC->GetDeviceCaps(LOGPIXELSY), 72);
				theLF.lfWidth = 0;
				ReleaseDC(theDC);
			}
		}

		itsFont = new CFont;
		itsFont->CreateFontIndirect(&theLF);

		SetFont(itsFont);
	}

	if (itsOVStyle & kSMS_ST_AUTOSIZE)
	{
		CDC* theDC = GetDC();
		if (theDC != 0)
		{
			CString	theString;

			GetWindowText(theString);
			CSize theSize(theDC->GetOutputTextExtent(theString));

			SetWindowPos(0,0,0,theSize.cx, theSize.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			ReleaseDC(theDC);
		}
	}

	LOVControl::Init();
}
