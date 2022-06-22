// ===========================================================================
//	CMainFrm.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#include "stdafx.h"
#include "OSReport.h"

#include "CMainFrm.h"
#include "COSReportView.h"
#include "COSReportDoc.h"

// ---------------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------
// Stolen in BARSTAT.CPP
#define CX_PANE_BORDER 6    // 3 pixels on each side of each pane

void CStatusBarEx::SetSBPaneSize(int aPaneID, const CString& aString)
{
	int theIndex = CommandToIndex(aPaneID);
	if (theIndex == -1)
		return;

	HFONT hFont = (HFONT)SendMessage(WM_GETFONT);
	CClientDC dcScreen(NULL);
	HGDIOBJ hOldFont = NULL;
	if (hFont != NULL)
		hOldFont = dcScreen.SelectObject(hFont);

	int theSize = dcScreen.GetTextExtent(aString).cx;
	if (theSize <= 0)
		return;
	SetPaneInfo(theIndex, aPaneID, SBPS_NORMAL, theSize + CX_PANE_BORDER);
	if (hOldFont != NULL)
		dcScreen.SelectObject(hOldFont);
}

// ---------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_ONTOP, OnUpdateOntop)
	ON_COMMAND(ID_ONTOP, OnOntop)
	ON_COMMAND(ID_HELP_INDEX, CWnd::OnHelpIndex)
	ON_COMMAND(ID_HELP_FINDER, CWnd::OnHelpFinder)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NEW_SOCKET, OnNewSocket)
	ON_UPDATE_COMMAND_UI(ID_PANE_LINE, OnUpdatePaneLine)
	ON_UPDATE_COMMAND_UI(ID_PANE_FILTERS, OnUpdatePaneFilters)
END_MESSAGE_MAP()

static UINT gStatusIndicators[] =
{
	ID_SEPARATOR,
/*
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
*/
	ID_PANE_LINE,
	ID_PANE_FILTERS,
};

// ---------------------------------------------------------------------------
CMainFrame::CMainFrame()
{
}

// ---------------------------------------------------------------------------
CMainFrame::~CMainFrame()
{
}

// ---------------------------------------------------------------------------
BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (CMDIFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		::AfxGetThread()->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);

		BYTE*				theBuffer;
		UINT				theCount;
		BOOL				thefShow = TRUE;
		if (::AfxGetApp()->GetProfileBinary(COSReportApp::sSettingsSection, COSReportApp::sWPEntry, &theBuffer, &theCount))
		{
			if (theCount == sizeof(WINDOWPLACEMENT))
			{
				WINDOWPLACEMENT* theWP = (WINDOWPLACEMENT*)theBuffer;
				if (theWP->length == sizeof(WINDOWPLACEMENT))
				{
					SetWindowPlacement(theWP);
					::AfxGetApp()->m_nCmdShow = theWP->showCmd;
					thefShow = FALSE;
				}
			}
			delete[] theBuffer;
		}
		if (thefShow)
			ShowWindow(::AfxGetApp()->m_nCmdShow);
		return TRUE;
	}

	return FALSE;
}

// ---------------------------------------------------------------------------
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	return CMDIFrameWnd::PreCreateWindow(cs);
}

// ---------------------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!itsToolBar.Create(this) || !itsToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	itsToolBar.SendMessage(TB_HIDEBUTTON, ID_ONTOP2, MAKELONG(TRUE, 0));
	itsToolBar.CalcFixedLayout(TRUE, TRUE);
	UINT theID, theStyle;
	itsToolBar.GetButtonInfo(itsToolBar.CommandToIndex(ID_ONTOP), theID, theStyle, itsNotTopImage);
	itsToolBar.GetButtonInfo(itsToolBar.CommandToIndex(ID_ONTOP2), theID, theStyle, itsOnTopImage);

	if (!itsStatusBar.Create(this) ||
		!itsStatusBar.SetIndicators(gStatusIndicators, sizeof(gStatusIndicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;
	}

	itsToolBar.SetBarStyle(itsToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	CString theString;
	theString.LoadString(AFX_IDS_APP_TITLE);
	itsToolBar.SetWindowText(theString);

	itsToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&itsToolBar);

	// Must be here else itsfOnTop initialized after first OnUpdateCmdUI
	itsfOnTop = !::AfxGetApp()->GetProfileInt(COSReportApp::sSettingsSection, COSReportApp::sOnTopEntry, 0);
	// Initialize the bitmap correctly. OnOntop inverts itsfOnTop.
	OnOntop();
	if (itsfOnTop)
		SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);

	if (!itsNAStr.LoadString(IDS_TRACES_NA))
		itsNAStr = "N/A";
	if (!itsUseStr.LoadString(IDS_TRACES_FILTERS))
		itsUseStr = "Using filters";
	if (!itsNoneStr.LoadString(IDS_TRACES_NONE))
		itsNoneStr = "No traces";
	if (!itsAllStr.LoadString(IDS_TRACES_ALL))
		itsAllStr = "All traces";
	if (!itsLineNbStr.LoadString(IDS_LINE_NUMBER))
		itsLineNbStr = "Line: %lu";

	return 0;
}

