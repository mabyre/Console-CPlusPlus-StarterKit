//=======================================================================
// LTab.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// Attributes:
//  PAGE=Dialog ID[,Image index[,title]] (Tab text is the dialog title or the title specified)
//	SIZETOPAGE
//	CENTERPAGES
//	IMAGE=ID,n
//  TRANSCOLOR=r,g,b
//	COLORBITS=n (n = 0(DDB), 1 (COLOR), 4, 8, 16, 24, 32)
//=======================================================================

#ifndef LTAB_H
#define LTAB_H

//-----------------------------------------------------------------------
#include "LControl.h"
#include <afxcmn.h>
#include <afxext.h>
class LPropertyPage;
class LImageList;

//-----------------------------------------------------------------------
class LTab : 
	public CTabCtrl,
	public LOVControl
{
public:
	virtual ~LTab();

	virtual CPropertyPage*	AddPage(UINT anID, UINT anILIndex = (UINT)-1, LPCTSTR aTitle = 0);
	virtual CPropertyPage*	AddPage(CPropertyPage* aPage, UINT anILIndex = (UINT)-1, LPCTSTR aTitle = 0);

	CPropertyPage*	GetPage(int anIndex);

	void			InitPagesSize();
	void			RepositionPages();

protected:
	LImageList*		itsIL;
	COLORREF		itsTransColor;
	UINT			itsColorBits;
	CDWordArray		itsInitPageWidth, itsInitPageHeight;
	CSize			itsInitSize, itsMaxPageSize, itsInitMaxPageSize;

	LTab(); 

	virtual void	DoAttribute(char* anAttribute, char* aValue);
	virtual void	Init();

	afx_msg void	OnChangingPropertyPage(NMHDR* aNMHDR, LRESULT* aLResult);
	afx_msg BOOL	OnChangedPropertyPage(NMHDR* aNMHDR, LRESULT* aLResult);
	afx_msg LRESULT	OnSetCurSel(WPARAM aWParam, LPARAM aLParam);
	afx_msg void	OnSize(UINT aType, int aCx, int aCy);

	DECLARE_OV_BASECONTROL(LTab)
	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------

#endif
