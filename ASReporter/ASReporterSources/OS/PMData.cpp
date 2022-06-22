// ===========================================================================
//	PMData.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMData.h"

#include "PMTrace.h"
#include "PMBuffer.h"

// ---------------------------------------------------------------------------

#define PM_TRACEMODULE DATA

PM_DEFINE_TRACE_CODE(Info, 1);
PM_DEFINE_TRACE_CODE(Check, 2);

// ===========================================================================
//	PMData
// ===========================================================================

// ---------------------------------------------------------------------------
//	Set default traits

void PMData::Defaults()
{	
	itsMinCapacity = 1000;
	itsMinOverhead = 10;
	itsOverheadFactor = 2;
	itsOverheadRepartition = 0;
}

// ---------------------------------------------------------------------------

PMData::PMData()
{	
	Defaults();
		//	Note that subclass implementation should not be called
		//	because we are at construction
	DoInit(0, 0, OverheadFor(0));
}

// ---------------------------------------------------------------------------

PMData::PMData(size_t aCapacity)
{	
	Defaults();
		//	Note that subclass implementation whould not be called
		//	cause we are at construction
	DoInit(0, 0, aCapacity + OverheadFor(aCapacity));
}

// ---------------------------------------------------------------------------

PMData::PMData(const void *aData, size_t aSize)
{	
	Defaults();
		//	Note that subclass implementation whould not be called
		//	cause we are at construction
	DoInit(aData, aSize, OverheadFor(aSize) + aSize);
}

// ---------------------------------------------------------------------------

PMData::PMData(const void *aData, size_t aSize, size_t aCapacity)
{	
	Defaults();
		//	Note that subclass implementation whould not be called
		//	cause we are at construction
	DoInit(aData, aSize, aCapacity);
}

// ---------------------------------------------------------------------------

PMData::PMData(const char *aCString)
{	
	Defaults();
	
	size_t theSize = ::pm_strlen(aCString);
		//	Note that subclass implementation whould not be called
		//	cause we are at construction
		// #### DoInit(aCString, theSize, theSize + OverheadFor(theSize)) ???
	DoInit(aCString, theSize, theSize);
}

// ---------------------------------------------------------------------------

PMData::PMData(	size_t aMinCapacity, size_t aMinOverhead, pmuint16 anOverheadFactor, pmint16 anOverheadRepartition)
{	
	itsMinCapacity = aMinCapacity;
	itsMinOverhead = aMinOverhead;
	itsOverheadFactor = anOverheadFactor;
	itsOverheadRepartition = anOverheadRepartition;

//	DoInit(0, itsMinCapacity, OverheadFor(itsMinCapacity));
	DoInit(0, 0, itsMinCapacity + OverheadFor(itsMinCapacity));
}

// ---------------------------------------------------------------------------

PMData::~PMData()
{	
	if (itsBuffer)		//	### Check if allowed by API
		::pm_free(itsBuffer);
}

// ---------------------------------------------------------------------------

//void *PMData::GetBuffer()
void* PMData::buffer()
{	
	return itsBuffer + itsStart;
}

// ---------------------------------------------------------------------------
// #### obsolete ?

/*size_t PMData::GetSize()
{	
	return itsSize;
}*/

// ---------------------------------------------------------------------------

size_t PMData::size()
{	
	return itsSize;
}

// ---------------------------------------------------------------------------

void PMData::SetSize(size_t aSize)
{	
	if (aSize>itsSize)
		Append(0, aSize-itsSize);
	else
		Delete(aSize, itsSize-aSize);
}

// ---------------------------------------------------------------------------

void PMData::Append(const void *aSourceBuffer, size_t aSourceLen)
{	
	DoReplace(itsSize, 0, aSourceBuffer, aSourceLen);
}

// ---------------------------------------------------------------------------

void PMData::Append(const PMDataRef &aData)
{	
	Append(aData->buffer(), aData->size());
}

// ---------------------------------------------------------------------------

void PMData::Insert(size_t aDestOffset, const void *aSourceBuffer, size_t aSourceLen)
{	
	DoReplace(aDestOffset, 0, aSourceBuffer, aSourceLen);
}

