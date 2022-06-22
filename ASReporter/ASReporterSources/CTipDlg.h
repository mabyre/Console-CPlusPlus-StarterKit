// ===========================================================================
//	CTipDlg.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef CTipDlg_h
#define CTipDlg_h

//-----------------------------------------------------------------------
#include "LDialog.h"

//-----------------------------------------------------------------------
class CTipDlg : public LDialog
{
public:
	CTipDlg(CWnd* aParent = NULL);
	virtual ~CTipDlg();

	BOOL	HasTips();
	BOOL	DisplayOnStartup();

protected:
	UINT		itsIndex;
	BOOL		itsfOnStartup;
	static UINT cCount;

	virtual void Start();
	virtual void End();

	afx_msg void OnNext();

	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------
#endif // CTipDlg_h
