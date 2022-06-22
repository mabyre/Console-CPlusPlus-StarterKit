//=======================================================================
// LURL.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#ifndef LURL_H
#define LURL_H

//-----------------------------------------------------------------------
#include "LStatic.h"

//-----------------------------------------------------------------------
// Options: 
//		CURSOR:			Id of the cursor
//		COLOR:			Normal color
//		CLICKEDCOLOR:	Once selected color
//		THREADED:		Launch browser in thread
//		URL:			Specify the URL. If not used the static text is used.

//-----------------------------------------------------------------------
class LURL : 
	public LStatic
{
public:
	virtual ~LURL();

protected:
	UINT		itsCursorID;
	HCURSOR		itsCursor;
	COLORREF	itsColor, itsClickedColor;
	CString		itsURL;

	LURL();

	virtual void DoAttribute(char* anAttribute, char* aValue);
	virtual void Init();

	static UINT LaunchBrowser(LPVOID aParam); // May be a Thread Func

	afx_msg UINT	OnNcHitTest(CPoint aPoint);
	afx_msg void	OnLButtonUp(UINT aFlags, CPoint aPoint);
	afx_msg BOOL	OnSetCursor(CWnd* aWnd, UINT aHitTest, UINT aMessage);
	afx_msg HBRUSH	CtlColor(CDC* aDC, UINT aCtlColor);
	
	DECLARE_OV_SUBCONTROL(LURL)
	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------
#endif //LURL_H
