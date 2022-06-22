// ===========================================================================
//	PMPushB.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================
//

#include "PMPushB.h"

// ---------------------------------------------------------------------------

#define PM_TRACEMODULE STREAM
PM_DEFINE_TRACE_CODE(Info, 1);
PM_DEFINE_TRACE_CODE(Debug, 2);

// ===========================================================================
//	PMPushBackBuffer
// ===========================================================================

static pmuint32 sPBID = 0;

// ---------------------------------------------------------------------------
//	Constructor

PMPushBack::PMPushBack()
{
	itsID = ++sPBID;
	itsArray = new PMPushBackArray();
}

// ---------------------------------------------------------------------------
//	Destructor

PMPushBack::~PMPushBack()
{
}

// ---------------------------------------------------------------------------

void PMPushBack::AddBuffer(const char *aBuffer, size_t aCount, pmbool afAtEnd)
{
	PM_ASSERT(aBuffer != 0, TL("PMPushBack::AddBuffer(): Cannot push back null pointer"));

	PMPushBackBufferRef	thePushBack = new PMPushBackBuffer(aBuffer, aCount);
	if (afAtEnd)
		itsArray->AppendObject(thePushBack);
	else
		itsArray->InsertObjectAtIndex(0, thePushBack);

	Dump();
}

// ---------------------------------------------------------------------------

void PMPushBack::Read(char *aBuffer, size_t aCount, size_t *aCopyCount)
{
	PM_ASSERT(HasPushBack(), TL("PMPushBack::Read(): No push back to retrieve from"));

	pmuint32	theCount = itsArray->GetCount() - 1;
	
	itsArray->GetObjectAtIndex(theCount)->Copy(aBuffer, aCount, aCopyCount);
	if (itsArray->GetObjectAtIndex(theCount)->IsEmpty())
		itsArray->RemoveObjectAtIndex(theCount);
		
	Dump();
}

// ---------------------------------------------------------------------------

pmbool PMPushBack::HasPushBack()
{
	return itsArray->GetCount() > 0;
}

// ---------------------------------------------------------------------------

void PMPushBack::Dump()
{
/*	PM_TRACE(Info, TL("[%ld] Dumping %t blocks", (long) itsID, itsArray->GetCount()));
	for (size_t i = 0; i < itsArray->GetCount() ; i++)
		itsArray->GetObjectAtIndex(i)->Dump();*/
}

// ===========================================================================
//	PMPushBackBuffer
// ===========================================================================

static pmuint32 sID = 0;
static pmuint32 sCount = 0;

// ---------------------------------------------------------------------------
//	Destructor

PMPushBackBuffer::PMPushBackBuffer(const char *aBuffer, size_t aCount) :
	itsLeft(0), itsOffset(0), itsData(0)
{
	itsID = ++sID;sCount++;
//	PM_TRACE(Info, TL("[%ld] Pushing back %t data", (long)itsID, aCount));
	
	itsOffset = 0;
	itsLeft = aCount;
	itsData = (char*)::pm_malloc(aCount);
	::pm_memcpy(itsData, aBuffer, aCount);
}

// ---------------------------------------------------------------------------
//	Destructor

PMPushBackBuffer::~PMPushBackBuffer()
{
//	PM_TRACE(Info, TL("[%ld] Deleting pushback, %ld remaining", (long)itsID,(long)--sCount));
	if (!IsEmpty())
		FreeBuffer();
}

// ---------------------------------------------------------------------------

pmbool PMPushBackBuffer::IsEmpty()
{	
	return itsData == 0;
}

// ---------------------------------------------------------------------------
// Copies aCount char from pushback into aBuffer. Empties this is necessary

void PMPushBackBuffer::Copy(char *aBuffer, size_t aCount, size_t *aCopyCount)
{	
	PM_ASSERT(!IsEmpty(), TL("PMPushBackBuffer::Copy(): Cannot copy empty buffer") );
	
	*aCopyCount = ::pm_min(itsLeft, aCount);
	::pm_memcpy(aBuffer, itsData+itsOffset, *aCopyCount );
	itsLeft -= *aCopyCount;
	if (!itsLeft)
		FreeBuffer();
	else
		itsOffset += *aCopyCount;

//	PM_TRACE(Info, TL("[%ld] Read %t data, remaining %t data", (long)itsID, *aCopyCount, itsLeft));
}

// ---------------------------------------------------------------------------
//	Destroy allocated pushback data

void PMPushBackBuffer::FreeBuffer()
{	
	PM_ASSERT(!IsEmpty(), TL("PMPushBackBuffer::FreeBuffer(): Cannot destroy an empty buffer") );

//	PM_TRACE(Info, TL("destroying pushback"));

	::pm_free(itsData);
	itsData = 0;
}

// ---------------------------------------------------------------------------
//	Destroy allocated pushback data

void PMPushBackBuffer::Dump()
{	
//	PM_TRACE(Info, B(itsData+itsOffset, itsLeft));
}
