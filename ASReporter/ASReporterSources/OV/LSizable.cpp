//=======================================================================
// LSizable.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LSizable.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
LSizable::LSizable()
{
	itsInitSize = CSize(-1, 0);
	itsfUseMinSize = FALSE;
}

//-----------------------------------------------------------------------------
LSizable::~LSizable()
{
	POSITION thePos;
	
	for (thePos = itsSizableCtls.GetHeadPosition(); thePos != NULL; )
		delete (TSizableCtl*)itsSizableCtls.GetNext(thePos);
}

//-----------------------------------------------------------------------------
BOOL LSizable::UseMinSize(BOOL afUse)
{
	TRACE0("LSizable::UseMinSize(): Obsolete");
	BOOL thefOld = itsfUseMinSize;
	itsfUseMinSize = afUse;

	return thefOld;
}

//-----------------------------------------------------------------------------
void LSizable::ManageOnSize(int aCx, int aCy)
{
	if (itsSizableCtls.GetCount() == 0 || itsInitSize.cx < 0)
		return;

	POSITION thePos;
	if ( (thePos = itsSizableCtls.GetHeadPosition()) == NULL)
		return;

	TSizableCtl* theCtl;
	LONG theDx = aCx - itsInitSize.cx, theDy = aCy - itsInitSize.cy;
	
	for (; thePos != NULL; )
	{
		theCtl = (TSizableCtl*)itsSizableCtls.GetNext(thePos);
		
		CPoint thePoint(theCtl->itsInitRect.TopLeft());

		if (theDx > 0 || !itsfUseMinSize)
			thePoint.x += (LONG)theCtl->itsTrans.cx * theDx / 100;
		if (theDy > 0 || !itsfUseMinSize)
			thePoint.y += (LONG)theCtl->itsTrans.cy * theDy / 100;

		CSize theNewSize = theCtl->itsInitRect.Size();
		if (theDx > 0 || !itsfUseMinSize)
			theNewSize.cx += (LONG)theCtl->itsHom.cx * theDx / 100;
		if (theDy > 0 || !itsfUseMinSize)
			theNewSize.cy += (LONG)theCtl->itsHom.cy * theDy / 100;

		::SetWindowPos(theCtl->itsHWnd, 0, thePoint.x, thePoint.y, theNewSize.cx, theNewSize.cy, SWP_NOZORDER);
	}
}

//-----------------------------------------------------------------------------
void LSizable::AddSizableCtl(const TSizableCtl& aSCtl)
{
	POSITION thePos;
	
	for (thePos = itsSizableCtls.GetHeadPosition(); thePos != NULL; )
		if ( ((TSizableCtl*)itsSizableCtls.GetNext(thePos))->itsHWnd == aSCtl.itsHWnd )
		{
			TRACE1("Control already registered for automatic resizing, HWND = %lu\n", aSCtl.itsHWnd);
			return;
		}
	
	itsSizableCtls.AddTail((void*)new TSizableCtl(aSCtl));
}

//-----------------------------------------------------------------------------
void LSizable::InitAllCtlsPos()
{
	CWnd* theCWnd = dynamic_cast<CWnd*>(this);
	ASSERT(theCWnd != 0);

	CRect theRect;
	theCWnd->GetClientRect(theRect);
	itsInitSize = theRect.Size();

	POSITION thePos;
	TSizableCtl* theCtl;
	for (thePos = itsSizableCtls.GetHeadPosition(); thePos != NULL; )
	{
		theCtl = (TSizableCtl*)itsSizableCtls.GetNext(thePos);
		::GetWindowRect(theCtl->itsHWnd, &theCtl->itsInitRect);
		theCWnd->ScreenToClient(&theCtl->itsInitRect);
	}
}

//-----------------------------------------------------------------------------
void LSizable::ReInitCtlPos(CWnd* aCtl)
{
	ASSERT(aCtl != 0);

	CWnd* theCWnd = dynamic_cast<CWnd*>(this);
	ASSERT(theCWnd != 0);
	ASSERT(itsInitSize.cx > 0);

	POSITION thePos;
	TSizableCtl* theCtl;
	HWND theCtlHwnd = aCtl->GetSafeHwnd();
	
	for (thePos = itsSizableCtls.GetHeadPosition(); thePos != NULL; )
	{
		theCtl = (TSizableCtl*)itsSizableCtls.GetNext(thePos);
		if (theCtlHwnd == theCtl->itsHWnd)
		{
			::GetWindowRect(theCtl->itsHWnd, &theCtl->itsInitRect);
			theCWnd->ScreenToClient(&theCtl->itsInitRect);
			break;
		}
	}
}
