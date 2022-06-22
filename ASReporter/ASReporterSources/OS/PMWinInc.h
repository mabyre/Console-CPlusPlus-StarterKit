// ===========================================================================
//	PMWinInc.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMWinInc_h
#define PMWinInc_h

//----------------------------------------------------------------------
#ifdef _INC_WINDOWS
	#ifndef STRICT
		#error STRICT must have been defined before included <windows.h>
	#endif
#else
	#ifndef STRICT
		#define STRICT 1
	#endif

	#ifndef WIN32_EXTRA_LEAN
		#define WIN32_EXTRA_LEAN 1
	#endif

	//	#define NOxxx
	#include <windows.h>
#endif

//---------------------------------------------------------------------------
#endif //PMWinInc_h

