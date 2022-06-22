// ===========================================================================
//	COSReportView.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#include "stdafx.h"
#include "OSReport.h"

#include "COSReportDoc.h"
#include "COSReportView.h"

#include "CGotoDlg.h"

// ---------------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(COSReportView, CEditView)

BEGIN_MESSAGE_MAP(COSReportView, CEditView)
	//{{AFX_MSG_MAP(COSReportView)
	ON_COMMAND(ID_FONT, OnFont)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, OnEditClearAll)
	ON_COMMAND(ID_EDIT_FIND_NEXT, OnEditFindNext)
	ON_COMMAND(ID_EDIT_FIND_PREV, OnEditFindPrev)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_LINE, OnLine)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_NEXT, OnUpdateNeedFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_PREV, OnUpdateNeedFind)
	ON_COMMAND(ID_EDIT_GOTO, OnEditGoto)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
	ON_MESSAGE(WM_SET_TITLE, OnSetTitle)
	ON_MESSAGE(WM_NEW_TRACE, OnNewTrace)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------

// This could be read from the registry...
// static const int kMinCutSize = 32768;	// 0x8000
#define PM_TRACEMODULE USER0
PM_DEFINE_TRACE_CODE(Debug, 1);


// ---------------------------------------------------------------------------
COSReportView::COSReportView()
{
	itsFont = 0;
}

// ---------------------------------------------------------------------------
COSReportView::~COSReportView()
{
	delete itsFont;
}

// ---------------------------------------------------------------------------
BOOL COSReportView::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping
	cs.style |= ES_READONLY;

	return bPreCreated;
}

// ---------------------------------------------------------------------------
#ifdef _DEBUG
COSReportDoc* COSReportView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COSReportDoc)));
	return (COSReportDoc*)m_pDocument;
}
#endif //_DEBUG

// ---------------------------------------------------------------------------
LRESULT COSReportView::OnSetTitle(WPARAM aWP, LPARAM aLP)
{
	CString* theString = (CString*)aLP;
	GetDocument()->SetTitle(*theString);
	delete theString;

	return 1;
}

// ---------------------------------------------------------------------------
LRESULT COSReportView::OnNewTrace(WPARAM aWP, LPARAM aLP)
{
	CString* theString = (CString*)aLP;
	DumpText(*theString);
	delete theString;

	return 1;
}

#if 0
// ---------------------------------------------------------------------------
void COSReportView::DumpText(const CString& aText)
{
	int			theTextLen = aText.GetLength();
	CEdit&		theEdit = GetEditCtrl();
	pmuint32	theMaxEditSize = COSReportApp::Get()->GetViewBufferSize();

	// Buffer is greater than max size. Replace all the edit with the end of the string.
	if (theTextLen > theMaxEditSize)
	{
		LPCSTR theText = ((LPCSTR)aText) + theTextLen - theMaxEditSize;
		// If not at a start of line, skip the line.
		if (theText[-1] != '\n')
		{
			LPCSTR theText2 = ::strstr(theText, "\r\n");
			if (theText2 != 0)
				theText = theText2 + 2;
		}
		theEdit.SetWindowText(theText);
		theEdit.SetSel(theMaxEditSize, theMaxEditSize);
		return;
	}

	// Here, buffer is less than theMaxEditSize so we know that it will be possible
	// to display at least some text.
	int theEditLen = theEdit.GetWindowTextLength();
	int theSelStart, theSelEnd;
	theEdit.GetSel(theSelStart, theSelEnd);
	BOOL thefAtEnd = (theSelStart == theSelEnd && theSelStart == theEditLen);
	if (!thefAtEnd)
		theEdit.SetRedraw(FALSE);

	if (theTextLen + theEditLen > theMaxEditSize)
	{
		if (thefAtEnd)
			SetRedraw(FALSE);
		int theCut = theEditLen + theTextLen - theMaxEditSize;
		// Don't allow to cut a little and cut again next time.
		if (theCut < kMinCutSize)
			theCut = kMinCutSize;
		int theLineCut = theEdit.LineFromChar(theCut);
		// Cut at start of a line
//		theCut = theEdit.LineIndex(theLineCut) + theEdit.LineLength(theLineCut);
		theCut = theEdit.LineIndex(theLineCut + 1);
		theEdit.SetSel(0, theCut, FALSE);
		theEdit.ReplaceSel("");
		theEditLen -= theCut;
		if (theSelStart < theCut)
			theSelStart = 0;
		else
			theSelStart -= theCut;
		if (theSelEnd < theCut)
			theSelEnd = 0;
		else
			theSelEnd -= theCut;
		if (thefAtEnd)
			SetRedraw(TRUE);
	}

	theEdit.SetSel(theEditLen, theEditLen, FALSE);
	theEdit.ReplaceSel(aText);

	if (thefAtEnd)
	{
		theEditLen += theTextLen;
		theEdit.SetSel(theEditLen, theEditLen, TRUE);
	}
	else
	{
		theEdit.SetSel(theSelStart, theSelEnd, TRUE);
		theEdit.SetRedraw(TRUE);
		Invalidate();
	}
/*
	// Make the trace application appear when receiving traces.
	CWnd* theMainWnd = ::AfxGetMainWnd();
	if (theMainWnd)
		theMainWnd->SetWindowPos(&wndTop, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
*/
}
#endif

