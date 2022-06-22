// ===========================================================================
//	PMDefs.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

// ---------------------------------------------------------------------------

#ifndef PMDefs_h
#define PMDefs_h

#ifndef PM_INTERNAL_INCLUDE_ALLOWED
	#error You cannot include 'PMDefs.h'. Include 'PMRC.h' instead.
#endif

// ---------------------------------------------------------------------------

#undef PMTARGET

#if defined(_MSC_VER) && _MSC_VER >= 1000
	#include "PMDefVC4.h"
	#define	PMTARGET	1
#endif

#ifndef PMTARGET
	// If you got this error and are using another compiler than VC++ 4.0 +
	// it is because these macros were checked only for this compiler.
	// You should write your own PMDefXXX.h using the information located in
	// PMStub.h and on top of this file and modify PMDefs.h to recognize your
	// compiler.
	#error No valid Definition file found for your system. Please contact our technical support.
#endif

// ---------------------------------------------------------------------------

#endif // PMDefs_h
