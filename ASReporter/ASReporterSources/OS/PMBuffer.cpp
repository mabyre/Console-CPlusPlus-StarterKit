// ===========================================================================
//	PMBuffer.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMBuffer.h"

// ===========================================================================
//	PMBuffer
// ===========================================================================

PMBuffer::PMBuffer(size_t aSize) : 
	PMRC(),
	itsPtr(0),
	itsSize(0)
{ 
	itsPtr = ::pm_malloc(aSize);
	itsSize = aSize;
}

// ---------------------------------------------------------------------------

PMBuffer::~PMBuffer()
{ 
	if (itsPtr != 0)
		::pm_free(itsPtr); 
}

// ---------------------------------------------------------------------------

void PMBuffer::Realloc(size_t aNewSize)
{ 
	if (itsPtr == 0)
		itsPtr = ::pm_malloc(aNewSize);
	else
		itsPtr = ::pm_realloc(itsPtr, aNewSize);

	itsSize = aNewSize;
}

// ---------------------------------------------------------------------------

void* PMBuffer::TakeOwnership()
{ 
	void*	thePtr = itsPtr;
	itsPtr = 0;
	itsSize = 0;
	return thePtr;
}