// ---------------------------------------------------------------------------

void PMData::Insert(size_t aDestOffset, const PMDataRef &aSourceData)
{	
	Insert(aDestOffset, aSourceData->buffer(), aSourceData->size());
}

// ---------------------------------------------------------------------------

void PMData::Delete(size_t aDestOffset, size_t aDestLen)
{	
	DoReplace(aDestOffset, aDestLen, 0, 0);
}

// ---------------------------------------------------------------------------

void PMData::Replace(size_t aDestOffset, size_t aDestLen, const void *aSourceBuffer, size_t aSourceLen)
{	
	DoReplace(aDestOffset, aDestLen, aSourceBuffer, aSourceLen);
}

// ---------------------------------------------------------------------------

void PMData::Replace(size_t aDestOffset, size_t aDestLen, const PMDataRef &aSourceData)
{	
	Replace(aDestOffset, aDestLen, aSourceData->buffer(), aSourceData->size());
}

// ---------------------------------------------------------------------------

void PMData::TruncateBegin(size_t aLen)
{	
	Delete(0, aLen);
}

// ---------------------------------------------------------------------------

void PMData::TruncateEnd(size_t aLen)
{	
	Delete(itsSize-aLen, aLen);
}

// ---------------------------------------------------------------------------

void PMData::Clear()
{	
	Delete(0, itsSize);	// ####
}

// ---------------------------------------------------------------------------

size_t PMData::GetCapacity()
{	
	return itsCapacity;
}

// ---------------------------------------------------------------------------

void* PMData::TakeBufferOwnership(void** aDataStart)
{	
	void* theBuffer = itsBuffer;
	if (aDataStart != 0)
		*aDataStart = itsBuffer + itsStart;
	DoInit(0,0,0);
	return theBuffer;
}

// ---------------------------------------------------------------------------

PMBufferRef	PMData::ExtractBuffer(size_t aFrom, size_t aLen)
{
	if (aFrom + 1 > itsSize)
		return 0;

	if (aLen == pmnpos)
		aLen = itsSize - aFrom;
	else
		if (aFrom + aLen > itsSize)
			return 0;

	if (aLen == 0)
		return new PMBuffer;

	PMBufferRef theBuffer = new PMBuffer(aLen);
	::pm_memcpy(theBuffer->buffer(), itsBuffer + itsStart + aFrom, aLen);

	return theBuffer;
}

// ---------------------------------------------------------------------------
//	IMPLEMENTATION RESERVED
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//	Initialize data object

void PMData::DoInit(const void *aPtr, size_t aSize, size_t aCapacity)
{	
	itsBuffer = 0;
	itsStart = 0;
	itsCapacity = aCapacity;
	itsSize = aSize;

	if (itsCapacity<itsSize)
		itsCapacity = itsSize;

	if (!itsCapacity)
		itsCapacity++;	//	#### cause pm_malloc(0) may not work (check it out)

	itsBuffer = (char *)::pm_malloc(itsCapacity);
	if (aPtr)
		::pm_memcpy(itsBuffer, aPtr, itsSize);
}

// ---------------------------------------------------------------------------
//	Replace a chunk of data