// ---------------------------------------------------------------------------
void CMainFrame::OnClose() 
{
	::AfxGetApp()->WriteProfileInt(COSReportApp::sSettingsSection, COSReportApp::sOnTopEntry, itsfOnTop);
	WINDOWPLACEMENT theWP;
	theWP.length = sizeof(WINDOWPLACEMENT);
	if (GetWindowPlacement(&theWP))
		::AfxGetApp()->WriteProfileBinary(COSReportApp::sSettingsSection, COSReportApp::sWPEntry, (BYTE*)&theWP, sizeof(WINDOWPLACEMENT));

	CMDIFrameWnd::OnClose();
}

// ---------------------------------------------------------------------------
LRESULT CMainFrame::OnNewSocket(WPARAM aWParam, LPARAM aLParam)
{
	PMSocketRef theSocket((PMSocket*)aLParam);
	COSReportApp::Get()->NewSocket(theSocket);

	return 1;
}

// ---------------------------------------------------------------------------
void CMainFrame::OnUpdateOntop(CCmdUI* pCmdUI) 
{
	// Don't check in LFlatToolBar
	if (pCmdUI->m_pOther == 0 || !pCmdUI->m_pOther->IsKindOf(RUNTIME_CLASS(LFlatToolBar)))
		pCmdUI->SetCheck(itsfOnTop);
}

// ---------------------------------------------------------------------------
void CMainFrame::OnOntop() 
{
	itsfOnTop = !itsfOnTop;
	SetWindowPos(itsfOnTop?&wndTopMost:&wndNoTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
	
	UINT theID, theStyle;
	int theIndex = itsToolBar.CommandToIndex(ID_ONTOP), theImage;
	itsToolBar.GetButtonInfo(theIndex, theID, theStyle, theImage);
	itsToolBar.SetButtonInfo(theIndex, theID, theStyle, itsfOnTop?itsOnTopImage:itsNotTopImage);

	CRect theRect;
	itsToolBar.GetItemRect(theIndex, &theRect);
	itsToolBar.InvalidateRect(&theRect);
}

// ---------------------------------------------------------------------------
void CMainFrame::OnUpdatePaneLine(CCmdUI *pCmdUI) 
{
	CView*		theView = 0;
	CFrameWnd*	theFrame = GetActiveFrame();
	if (theFrame != 0)
		theView = theFrame->GetActiveView();

	CString theString;
	if (theView == 0 || !theView->IsKindOf(RUNTIME_CLASS(CEditView)))
		theString = itsNAStr;
	else
		theString.Format(itsLineNbStr, (unsigned long)((CEditView*)theView)->GetEditCtrl().LineFromChar());

	pCmdUI->Enable();
	pCmdUI->Enable();
	if (itsStatusBar.GetPaneText(itsStatusBar.CommandToIndex(ID_PANE_LINE)).Compare(theString) == 0)
		return;
	itsStatusBar.SetSBPaneSize(ID_PANE_LINE, theString);
    pCmdUI->SetText(theString); 
} 

// ---------------------------------------------------------------------------
void CMainFrame::OnUpdatePaneFilters(CCmdUI *pCmdUI) 
{
	CView* theView = 0;
	CFrameWnd* theFrame = GetActiveFrame();
	if (theFrame != 0)
		theView = theFrame->GetActiveView();
	
	CString theString;
	if (theView == 0 || !theView->IsKindOf(RUNTIME_CLASS(COSReportView)))
		theString = itsNAStr;
	else
	{
		CTraceConfigRef theConfig = ((COSReportView*)theView)->GetDocument()->GetConfig();
		if (theConfig->GetDisplayAll())
			theString = itsAllStr;
		else
			if (theConfig->GetDisplayNone())
				theString = itsNoneStr;
			else
				theString = itsUseStr;
	}

	pCmdUI->Enable();
	if (itsStatusBar.GetPaneText(itsStatusBar.CommandToIndex(ID_PANE_FILTERS)).Compare(theString) == 0)
		return;
	itsStatusBar.SetSBPaneSize(ID_PANE_FILTERS, theString);
    pCmdUI->SetText(theString); 
}
