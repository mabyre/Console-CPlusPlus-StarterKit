// ===========================================================================
//	PMMemVC4.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================


#include "PMWinInc.h"
#include <winreg.h>

// This will include PMEnv.h that defines PMFinalRelease
#include "PMTrace.h"

#ifndef PMFinalRelease
//-----------------------------------------------------------------------
#define PM_INTERNAL_INCLUDE_ALLOWED
#include "PMMemVC4.h"
#undef PM_INTERNAL_INCLUDE_ALLOWED
#include "PMStr.h"

//-----------------------------------------------------------------------
/*
	This file provides VC++ dependent stuff.
	It provides enhanced memory leaks tracking supported by VC++.
	You should use it in projects only if you are compiling with VC++
	version 4.1, 4.2 or 5.0. 
	To use it from another file, as no header is provided, you should
	add the following lines before using it.
	Notice that those definitions are valid only if not in Release mode.

	#ifndef PMFinalRelease
		extern BOOL EnableMemoryTracking(BOOL afEnable);
		extern void InitMemoryTracking(HINSTANCE anInstance);
		extern void CloseMemoryTracking();
	#endif // PMFinalRelease
*/

//-----------------------------------------------------------------------
#define PM_TRACEMODULE STRUCT
PM_DEFINE_TRACE_CODE(Info, 1); // -> PMTrace_kSTRUCTInfoCode

// Must be the same as in PMW32Hlp.cpp
static const char sSettingsSection[] = "ReportSettings";

extern HKEY	PMWin32_OpenSectionKey(HINSTANCE anInstance, const char* aSection);

//-----------------------------------------------------------------------
BOOL EnableMemoryTracking(BOOL afEnable); // For external access
void InitMemoryTracking(HINSTANCE anInstance);
void CloseMemoryTracking();


static PMVCMemoryTracker* cMemoryTracker;

//-----------------------------------------------------------------------
BOOL EnableMemoryTracking(BOOL afEnable)
{
	return PMVCMemoryTracker::EnableTracking(afEnable);
}

void InitMemoryTracking(HINSTANCE anInstance)
{
//		PMVCMemoryTracker::EnableTracking(FALSE);
	cMemoryTracker = new PMVCMemoryTracker(anInstance);
}

void CloseMemoryTracking()
{
	delete cMemoryTracker;
}

//-----------------------------------------------------------------------------------

#ifdef _CPPRTTI
	#include <typeinfo.h>
#endif

//-----------------------------------------------------------------------------------
PMVCMemoryTracker::PMVCMemoryTracker(HINSTANCE anInstance)
{
	static const char sMemoryKey[] = "CheckMemory";

	itsfInit = FALSE;
	itsLeaksSize = itsLeaksCount = 0;

	HKEY theDebugKey = ::PMWin32_OpenSectionKey(anInstance, sSettingsSection);
	if (theDebugKey != 0)
	{
		DWORD theType, theValue, theSize = sizeof(DWORD);
		if (::RegQueryValueEx(theDebugKey, sMemoryKey, NULL, &theType, (LPBYTE)&theValue, &theSize) != ERROR_SUCCESS
			|| theType != REG_DWORD)
		{
			// Memory leaks are disabled by default.
			theValue = 0;
			::RegSetValueEx(theDebugKey, sMemoryKey, 0, REG_DWORD, (LPBYTE)&theValue, sizeof(DWORD));
		}
		itsfInit = theValue;

		::RegCloseKey(theDebugKey);
	}

	if (itsfInit)
	{
		_CrtMemState theState;
		::_CrtMemCheckpoint(&theState);
		if (theState.pBlockHeader != 0)
			itsFirstRequest = theState.pBlockHeader->lRequest;
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);
	}
	else
		_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
}

//-----------------------------------------------------------------------------------
PMVCMemoryTracker::~PMVCMemoryTracker()
{
	Report();

	itsfInit = FALSE;
}

//-----------------------------------------------------------------------------------
BOOL PMVCMemoryTracker::EnableTracking(BOOL afEnable)
{
	int theFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	BOOL theOldFlag = ((theFlag & _CRTDBG_ALLOC_MEM_DF) != 0);

	if (afEnable)
		theFlag |= _CRTDBG_ALLOC_MEM_DF;
	else
		theFlag &= ~_CRTDBG_ALLOC_MEM_DF;
	
	_CrtSetDbgFlag(theFlag);

	return theOldFlag;
}