void PMData::DoReplace(size_t aStart, size_t aLen, const void *aData, size_t aDataLen)
{
		//	For debugging
	CheckCapacity();

	//	Algo:
	//	1 - Si les nouvelles donnees font la meme taille, remplacer
	//	2 - Si cela ne tient pas, reallouer et copier
	//	3 - Si la diminution impose un overhead non-acceptable, reallouer et copier
	//	4 - Si cela tient choisir la strategie possible la moins couteuse entre:
	//	4.1 - Deplacer le debut des donnees, copier le remplacement
	//	4.2 - Deplacer la fin des donnees, copier le remplacement
	//	4.3 - Deplacer le debut et la fin des donnes, copier le remplacement

	if (aLen == aDataLen)
	{		//	Just copy
		PM_TRACE(Info, TL("PMData::DoReplace(): COPY(%lu)", (unsigned long)aDataLen));
		if (aData)
				//	Changed to memove(...) because DoReplace can be called with part of
				//	itself in others implementation
			::pm_memmove(itsBuffer+itsStart+aStart, aData, aDataLen);
		return;
	}

	if (itsCapacity<itsSize+aDataLen-aLen
		|| !IsOverheadAcceptable(itsSize+aDataLen-aLen, itsCapacity-(itsSize+aDataLen-aLen)))
	{		//	Not enough room or not acceptable
		DoRecreate(itsBuffer+itsStart, aStart,
				aData, aDataLen,
				itsBuffer+itsStart+aStart+aLen, itsSize-aStart-aLen);
		return;
	}

	pmbool thefStart = pmtrue;
	pmbool thefEnd = pmtrue;

	if (itsStart+aLen<aDataLen)
	{	//	Impossible to move only start
		thefStart = pmfalse;
	}

	if (itsCapacity-itsSize-itsStart+aLen<aDataLen)
	{	//	Impossible to move only end
		thefEnd = pmfalse;
	}

	if (thefStart && thefEnd)
	{	//	We can choose the "best" strategy
		//	(#### pas vraiment, si les deux sont aussi efficients, on deplace arbitrairement
		//			le debut. Ca peux avoir des effets de bord desagreables...
		//			la solution est peut-etre de faire un MOVEBOTH si une des taille est nulle...)
		if (aStart<itsSize-aLen-aStart)
		{	//	Move beginning is more efficient
			_DoReplaceBegin(aStart, aLen, aData, aDataLen);
			return;
		}
		else
		{	//	Move end is more efficient
			_DoReplaceEnd(aStart, aLen, aData, aDataLen);
			return;
		}
	}

	if (thefStart)
	{	_DoReplaceBegin(aStart, aLen, aData, aDataLen);
		return;
	}

	if (thefEnd)
	{	_DoReplaceEnd(aStart, aLen, aData, aDataLen);
		return;
	}

	//	### On devrait demander le HeadOverhead et coder les deux cas
	//	(deplacement a gauche et deplacenent a droite)

		//	We need to copy both
	PM_TRACE(Info, TL("PMData::DoReplace(): MOVEBOTH(%lu, %lu), COPY(%lu)", aStart, itsSize-aStart-aLen, aDataLen));
	::pm_memmove(itsBuffer, itsBuffer+itsStart, aStart);
	::pm_memmove(itsBuffer+aStart+aDataLen, itsBuffer+itsStart+aStart+aLen, itsSize-aStart-aLen);
	if (aData)
		::pm_memcpy(itsBuffer+aStart, aData, aDataLen);
	itsStart = 0;
	itsSize = itsSize - aLen + aDataLen;

	return;
}

// ---------------------------------------------------------------------------
//	Low level (1)

void PMData::_DoReplaceBegin(size_t aStart, size_t aLen, const void *aData, size_t aDataLen)
{	
	PM_TRACE(Info, TL("PMData::_DoReplaceBegin(): MOVE_BEGIN(%lu bytes), COPY(%lu)", (unsigned long)aStart, (unsigned long)aDataLen));
	::pm_memmove(itsBuffer+itsStart+aLen-aDataLen, itsBuffer+itsStart, aStart);
	if (aData)
		::pm_memcpy(itsBuffer+itsStart+aStart+aLen-aDataLen, aData, aDataLen);
	itsStart -= aDataLen-aLen;
	itsSize += aDataLen-aLen;
}

// ---------------------------------------------------------------------------
//	Low level (2)

void PMData::_DoReplaceEnd(size_t aStart, size_t aLen, const void *aData, size_t aDataLen)
{	
	size_t	theMove = itsSize - aLen - aStart;
	if (theMove != 0) 
	{
		PM_TRACE(Info, TL("PMData::_DoReplaceEnd(): MOVE_END(%lu bytes), COPY(%lu)", (unsigned long)theMove, (unsigned long)aDataLen));
		::pm_memmove(itsBuffer+itsStart+aStart+aDataLen, itsBuffer+itsStart+aStart+aLen, theMove);
	}
	if (aData)
		::pm_memcpy(itsBuffer+itsStart+aStart, aData, aDataLen);
	itsSize += aDataLen-aLen;
}

