// ===========================================================================
//	PMCharS.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMCharS.h"
#include "PMTrace.h"

// ===========================================================================
//		PMCharSet
// ===========================================================================

// ---------------------------------------------------------------------------

PMCharSet::PMCharSet()
{
	for (size_t theIndex = 0; theIndex < 8 ; theIndex++)
		itsFlags[theIndex] = 0;
}

// ---------------------------------------------------------------------------

PMCharSet::PMCharSet(const char* aCharSetString)
{
	for (size_t theIndex = 0; theIndex < 8 ; theIndex++)
		itsFlags[theIndex] = 0;

	Add(aCharSetString);
}

// ---------------------------------------------------------------------------

PMCharSet::PMCharSet(const PMCharSet& aSet)
{
	for (size_t theIndex = 0; theIndex < 8 ; theIndex++)
		itsFlags[theIndex] = aSet.itsFlags[theIndex];
}

// ---------------------------------------------------------------------------

PMCharSet& PMCharSet::operator=(const PMCharSet& aSet)
{
	if (this != &aSet)
	{
		for (size_t theIndex = 0; theIndex < 8 ; theIndex++)
			itsFlags[theIndex] = aSet.itsFlags[theIndex];
	}

	return *this;
}

// ---------------------------------------------------------------------------

void PMCharSet::Add(char aChar)
{
	size_t theIndex = ((unsigned char) aChar) / 32; 
	
	itsFlags[theIndex] |= (1L << (((unsigned char) aChar) % 32));
}

// ---------------------------------------------------------------------------

void PMCharSet::Add(const char* aCharSetString)
{
	PM_ASSERT(aCharSetString != 0, TL("Null string") );
	
	while ((*aCharSetString) != 0)
	{
		Add(*aCharSetString);
		aCharSetString++;
	}
}

// ---------------------------------------------------------------------------

void PMCharSet::Remove(char aChar)
{
	size_t theIndex = ((unsigned char) aChar) / 32; 
	
	itsFlags[theIndex] &= ~(1L << (((unsigned char) aChar) % 32));
}

// ---------------------------------------------------------------------------

void PMCharSet::Remove(const char* aCharSetString)
{
	PM_ASSERT(aCharSetString != 0, TL("Null string"));
	
	while ((*aCharSetString) != 0)
	{
		Remove(*aCharSetString);
		aCharSetString++;
	}
}

// ---------------------------------------------------------------------------

void PMCharSet::UnionWith(const PMCharSet& aCharSet)
{
	for (size_t theIndex = 0; theIndex < 8 ; theIndex++)
		itsFlags[theIndex] |= aCharSet.itsFlags[theIndex];
}

// ---------------------------------------------------------------------------

void PMCharSet::IntersectionWith(const PMCharSet& aCharSet)
{
	for (size_t theIndex = 0; theIndex < 8 ; theIndex++)
		itsFlags[theIndex] &= aCharSet.itsFlags[theIndex];
}

// ---------------------------------------------------------------------------

void PMCharSet::Substract(const PMCharSet& aCharSet)
	{
	for (size_t theIndex = 0; theIndex < 8 ; theIndex++)
		itsFlags[theIndex] &= ~(aCharSet.itsFlags[theIndex]);
	}

// ---------------------------------------------------------------------------

void PMCharSet::Negate()
{
	for (size_t theIndex = 0; theIndex < 8 ; theIndex++)
		itsFlags[theIndex] = ~(itsFlags[theIndex]);
}

// ---------------------------------------------------------------------------

const PMCharSet& PMCharSet::EmptySet()
{
	static PMCharSet sSet;

	return sSet;
}

// ---------------------------------------------------------------------------

const PMCharSet& PMCharSet::Digits()
{
	static PMCharSet sSet("0123456789");

	return sSet;
}

// ---------------------------------------------------------------------------

const PMCharSet& PMCharSet::LowerAlphas()
{
	static PMCharSet sSet("abcdefghijklmnopqrstuvwxyz");

	return sSet;
}

// ---------------------------------------------------------------------------

const PMCharSet& PMCharSet::UpperAlphas()
{
	static PMCharSet sSet("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	return sSet;
}

// ---------------------------------------------------------------------------

const PMCharSet& PMCharSet::AlphasDigits()
{	static PMCharSet	sSet;
	static pmbool		sfInited = pmfalse;

	if (!sfInited)
	{	sSet = PMCharSet::LowerAlphas();
		sSet.UnionWith(PMCharSet::UpperAlphas());
		sSet.UnionWith(PMCharSet::Digits());
	}
	return sSet;
}

// ---------------------------------------------------------------------------

const PMCharSet& PMCharSet::Hex()
{
	static PMCharSet sSet("0123456789ABCDEFabcdef");

	return sSet;
}

// ---------------------------------------------------------------------------

const PMCharSet& PMCharSet::WhiteSpaces()
{
	static PMCharSet sSet("\011\012\013\014\015 ");

	return sSet;
}

// ---------------------------------------------------------------------------

const PMCharSet& PMCharSet::SevenBits()
{
	static PMCharSet	sSet;
	static pmbool		sfInited = pmfalse;

	if (!sfInited)
	{
		for (unsigned char theChar = 0; theChar <= 127; theChar++)
			sSet.Add(theChar);
		sfInited = pmtrue;
	}

	return sSet;
}

// ---------------------------------------------------------------------------

const PMCharSet& PMCharSet::Controls()
{	static PMCharSet	sSet;
	static pmbool		sfInited = pmfalse;

	if (!sfInited)
	{
		for (unsigned char theChar = 0; theChar <= 31; theChar++)
			sSet.Add(theChar);
		sfInited = pmtrue;
	}

	return sSet;
}

// ===========================================================================
//		PMStringSet
// ===========================================================================

// ---------------------------------------------------------------------------

PMStringSet::PMStringSet(const char**aStringSet )
{
	itsStringSet = aStringSet;
	itsStringCount = 0;
	
	while (itsStringSet[itsStringCount] != 0)
		itsStringCount++;
}

// ---------------------------------------------------------------------------

const char*	PMStringSet::StringAtIndex(size_t anIndex) const
{
	if (anIndex < itsStringCount)
		return itsStringSet[anIndex];
		
	return 0;
}

// ---------------------------------------------------------------------------

const PMStringSet& PMStringSet::Day_3()
{
	static const char*	sDay3String[] = 
		{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", 0 };
	
	static PMStringSet	sDay3(sDay3String);
	
	return sDay3;
}

// ---------------------------------------------------------------------------

const PMStringSet& PMStringSet::Day_n()
{
	static const char*	sDaynString[] = 
		{ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", 0 };
	
	static PMStringSet	sDayn(sDaynString);
	
	return sDayn;
}

// ---------------------------------------------------------------------------

const PMStringSet& PMStringSet::Month_3()
{
	static const char*	sMonth3String[] = 
		{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", 0 };
	
	static PMStringSet	sMonth3(sMonth3String);
	
	return sMonth3;
}

// ---------------------------------------------------------------------------



	

