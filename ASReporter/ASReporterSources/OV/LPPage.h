//=======================================================================
// LPPage.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#ifndef LPPAGE_H
#define LPPAGE_H

#include "LContain.h"
#include <afxdlgs.h>

#ifndef OVF_CF_PROPERTYPAGE
	#error You must define OVF_CF_PROPERTYPAGE in LDefines.h to use property pages
#endif
#ifdef OVF_CF_SIZABLE
	#include "LSizable.h"
#endif

//-----------------------------------------------------------------------
class LPropertyPage : 
	public CPropertyPage, 
	public LContainer
#ifdef OVF_CF_SIZABLE
	, public LSizable
#endif
{
public:
	LPropertyPage();
	// afAutoDelete should be set to false for Pages in CPropertySheets
	LPropertyPage(UINT aDialogID, BOOL afAutoDelete = TRUE, UINT aCaptionID = 0);
	virtual ~LPropertyPage();

protected:
	BOOL	itsfAutoDelete;

#ifdef OVF_CF_TIPS
	afx_msg BOOL	OnTipNeedText(UINT id, NMHDR *aTTTStruct, LRESULT *aResult);
	virtual int		OnToolHitTest(CPoint aPoint, TOOLINFO* aTI) const;
#endif

	virtual BOOL 	OnInitDialog();

	virtual void	PostNcDestroy();
	
	afx_msg	int	  	OnCreate(LPCREATESTRUCT aCS);
	afx_msg	void  	OnDestroy();
	
#ifdef OVF_CF_SIZABLE
	BOOL			itsfCanManageOnSize;

	afx_msg void	OnSize(UINT aType, int aCx, int aCy);
#endif
	afx_msg void	OnContextMenu(CWnd* aWnd, CPoint aPoint);

	DECLARE_DYNAMIC(LPropertyPage)
    DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------
#endif
