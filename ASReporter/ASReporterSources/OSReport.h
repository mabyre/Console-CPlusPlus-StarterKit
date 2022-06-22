// ===========================================================================
//	OSReport.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#if !defined(AFX_OSREPORT_H__EAA45CE5_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_)
#define AFX_OSREPORT_H__EAA45CE5_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_

#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// ---------------------------------------------------------------------------
#include "resource.h"
#include "CTrGroups.h"
#include "PMTCPSoc.h"
class LMultiDocTemplate;

#define WM_NEW_SOCKET	(WM_USER + 0x3124)
#define WM_SET_TITLE	(WM_USER + 0x3125)
#define WM_NEW_TRACE	(WM_USER + 0x3126)

// ---------------------------------------------------------------------------
class COSReportApp : public CWinApp
{
	friend class COSReportDoc;
	friend class CFiltersDlg;
public:
	static const char* const	sSettingsSection;
	static const char* const	sFontEntry;
	static const char* const	sWPEntry;
	static const char* const	sOnTopEntry;
	static const char* const	sMDIMaximizedEntry;

	static const char* const	sFiltersEntry;
	static const char* const	sDisplayNameEntry;
	static const char* const	sDisplayTSEntry;
	static const char* const	sDisplaySpecialsEntry;
	static const char* const	sAllFiltersEntry;
	static const char* const	sNoFilterEntry;
	static const char* const	sDisplayTIDEntry;
	static const char* const	sSeparatorEntry;
	static const char* const	sViewBufferSizeEntry;
	static const char* const	sListenPortEntry;
	static const char* const	sDisplayBAddrEntry;

	COSReportApp();

	static COSReportApp*	Get() { return (COSReportApp*)::AfxGetApp(); }

	CTraceConfigRef			GetMainConfig() { return itsMainTraceConfig; }
	void					SetMainConfig(CTraceConfigRef aConfig);
	LOGFONT*				GetMainFont() { return itsMainFont; }
	pmuint32				GetViewBufferSize() { return itsViewBufferSize; }
	pmuint16				GetListenPort() { return itsListenPort; }
	pmbool					IsNTRunning() { return itsfNT; }

	void					NewSocket(PMSocketRef aSocket);

protected:
	LMultiDocTemplate*		itsDocTemplate;
	CTraceConfigRef			itsMainTraceConfig;
	LOGFONT*				itsMainFont;
	pmuint32				itsViewBufferSize;
	pmuint16				itsListenPort;
	pmbool					itsfNT;

	CWinThread*				itsListenThread;
	PMTCPSocketAcceptorRef	itsAcceptor;

	static UINT OSRListenThreadProc(LPVOID pParam);
	static UINT OSRReadThreadProc(LPVOID pParam);
	static UINT OSRFormatThreadProc(LPVOID pParam);
	void	Listen();

	void RegisterControlClasses();
	void UnregisterControlClasses();

	//{{AFX_VIRTUAL(COSReportApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COSReportApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFontGlobal();
	afx_msg void OnFiltersGlobal();
	afx_msg void OnWindowCloseAll();
	afx_msg void OnHelpTipday();
	afx_msg void OnUpdateHelpTipday(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

// ---------------------------------------------------------------------------
//{{AFX_INSERT_LOCATION}}

// ---------------------------------------------------------------------------
#endif // !defined(AFX_OSREPORT_H__EAA45CE5_E943_11D0_9EFC_0020AFFA70C1__INCLUDED_)
