// ===========================================================================
//	OSReport.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#include "stdafx.h"
#include "OSReport.h"

#include "CMainFrm.h"
#include "COSReportDoc.h"
#include "COSReportView.h"
#include "CAboutDlg.h"
#include "CFiltersDlg.h"
#include "CTipDlg.h"
#include "CChildFrame.h"

#include "LMultDoc.h"
#include "LOVReg.h"
#include "LBitmap.h"
#include "LButton.h"
#include "LTab.h"
#include "LStatic.h"
#include "LURL.h"
#include "LHLine.h"

#include "PMW32Hlp.h"
#include "PMTCPTr.h" // For PMTCPTracer_kPort

// ---------------------------------------------------------------------------
#define PM_TRACEMODULE USER0
PM_DEFINE_TRACE_CODE(Debug, 1);

#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(COSReportApp, CWinApp)
	//{{AFX_MSG_MAP(COSReportApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FONT_GLOBAL, OnFontGlobal)
	ON_COMMAND(ID_FILTERS_GLOBAL, OnFiltersGlobal)
	ON_COMMAND(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
	ON_COMMAND(ID_HELP_TIPDAY, OnHelpTipday)
	ON_UPDATE_COMMAND_UI(ID_HELP_TIPDAY, OnUpdateHelpTipday)
	//}}AFX_MSG_MAP
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

COSReportApp gApp;

const char* const COSReportApp::sSettingsSection = _T("Settings");
const char* const COSReportApp::sFontEntry = _T("Font");
const char* const COSReportApp::sWPEntry = _T("LastWP");
const char* const COSReportApp::sOnTopEntry = _T("OnTop");
const char* const COSReportApp::sMDIMaximizedEntry = _T("MDIMaximized");

const char* const COSReportApp::sDisplaySpecialsEntry = _T("DisplaySpecials");
const char* const COSReportApp::sDisplayNameEntry = _T("DisplayName");
const char* const COSReportApp::sDisplayTSEntry = _T("DisplayTS");
const char* const COSReportApp::sAllFiltersEntry = _T("AllFilters");
const char* const COSReportApp::sNoFilterEntry = _T("NoFilter");
const char* const COSReportApp::sDisplayTIDEntry = _T("DisplayTID");
const char* const COSReportApp::sSeparatorEntry = _T("Separator");
const char* const COSReportApp::sViewBufferSizeEntry = _T("ViewBufferSize");
const char* const COSReportApp::sListenPortEntry = _T("ListenPort");
const char* const COSReportApp::sDisplayBAddrEntry = _T("DisplayBAddr");

// ---------------------------------------------------------------------------
COSReportApp::COSReportApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	itsListenThread = 0;
	itsMainFont = 0;
}

