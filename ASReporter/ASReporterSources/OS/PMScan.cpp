// ===========================================================================
//	PMScan.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMScan.h"

// ----------------------------------------------------------------------------

#define PM_TRACEMODULE STRUCT
PM_DEFINE_TRACE_CODE(Info, 1);

// ===========================================================================
//	PMScanner
// ===========================================================================

PMStrRef PMScanner::StringFromScanners(PMScanner& aScan1, PMScanner& aScan2 )
{
	PM_ASSERT(!aScan1.itsString.IsNull(),TL("PMScanner::StringFromScanners(): Null string 1"));
	PM_ASSERT(!aScan2.itsString.IsNull(),TL("PMScanner::StringFromScanners(): Null string 2"));
	PM_ASSERT(aScan1.itsString.Get() == aScan2.itsString.Get(),TL("Different strings"));
	PM_ASSERT(aScan2.itsPosition >= aScan1.itsPosition,TL("Bad positions"));
	
	size_t	theSize = (size_t) (aScan2.itsPosition - aScan1.itsPosition);
	
	return PMStr::NewFromCString(aScan1.itsPosition, PMStr_kCopy, theSize);
}

// ----------------------------------------------------------------------------

PMScanner::PMScanner() :
		itsString()
{
	itsStringStart = itsPosition = 0L;
}

// ----------------------------------------------------------------------------

PMScanner::~PMScanner()
{
}

// ----------------------------------------------------------------------------

PMScanner::PMScanner(const PMStrRef& aString) :
		itsString(aString)
{
	itsStringStart = itsPosition = aString->c_str();
}

// ----------------------------------------------------------------------------

PMScanner::PMScanner(const PMScanner& aScanner) :
		itsString(aScanner.itsString),
		itsStringStart(aScanner.itsStringStart),
		itsPosition(aScanner.itsPosition)
{
}

// ----------------------------------------------------------------------------

PMScanner& PMScanner::operator= (const PMScanner& aScanner)
{
	if (&aScanner == this)
		return (*this);
		
	itsString = aScanner.itsString;
	itsStringStart = aScanner.itsStringStart;
	itsPosition = aScanner.itsPosition;
	
	return (*this);
}

// ----------------------------------------------------------------------------