// ---------------------------------------------------------------------------
void COSReportView::DumpText(const CString& aText)
{
	int			theTextLen		= aText.GetLength();
	CEdit&		theEdit			= GetEditCtrl();
	pmuint32	theMaxEditSize	= COSReportApp::Get()->GetViewBufferSize();

	// Buffer is greater than max size. Replace all the edit with the end of the string.
	PM_TRACE(Debug, TL("COSReportView::DumpText(): TextLen (%lu), MaxEditSize (%lu).", (pmuint32)theTextLen, (pmuint32)theMaxEditSize));

	if (theTextLen > theMaxEditSize)
	{
		PM_TRACE(Debug, TL("COSReportView::DumpText(): Case 1."));

		LPCSTR theText = ((LPCSTR)aText) + theTextLen - theMaxEditSize;
		// If not at a start of line, skip the line.
		if (theText[-1] != '\n')
		{
			LPCSTR theText2 = ::strstr(theText, "\r\n");
			if (theText2 != 0)
				theText = theText2 + 2;
		}
		theEdit.SetWindowText(theText);
		theEdit.SetSel(theMaxEditSize, theMaxEditSize);
		return;
	}

	// Here, buffer is less than theMaxEditSize so we know that it will be possible
	// to display at least some text.

	int theEditLen = theEdit.GetWindowTextLength();
	int theSelStart, theSelEnd;
	theEdit.GetSel(theSelStart, theSelEnd);
	BOOL thefAtEnd = (theSelStart == theSelEnd && theSelStart == theEditLen);
	if (!thefAtEnd)
		theEdit.SetRedraw(FALSE);

	PM_TRACE(Debug, TL("COSReportView::DumpText(): EditLen (%lu), SelStart (%lu), SelEnd(%lu).", (pmuint32)theEditLen, (pmuint32)theSelStart, (pmuint32)theSelEnd));

	if (theTextLen + theEditLen > theMaxEditSize)
	{
		if (thefAtEnd)
			SetRedraw(FALSE);

		int theCut = theMaxEditSize / 2;
		
		// If removing theMaxEditSize / 2 is not enough, remove more...
		if (theEditLen + theTextLen - theCut > theMaxEditSize)
			theCut += (theMaxEditSize + 1) / 2;
		
		// Cut at start of a line
		PM_TRACE(Debug, TL("COSReportView::DumpText(): Case 2: Cut1 (%lu).", (pmuint32)theCut));

		if (theCut >= theEditLen)
		{
			theEdit.SetWindowText(aText);
			theEdit.EmptyUndoBuffer();
			theEditLen = theEdit.GetWindowTextLength();
			theEdit.SetSel(theEditLen, theEditLen, FALSE);
			if (thefAtEnd)
				SetRedraw(TRUE);
			PM_TRACE(Debug, TL("COSReportView::DumpText(): Case 3: EditLen final (%lu).", (pmuint32)theEditLen));
			return;
		}

		// This may return -1 if theCut is bigger than number of characters
		// This is not a problem because SetSel will then select all...

		int theLineCut = theEdit.LineFromChar(theCut);
		if (theLineCut == -1)
			theCut = -1;
		else
			theCut = theEdit.LineIndex(theLineCut + 1);
		
		PM_TRACE(Debug, TL("COSReportView::DumpText(): Case 2: LineCut (%lu), Cut2 (%lu).", (pmuint32)theLineCut, (pmuint32)theCut));

		theEdit.SetSel(0, theCut, FALSE);
		theEdit.ReplaceSel("[...]");

		theEditLen = theEdit.GetWindowTextLength();

		if (theSelStart < theCut)
			theSelStart = 0;
		else
			theSelStart -= theCut;

		if (theSelEnd < theCut)
			theSelEnd = 0;
		else
			theSelEnd -= theCut;
		
		PM_TRACE(Debug, TL("COSReportView::DumpText(): Case 2: EditLen (%lu), SelStart (%lu), SelEnd(%lu).", (pmuint32)theEditLen, (pmuint32)theSelStart, (pmuint32)theSelEnd));

		if (thefAtEnd)
			SetRedraw(TRUE);
	}

	theEdit.SetSel(theEditLen, theEditLen, FALSE);
	theEdit.EmptyUndoBuffer();
	
	// FALSE means cannot undo...
	if (COSReportApp::Get()->IsNTRunning())
		theEdit.ReplaceSel(aText, FALSE);
	else
	{
		char	theBuffer[1024];
		size_t	theLen = theTextLen, theSize;
		char*	thePtr = (char*)(const char*)aText;
		
		while (theLen)
		{
			if (theLen > sizeof(theBuffer) - 1)
				theSize = sizeof(theBuffer) - 1;
			else
				theSize = theLen;
			
			memcpy(theBuffer, thePtr, theSize);
			theBuffer[theSize] = 0;
			theEdit.ReplaceSel(theBuffer, FALSE);
			theEditLen = theEdit.GetWindowTextLength();
			theEdit.SetSel(theEditLen, theEditLen, FALSE);

			thePtr += theSize;
			theLen -= theSize;
		}
	}

	theEditLen = theEdit.GetWindowTextLength();
	
	PM_TRACE(Debug, TL("COSReportView::DumpText(): EditLen final (%lu).", (pmuint32)theEditLen));

	if (thefAtEnd)
		theEdit.SetSel(theEditLen, theEditLen, TRUE);
	else
	{
		theEdit.SetSel(theSelStart, theSelEnd, TRUE);
		theEdit.SetRedraw(TRUE);
		Invalidate();
	}
}

