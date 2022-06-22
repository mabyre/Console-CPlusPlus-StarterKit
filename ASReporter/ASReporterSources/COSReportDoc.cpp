// ===========================================================================
//	COSReportDoc.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#include "stdafx.h"
#include "OSReport.h"

#include "COSReportDoc.h"
#include "COSReportView.h"
#include "CFiltersDlg.h"

#include "PMBuffer.h"
#include "PMWTCPTr.h"

// ---------------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

#define PM_TRACEMODULE USER1
PM_DEFINE_TRACE_CODE(Debug, 1);

/* #define MAX_CURRENT_STRING_SIZE			64000 */
#define FORMAT_LOOP_DELAY				500
/* #define MAX_TRACE_LINES_BEFORE_DISPLAY	200 */

// ---------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(COSReportDoc, LDocument)

BEGIN_MESSAGE_MAP(COSReportDoc, LDocument)
	//{{AFX_MSG_MAP(COSReportDoc)
	ON_COMMAND(ID_FILTERS, OnFilters)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------
COSReportDoc::COSReportDoc() : 
	LDocument(),
	itsSocket(0),
/*
	PMCRITICALSECTION_MEMBER(COSReportDoc, itsBuffersCS),
	PMCRITICALSECTION_MEMBER(COSReportDoc, itsSocketCS),
*/
	itsBuffersCS(),
	itsSocketCS(),
	itsTracer(0)
{
	itsReadThread = 0;
	itsFormatThread = 0;
	itsBuffers = new PMArray<PMBufferRef>;
	itsfStartOfLine = pmtrue;
	itsfStop = pmfalse;
	itsFirstTS = 0;
	// VC++ Bug ??? The next line does not work but the 2 next do.
//	itsTracer = new PMWin32TCPTracer(0, 0);
	PMStrRef theStr;
	itsTracer = new PMWin32TCPTracer(theStr, 0);
	// Get the app main config
	itsConfig = new CTraceConfig(*COSReportApp::Get()->GetMainConfig().Get());
}

// ---------------------------------------------------------------------------
COSReportDoc::~COSReportDoc()
{
}