//-----------------------------------------------------------------------------------
void PMVCMemoryTracker::DumpBlock(_CrtMemBlockHeader* aBlock)
{
	size_t	theSize = aBlock->nDataSize;
	void*	theData = pbData(aBlock);

	if (_BLOCK_TYPE(aBlock->nBlockUse) == _IGNORE_BLOCK ||
        _BLOCK_TYPE(aBlock->nBlockUse) == _FREE_BLOCK ||
        _BLOCK_TYPE(aBlock->nBlockUse) == _CRT_BLOCK ||
		aBlock->lRequest == 0L) // IGNORE_REQ
		return;

	itsLeaksCount++;
	itsLeaksSize += theSize;

	PM_TRACE(Info, T("%lu - ", aBlock->lRequest));

#ifdef _CPPRTTI
	try
	{
		BOOL thefValid = ::_CrtIsValidPointer(theData, sizeof(void*), FALSE);
		if (thefValid)
		{
			void** theVTBLPtr = (void**)theData;
			thefValid = ::_CrtIsValidPointer(*theVTBLPtr, sizeof(void*), FALSE);
		}
		if (thefValid)
		{
			const type_info& theTypeInfo = typeid(*((PMVCMemoryTracker*)theData));
			PM_TRACE(Info, T("%s", theTypeInfo.name()));
			PMRC* thePMRC = dynamic_cast<PMRC*>((PMRC*)theData);
			if (thePMRC != 0)
				PM_TRACE(Info, T(" (RC=%lu)", thePMRC->_GetRefCount()));
			PM_TRACE(Info, T(", "));
		}
	}
	catch (...)
	{
	}
#endif // _CPPRTTI

#define DUMP_MAX_LEN	48
	PM_TRACE(Info, TL("Mem: 0x%lx, %lu bytes:", theData, theSize));
	if (aBlock->szFileName != 0)
		if (::_CrtIsValidPointer(aBlock->szFileName, 1, FALSE))
			PM_TRACE(Info, T(" (%s:%lu)", aBlock->szFileName, aBlock->nLine));
	PM_TRACE(Info, B(theData, theSize<DUMP_MAX_LEN?theSize:DUMP_MAX_LEN));
	PM_TRACE(Info, TL(""));
#undef DUMP_MAX_LEN
}

//-----------------------------------------------------------------------------------
void PMVCMemoryTracker::DumpMemory(_CrtMemState* theState)
{
	_RPT0(_CRT_WARN,"++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	_RPT0(_CRT_WARN,"Please ignore next Access Violation and C++ exceptions\n");

	_CrtMemBlockHeader* theBlock = theState->pBlockHeader;

	itsLeaksCount = itsLeaksSize = 0;

	while (theBlock != 0 && theBlock->lRequest > itsFirstRequest)
	{
		if (theBlock->nBlockUse == _NORMAL_BLOCK || theBlock->nBlockUse == _CLIENT_BLOCK)
			DumpBlock(theBlock);
		theBlock = theBlock->pBlockHeaderNext;
	}

	PM_TRACE(Info, TL(""));
	switch (itsLeaksCount)
	{
		case 0:
			PM_TRACE(Info, TL(" No memory leaks. Well done."));
			break;

		case 1:
			PM_TRACE(Info, TL(" There is 1 memory leak of %lu bytes.", itsLeaksSize));
			break;
		
		default:
			PM_TRACE(Info, TL(" There are %lu memory leaks, totaling %lu bytes.", itsLeaksCount, itsLeaksSize));
			break;
	}
	_RPT0(_CRT_WARN,"++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

//-----------------------------------------------------------------------------------
void PMVCMemoryTracker::Report()
{
	if (!itsfInit)
		return;

	// Obtain how many blocks were allocated
	void* thePtr = ::malloc(1);
	long theRequest = (((_CrtMemBlockHeader*)thePtr)-1)->lRequest -1;
	::free(thePtr);

	_CrtMemState theState;
	::_CrtMemCheckpoint(&theState);
	// Some of the traces emitted during memory tracking may allocate memory.
	// We should disable the memory tracking for the time we do this
	BOOL thefOld = EnableTracking(FALSE);

	PM_TRACE(Info, TL(""));
	PM_TRACE(Info, TL("------------ VC4 Stub memory report: ------------"));
	PM_TRACE(Info, TL("%ld blocks totally allocated. Max size: %ld bytes.", theRequest, theState.lHighWaterCount));
	PM_TRACE(Info, TL(""));

	DumpMemory(&theState);

	PM_TRACE(Info, TL("-------------------------------------------------"));

	BOOL EnableTracking(thefOld);
}

//-----------------------------------------------------------------------------------
#endif // PMFinalRelease