// ---------------------------------------------------------------------------
void COSReportView::ChangeFont()
{
	if (itsFont == 0)
		itsFont = new CFont;
	else
		itsFont->DeleteObject();

	if (!itsFont->CreateFontIndirect(&itsLF))
		itsFont->Attach(::GetStockObject(ANSI_FIXED_FONT));
	SetFont(itsFont); 
}

// ---------------------------------------------------------------------------
void COSReportView::OnInitialUpdate() 
{
	PM_TRACE(Debug, TL("CEditView::OnInitialUpdate()"));
	CEditView::OnInitialUpdate();

    GetEditCtrl().LimitText(0);
	
	LOGFONT* theLF = COSReportApp::Get()->GetMainFont();
	if (theLF != 0)
		::memcpy(&itsLF, theLF, sizeof(LOGFONT));
	else
	{
		CDC* theDC = GetDC();
		int theFontHeight = -(11 * theDC->GetDeviceCaps(LOGPIXELSY))/72;
		ReleaseDC(theDC);

		::memset(&itsLF, 0, sizeof(LOGFONT));
		itsLF.lfHeight = theFontHeight;
//		itsLF.lfWidth = 0;
//		itsLF.lfEscapement = 0;
//		itsLF.lfOrientation = 0;
		itsLF.lfWeight = FW_NORMAL;
//		itsLF.lfItalic = 0;
//		itsLF.lfUnderline = 0;
//		itsLF.lfStrikeOut = 0;
		itsLF.lfCharSet = ANSI_CHARSET;
//		itsLF.lfOutPrecision = 0;
//		itsLF.lfClipPrecision = 0;
		itsLF.lfQuality = DEFAULT_QUALITY;
		itsLF.lfPitchAndFamily = FIXED_PITCH | TMPF_TRUETYPE;
//		itsLF.lfFaceName[0] = 0;
	}

	ChangeFont();

	CString theString, theName, theCopyRight;
	if (theName.LoadString(AFX_IDS_APP_TITLE) && theCopyRight.LoadString(IDS_COPYRIGHT))
	{
		theString.Format(theCopyRight, (LPCTSTR)theName);
		SetWindowText(theString);
	}

	CEdit& theEdit = GetEditCtrl();
	int theLen = theEdit.GetWindowTextLength();
	theEdit.SetSel(theLen, theLen);
}