// ---------------------------------------------------------------------------
//	Allocate buffer with specified overhead

void PMData::_AllocateBufferAndStart(size_t aSize)
{	
	/*size_t theOverhead = OverheadFor(aSize);
	itsSize = aSize;
	itsCapacity = itsSize+theOverhead;
	itsStart = HeadOverheadFor(theOverhead);	//	Ordering of lines important in case of exception in HeadOverheadFor()
	itsBuffer = (char *)::pm_malloc(itsCapacity);*/


	size_t theOverhead = OverheadFor(aSize);
	itsBuffer = (char *)::pm_malloc(aSize + theOverhead);
	
		//	Adjust instance variables only now. Because if pm_malloc throw an exception the
		//	object will still be valid
	itsSize = aSize;
	itsCapacity = aSize + theOverhead;
	itsStart = HeadOverheadFor(theOverhead);
}

// ---------------------------------------------------------------------------
//	Re-construct from 3 buffers

void PMData::DoRecreate(const void *aBuffer1, size_t aLen1, const void *aBuffer2, size_t aLen2, const void *aBuffer3, size_t aLen3)
{	
	//	PM_TRACE(Info, TL("PMData::DoRecreate(): RECREATE(%lu)", (unsigned long)((aBuffer1?aLen1:0)+(aBuffer2?aLen2:0)+(aBuffer3?aLen3:0))));
	
	char *theOldBuffer = itsBuffer;
	
	_AllocateBufferAndStart(aLen1+aLen2+aLen3);

	if (aBuffer1)
		::pm_memcpy(itsBuffer + itsStart, aBuffer1, aLen1);
	if (aBuffer2)
		::pm_memcpy(itsBuffer + itsStart + aLen1, aBuffer2, aLen2);
	if (aBuffer3)
		::pm_memcpy(itsBuffer + itsStart + aLen1 + aLen2, aBuffer3, aLen3);

	::pm_free(theOldBuffer);
}


// ---------------------------------------------------------------------------
//	This specify the admitted overhead for a certain size, when building 
//	buffer from scratch

size_t PMData::OverheadFor(size_t aSize)
{	
	return ::pm_max((size_t)(aSize * itsOverheadFactor / 100.0), itsMinOverhead);
}

// ---------------------------------------------------------------------------
//	This specify where the free space should be placed, if we have choice
//	This is totally arbitrary

size_t PMData::HeadOverheadFor(size_t anOverhead)
{		
	return (size_t)((anOverhead / 2) * (1 - itsOverheadRepartition / 100.0));
}

// ---------------------------------------------------------------------------
//	This control the overhead when shrinking

pmbool PMData::IsOverheadAcceptable(size_t aSize, size_t anOverhead)
{		
		//	This is our low water mark
	if (aSize+anOverhead<=itsMinCapacity)
		return pmtrue;
		
		//	Overhead is acceptable iff it is less than the overhead we'll have choosed
		//	Which is bad, as if we remove bytes after bytes, it'll alway realloc
	return OverheadFor(aSize)>anOverhead;
}

// ---------------------------------------------------------------------------
//	This does check to see if PMData::*Overhead*()
//	looks correct

void PMData::CheckCapacity()
{
/*
	size_t theAdmittedOverhead = OverheadFor(itsSize);
	if (theAdmittedOverhead<itsCapacity-itsSize)
	{

		PM_TRACE(Check, TL("Overhead too big (%ld instead of %ld) for size %ld",
			(long)itsCapacity-itsSize,
			(long)theAdmittedOverhead,
			(long)itsSize));
	}
	if (!IsOverheadAcceptable(itsSize, itsCapacity-itsSize))
	{
		PM_TRACE(Check, TL("Overhead inaceptable (%ld), for %ld",
			(long)itsCapacity-itsSize,
			(long)itsSize));
	}
*/
}

