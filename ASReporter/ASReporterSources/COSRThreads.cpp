// ===========================================================================
//	COSRThreads.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#include "stdafx.h"
#include "OSReport.h"
#include "COSReportDoc.h"

#include "PMTrace.h"

// ---------------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

#define PM_TRACEMODULE USER0
PM_DEFINE_TRACE_CODE(Debug, 1);

// ---------------------------------------------------------------------------
UINT COSReportApp::OSRListenThreadProc(LPVOID pParam)
{
	BOOL thefQuit = FALSE;
	while (!thefQuit)
	{
		try
		{
			COSReportApp::Get()->Listen();
			thefQuit = TRUE;
		}
		catch(...)
		{
#ifndef NDEBUG
			::MessageBeep(MB_ICONSTOP);
			::MessageBox(0, "Exception caught in listening thread.", "ObjectSet Reporter", MB_OK | MB_ICONSTOP);
#endif
			PM_TRACE(Debug, TL("COSReportApp::OSRListenThreadProc(): Exception caught. Relistening."));
		}
	}

	return 0;
}

// ---------------------------------------------------------------------------
UINT COSReportApp::OSRReadThreadProc(LPVOID pParam)
{
	COSReportDoc*	theDoc = (COSReportDoc*)pParam;

	try
	{
		theDoc->DoReadLoop();
	}
	catch(...)
	{
		PM_TRACE(Debug, TL("COSReportApp::OSRReadThreadProc(): Exception caught"));
	}
	PM_TRACE(Debug, TL("COSReportApp::OSRReadThreadProc(): Reader thread (%lu) terminated.", ::GetCurrentThreadId()));
	return 0;
}

// ---------------------------------------------------------------------------
UINT COSReportApp::OSRFormatThreadProc(LPVOID pParam)
{
	COSReportDoc*	theDoc = (COSReportDoc*)pParam;

	try
	{
		theDoc->DoFormatLoop();
	}
	catch(...)
	{
		PM_TRACE(Debug, TL("COSReportApp::OSRFormatThreadProc(): Exception caught."));
	}
	PM_TRACE(Debug, TL("COSReportApp::OSRReadThreadProc(): Format thread (%lu) terminated.", ::GetCurrentThreadId()));
	return 0;
}

// ---------------------------------------------------------------------------
void COSReportApp::Listen()
{
	PMSocketRef theSocket;

	itsAcceptor = PMTCPSocketFactory::NewDefaultAcceptorFor(COSReportApp::Get()->GetListenPort());
	if (itsAcceptor.IsNull())
	{
		PM_TRACE(Debug, TL("COSReportApp::Listen(): listen failed."));
		::MessageBeep(MB_ICONSTOP);
		::AfxMessageBox(IDS_ERROR_LISTEN, MB_OK | MB_ICONSTOP);
		::AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);
		return;
	}

	while (1)
	{
		theSocket = itsAcceptor->WaitForConnection();
		if (theSocket.IsNull())
		{
			PM_TRACE(Debug, TL("COSReportApp::Listen(): Acceptor aborted."));
			itsAcceptor = 0;
			return;
		}
		// We can not call OnFileNew directly because we are not in the right (main) thread
		// This is safe to pass a PMSocket* as this is a SendMessage and then a ref will be
		// taken by the message handler without exiting this function
		// PostThreadMessage can not be used for this reason
		// Unfortunately this message will not reach the app but the mainframe from where
		// it will be rerouted to the app :-(
		::AfxGetApp()->m_pMainWnd->SendMessage(WM_NEW_SOCKET, 0, (LPARAM)theSocket.Get());
	}
}
