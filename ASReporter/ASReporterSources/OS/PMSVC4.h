// ===========================================================================
//	PMSVC4.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMSVC4_h
#define PMSVC4_h

// ---------------------------------------------------------------------------
#ifndef PM_INTERNAL_INCLUDE_ALLOWED
	#error You cannot include 'PMSVC4.h'. Include 'PMRC.h' instead.
#endif

// ---------------------------------------------------------------------------
/** Converts a string to an uint32 */
pmuint32	pm_atouint32_imp(const char *aString);
/** Converts an uint32 to aString. aLen must specify the total length of aString buffer. */
char*		pm_uint32toa_imp(pmuint32 anUInt32, char* aString, size_t aLen);

/**	Returns a random number between aMin and aMax, both included. */
pmint32		pm_rand_imp(pmint32 aMin, pmint32 aMax);

// ---------------------------------------------------------------------------
#endif // PMSVC4_h