// ---------------------------------------------------------------------------
BOOL COSReportDoc::OnNewDocument()
{
	if (!LDocument::OnNewDocument())
		return FALSE;

	itsSocket = (PMSocket*)GetCreateParams();
	CString theTitle;
	theTitle.LoadString(IDS_DEFAULT_DOC_TITLE);
	SetTitle(theTitle);

	itsFormatStopEvent = ::CreateEvent(0, TRUE, FALSE, 0);
	if (itsFormatStopEvent == 0)
		return FALSE;

	CWinThread* theThread = ::AfxBeginThread(COSReportApp::OSRFormatThreadProc, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (theThread == 0)
	{
		PM_TRACE(Debug, TL("COSReportDoc::OnNewDocument(): Unable to start the format thread"));
		::MessageBeep(MB_ICONSTOP);
		::AfxMessageBox(IDS_ERROR_FORMAT, MB_OK | MB_ICONSTOP);
		GetReportView()->GetParentFrame()->SendMessage(WM_CLOSE);
	}
	else
	{
		PM_TRACE(Debug, TL("COSReportDoc::OnNewDocument(): Format thread (%lu) started successfully.", theThread->m_nThreadID));
		theThread->m_bAutoDelete = FALSE;
		theThread->ResumeThread();
		itsFormatThread = theThread;
	}
	if (!m_strPathName.LoadString(IDS_DEFAULT_DOC_NAME))
		m_strPathName = "Traces.txt";

	return TRUE;
}

// ---------------------------------------------------------------------------
void COSReportDoc::OnCloseDocument() 
{
	{
		PMCriticalSectionLocker theLocker(&itsSocketCS);
		if (!itsSocket.IsNull())
			itsSocket->ImmediateClose();
	}
	itsfStop = pmtrue;
	if (itsReadThread != 0)
	{
		itsReadThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
		if (::WaitForSingleObject(itsReadThread->m_hThread, 3000) != WAIT_OBJECT_0)
			PM_TRACE(Debug, TL("Unable to stop a reader thread."));
		else
		{
			PM_TRACE(Debug, TL("Reader thread (%lu) stopped successfully.", itsReadThread->m_nThreadID));
			delete itsReadThread;
			itsReadThread = 0;
		}
	}
	if (itsFormatThread != 0)
	{
		::SetEvent(itsFormatStopEvent);
		itsFormatThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
		if (::WaitForSingleObject(itsFormatThread->m_hThread, 3000) != WAIT_OBJECT_0)
			PM_TRACE(Debug, TL("Unable to stop a format thread."));
		else
		{
			PM_TRACE(Debug, TL("Format thread (%lu) stopped successfully.", itsFormatThread->m_nThreadID));
			delete itsFormatThread;
			itsFormatThread = 0;
		}
	}
	::CloseHandle(itsFormatStopEvent);
	
	LDocument::OnCloseDocument();
}

// ---------------------------------------------------------------------------
void COSReportDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

// ---------------------------------------------------------------------------
COSReportView* COSReportDoc::GetReportView()
{
	POSITION thePos = GetFirstViewPosition();
	ASSERT(thePos != 0);
	CView* theView = GetNextView(thePos);
	ASSERT(theView != 0);
	ASSERT_KINDOF(COSReportView, theView);

	return (COSReportView*)theView;
}

// ---------------------------------------------------------------------------
void COSReportDoc::SetTitleEx(LPCSTR aString)
{
	CString* theTitle = new CString(aString);
	GetReportView()->PostMessage(WM_SET_TITLE, 0, (LPARAM)theTitle);
}

// ---------------------------------------------------------------------------
void COSReportDoc::DoDisplay(COSReportView* aView)
{
	if (itsCurrentString.GetLength() != 0)
	{
		aView->PostMessage(WM_NEW_TRACE, 0, (LPARAM)new CString(itsCurrentString));
		itsCurrentString.Empty();
	}
}

// -----------------------------------------------------------------------
void COSReportDoc::OnFilters() 
{
	CFiltersDlg	theDlg(itsConfig, pmtrue, GetReportView());
	theDlg.DoModal();
}

// ---------------------------------------------------------------------------
void COSReportDoc::DoReadLoop()
{
	PMDataRef		theData = new PMData(2048, 2048, 2, 100);
	size_t			theTraceSize;
	PMBufferRef		theTrace;
	const pmbyte*	thePtr;
	COSReportView*	theView = GetReportView();

	while (1)
	{
		if (itsfStop)
			break;
		theTraceSize = 0;
		do
		{
			if (theData->size() >= 6)
			{
				thePtr = (const pmbyte*)theData->buffer();
				thePtr += 2; // Skip version number
				theTraceSize = ::pm_get_networku32(&thePtr);
			}
			else
				// ReadAppend in theData, sizeMax = 0 means what you can
				if (itsSocket->Read(theData, 0).IsNull())
					break;
		} while (theTraceSize == 0);

		if (theTraceSize == 0)
			// An error occured while reading
			break;

		while (theData->size() < theTraceSize)
			if (itsSocket->Read(theData, 0).IsNull())
				break;

		if (theData->size() < theTraceSize)
			// An error occured
			break;

		theTrace = theData->ExtractBuffer(0, theTraceSize);
		theData->TruncateBegin(theTraceSize);

		{
			PMCriticalSectionLocker theLocker(&itsBuffersCS);
			itsBuffers->AppendObject(theTrace);
		}
	}
	{
		PMCriticalSectionLocker theLocker(&itsSocketCS);
		itsSocket = 0;
	}
	if (!itsfStop)
	{
		CString theTitle(GetTitle());
		CString	theString;
		if (!theString.LoadString(IDS_CLOSED))
			theString = " --- Closed ---";
		theTitle += theString;
		SetTitleEx(theTitle);
	}
}

// ---------------------------------------------------------------------------
void COSReportDoc::DoFormatLoop()
{
	COSReportView* theView = GetReportView();
	while (!itsfStop)
	{
		PMBufferArrayRef	theArray = 0;
		DWORD				thePendingCount = 0;
		{
			PMCriticalSectionLocker theLocker(&itsBuffersCS);

			// Get itsBuffers for managing and allocate a new empty one
			theArray = itsBuffers;
			itsBuffers = new PMArray<PMBufferRef>;
			// Release the critical section as soon as possible
		}
		thePendingCount = theArray->GetCount();
		if (thePendingCount != 0)
		{
#ifdef _DEBUG
			if (thePendingCount != 1)
				PM_TRACE(Debug, TL("COSReportDoc::DoNewTrace(): Pending count: %lu", thePendingCount));
#endif

			for (size_t i = 0; i < thePendingCount && !itsfStop; i++)
			{
				DumpTraceBlock(theView, theArray->At(i));
/*
				if (((i+1) % MAX_TRACE_LINES_BEFORE_DISPLAY) == 0)
					DoDisplay(theView);
*/
			}
		}

		// Free memory. Not needed in fact as theArray is automatic
		// theArray = 0;
		if (!itsfStop)
		{
			DoDisplay(theView);
			// Just wait the timeout or until the Stop event is signaled.
			if (::WaitForSingleObject(itsFormatStopEvent, FORMAT_LOOP_DELAY) == WAIT_OBJECT_0)
				PM_TRACE(Debug, TL("COSReportDoc::DoFormatLoop(): Thread aborted during wait."));
		}

		// The reader thread is finished (connection closed) and nothing more to display.
		if (itsSocket.IsNull() && itsBuffers->GetCount() == 0)
			break;
	}
}

// ---------------------------------------------------------------------------
BOOL COSReportDoc::SaveModified() 
{
	// Prevent from asking to save...
	SetModifiedFlag(FALSE);
	
	return LDocument::SaveModified();
}

// -----------------------------------------------------------------------
#define MAX_INDENT_LEVEL	32
// Must be MAX_INDENT_LEVEL*2 space char + A dummy char never displayed.
static char sIndentString[] = "                                                                +";
static const char sHexDigits[] = "0123456789ABCDEF";
static char* sCtrlChars[] = 
{
	"\\x00", "\\x01", "\\x02", "\\x03", "\\x04", "\\x05", "\\x06", "\\x07", "\\b", "\\t", "\\n", "\\x0b", "\\x0c", "\\r", "\\x0e", "\\x0f", 
	"\\x10", "\\x11", "\\x12", "\\x13", "\\x14", "\\x15", "\\x16", "\\x17", "\\x18", "\\x19", "\\x1a", "\\x1b", "\\x1c", "\\x1d", "\\x1e", "\\x1f"
};

void COSReportDoc::DumpTraceBlock(COSReportView* aView, PMBufferRef aTrace)
{
	// ALWAYS set theParam.itsParam.itsData = 0; before returning else it will be deleted
	// while not allocated (a ptr in the trace block)
	PMWin32TCPTracerInfo	theParam;
	CString					theString;
	BOOL					thefSpecials, thefBAddr;

	if (!itsTracer->DecodeBlock(aTrace, &theParam.itsParam, &theParam))
	{
		PM_TRACE(Debug, TL("COSReportDoc::DumpTraceBlock(): *** Bad Block -> ignored ***"));
		theString = "Bad block (version) received.\r\n";
		goto DumpTraceBlock_end;
	}
	/* ALWAYS Check for app name even if this code is disabled. */
	if (!theParam.itsAppName.IsNull() && theParam.itsAppName->size() != 0)
	{
		CString theFormat, theTitle;
		if (theParam.itsProcessID != 0)
		{
			if (theFormat.LoadString(IDS_DOC_TITLE))
				theTitle.FormatMessage(theFormat, theParam.itsAppName->c_str(), theParam.itsProcessID);
		}
		else
		{
			if (theFormat.LoadString(IDS_DOC_TITLE_NO_PID))
				theTitle.FormatMessage(theFormat, theParam.itsAppName->c_str());
		}
		if (!theTitle.IsEmpty())
			SetTitleEx(theTitle);
	}
	if (!itsConfig->IsEntryEnabledForCode(theParam.itsParam.itsIdent))
	{
		theParam.itsParam.itsData = 0;
		return;
	}
	thefSpecials = itsConfig->GetDisplaySpecials();

	if (theParam.itsParam.itsKind == PMTrace_kKindBinary)
	{
		// XX XX XX XX XX XX.... XX   xxxx...xxxx\n
		const size_t	kBlockSize = 16;

		size_t			theSize = theParam.itsParam.itsDataLen, theBlockLen, i, thePos = 0;
		unsigned char*	theBuffer = (unsigned char*)theParam.itsParam.itsData;

		thefBAddr = itsConfig->GetDisplayBAddr();

		// 6 + 16*3 + 2 + 16 + 2 + 1
		char			theData[75];
		char*			thePtr;
		theData[4] = ':';
		theData[5] = ' ';
		theData[54] = ' ';
		theData[55] = ' ';
		theData[72] = '\r';
		theData[73] = '\n';
		theData[74] = 0;

		// Always go to a new line before buffer
		if (!itsfStartOfLine)
			theString += "\r\n";

		while (theSize > 0)
		{
			// Indent it as needed
			if (theParam.itsParam.itsIndentLevel > 0)
			{
				pmuint32 theLevel;
				if (theParam.itsParam.itsIndentLevel > MAX_INDENT_LEVEL)
					theLevel = MAX_INDENT_LEVEL;
				else
					theLevel = theParam.itsParam.itsIndentLevel;
				sIndentString[theLevel * 2] = 0;
				theString += sIndentString;
				sIndentString[theLevel * 2] = ' ';
			}

			if (thefBAddr)
			{
				thePtr = &theData[0];
				*thePtr++ = sHexDigits[thePos >> 12];
				*thePtr++ = sHexDigits[(thePos >> 8) & 15];
				*thePtr++ = sHexDigits[(thePos >> 4) & 15];
				*thePtr++ = sHexDigits[thePos & 15];
				thePtr += 2;
			}
			else
				thePtr = &theData[6];

			theBlockLen = ::pm_min(kBlockSize, theSize);
			for (i = 0; i < theBlockLen; i++)
			{
				*thePtr++ = sHexDigits[theBuffer[i] >> 4];
				*thePtr++ = sHexDigits[theBuffer[i] & 15];
				*thePtr++ = ' ';
			}

			for (i = theBlockLen; i < kBlockSize; i++)
			{
				thePtr[0] = ' ';
				thePtr[1] = ' ';
				thePtr[2] = ' ';
				thePtr += 3;
			}
			thePtr += 2;

			for (i = 0; i < theBlockLen; i++)
				if (theBuffer[i] >= 32 && (theBuffer[i] < 127 || thefSpecials))
					*thePtr++ = theBuffer[i];
				else
					*thePtr++ = '.';
				/* *thePtr++ = (char)((theBuffer[i] >= 32 && theBuffer[i] < 127)?theBuffer[i]:'.'); */
			
			if (theBlockLen < kBlockSize)
				::pm_memset(thePtr, ' ', kBlockSize - theBlockLen);

			if (thefBAddr)
				theString += theData;
			else
				theString += (theData + 6);

			theSize -= theBlockLen;
			theBuffer += theBlockLen;
			thePos += theBlockLen;
		}

		// Always at start of a new line after a buffer
		itsfStartOfLine = pmtrue;
	}
	else
	{
		// ASCII
		if (itsfStartOfLine)
		{
			char theStr[16];
			// In PMXTrace.cpp we ensured that the timestamp is not 0.
			// Receiving a 0 timestamp is possible if the version used to send traces is older...
			if (itsConfig->GetDisplayTS() && theParam.itsParam.itsTimeStamp != 0)
			{
				if (itsFirstTS == 0)
					itsFirstTS = theParam.itsParam.itsTimeStamp;
				::sprintf(theStr, "%08lu%c", theParam.itsParam.itsTimeStamp - itsFirstTS, itsConfig->GetSeparator());
				theString += theStr;
			}

			pmuint16 theDisplayTID = itsConfig->GetDisplayTID();
			if (theDisplayTID != 0 && theParam.itsThreadID != 0)
			{
				if (theDisplayTID == 4)
					::sprintf(theStr, "%04x%c", theParam.itsThreadID, itsConfig->GetSeparator());
				else
					::sprintf(theStr, "%08x%c", theParam.itsThreadID, itsConfig->GetSeparator());
				theString += theStr;
			}
			if (itsConfig->GetDisplayName())
			{
				PMStrRef theName = itsConfig->GetEntryShortNameForCode(theParam.itsParam.itsIdent);
				if (theName.IsNull())
					::sprintf(theStr, "0x%04x%c", (pmuint16)(theParam.itsParam.itsIdent >> 16), itsConfig->GetSeparator());
				else
					::sprintf(theStr, "%s%c", theName->c_str(), itsConfig->GetSeparator());

				theString += theStr;
			}
			
			if (theParam.itsParam.itsIndentLevel > 0)
			{
				pmuint32 theLevel;
				if (theParam.itsParam.itsIndentLevel > MAX_INDENT_LEVEL)
					theLevel = MAX_INDENT_LEVEL;
				else
					theLevel = theParam.itsParam.itsIndentLevel;
				sIndentString[theLevel * 2] = 0;
				theString += sIndentString;
				sIndentString[theLevel * 2] = ' ';
			}
		}

		/* Escaping chars < 32 and > 126 (if necessary). */
		BYTE	theChar;
		BYTE	*thePtr, *thePtr2;
		thePtr = (BYTE*)theParam.itsParam.itsData;
		while (1)
		{
			thePtr2 = thePtr;
			while (*thePtr2 >= ' ' && (thefSpecials || *thePtr2 < 127))
				thePtr2++;
			if (*thePtr2 == 0)
				break;
			theChar = *thePtr2;
			*thePtr2 = 0;
			theString += thePtr;
			if (theChar < ' ')
				theString += sCtrlChars[(size_t)theChar];
			else
				theString += '.';
			*thePtr2 = theChar;
			thePtr = thePtr2 + 1;
		}
		if (*thePtr != 0)
			theString += thePtr;

		/* theString += (char*)theParam.itsParam.itsData; */
		itsfStartOfLine = (theParam.itsParam.itsKind == PMTrace_kKindEOT);
		if (itsfStartOfLine)
			theString += "\r\n";
	}

DumpTraceBlock_end:
	theParam.itsParam.itsData = 0;

	if (itsfStop)
		return;

	itsCurrentString += theString;
	/* if (itsCurrentString.GetLength() > MAX_CURRENT_STRING_SIZE) */
	if (itsCurrentString.GetLength() > COSReportApp::Get()->GetViewBufferSize() / 2)
		DoDisplay(aView);
}
