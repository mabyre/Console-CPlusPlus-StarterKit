// ===========================================================================
//	COSReportDoc.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#if !defined(AFX_COSREPORTDOC_H__EAA45CED_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_)
#define AFX_COSREPORTDOC_H__EAA45CED_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_

// ---------------------------------------------------------------------------
#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

#include "LDoc.h"

#include "PMRC.h"
#include "PMCriSec.h"
PMARRAYDEFINE(PMBuffer);
PMDEFINE(PMXTracer);
class COSReportView;

// ---------------------------------------------------------------------------
class COSReportDoc : public LDocument
{
	friend class COSReportApp;
public:
	virtual ~COSReportDoc();

	// Public because accessed by the app
	void	DoReadLoop();
	void	DoFormatLoop();
	
	COSReportView*	GetReportView();
	CTraceConfigRef	GetConfig() { return itsConfig; }

protected:
	CWinThread*			itsReadThread;
	CWinThread*			itsFormatThread;
	pmbool				itsfStop;
	PMSocketRef			itsSocket;
	PMBufferArrayRef	itsBuffers;
	PMCriticalSection	itsBuffersCS, itsSocketCS;
	PMXTracerRef		itsTracer; // Used to decode blocks
	pmbool				itsfStartOfLine;
	CString				itsCurrentString;
	CTraceConfigRef		itsConfig;
	HANDLE				itsFormatStopEvent;
	pmuint32			itsFirstTS;

	COSReportDoc();

	// Extended function to use across threads
	void SetTitleEx(LPCSTR aString);

	void DumpTraceBlock(COSReportView* aView, PMBufferRef aTrace);

	void DoDisplay(COSReportView* aView);

	//{{AFX_VIRTUAL(COSReportDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COSReportDoc)
	afx_msg void OnFilters();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(COSReportDoc)
};

// ---------------------------------------------------------------------------
//{{AFX_INSERT_LOCATION}}

// ---------------------------------------------------------------------------
#endif // !defined(AFX_COSREPORTDOC_H__EAA45CED_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_)