// ---------------------------------------------------------------------------
void COSReportView::OnFont()
{
	CFontDialog theDlg(&itsLF, CF_EFFECTS | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_FORCEFONTEXIST | CF_FIXEDPITCHONLY, 0, this);
	if (theDlg.DoModal() == IDOK)
	{
		theDlg.GetCurrentFont(&itsLF);
		ChangeFont();
	}
}

// ---------------------------------------------------------------------------
void COSReportView::OnEditClearAll() 
{
	SetWindowText(0);
}

//-----------------------------------------------------------------------
void COSReportView::OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase)
{
	itsFindString = lpszFind;
	itsfFindNext = bNext;
	itsfFindCase = bCase;

	CEditView::OnFindNext(lpszFind, bNext, bCase);
}

// ---------------------------------------------------------------------------
void COSReportView::OnEditFindNext() 
{
	if (!FindText(itsFindString, itsfFindNext, itsfFindCase))
		OnTextNotFound(itsFindString);
}

// ---------------------------------------------------------------------------
void COSReportView::OnEditFindPrev() 
{
	itsfFindNext = !itsfFindNext;
	if (!FindText(itsFindString, itsfFindNext, itsfFindCase))
		OnTextNotFound(itsFindString);
	itsfFindNext = !itsfFindNext;
}

// ---------------------------------------------------------------------------
void COSReportView::OnTextNotFound(LPCTSTR lpszFind)
{
	// If the text is not found wrap and try once more.
	CEdit& theEdit = GetEditCtrl();
	DWORD theSel = theEdit.GetSel();
	if (itsfFindNext)
		theEdit.SetSel(0, 0);
	else
	{
		int theLen = theEdit.GetWindowTextLength();
		theEdit.SetSel(theLen, theLen);
	}
	if (!FindText(itsFindString, itsfFindNext, itsfFindCase))
	{
		theEdit.SetSel(theSel);
		CEditView::OnTextNotFound(itsFindString);
	}
}

// ---------------------------------------------------------------------------
void COSReportView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CFrameWnd* theFrame = GetParentFrame();
	// make sure window is active
	theFrame->ActivateFrame();

	CMenu theMenu;
	if (theMenu.LoadMenu(IDM_CONTEXT_EDIT))
	{
		CMenu* thePopup = theMenu.GetSubMenu(0);
		ASSERT(thePopup != NULL);

		CPoint thePoint;
		if (!::GetCursorPos(&thePoint))
			thePoint = point;

//		if (theDefaultItem >= 0)
//			::SetMenuDefaultItem(thePopup->GetSafeHmenu(), theDefaultItem, TRUE);
		thePopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, thePoint.x, thePoint.y, theFrame);
	}
}

// ---------------------------------------------------------------------------
void COSReportView::OnLine() 
{
	CString theLine;
	if (!theLine.LoadString(IDS_LINE))
		theLine = "----------------------------------------------------------\r\n";

	DumpText(theLine);
}

// ---------------------------------------------------------------------------
void COSReportView::OnEditGoto() 
{
	CGotoDlg theDlg;
	if (theDlg.DoModal() == IDOK)
	{
		int theIndex = GetEditCtrl().LineIndex(theDlg.itsLineNumber);
		if (theIndex == -1)
		{
			CString theString, theString2;
			if (!theString.LoadString(IDS_ERROR_LINE_NUMBER))
				theString = "Line number %lu does not exist (less lines).";
			theString2.Format(theString, theDlg.itsLineNumber);
			AfxMessageBox(theString2, MB_ICONEXCLAMATION);
			return;
		}

		// This may be buggy if the Edit do not have at least 5 visible lines.
		int theTopLine = theDlg.itsLineNumber - 5;
		if (theTopLine <= 0)
			theTopLine = 0;
		int theTopIndex = GetEditCtrl().LineIndex(theTopLine);
		if (theTopIndex != -1)
			GetEditCtrl().SetSel(theTopIndex, theTopIndex);
		theTopLine = theDlg.itsLineNumber + 5;
		theTopIndex = GetEditCtrl().LineIndex(theTopLine);
		if (theTopIndex != -1)
			GetEditCtrl().SetSel(theTopIndex, theTopIndex);

		GetEditCtrl().SetSel(theIndex, theIndex);
	}
}
