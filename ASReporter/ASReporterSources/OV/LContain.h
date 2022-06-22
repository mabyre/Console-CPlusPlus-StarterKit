//=======================================================================
// LContain.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// Description:
//		Base class providing default common tools for Control containers
//      (LDialog, LFormView, LPropertyPage, ...)
//
// Revision history :
//
//=======================================================================

#ifndef LCONTAIN_H
#define LCONTAIN_H

#include <afxwin.h>
#include "LDefines.h"

//-----------------------------------------------------------------------
#ifdef OVF_CF_PROPERTYPAGE
	class LPropertyPage;
#endif

//-----------------------------------------------------------------------
class LContainer
{
public:
	LContainer();
	virtual ~LContainer();

#ifdef OVF_CF_TIPS
	virtual void	RegisterTip(HWND aHwnd, LPCSTR aText);
#endif

	virtual void	XCenterCtl(CWnd* aCWnd);
	virtual void	YCenterCtl(CWnd* aCWnd);
	virtual void	CenterCtl(CWnd* aCWnd);
	virtual void	XCenterCtl(UINT anID);
	virtual void	YCenterCtl(UINT anID);
	virtual void	CenterCtl(UINT anID);

	virtual void	GetCtlRect(UINT anID, RECT* aRect);
	virtual void	GetCtlRect(CWnd* aCWnd, RECT* aRect);

#ifdef OVF_CF_PROPERTYPAGE
	virtual LPropertyPage* CreatePropertyPage(UINT anID);
#endif

protected:
	CWnd*		itsCWndThis;

	virtual void	Start();
	virtual void 	End();

	virtual BOOL 	DoInit(CWnd* aCWnd);
	virtual void  	DoDestroy();

	// Return 0 if you don't want context menu
	virtual UINT	GetContextMenuID(CWnd* aWnd, const CPoint& aPoint, int* aDefaultItem, CWnd** aCommandHandler);
	// By default the command handler is the parent frame for a view or the dialog itself
	// FALSE is returned is the context menus are not managed. Default should be called in that case
	virtual BOOL	DoContextMenu(CWnd* aWnd, CPoint aPoint, CWnd* aDefaultCommandHandler);

#ifdef OVF_CF_TIPS
	CMapPtrToPtr	itsTipsMap;
	virtual int		DoToolHitTest(int aHit, CPoint aPoint, TOOLINFO* aTI) const;
	virtual BOOL	DoTipNeedText(TOOLTIPTEXT* aTTT);
#endif
};

//-----------------------------------------------------------------------
#endif //LCONTAIN_H
