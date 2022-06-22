// ===========================================================================
//	PMFileTr.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMFileTr.h"
#include "PMStr.h"

//-----------------------------------------------------------------------

#define PM_TRACEMODULE STRUCT
PM_DEFINE_TRACE_CODE(Trace, 1);

// ---------------------------------------------------------------------------

PMANSIFileTracer::PMANSIFileTracer(const char* aFilePath) :
	itsfMustIndentNext(pmtrue),
	itsfTryOpen(pmtrue)
{
	PM_ASSERT(aFilePath != 0 && ::pm_strlen(aFilePath) != 0, TL("PMANSIFileTracer::PMANSIFileTracer(const char*): Invalid name."));
	itsFile = 0;
	itsFileName = aFilePath,
	itsfClose = pmtrue;
}

// ---------------------------------------------------------------------------

PMANSIFileTracer::PMANSIFileTracer(FILE* aFile, pmbool afClose) :
	itsfMustIndentNext(pmtrue),
	itsfTryOpen(pmfalse)
{
	itsFile = aFile;
	itsfClose = afClose;
}

// ---------------------------------------------------------------------------

PMANSIFileTracer::~PMANSIFileTracer()
{
	if (itsFile != 0 && itsfClose)
	{
		::fclose(itsFile);
		itsFile = 0;
	}
}

// ---------------------------------------------------------------------------
pmbool PMANSIFileTracer::DoOpen()
{
	if (itsFile != 0)
		return pmtrue;
	if (itsfTryOpen == pmfalse)
		return pmfalse;
	// We don't want to try to open for every trace. If it fails once we never try again
	itsfTryOpen = pmfalse;
	itsFile = ::fopen(itsFileName->c_str(), "wb");

	return (itsFile != 0);
}

// ---------------------------------------------------------------------------

void PMANSIFileTracer::Indent(pmuint32 aLevel)
{
	const pmuint32	kTraceMaxIndent = 64;
	const pmuint32	kTraceIndentWidth = 2;
	
	static char sBuffer[kTraceIndentWidth * kTraceMaxIndent];

	if (DoOpen())
	{
		if (aLevel > kTraceMaxIndent)
			aLevel = kTraceMaxIndent;

		::pm_memset(sBuffer, ' ', aLevel * kTraceIndentWidth);
		::fwrite(sBuffer, kTraceIndentWidth, aLevel, itsFile);
		::fflush(itsFile);
	}
}

// ---------------------------------------------------------------------------

PMTraceSendParams* PMANSIFileTracer::B(const void *aBuffer, size_t aSize)
{
		// XX XX XX XX XX XX.... XX   xxxx...xxxx\n
	const size_t kBlockSize = 16;

	if (DoOpen())
	{
		const char*	theEOL = PMTracer::PlatformEOL();
		size_t		theEOLLen = ::pm_strlen(theEOL);
		size_t		theSize = 0, theBlockLen, i, j;
		pmbyte*		theBuffer = (pmbyte*)aBuffer;

		PMTraceSendParams* theParams = new PMTraceSendParams;
		theParams->itsKind = PMTrace_kKindBinary;
		
		theParams->itsData = ::pm_malloc((kBlockSize * 3 + 2 + kBlockSize + theEOLLen) * (aSize / kBlockSize + 1));
		char* theData = (char*)theParams->itsData;

		while (aSize > 0)
		{
			theBlockLen = ::pm_min(kBlockSize, aSize);
			for (i = 0; i < theBlockLen; i++)
			{
				::sprintf(theData, "%02X ", theBuffer[i]);
				theData += 3;
			}

			for (j = theBlockLen; j < kBlockSize; j++)
			{
				::sprintf(theData, "   ");
				theData += 3;
			}

			::sprintf(theData, "  ");
			theData += 2;

			for (i = 0; i < theBlockLen; i++)
				::sprintf(theData + i, "%c", (theBuffer[i] >= 32 && theBuffer[i] < 127)?theBuffer[i]:'.');
			theData += theBlockLen;

			::sprintf(theData, "%s", theEOL);
			theData += theEOLLen;

			aSize -= theBlockLen;
			theBuffer += theBlockLen;
		}
		
		// +1 is for the trailing 0
		theParams->itsDataLen = theData - (char*)theParams->itsData + 1;

		return theParams;
	}
	else
		return 0;
}

// ---------------------------------------------------------------------------

void PMANSIFileTracer::Send(PMTraceSendParams* aParams)
{
	if (aParams != 0 && DoOpen())
	{
		const char* theEOL = PMTracer::PlatformEOL();
		if (aParams->itsKind == PMTrace_kKindBinary)
			::fwrite(theEOL, 1, ::pm_strlen(theEOL), itsFile);
		else
			if (itsfMustIndentNext)
				Indent(aParams->itsIndentLevel);

		if (aParams->itsDataLen != 0)
			::fwrite(aParams->itsData, 1, aParams->itsDataLen - 1, itsFile);
		if (aParams->itsKind == PMTrace_kKindEOT)
			::fwrite(theEOL, 1, ::pm_strlen(theEOL), itsFile);
		::fflush(itsFile);

		itsfMustIndentNext = (aParams->itsKind != PMTrace_kKindNormal);
	}
	else
		itsfMustIndentNext = pmtrue;
}

// ---------------------------------------------------------------------------

void PMANSIFileTracer::DisplayAssert(const char* aTest, PMTraceSendParams* aParams)
{
	if (aParams != 0)
	{
		if (aParams->itsFile != 0 && aTest != 0 && aParams->itsData != 0)
			if (::pm_strlen(aTest) != 0)
				PM_TRACE(Trace, TL("ASSERTION: \"%s\" ([%s] failed) in file: %s, line: %lu", aParams->itsData, aTest, aParams->itsFile, (unsigned long)aParams->itsLine));
			else
				PM_TRACE(Trace, TL("IMMEDIATE ASSERTION: \"%s\" in file: %s, line: %lu", aParams->itsData, aParams->itsFile, (unsigned long)aParams->itsLine));
	}

	PMTracer::DisplayAssert(aTest, aParams);
}