PMScanner_eScannerStatus PMScanner::GetStatus() const
{
	if (IsAtEnd()) return PMScanner_kAtEnd;
	return PMScanner_kOk;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::IsAtEnd() const
{	
	return !*itsPosition;
}

// ----------------------------------------------------------------------------

PMStrRef PMScanner::ExtractStringToEnd()
{
	PMScanner theScanner(*this);
	theScanner.GoToEnd();

	return StringFromScanners(*this, theScanner);
}

// ----------------------------------------------------------------------------

PMStrRef PMScanner::ExtractStringFromStart()
{
	PMScanner theScanner(*this);
	theScanner.GoToStart();

	return StringFromScanners(theScanner, *this);
}

// ----------------------------------------------------------------------------

pmbool PMScanner::Forward()
{	
	if (IsAtEnd()) return pmfalse;
	itsPosition++;
	return pmtrue;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::Backward()
{	
	if (itsPosition == itsStringStart)
		return pmfalse;
	itsPosition--;
	return pmtrue;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::Forward(size_t anOffset)
{	
	// must return pmfalse if we are at end even if anOffset == 0
	if (anOffset == 0 && IsAtEnd())
		return pmfalse; 

	while (anOffset--)
	{
		if (IsAtEnd())
			return pmfalse;
		itsPosition++;
	}
	return pmtrue;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::Backward(size_t anOffset)
{	
	if (anOffset == 0 && itsPosition == itsStringStart)
		return pmfalse;

	while (anOffset--)
	{
		if (itsPosition == itsStringStart)
			return pmfalse;
		itsPosition--;
	}
	return pmtrue;
}

// ----------------------------------------------------------------------------

void PMScanner::GoToStart()
{	
	itsPosition=itsStringStart;
}

// ----------------------------------------------------------------------------

void PMScanner::GoToEnd()
{	
	itsPosition = itsStringStart+itsString->size();
}

// ----------------------------------------------------------------------------

pmbool PMScanner::Peek(char* aChar)
{
	if (IsAtEnd())
		return pmfalse;

	*aChar = *itsPosition;
	return pmtrue;
}

// ----------------------------------------------------------------------------
// set the scanner position
// setting the position after the end of the string is an error (this should assert)

void PMScanner::SetPos(size_t aPos)
{
	PM_ASSERT(aPos <= itsString->size(), TL("SetPos out of range"));

	itsPosition = itsStringStart + aPos;
}

// ----------------------------------------------------------------------------
// get the scanner position (0 based index in the string)

size_t PMScanner::GetPos()
{
	return (itsPosition - itsStringStart);
}

// ----------------------------------------------------------------------------

pmbool PMScanner::ForwardAcceptOne( char aChar )
{
	if (!IsAtEnd())
		if ((*itsPosition) == aChar)
		{
			itsPosition++;
			return pmtrue;
		}
			
	return pmfalse;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::ForwardAcceptMany( char aChar )
{
	if (!ForwardAcceptOne(aChar))
		return pmfalse;

	while (ForwardAcceptOne(aChar))
		;

	return pmtrue;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::ForwardScanTo(char aChar)
{
	const char* thePos = itsPosition;

	while (!IsAtEnd())
	{
		if (*itsPosition == aChar)
			return pmtrue;

		itsPosition++;
	}

	itsPosition = thePos;
	return pmfalse;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::ForwardAcceptOne(const PMCharSet& aSet, char* aCharAccepted)
{
	if (IsAtEnd())
		return pmfalse;

	if (aSet.IsPresent(*itsPosition))
	{
		if (aCharAccepted != 0)
			*aCharAccepted = *itsPosition;
		itsPosition++;
		return pmtrue;
	}

	return pmfalse;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::ForwardAcceptMany( const PMCharSet &aSet )
{
	char theChar;

	if (!ForwardAcceptOne(aSet, &theChar))
		return pmfalse;

	while (ForwardAcceptOne(aSet, &theChar))
		;

	return pmtrue;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::ForwardScanTo(const PMCharSet& aSet)
{
	// Optimistic algorith: we assume that we more often succeed
	const char* thePos = itsPosition;

	while (!IsAtEnd())
	{
		if (aSet.IsPresent(*itsPosition))
			return pmtrue;

		itsPosition++;
	}

	itsPosition = thePos;
	return pmfalse;
}
	
// ----------------------------------------------------------------------------

pmbool PMScanner::ScanInt32(pmint32* anInt32)
{
	size_t		thePos = GetPos();
	pmbool		theNeg = ForwardAcceptOne('-');

	pmuint32	theInt32;
	if (!ScanUInt32(&theInt32))
	{
		SetPos(thePos);
		return pmfalse;
	}

	if (theNeg)
	{
		if (theInt32 > (1UL << 31))
		{
			SetPos(thePos);
			return pmfalse;
		}
		*anInt32 = -(pmint32)theInt32;
	}
	else
	{
		if (theInt32 >= (1UL << 31))
		{
			SetPos(thePos);
			return pmfalse;
		}
		*anInt32 = theInt32;
	}

	return pmtrue;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::ScanUInt32(pmuint32* anUInt32)
{
	pmuint32			theResult = 0, thePrevResult;
	pmbool				thefOk = pmfalse;
	char				theDigit;
	const PMCharSet&	theDigits = PMCharSet::Digits();
	size_t				thePos = GetPos();

	while (ForwardAcceptOne(theDigits, &theDigit))
	{
		thefOk = pmtrue;
		thePrevResult = theResult;
		theResult = theResult * 10 + (theDigit - '0');
		if (theResult < thePrevResult) // wrap around UINT_MAX
		{
			SetPos(thePos);
			return pmfalse;
		}
	}

	if (thefOk)
	{
		*anUInt32 = theResult;
		return pmtrue;
	}

	return pmfalse;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::ForwardAccept(const char* aString)
{
	PM_ASSERT(aString != 0, TL("PMScanner::ForwardAccept: aString = 0"));
	size_t thePos = GetPos();
	
	while (*aString != 0)
		if (!ForwardAcceptOne(*aString++))
		{
			SetPos(thePos);
			return pmfalse;
		}
		
	return pmtrue;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::ForwardScanTo(const char *aString)
{
	PM_ASSERT(aString != 0, TL("PMScanner::ForwardScanTo: aString = 0"));
	if (*aString == 0)
	{
		PM_TRACE( Info, TL( "PMScanner::ForwardScanTo: Empty string"));
		return pmfalse;
	}

	size_t theEntryPos = GetPos();

	while (!IsAtEnd())
	{
		if (!ForwardScanTo(*aString))
		{
			SetPos(theEntryPos);
			return pmfalse;
		}
		size_t thePos = GetPos();
		Forward();

		if (ForwardAccept(aString+1))
		{
			SetPos(thePos);
			return pmtrue;
		}
	}

	SetPos(theEntryPos);
	return pmfalse;
}

// ----------------------------------------------------------------------------

int PMScanner::Peek()
{
	char theChar;
	if (!Peek(&theChar))
		return -1;

	return theChar;
}

// ----------------------------------------------------------------------------

pmbool PMScanner::ForwardAcceptOne( const PMStringSet &aSet, size_t* anIndex)
{
	const char*	theString;
	size_t		theIndex = 0;
	
	while ((theString = aSet.StringAtIndex(theIndex)) != 0)
	{
		if (ForwardAccept(theString))
		{
			if (anIndex != 0)
				*anIndex = theIndex;
				
			return pmtrue;
		}
		
		theIndex++;
	}
	
	return pmfalse;
}