// ---------------------------------------------------------------------------
BOOL COSReportApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	if (GetVersion() < 0x80000000)
		itsfNT = pmtrue;
	else
		itsfNT = pmfalse;

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(AFX_IDS_COMPANY_NAME);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	itsDocTemplate = new LMultiDocTemplate(
		IDR_OSREPOTYPE,
		RUNTIME_CLASS(COSReportDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(COSReportView));
	AddDocTemplate(itsDocTemplate);

	PMError theError = PMOSWin32Helper::Init(m_hInstance, pmtrue);
	if (theError.Error())
	{
		::MessageBeep(MB_ICONSTOP);
		switch (theError.GetError())
		{
		case kPMOSHelperInitTCPError:
			::AfxMessageBox(IDS_ERROR_OSINIT_TCP, MB_OK | MB_ICONSTOP);
			break;

//		case kPMOSHelperGeneralError:
//		case kPMOSHelperInitPMError:
		default:
			::AfxMessageBox(IDS_ERROR_OSINIT_PM, MB_OK | MB_ICONSTOP);
			break;
		}
		return FALSE;
	}

	RegisterControlClasses();

	// Build the filter list
	itsMainTraceConfig = new CTraceConfig;
	if (!itsMainTraceConfig->Load())
	{
		::AfxMessageBox(IDS_CANNOT_LOAD_FILTERS, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	CChildFrame::sfMDIMaximized = GetProfileInt(sSettingsSection, sMDIMaximizedEntry, 1);

	{
		pmuint32	theMin, theMax, theDefault;

		if (itsfNT)
		{
			// NT
			theMin = 131072;
			// (Must be < 1024*1024-1 (See ViewEdit.cpp))
			theMax = 1040000;
			theDefault = 262144;
		}
		else
		{
			// Win3.1, 95, 98
			theMin = 16000;
			// (Must be < 1024*1024-1 (See ViewEdit.cpp))
			theMax = 32000;
			theDefault = 32000;
		}
		itsViewBufferSize = GetProfileInt(sSettingsSection, sViewBufferSizeEntry, theDefault);
		if (itsViewBufferSize < theMin)
			itsViewBufferSize = theMin;
		else
			if (itsViewBufferSize > theMax)
				itsViewBufferSize = theMax;
	}
	WriteProfileInt(sSettingsSection, sViewBufferSizeEntry, itsViewBufferSize);

	itsListenPort = GetProfileInt(sSettingsSection, sListenPortEntry, 0);
	if (itsListenPort == 0)
	{
		itsListenPort = PMTCPTracer_kPort;
		WriteProfileInt(sSettingsSection, sListenPortEntry, itsListenPort);
	}

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		// if you specify /EMBEDDED, we won't make an success/failure box
		// this use of /EMBEDDED is not related to OLE
		cmdInfo.m_bRunEmbedded = TRUE;
		Unregister();
		return FALSE;
	}

	// Don't load before else Unregister does not work
//	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	// Shown in CMainFrame::LoadFrame
	//pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	itsListenThread = ::AfxBeginThread(OSRListenThreadProc, 0);
	if (itsListenThread == 0)
	{
		::MessageBeep(MB_ICONSTOP);
		::AfxMessageBox(IDS_ERROR_LISTEN, MB_OK | MB_ICONSTOP);
		PM_TRACE(Debug, TL("Unable to start the listening thread."));
		return FALSE;
	}
	else
		PM_TRACE(Debug, TL("Listening thread started successfully."));
	itsListenThread->m_bAutoDelete = FALSE;

	BYTE*	theBuffer;
	UINT	theCount;
	if (::AfxGetApp()->GetProfileBinary(COSReportApp::sSettingsSection, COSReportApp::sFontEntry, &theBuffer, &theCount))
	{
		if (theCount == sizeof(LOGFONT))
		{
			itsMainFont = new LOGFONT;
			::memcpy(itsMainFont, theBuffer, sizeof(LOGFONT));
		}
		delete[] theBuffer;
	}

	// m_nCmdShow may be set to SW_SHOWMINIMIZED by CMainFrame::LoadFrame
	if (cmdInfo.m_bShowSplash && m_nCmdShow != SW_SHOWMINIMIZED)
	{
		CTipDlg theDlg(m_pMainWnd);
		if (theDlg.DisplayOnStartup() && theDlg.HasTips())
			theDlg.DoModal();
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
#include <afxadv.h>
int COSReportApp::ExitInstance() 
{
	// For memory leaks only
	_AFX_THREAD_STATE* pThreadState = ::AfxGetThreadState();

	CToolTipCtrl* pToolTip = pThreadState->m_pToolTip;
	if (pToolTip != NULL)
	{
		pToolTip->DestroyWindow();
		delete pToolTip;
		pThreadState->m_pToolTip = NULL;
		pToolTip = NULL;
	}

	SaveStdProfileSettings();
	if (m_pRecentFileList != NULL)
	{
		delete m_pRecentFileList;
		m_pRecentFileList = 0;
	}

	// A mutex may be necessary here between the ListenLoop
	if (!itsAcceptor.IsNull())
	{
		itsAcceptor->Cancel();
		// Don't do this here or it will crashes as the acceptor is still
		// executing code and the this pointer will become invalid
//			itsAcceptor = 0;
	}
	if (itsListenThread != 0)
	{
		itsListenThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
		if (::WaitForSingleObject(itsListenThread->m_hThread, 3000) != WAIT_OBJECT_0)
			PM_TRACE(Debug, TL("Unable to stop the listening thread."));
		else
			PM_TRACE(Debug, TL("Listening thread stopped successfully."));

		delete itsListenThread;
	}

	delete itsMainFont;

	UnregisterControlClasses();
	
	itsMainTraceConfig = 0;
	PMOSWin32Helper::Close();

	return CWinApp::ExitInstance();
}

//----------------------------------------------------------------------------
void COSReportApp::RegisterControlClasses()
{
	REGISTER_OV_CONTROL(LButton);
	REGISTER_OV_CONTROL(LBitmap);
	REGISTER_OV_CONTROL(LTab);
	REGISTER_OV_CONTROL(LStatic);
	REGISTER_OV_CONTROL(LURL);
	REGISTER_OV_CONTROL(LHLine);
}

//----------------------------------------------------------------------------
void COSReportApp::UnregisterControlClasses()
{
	UNREGISTER_OV_CONTROL(LButton);
	UNREGISTER_OV_CONTROL(LBitmap);
	UNREGISTER_OV_CONTROL(LTab);
	UNREGISTER_OV_CONTROL(LStatic);
	UNREGISTER_OV_CONTROL(LURL);
	UNREGISTER_OV_CONTROL(LHLine);
}

// ---------------------------------------------------------------------------
void COSReportApp::OnAppAbout()
{
	CAboutDlg theDlg(m_pMainWnd);
	theDlg.DoModal();
}

// ---------------------------------------------------------------------------
void COSReportApp::NewSocket(PMSocketRef aSocket)
{
	COSReportDoc* theDoc = (COSReportDoc*)itsDocTemplate->OpenDocumentFile(0, aSocket.Get());
	if (theDoc == 0)
	{
		PM_TRACE(Debug, TL("COSReportApp::NewSocket(): Failed to create the new document."));
		return;
	}
	CWinThread* theThread = ::AfxBeginThread(OSRReadThreadProc, theDoc, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (theThread == 0)
	{
		PM_TRACE(Debug, TL("COSReportApp::NewSocket(): Unable to start the reader thread"));
		::MessageBeep(MB_ICONSTOP);
		::AfxMessageBox(IDS_ERROR_READ, MB_OK | MB_ICONSTOP);
		theDoc->GetReportView()->GetParentFrame()->SendMessage(WM_CLOSE);
	}
	else
	{
		PM_TRACE(Debug, TL("COSReportApp::NewSocket(): Reader thread (%lu) started successfully.", theThread->m_nThreadID));
		theThread->m_bAutoDelete = FALSE;
		theThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
		theThread->ResumeThread();
		theDoc->itsReadThread = theThread;
	}
}

// ---------------------------------------------------------------------------
void COSReportApp::OnFontGlobal() 
{
	DWORD theFlags = CF_EFFECTS | CF_SCREENFONTS | CF_FORCEFONTEXIST | CF_FIXEDPITCHONLY;
	if (itsMainFont != 0)
		theFlags |= CF_INITTOLOGFONTSTRUCT;

	CFontDialog theDlg(itsMainFont, theFlags, 0, m_pMainWnd);
	if (theDlg.DoModal() == IDOK)
	{
		if (itsMainFont == 0)
			itsMainFont = new LOGFONT;
		::memset(itsMainFont, 0, sizeof(LOGFONT));

		theDlg.GetCurrentFont(itsMainFont);
		WriteProfileBinary(COSReportApp::sSettingsSection, COSReportApp::sFontEntry, (BYTE*)itsMainFont, sizeof(LOGFONT));
	}
}

// ---------------------------------------------------------------------------
void COSReportApp::OnFiltersGlobal() 
{
	CFiltersDlg	theDlg(itsMainTraceConfig, pmfalse, m_pMainWnd);
	if (theDlg.DoModal() == IDOK)
		itsMainTraceConfig->Save();
}

// ---------------------------------------------------------------------------
void COSReportApp::OnWindowCloseAll() 
{
	itsDocTemplate->CloseAllDocuments(FALSE);
}

// ---------------------------------------------------------------------------
void COSReportApp::OnHelpTipday() 
{
	CTipDlg theDlg(::AfxGetMainWnd());
	if (theDlg.HasTips())
		theDlg.DoModal();
}

// ---------------------------------------------------------------------------
void COSReportApp::OnUpdateHelpTipday(CCmdUI* pCmdUI) 
{
	CTipDlg theDlg(::AfxGetMainWnd());
	pCmdUI->Enable(theDlg.HasTips());
}

// ---------------------------------------------------------------------------
void COSReportApp::SetMainConfig(CTraceConfigRef aConfig)
{
	itsMainTraceConfig = new CTraceConfig(*aConfig.Get());
}
