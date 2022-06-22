// ===========================================================================
//	CMainFrm.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#if !defined(AFX_CMAINFRM_H__EAA45CE9_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_)
#define AFX_CMAINFRM_H__EAA45CE9_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_

// ---------------------------------------------------------------------------
#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

#include "LFlatBar.h"

// ---------------------------------------------------------------------------
class CStatusBarEx : public CStatusBar
{
public:
	virtual void SetSBPaneSize(int aPaneID, const CString& aString);
};

// ---------------------------------------------------------------------------
class CMainFrame : public CMDIFrameWnd
{
public:
	CMainFrame();
	virtual ~CMainFrame();

protected:
	CStatusBarEx	itsStatusBar;
	LFlatToolBar	itsToolBar;
	BOOL			itsfOnTop;
	int				itsOnTopImage, itsNotTopImage;
	CString			itsNAStr, itsUseStr, itsNoneStr, itsAllStr, itsLineNbStr;

	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

	void SetSBPaneSize(int aPaneID, const CString& aString);

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateOntop(CCmdUI* pCmdUI);
	afx_msg void OnOntop();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LRESULT OnNewSocket(WPARAM aWParam, LPARAM aLParam);
	afx_msg void OnUpdatePaneLine(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaneFilters(CCmdUI *pCmdUI);

	DECLARE_DYNAMIC(CMainFrame)
	DECLARE_MESSAGE_MAP()
};

// ---------------------------------------------------------------------------
//{{AFX_INSERT_LOCATION}}

// ---------------------------------------------------------------------------
#endif // !defined(AFX_CMAINFRM_H__EAA45CE9_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_)
