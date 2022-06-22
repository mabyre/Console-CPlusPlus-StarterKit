//=======================================================================
// LDialog.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#ifndef LDIALOG_H
#define LDIALOG_H

#include "LContain.h"

#ifdef OVF_CF_SIZABLE
	#include "LSizable.h"
	#include "LMSWnd.h"
#endif

//-----------------------------------------------------------------------
class LDialog : 
	public CDialog, 
	public LContainer
#ifdef OVF_CF_SIZABLE
	, public LSizable
	, public LMinSizeWnd
#endif
{
public:

	enum ePos
	{
		kDefault,
		kCentered
	};

	LDialog(ePos aPosition = kCentered);
	LDialog(UINT aDialogID, CWnd* aParent = NULL, ePos aPosition = kCentered);
	virtual ~LDialog();

protected:
    ePos	itsPosition;

#ifdef OVF_CF_TIPS
	afx_msg BOOL	OnTipNeedText(UINT id, NMHDR *aTTTStruct, LRESULT *aResult);
	virtual int		OnToolHitTest(CPoint aPoint, TOOLINFO* aTI) const;
#endif

	virtual BOOL 	OnInitDialog();

//	virtual void	Start(); // You can inherit it from LContainer
//	virtual void 	End();   // You can inherit it from LContainer
#ifdef OVF_CF_SIZABLE
	BOOL			itsfCanManageOnSize;

	afx_msg void	OnSize(UINT aType, int aCx, int aCy);
	afx_msg void	OnGetMinMaxInfo(MINMAXINFO* aMMI);
#endif

	afx_msg	int	  	OnCreate(LPCREATESTRUCT aCS);
	afx_msg	void  	OnDestroy();
	afx_msg void	OnContextMenu(CWnd* aWnd, CPoint aPoint);
	
	DECLARE_DYNAMIC(LDialog)
    DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------
#endif
