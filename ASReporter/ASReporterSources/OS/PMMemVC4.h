// ===========================================================================
//	PMMemVC4.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PM_INTERNAL_INCLUDE_ALLOWED
	#error YOU CANNOT INCLUDE PMMemVC4.h.
#endif

#ifndef PMMemVC4_h
#define PMMemVC4_h
// ---------------------------------------------------------------------------
#include "PMWinInc.h"

#define _CRTBLD // Hack to allow to include an internal CRT header file.
	#include <dbgint.h> // For _CrtMemBlockHeader found in VCxx/CRT/SRC
#undef _CRTBLD

// ---------------------------------------------------------------------------
class PMVCMemoryTracker
{
public:
	PMVCMemoryTracker(HINSTANCE anInstance);
	virtual ~PMVCMemoryTracker();

	static BOOL	EnableTracking(BOOL afEnable);
	void		Report();

protected:
	BOOL		itsfInit;
	DWORD		itsLeaksCount, itsLeaksSize;
	long		itsFirstRequest;

	void		DumpMemory(_CrtMemState* theState);
	void		DumpBlock(_CrtMemBlockHeader* aBlock);
};

// ---------------------------------------------------------------------------
#endif /* PMMemVC4_h */
