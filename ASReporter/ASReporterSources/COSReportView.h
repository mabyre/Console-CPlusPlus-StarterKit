// ===========================================================================
//	COSReportView.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#if !defined(AFX_COSREPORTVIEW_H__EAA45CEF_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_)
#define AFX_COSREPORTVIEW_H__EAA45CEF_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_

// ---------------------------------------------------------------------------
#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

// ---------------------------------------------------------------------------
class COSReportView : public CEditView
{
public:
	virtual	~COSReportView();

	COSReportDoc* GetDocument();

	void	DumpText(const CString& aText);

protected:
	COSReportView();

	LOGFONT		itsLF;
	CFont*		itsFont;
	CString		itsFindString;
	BOOL		itsfFindNext, itsfFindCase;

	void	ChangeFont();
//	void	CheckSpace(int aLen);

	//{{AFX_VIRTUAL(COSReportView)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase);
	virtual void OnTextNotFound(LPCTSTR lpszFind);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COSReportView)
	afx_msg void OnFont();
	afx_msg void OnEditClearAll();
	afx_msg void OnEditFindNext();
	afx_msg void OnEditFindPrev();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLine();
	afx_msg void OnEditGoto();
	//}}AFX_MSG
	LRESULT OnSetTitle(WPARAM aWP, LPARAM aLP);
	LRESULT OnNewTrace(WPARAM aWP, LPARAM aLP);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(COSReportView)
};

// ---------------------------------------------------------------------------
#ifndef _DEBUG  // debug version in COSReportView.cpp
inline COSReportDoc* COSReportView::GetDocument()
{
	return (COSReportDoc*)m_pDocument;
}
#endif

//{{AFX_INSERT_LOCATION}}

// ---------------------------------------------------------------------------
#endif // !defined(AFX_COSREPORTVIEW_H__EAA45CEF_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_)
