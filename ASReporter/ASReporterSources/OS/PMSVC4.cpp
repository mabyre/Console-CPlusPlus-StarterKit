// ===========================================================================
//	PMSVC4.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMWinInc.h"
#include "PMTrace.h"
#include "PMAstExc.h"

//-----------------------------------------------------------------------
/*
	This file provides VC++ dependent stuff.
	The most important thing provided that is VC++ specific is the
	memory leaks mechanism using PMMemVC4.cpp
	You should use it in projects only if you are compiling with VC++
	version 4.1, 4.2 or 5.0. If you use another compiler, you should duplicate
	this file with another name and make changes required.
	Let's imagine that you want to compile with BC5.
	Duplicate this file and call it PMSBC5.cpp.
	Remove all stuff related to memory leaks.
	You should also provide and remove the different implementations
	for macros defined in your PMDefs.h.
	Create a new PMDefs.h and change macros mappings.
	BUT keep different function pointers otherwise your application will
	CRASH. A minimum Init file like this one should be:

// ===========================================================================
//	PMSGen.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMWinInc.h"
#include "PMRC.h"

//-----------------------------------------------------------------------
// PMOSWin32Helper::Init calls it. So a do-nothing must be provided.
// The real one is in PMMemVC4.cpp
#ifndef PMFinalRelease
	BOOL EnableMemoryTracking(BOOL afEnable)
	{
		return FALSE;
	}
#endif // PMFinalRelease

//-----------------------------------------------------------------------
//	Initializations
//-----------------------------------------------------------------------
static pmbool pm_gen_init(void* aParams)
{
	return pmtrue;
}

//-----------------------------------------------------------------------
static void pm_gen_close(void *)
{
}

//-----------------------------------------------------------------------
extern pmbool (*pm_init_hook)( void * ) = pm_gen_init;
extern void (*pm_close_hook)( void * ) = pm_gen_close;

//-----------------------------------------------------------------------
//	Memory allocations
//-----------------------------------------------------------------------
void* (*pm_malloc_hook)(size_t) = ::malloc;
void (*pm_free_hook)(void*) = ::free;
void* (*pm_realloc_hook)(void*, size_t) = ::realloc;

//-----------------------------------------------------------------------
//	Assertions
//-----------------------------------------------------------------------
// You should change this if your compiler do not support exceptions.
// This function must not allow to return as the next line after the code
// that called it may crash. If exceptions are not supported, you should
// call abort or exit.

#include "PMAstExc.h"
void pm_gen_throw_assert()
{	
	throw PMAssertException();
}

//-----------------------------------------------------------------------
void (*pm_throw_assert_hook)() = pm_gen_throw_assert;

//-----------------------------------------------------------------------
//	Default busy hook (does nothing)
//-----------------------------------------------------------------------
static void pm_gen_busy()
{
}

//-----------------------------------------------------------------------
void (*pm_busy_hook)() = pm_gen_busy;

*/

//-----------------------------------------------------------------------
#define PM_TRACEMODULE STRUCT
PM_DEFINE_TRACE_CODE(Info, 1); // -> PMTrace_kSTRUCTInfoCode

#ifndef PMFinalRelease
	extern void InitMemoryTracking(HINSTANCE anInstance);
	extern void CloseMemoryTracking();
#endif // PMFinalRelease

//-----------------------------------------------------------------------
//	Initializations
//-----------------------------------------------------------------------
static pmbool pm_vc4_init( void* aParams )
{
#ifndef PMFinalRelease
	::InitMemoryTracking((HINSTANCE)aParams); // Must be called after InitTraces that creates Strings
#endif

	return pmtrue;
}

//-----------------------------------------------------------------------
static void pm_vc4_close( void * )
{
#ifndef PMFinalRelease
	::CloseMemoryTracking();
#endif
}

//-----------------------------------------------------------------------
pmbool (*pm_init_hook)( void * ) = pm_vc4_init;
void (*pm_close_hook)( void * ) = pm_vc4_close;

//-----------------------------------------------------------------------
//	Memory allocations
//-----------------------------------------------------------------------
//void* (*::pm_malloc_hook)(size_t) = ::malloc; // Causes VC++ crash
void* (*pm_malloc_hook)(size_t) = ::malloc;
void (*pm_free_hook)(void*) = ::free;
void* (*pm_realloc_hook)(void*, size_t) = ::realloc;

//-----------------------------------------------------------------------
//	Assertions
//-----------------------------------------------------------------------
void pm_vc4_throw_assert()
{	
	throw PMAssertException();
}

//-----------------------------------------------------------------------
void (*pm_throw_assert_hook)() = pm_vc4_throw_assert;

//-----------------------------------------------------------------------
//	Default busy hook (does nothing)
//-----------------------------------------------------------------------
static void pm_vc4_busy()
{
}

//-----------------------------------------------------------------------
void (*pm_busy_hook)() = pm_vc4_busy;

//-----------------------------------------------------------------------
//	Stub specific implementations
//-----------------------------------------------------------------------
char* pm_uint32toa_imp(pmuint32 anUInt32, char* aString, size_t aLen)
{
	char theString[34]; // Read _ltoa doc
	::_ltoa(anUInt32, theString, 10);
	PM_ASSERT(::strlen(theString) + 1 < aLen, TL("pm_uint32toa_imp(): Buffer too small"));

	return ::strcpy(aString, theString);
}

//-----------------------------------------------------------------------
pmuint32 pm_atouint32_imp(const char* aString)
{
	// sscanf(aString, "%i", &theResult) should allow to convert from hex
	return (pmuint32) ::atol(aString);
}

//-----------------------------------------------------------------------
pmint32	pm_rand_imp(pmint32 aMin, pmint32 aMax)
{
	return ::rand() / (RAND_MAX / (aMax - aMin + 1) + 1) + aMin;
}
