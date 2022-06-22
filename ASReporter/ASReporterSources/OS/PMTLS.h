// ===========================================================================
//	PMTLS.h
//	Copyright (C) 1996 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMTLS_h
#define PMTLS_h

//-----------------------------------------------------------------------
#include "PMRC.h"

//-----------------------------------------------------------------------
/** Internal use. */
//	This class should work like a struct. Its members are public.
class PMTLS
{
public:
	static pmbool	Init(void* = 0);
	static void		Close(void* = 0);

	static pmuint32	GetUniqueID();

//	static PMTLS*	Get();
//	PMTLS();

//	pmuint32	itsUniqueID;
};

//-----------------------------------------------------------------------
#endif // PMTLS_h

