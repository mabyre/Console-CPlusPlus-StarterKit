// ===========================================================================
//	CChildFrame.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#if !defined(AFX_CCHILDFRAME_H__324462D1_13B2_11D1_9F2A_0020AFFA70C1__INCLUDED_)
#define AFX_CCHILDFRAME_H__324462D1_13B2_11D1_9F2A_0020AFFA70C1__INCLUDED_

// ---------------------------------------------------------------------------
#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

// ---------------------------------------------------------------------------
class CChildFrame : public CMDIChildWnd
{
friend class COSReportApp;

protected:
	BOOL		itsfManageMaximized;
	static BOOL	sfMDIMaximized;

	CChildFrame();
	virtual ~CChildFrame();

	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	protected:
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChildFrame)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_DYNCREATE(CChildFrame)
	DECLARE_MESSAGE_MAP()
};

// ---------------------------------------------------------------------------
//{{AFX_INSERT_LOCATION}}

// ---------------------------------------------------------------------------
#endif // !defined(AFX_CCHILDFRAME_H__324462D1_13B2_11D1_9F2A_0020AFFA70C1__INCLUDED_)
