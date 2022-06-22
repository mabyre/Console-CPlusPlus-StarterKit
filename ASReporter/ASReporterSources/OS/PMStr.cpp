// ===========================================================================
//	PMStr.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMStr.h"
#include "PMBuffer.h"
#include "PMScan.h"
#include "PMTrace.h"
#include "PMOSHelp.h"
#include "PMCriSec.h"

// ---------------------------------------------------------------------------

#define PM_TRACEMODULE STRUCT

// ---------------------------------------------------------------------------
// #### NewFromCString was splitted in 2 for GCC 2.5.8, and we removed default 
// arguments

PMStrRef PMStr::NewFromCString(const char *aCString, PMStr_eTrait aTrait, size_t aSize)
{	
	PMStrRef	theRef;
	PMStr		*theStr = new PMStr();
	
	theRef = theStr;	
	theStr->itsLen = (aSize != PMStr_kUnknown) ? aSize: ::pm_strlen(aCString);
	theStr->itsTrait = aTrait;
	
	if (theStr->itsTrait==PMStr_kCopy)
	{	
		theStr->itsData = (char *)::pm_malloc(theStr->itsLen + 1);
		PMCOUNTER("PMStr created by duplication");
		::pm_memcpy((char *) theStr->itsData, aCString, theStr->itsLen);
		((char *) (theStr->itsData))[theStr->itsLen] = 0;
	}
	else
		theStr->itsData = aCString;
		
	return theRef;
}

// ---------------------------------------------------------------------------

PMStrRef PMStr::NewFromCString(const char *aCString)
{	
	return NewFromCString(aCString, PMStr_kCopy, PMStr_kUnknown);
}

// ----------------------------------------------------------------------------
// Register the empty string for ref counting cleanup

static PMStrRef gEmptyString, gCRLFString, gPlatformEOLString;

// ---------------------------------------------------------------------------

PMStrRef PMStr::Empty()
{
	if (gEmptyString.IsNull())
	{
		PMCriticalSectionLocker theLocker(PMRC::GetGlobalCriticalSection());
		// Double test is necessary to check if the string has not been
		// initialized before locking the critical section.
		if (gEmptyString.IsNull())
		{	
			PMRC::RegisterRefCleanUp(&::gEmptyString);
			gEmptyString = NewFromCString("", PMStr_kRef, 0);
		}
	}
	
	return gEmptyString;
}

// ---------------------------------------------------------------------------

PMStrRef PMStr::CRLF()
{	
	if (gCRLFString.IsNull())
	{
		PMCriticalSectionLocker theLocker(PMRC::GetGlobalCriticalSection());
		if (gCRLFString.IsNull())
		{	
			PMRC::RegisterRefCleanUp(&::gCRLFString);
			gCRLFString = PMSTRSTATIC("\r\n");
		}
	}
	
	return gCRLFString;
}

// ---------------------------------------------------------------------------

