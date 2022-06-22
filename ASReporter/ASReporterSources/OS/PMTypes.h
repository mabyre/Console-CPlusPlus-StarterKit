// ===========================================================================
//	PMTypes.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PM_INTERNAL_INCLUDE_ALLOWED
	#error YOU CANNOT INCLUDE PMTypes.h. Include PMRC.h instead.
#endif

// ---------------------------------------------------------------------------

#ifndef PMTypes_h
#define PMTypes_h

/*
IMPORTANT:
pm_refcount is the type that will handle reference counter.
It must be defined in PMDefs.h that is platform dependant because this type
is very platform dependent.
It MUST be a signed type.
*/

// ===========================================================================
//	Manages ANSI
// ===========================================================================

#ifdef NDEBUG
	/** 
		Define this macro to build a release version of ObjectSet.
		It is automatically defined if NDEBUG is defined that is ANSI compliant.
		The difference between a typical release version of ObjectSet and a typical
		debug version is that traces and memory tracking are removed.
	*/
	#define PMFinalRelease 1
#endif


// ===========================================================================
//	BASIC TYPE DEFINITIONS
// ===========================================================================
/**
There is a choice for basic type definitions:
PM_32BITS_TYPE	Types from typical 32-bit architecture.
We default to PM_32BITS_TYPE.
*/

#ifndef PM_32BITS_TYPE
	#define PM_32BITS_TYPE	1
#endif


	// -----------------------------------------------------------------------
	//	Most common 32-bit architecture
	// -----------------------------------------------------------------------

#ifdef PM_32BITS_TYPE
	#include <stdlib.h>	// For size_t

	typedef unsigned short	pmuint16;
	typedef signed short	pmint16;

	typedef unsigned long	pmuint32;
	typedef signed long		pmint32;

	typedef unsigned char	pmbyte;

	typedef unsigned int	pmbool;	// Should be a C++ bool, as soon as possible
	const pmbool pmtrue  = 1;
	const pmbool pmfalse = 0;
#endif

// ---------------------------------------------------------------------------

#endif // PMTypes_h
