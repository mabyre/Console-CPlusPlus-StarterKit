// ===========================================================================
//	PMAstExc.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMAstExc_h
#define	PMAstExc_h

// ---------------------------------------------------------------------------

#include "PMRC.h"

// ===========================================================================
//	PMAssertException
// ===========================================================================
/**
pm_throw_assert should throw an instance of this class.
This class does nothing on it own other than allowing ObjectSet assertions to 
be caught. BY DESIGN IT IS NOT REFERENCE-COUNTED.
*/

class PMAssertException
{
public:
	PMAssertException() {}
	virtual ~PMAssertException() {}
};

// ---------------------------------------------------------------------------
#endif // PMAstExc_h
