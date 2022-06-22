// ===========================================================================
//	PMWFilTr.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMWFilTr.h"
#include "PMW32Hlp.h"

// ---------------------------------------------------------------------------

PMWin32FileTracer::PMWin32FileTracer(const char* aFilePath) :
	PMANSIFileTracer(aFilePath)
{
}

// ---------------------------------------------------------------------------

PMWin32FileTracer::PMWin32FileTracer(FILE* aFile, pmbool afClose) :
	PMANSIFileTracer(aFile, afClose)
{
}

// ---------------------------------------------------------------------------

PMWin32FileTracer::~PMWin32FileTracer()
{
}

// ---------------------------------------------------------------------------

void PMWin32FileTracer::DisplayAssert(const char* aTest, PMTraceSendParams* aParams)
{
	PMOSWin32Helper::GetCurrent()->DisplayAssert(aTest, aParams);

	// Skip PMANSIFileTracer::DisplayAssert
	PMTracer::DisplayAssert(aTest, aParams);
}