PMStrRef PMStr::PlatformEOL()
{	
	if (gPlatformEOLString.IsNull())
	{
		PMCriticalSectionLocker theLocker(PMRC::GetGlobalCriticalSection());
		if (gPlatformEOLString.IsNull())
		{	
			PMRC::RegisterRefCleanUp(&::gPlatformEOLString);
			
			switch (PMOSHelper::GetCurrent()->GetPlatformEndOfLine())
			{
				case PMEndOfLine_kCR:
					gPlatformEOLString = PMSTRSTATIC("\r");
					break;

				case PMEndOfLine_kCRLF:
					gPlatformEOLString = PMSTRSTATIC("\r\n");
					break;

				case PMEndOfLine_kLF:
				default:
					gPlatformEOLString = PMSTRSTATIC("\n");
					break;
			}
		}
	}
	
	return gPlatformEOLString;
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::NewFromNumber(pmint32 aNumber, size_t aDigitCount)
{	
	char	*thePtr = (char *)::pm_malloc(aDigitCount + 1);
	char	*theData = thePtr + aDigitCount;
	
	*theData-- = 0;
	while (aDigitCount--)
	{	
		*theData-- = '0' + aNumber % 10;
		aNumber /= 10;
	}

	return NewFromCString(thePtr, PMStr_kDelete, aDigitCount);
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::NewFromUInt32(pmuint32 aNumber)
{
	char theString[34];
	::pm_uint32toa(aNumber, theString, sizeof(theString));

	return NewFromCString(theString);
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::NewRandomString(const PMStrRef &aSource, size_t aLen)
{	
	size_t		theLen = aSource->size();
	const char	*theSource = aSource->c_str();
	char		*thePtr = (char *) ::pm_malloc(aLen + 1);
	
	for (size_t i = 0 ; i != aLen ; i++)
		thePtr[i] = theSource[::pm_rand(0, theLen - 1)];
	thePtr[aLen] = 0;
	
	return NewFromCString(thePtr, PMStr_kDelete, aLen);
}

// ----------------------------------------------------------------------------

PMStr::PMStr() : 
	itsTrait(PMStr_kRef), 
	itsData(0), 
	itsLen(0) 
{
}  

// ----------------------------------------------------------------------------
// Destructor

PMStr::~PMStr()
{	
	if (itsTrait != PMStr_kRef)
		::pm_free((void *) itsData);
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::UpperCase()
{	
	char		*theBuffer = (char *) ::pm_malloc(itsLen+1);
	char		*thePtr = theBuffer;
	const char	*theData = itsData;
	
		// #### Should be safe. May not hurt to check again
	for (size_t i = 0 ; i <= itsLen ; i++)
		*thePtr++ = (char) ::pm_toupper((unsigned char) *theData++);
			
	return NewFromCString(theBuffer, PMStr_kDelete, itsLen);
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::LowerCase()
{	
	char		*theBuffer = (char *)::pm_malloc(itsLen+1);
	char		*thePtr = theBuffer;
	const char	*theData = itsData;
	
		// #### Should be safe. May not hurt to check again
	for (size_t i = 0 ; i <= itsLen ; i++)
		*thePtr++ = (char)::pm_tolower((unsigned char) *theData++);
		
	return NewFromCString(theBuffer, PMStr_kDelete, itsLen);
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::SkipLeading(char aChar)
{	
		// Special case of NULL char
	if (!aChar)		
		return this;
		
	if (*itsData != aChar)
		return this;
		
	const char	*theData = itsData;
	while (*theData++ == aChar)
		;		
	theData--;
	
	return NewFromCString(theData, PMStr_kCopy, itsLen - (theData - itsData));
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::SkipTrailing(char aChar)
{	
		// Special case of NULL char
	if (!aChar)		
		return this;
		
	size_t theLen = itsLen;
	while (theLen > 0 && itsData[theLen - 1] == aChar)
		theLen--;

	if (theLen == itsLen)
		return this;

	return NewFromCString(itsData, PMStr_kCopy, theLen);
}

// ----------------------------------------------------------------------------

PMStrArrayRef PMStr::SplitBy(char aChar)
{	
	const char		*theStart = itsData;
	const char		*theCurrent = itsData;
	PMStrArrayRef	theArray = new PMArray<PMStrRef>;
	size_t			theCount = itsLen;

		//	Each one
	while (theCount--)
		if (*theCurrent++ == aChar)
		{	
			theArray->AppendObject(NewFromCString(theStart, PMStr_kCopy, theCurrent - theStart - 1));
			theStart = theCurrent;
		}

		//	Last one
	if (theArray->GetCount())
		theArray->AppendObject(NewFromCString(theStart, PMStr_kCopy, theCurrent - theStart));
	else		//	We don't copy ourselves if no split occured...
		theArray->AppendObject(this);

	return theArray;
}

// ----------------------------------------------------------------------------

PMStrArrayRef PMStr::SplitBy(const PMStrRef &aSplitString)
{	
	PMScanner		theStart(itsData), theCurrent(theStart);
	PMStrArrayRef	theArray = new PMArray<PMStrRef>;
	
	if (aSplitString.IsNull())
		return theArray;

		//	Each one
	while (!theCurrent.IsAtEnd())
		if (theCurrent.ForwardScanTo(aSplitString->c_str()))
		{
			theArray->AppendObject(PMScanner::StringFromScanners(theStart, theCurrent));
			theCurrent.ForwardAccept(aSplitString->c_str());
			theStart = theCurrent;
		}
		else
			theCurrent.GoToEnd();

		//	Last one
	if (theArray->GetCount())
		theArray->AppendObject(PMScanner::StringFromScanners(theStart, theCurrent));
	else		//	We don't copy ourselves if no split occured...
		theArray->AppendObject(this);

	return theArray;
}

// ----------------------------------------------------------------------------

PMStrArrayRef PMStr::SplitBy(const PMCharSet& aCharSet)
{	
	PMScanner		theStart(itsData), theCurrent(theStart);
	PMStrArrayRef	theArray = new PMArray<PMStrRef>;

		//	Each one
	while (!theCurrent.IsAtEnd())
		if (theCurrent.ForwardScanTo(aCharSet))
		{
			theArray->AppendObject(PMScanner::StringFromScanners(theStart, theCurrent));
			theCurrent.ForwardAcceptMany(aCharSet);
			theStart = theCurrent;
		}
		else
			theCurrent.GoToEnd();

		//	Last one
	if (theArray->GetCount())
		theArray->AppendObject(PMScanner::StringFromScanners(theStart, theCurrent));
	else		//	We don't copy ourselves if no split occured...
		theArray->AppendObject(this);

	return theArray;
}

// ----------------------------------------------------------------------------

PMStrArrayRef PMStr::SplitByFirst(char aChar)
{	
	PMStrArrayRef	theArray = new PMArray<PMStrRef>;
	const char		*thePtr = ::pm_strchr(itsData, aChar);

	if (thePtr)
	{	
		theArray->AppendObject(NewFromCString(itsData, PMStr_kCopy, thePtr - itsData));
		theArray->AppendObject(NewFromCString(thePtr + 1, PMStr_kCopy, itsLen - (thePtr - itsData) - 1));
	}
	else
		theArray->AppendObject(this);

	return theArray;
}

// ----------------------------------------------------------------------------

PMStrArrayRef PMStr::SplitByLast(char aChar)
{	
	PMStrArrayRef	theArray = new PMArray<PMStrRef>;
	size_t			thePos = ReverseFindChar(aChar, size());

	if (thePos != pmnpos)
	{	
		theArray->AppendObject(NewFromCString(itsData, PMStr_kCopy, thePos));
		theArray->AppendObject(NewFromCString(itsData+thePos + 1, PMStr_kCopy, itsLen - thePos - 1));
	}
	else
		theArray->AppendObject(this);

	return theArray;
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::Append(const PMStrRef &anOther)
{	
	char	*thePtr = (char *) ::pm_malloc(itsLen+anOther->itsLen + 1);
	
	::pm_strcpy(thePtr, itsData);
	::pm_strcpy(thePtr+itsLen, anOther->itsData);
	return NewFromCString(thePtr, PMStr_kDelete, itsLen+anOther->itsLen);
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::Append(const char* aCStr)
{	
	size_t	theSize = ::pm_strlen(aCStr);
	char	*thePtr = (char *) ::pm_malloc(itsLen + theSize + 1);
	
	::pm_strcpy(thePtr, itsData);
	::pm_strcpy(thePtr + itsLen, aCStr);
	return NewFromCString(thePtr, PMStr_kDelete, itsLen + theSize);
}

// ----------------------------------------------------------------------------
//	#### Stupid quoting that doesn't works if anything needs to be quoted...

PMStrRef PMStr::Quote(char aStartQuote, char aEndQuote)
{	
	char	*thePtr = (char *) ::pm_malloc(itsLen+3);
	
	thePtr[0] = aStartQuote;
	::pm_strcpy(thePtr + 1, itsData);
	thePtr[itsLen + 1] = aEndQuote;
	thePtr[itsLen + 2] = 0;
	
	return NewFromCString(thePtr, PMStr_kDelete, itsLen+2);
}

// ----------------------------------------------------------------------------
//	#### Not a very nice coding, but...

PMStrRef PMStr::Unquote(char aStartQuote, char aEndQuote)
{
	if (itsLen == 0)
		return this;

	const char	*theStart = itsData;
	const char	*theEnd = itsData + itsLen - 1;

	//	Skip leading Spaces
	while (*theStart == ' ' || *theStart == '\t')
		theStart++;

	//	Skip trailing spaces
	while (theEnd >= theStart && (*theEnd == ' ' || *theEnd == '\t'))
		theEnd--;

	//	Remove eventual quotes
	if (*theStart == aStartQuote)
	{	
		theStart++;
		if (*theEnd == aEndQuote && theEnd >= theStart)
			theEnd--;
	}

	//	If nothing to change
	//	Cast is safe as theEnd>=theStart
	if (theStart == itsData && (size_t)(theEnd - theStart) + 1 == itsLen)
		return this;

	//	Create new string
	return NewFromCString(theStart, PMStr_kCopy, theEnd - theStart + 1);
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::StripParenthesis()
{	
	PMBuffer	theBuffer(itsLen);
	char		*thePtr = (char*)theBuffer.buffer();
	const char	*theData = itsData;
	size_t		theLen = itsLen;
	pmint32		theCLevel = 0;

	while (theLen--)
	{	
		char	theChar = *theData++;
		
		if (theChar == '(')
			theCLevel++;
			
		if (!theCLevel)
			*thePtr++ = theChar;
			
		if (theChar == ')')
			theCLevel--;
	}

	return NewFromCString((char*)theBuffer.buffer(), PMStr_kCopy, thePtr - (char*) theBuffer.buffer());
}

// ----------------------------------------------------------------------------
// string hash function
// (same as ELF hash function)

	static pmuint32 PMHashStr(const char *aStr)
	{	
		const unsigned char	*thePtr = (const unsigned char *) aStr;
		pmuint32			theHash = 0;
		pmuint32			theValue;
		
		while (*thePtr)
		{	
			theHash = (theHash << 4) + *thePtr++;
			theValue = theHash & 0xf0000000;
			if (theValue)
				theHash ^= theValue >> 24;
			theHash &= ~theValue;
		}
		
		return theHash;
	}

// ----------------------------------------------------------------------------

pmuint32 PMStr::Hash()
{	
	return PMHashStr(itsData);
}

// ----------------------------------------------------------------------------

int PMStr::cCompare(const char *aStr)
{	
	return ::pm_strcmp(itsData, aStr);
}

// ----------------------------------------------------------------------------

pmbool PMStr::IsPrefix(const PMStrRef &aStr, pmbool afAnyCase)
{	
	if (aStr.IsNull())
		return pmfalse;
		
	if (aStr->itsLen < itsLen)
		return pmfalse;
		
	return afAnyCase ? ::pm_strncasecmp(itsData, aStr->itsData, itsLen) == 0 :
 	    			   ::pm_strncmp(itsData, aStr->itsData, itsLen) == 0;
}

// ----------------------------------------------------------------------------

pmbool PMStr::IsSuffix(const PMStrRef &aStr, pmbool afAnyCase)
{	
	if (aStr.IsNull())
		return pmfalse;
		
	if (aStr->itsLen<itsLen)
		return pmfalse;
		
 	return afAnyCase ? ::pm_strncasecmp(aStr->itsData + aStr->itsLen - itsLen, itsData, itsLen) == 0 :
					   ::pm_strncmp( aStr->itsData + aStr->itsLen - itsLen, itsData, itsLen ) == 0;
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::ReplaceCharSetByChar(const PMCharSet &aCharSet, char aChar)
{	
	char	*theBuffer = (char *) ::pm_malloc(itsLen + 1);
	
	::pm_memcpy(theBuffer, itsData, itsLen + 1);
	for (size_t i = 0 ; i != itsLen ; i++)
		if (aCharSet.IsPresent(theBuffer[i]))
			theBuffer[i] = aChar;
			
	return NewFromCString(theBuffer, PMStr_kDelete, itsLen);
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::ReplaceCharByChar(char aChar, char aReplace)
{	
		// optimize if the char is not part of the string
	char	*thePtr = ::pm_strchr(itsData, aChar);
	
	if (thePtr==0)
		return this;
		
	char	*theBuffer = (char *) ::pm_malloc(itsLen + 1);
	
	::pm_memcpy(theBuffer, itsData, itsLen + 1);
	for (size_t i = (thePtr - itsData) ; i != itsLen ; i++)
		if (theBuffer[i] == aChar)
			theBuffer[i] = aReplace;
			
	return NewFromCString(theBuffer, PMStr_kDelete, itsLen);
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::SubString(size_t aStart, size_t aLen)
{	
	size_t	theEnd = aStart + aLen;
	
	if (theEnd > itsLen)
		theEnd = itsLen;
	if (aStart > theEnd)
		return Empty();
		
	return NewFromCString(itsData + aStart, PMStr_kCopy, theEnd - aStart);
}

//	------------------------------------------------------------------------------------

PMStrRef PMStr::Left(size_t aLen)
{	
	return SubString(0, aLen);
}

//	------------------------------------------------------------------------------------

PMStrRef PMStr::Right(size_t aLen)
{	
	if (aLen > itsLen)
		aLen = itsLen;
		
	return SubString(itsLen - aLen, aLen);
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::RemoveCharSet(const PMCharSet &aCharSet)
{	
	PMBuffer	theBuffer(itsLen + 1);
	char*		thePtr = (char*)theBuffer.buffer();
	
	::pm_memcpy(thePtr, itsData, itsLen + 1);
	for (size_t i = 0 ; i != itsLen ; i++)
		if (!aCharSet.IsPresent(itsData[i]))
			*thePtr++ = itsData[i];
			
	*thePtr++ = 0;
	
	return NewFromCString((char*)theBuffer.buffer(), PMStr_kCopy, thePtr - (char*)theBuffer.buffer() - 1);
}

// ----------------------------------------------------------------------------

PMStrRef PMStr::ReplaceSubString(size_t aStart, size_t aLen, const PMStrRef& aReplace)
{
	PMBuffer theBuffer(itsLen + 1);
	char *thePtr = (char*)theBuffer.buffer();


	if (aStart >= itsLen)
		return NewFromCString(itsData, PMStr_kCopy, itsLen);

	if (aStart + aLen > itsLen)
		aLen = itsLen - aStart;

	if (aLen > aReplace->itsLen)
		aLen = aReplace->itsLen;

	::pm_memcpy(thePtr, itsData, itsLen + 1);

	::pm_memcpy(thePtr + aStart, aReplace->itsData, aLen);

	return NewFromCString(thePtr, PMStr_kCopy, itsLen);
}

// ----------------------------------------------------------------------------

size_t PMStr::FindChar(char aChar, size_t aStart)
{
	if (itsLen == 0 || aStart == itsLen)
		return pmnpos;

	PM_ASSERT(aStart < itsLen || aStart == pmnpos, TL("PMStr::FindChar() bad start"));
	
	char	*thePtr = ::pm_strchr(itsData + aStart + 1, aChar);
	if (!thePtr)
		return pmnpos;
		
	return thePtr - itsData;
}

// ----------------------------------------------------------------------------

size_t PMStr::FindStr(const PMStrRef& aStr, size_t aStart)
{	
	PM_ASSERT(aStart <= itsLen || aStart == pmnpos, TL("PMStr::FindStr() bad start"));
	
	char	*thePtr = ::pm_strstr(itsData + aStart + 1, aStr->c_str());
	if (!thePtr)
		return pmnpos;
		
	return thePtr - itsData;
}
  
// ----------------------------------------------------------------------------
  
size_t PMStr::FindStrncase(const PMStrRef& aStr, size_t aStart)
{	
	PM_ASSERT(aStart <= itsLen || aStart == pmnpos, TL("PMStr::FindStrncase() bad start"));
	
	const char *thePtr = ::pm_strstrncase(itsData + aStart + 1, aStr->c_str());
	if (!thePtr)
		return pmnpos;
		
	return thePtr - itsData;
}

// ----------------------------------------------------------------------------

size_t PMStr::ReverseFindChar(char aChar, size_t aStart)
{
	if (itsLen == 0)
		return pmnpos;

	PM_ASSERT(aStart <= itsLen, TL("PMStr::ReverseFindChar() bad start"));

		//	We do not use a (non existent) pm_strrchr() because
		//	it wouldn't be efficient (and it wouldn't work)
	while (--aStart != pmnpos && itsData[aStart] != aChar)
		;

	return aStart;
}

// ===========================================================================
//	PMStrRef
// ===========================================================================

// ----------------------------------------------------------------------------

PMStrRef::PMStrRef()

#if defined(PM_DEBUG_STR)
: itsStr((itsPointer == 0) ? "*** Null PMStrRef" : ((PMStr *)itsPointer)->itsData)
#endif

{
}

// ----------------------------------------------------------------------------

PMStrRef::PMStrRef(int aValue)
: PMRef<PMStr>(aValue)

#if defined(PM_DEBUG_STR)
, itsStr((itsPointer == 0) ? "*** Null PMStrRef" : ((PMStr *)itsPointer)->itsData)
#endif

{
}

// ----------------------------------------------------------------------------

PMStrRef::PMStrRef(const PMStrRef &aStr) :
	PMRef<PMStr>(aStr.Get()) 

#if defined(PM_DEBUG_STR)
, itsStr((itsPointer == 0) ? "*** Null PMStrRef" : ((PMStr *)itsPointer)->itsData)
#endif

{
}
	
// ----------------------------------------------------------------------------

PMStrRef::PMStrRef(PMStr *anObject) :
	PMRef<PMStr>(anObject) 

#if defined(PM_DEBUG_STR)
, itsStr((itsPointer == 0) ? "*** Null PMStrRef" : ((PMStr *)itsPointer)->itsData)
#endif

{
}

// ----------------------------------------------------------------------------
// C string constructor
// #### Inefficient
//		Buffer copy (may not be necessary)
//		Construction of a PMStr *, copy in a PMStrRef
//		copy constructing us with that PMStrRef
//		destruction of this PMStrRef

PMStrRef::PMStrRef(const char *aCStr) :
	PMRef<PMStr>(PMStr::NewFromCString(aCStr))

#if defined(PM_DEBUG_STR)
, itsStr((itsPointer == 0) ? "*** Null PMStrRef" : ((PMStr *)itsPointer)->itsData)
#endif

{
}

// ----------------------------------------------------------------------------
// buffer constructor
// #### Inefficient(copy) if aTrait == PMStr_kCopy (defaut value)

PMStrRef::PMStrRef(const void *aBuffer, size_t aLen, PMStr_eTrait aTrait) :
	PMRef<PMStr>(PMStr::NewFromCString((char*) aBuffer, aTrait, aLen))

#if defined(PM_DEBUG_STR)
, itsStr((itsPointer == 0) ? "*** Null PMStrRef" : ((PMStr *)itsPointer)->itsData)
#endif

{
}

// ----------------------------------------------------------------------------

PMStrRef::~PMStrRef()
{
}

// ----------------------------------------------------------------------------

PMStrRef &PMStrRef::operator+=(const PMStrRef &aStr)
{
	*this = (*this)->Append(aStr);
	return *this;
}

// ----------------------------------------------------------------------------

PMStrRef &PMStrRef::operator+=(const char *aStr)
{
	*this = (*this)->Append(aStr);
	return *this;
}

// ----------------------------------------------------------------------------

PMStrRef PMStrRef::operator+(const PMStrRef &aStr)
{
	return (*this)->Append(aStr);
}

// ----------------------------------------------------------------------------

pmbool PMStrRef::operator==(const PMStrRef &aStr) const
{
	return (*this)->cCompare(aStr->c_str())==0;
}

// ----------------------------------------------------------------------------

pmbool PMStrRef::operator!=(const PMStrRef &aStr) const
{
	return !(*this==aStr);
}

// ----------------------------------------------------------------------------

PMStrRef PMStrRef::operator+(const char *aStr)
{ 
	return (*this)->Append(aStr); 
}

// ----------------------------------------------------------------------------

#if defined(PM_DEBUG_STR)

PMStrRef PMStrRef::operator=(const PMStrRef& aStr)
{
	PMRef<PMStr>::operator=(aStr);
	itsStr = (itsPointer == 0) ? "*** Null PMStrRef" : ((PMStr*)itsPointer)->itsData;
	return *this;
}

#endif  // PM_DEBUG_STR

// ----------------------------------------------------------------------------

PMStrRef::operator const char *() const
{
	return (*this)->c_str(); 				
}

// ----------------------------------------------------------------------------
// Buffer manipulation for system calls
// The ref now points to a new PMStr with a buffer a aSize+1 allocated
// using pm_malloc that will be deleted (PMStr_kDelete).
// Returns the allocated buffer

char* PMStrRef::NewBuffer(size_t aSize)
{
	operator=(PMStrRef(::pm_malloc(aSize + 1), aSize, PMStr_kDelete));

	return (char*) ((PMStr*) itsPointer)->itsData;
}


// ----------------------------------------------------------------------------
// You must call this method after a call to NewBuffer and manipulating
// the buffer. It allows the string to update its content.
// If aSize is default or PMStr_kUnknown, pm_strlen is used to obtain the string len.
// If aSize is >=0 a null char is appended at itsBuffer[aSize] and itsLen is set to aSize
// Returns the size (as returned by size())

size_t PMStrRef::Adjust(size_t aSize)
{
	PMStr	*theStr = (PMStr*) itsPointer;

	if (aSize == PMStr_kUnknown)
		aSize = ::pm_strlen(theStr->itsData);
	else
		((char*) theStr->itsData)[aSize] = 0;

	return (theStr->itsLen = aSize);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

pmuint32 Hash(const PMStrRef &aString)
{	
	return aString->Hash();
}

// ----------------------------------------------------------------------------
// #### Would be better if returns something else than plain 'int'

int Compare(const PMStrRef &aString1, const PMStrRef &aString2)
{	
	return aString1->cCompare(aString2->c_str());
}

// ----------------------------------------------------------------------------

PMStrRef PMFileNameHelper::DoConversion(const PMStrRef &aFileName, const PMCharSet& aCharSet, size_t aNameLen, size_t anExtLen)
{
	PMStrRef	theExtension;
	PMStrRef	theName;
	size_t		theExtensionPos = aFileName->ReverseFindChar('.', aFileName->size());
	
	// No extension or extension limited and too long
	if (theExtensionPos == pmnpos || (anExtLen != 0 && aFileName->size() - theExtensionPos > anExtLen + 1))
	{
		theName = aFileName;
		theExtension = PMStr::Empty();
	}
	else
	{	
		theName = aFileName->SubString(0, theExtensionPos);
		if (anExtLen != 0)
			theExtension = aFileName->SubString(theExtensionPos + 1, anExtLen);
		else
			theExtension = aFileName->SubString(theExtensionPos + 1, aFileName->size());

		theExtension->ReplaceCharSetByChar(aCharSet, '_');

		theExtension = (PMStrRef)"." + theExtension;
	}

	if (aNameLen != 0)
		theName = theName->SubString(0, aNameLen);

	theName = theName->ReplaceCharSetByChar(aCharSet, '_');
	theName += theExtension;

	return theName;
}

// ----------------------------------------------------------------------------

PMStrRef PMFileNameHelper::ToDOSFileName(const PMStrRef &aFileName) 
{
	static pmbool		sfInited = pmfalse;
	static PMCharSet	sInvDOSCharSet;

	if (!sfInited)
	{	
		sInvDOSCharSet = PMCharSet::UpperAlphas();
		sInvDOSCharSet.UnionWith(PMCharSet::Digits());
		sInvDOSCharSet.Add("_-~%$#=!");
		sInvDOSCharSet.Negate();
		sfInited = pmtrue;
	}

	return DoConversion(aFileName->UpperCase(), sInvDOSCharSet, 8, 3);
}

// ----------------------------------------------------------------------------

PMStrRef PMFileNameHelper::ToWin32FileName(const PMStrRef &aFileName)
{
	PMCharSet theSet("<>:\"/\\| ");
	theSet.UnionWith(PMCharSet::Controls());

	return DoConversion(aFileName, theSet, 60, 4);
}

// ----------------------------------------------------------------------------

PMStrRef PMFileNameHelper::ToMacFileName(const PMStrRef &aFileName)
{
	PMCharSet theSet(":");
	theSet.UnionWith(PMCharSet::Controls());

	return aFileName->SubString(0,31)->ReplaceCharSetByChar(theSet, '_');
}

// ----------------------------------------------------------------------------

PMStrRef PMFileNameHelper::ToUNIXFileName(const PMStrRef &aFileName)
{
	PMCharSet theSet("/");
	theSet.UnionWith(PMCharSet::Controls());

	return DoConversion(aFileName, theSet, 31, 4);
}

// ===========================================================================
//	PMStrConverter
// ===========================================================================

// ----------------------------------------------------------------------------

	static const char* kPMHexDigits = "0123456789ABCDEF";

	static unsigned char CharHexValue(char aChar)
	{
	    if (aChar > 'a')
			return aChar - 'a' + 10;
			
	    if (aChar > 'A')
			return aChar - 'A' + 10;
			
	    return aChar - '0';
	}

// ----------------------------------------------------------------------------

pmbool PMStrConverter::ToMacType(const PMStrRef &aStr, pmuint32 *outType)
{
    size_t i = 0;

    if (aStr->size() == 8)
    {	
    	pmuint32 theType = 0;
        
        while (i < 8)
        {
            theType <<= 8;
            theType |= CharHexValue(aStr->c_str()[i]) * 16 + CharHexValue(aStr->c_str()[i + 1]);
			i += 2;
        }       
        
        *outType = theType;
        return pmtrue;
    }       
    
    return pmfalse;
}

// ----------------------------------------------------------------------------

PMStrRef PMStrConverter::FromMacType(pmuint32 aTypeCreator)
{       
	char	theBuffer[8];
    char	*thePtr = theBuffer;

    for (size_t i = 0 ; i != 4; i++)
    {	
    	unsigned char	theChar = (unsigned char) (aTypeCreator >> 24);
        char			theChar1 = theChar >> 4;
        char			theChar2 = theChar & 0xf;
        
        *thePtr++ = kPMHexDigits[theChar1];
        *thePtr++ = kPMHexDigits[theChar2];
        aTypeCreator <<= 8;
    }
    
    return PMStr::NewFromCString(theBuffer, PMStr_kCopy, 8);
}

// ----------------------------------------------------------------------------

void PMStrConverter::ToHex(unsigned char aChar, char* aStr)
{       
	aStr[0] = kPMHexDigits[aChar >> 4];
	aStr[1] = kPMHexDigits[aChar & 15];
}

// ----------------------------------------------------------------------------

pmbool PMStrConverter::FromHex(const char* aStr, char* aChar)
{
	unsigned char	theChar1 = *aStr++;	
	if (theChar1 == 0)
		return pmfalse;
		
	unsigned char	theChar2 = *aStr++;
	if (theChar2 == 0)
		return pmfalse;
		
	const PMCharSet&	theHex = PMCharSet::Hex();	
	if (!theHex.IsPresent(theChar1) || !theHex.IsPresent(theChar2))
		return pmfalse;

	if (theChar1 >= 'A')
		theChar1 = (theChar1 & 0x5f) - 'A' + 10;
	else
		theChar1 -= '0';
		
	if (theChar2 >= 'A')
		theChar1 = (theChar1*16) + (theChar2 & 0x5f) - 'A' + 10;
	else
		theChar1 = (theChar1*16) + (theChar2 - '0');
		
	(*aChar) = theChar1;
	return pmtrue;
}
