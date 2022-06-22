// ===========================================================================
//	PMStub.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#define PM_INTERNAL_INCLUDE_ALLOWED
	#include "PMStub.h"
#undef PM_INTERNAL_INCLUDE_ALLOWED

#include "PMRC.h"
#include "PMTrace.h"

// Needed for initialization stuff
#include "PMStr.h"
#include "PMOSHelp.h"
#include "PMTCPSoc.h"

// ---------------------------------------------------------------------------
// P-Machinery initialization
// this must be the first routine called in P-Machinery
// call the user defined initialization hook (if any) passing it the parameter
// pm_init must only be called once, and must be balanced by a call to pm_close
// in case pm_init is called more than one time, this must work, because we
// manage internally a 'number of call' counter

static pmint32	cInitCount;

pmbool pm_init(void* aParam)
{	
	if (cInitCount > 0)
	{
		cInitCount++;
		return pmtrue;
	}

	cInitCount++;
	
		//	Signal ask the core to init
	PMRC::_Init();

	// Force initialization of some global shared objects
	// This was causing a problem as PlatformEOL needs the OS helper...
/*
	PMStr::Empty();
	PMStr::CRLF();
	PMStr::PlatformEOL();
*/
	PMOSHelper::SetCurrent(PMOSHelperRef(0));
	PMTCPSocketFactory::SetDefaultFactory(PMTCPSocketFactoryRef(0));

		//	Call the user-defined init
	if (pm_init_hook == 0 || !(*::pm_init_hook)(aParam))
		return pmfalse;

	return pmtrue;
}

// ---------------------------------------------------------------------------
// P-Machinery finalization
// this must be the last routine called in P-Machinery
// call the user defined finalization hook (if any) passing it the parameter

void pm_close(void* aParam)
{
	cInitCount--;

	if (cInitCount == 0)
	{
			//	Clean up static objects
		PMRC::_CleanUp();

			//	Call the user-defined close
		(*::pm_close_hook)(aParam);

			//	Ask the core to close
		PMRC::_Close();
	}
	
	if (cInitCount < 0)
		cInitCount = 0;
}

// ---------------------------------------------------------------------------
// memory allocation
// if the requested size is 0, returns 0
// call the user defined allocation hook (if any)

void* pm_malloc_imp(size_t aSize)
{	
	if (aSize == 0)
		return 0;
		
	return (*::pm_malloc_hook)(aSize);
}

// ---------------------------------------------------------------------------
// memory deallocation
// if the pointer is 0, does nothing
// call the user defined deallocation hook (if any)

void pm_free_imp(void *aPtr)
{	
	if (aPtr)
		(*::pm_free_hook)(aPtr);
}

// ---------------------------------------------------------------------------
// memory reallocation
// if the pointer is 0, allocate a new block
// call the user defined reallocation hook (if any)

void* pm_realloc_imp(void *aPtr, size_t aSize)
{	
	if (aPtr == 0)
		return ::pm_malloc(aSize);
	return (*::pm_realloc_hook)(aPtr, aSize);
}

// ---------------------------------------------------------------------------
// smart memory deallocation (call pm_free)
// if the pointer is 0, does nothing
// on returns the pointer is set to 0

void pm_forget(void** aPtr)
{	
	if (aPtr == 0)
		return;
		
	::pm_free(*aPtr);
	*aPtr = 0;
}

// ---------------------------------------------------------------------------
// throw an exception
// call the user defined hook (if any)

void pm_throw_assert()
{	
	(*::pm_throw_assert_hook)();
}

// ---------------------------------------------------------------------------
pmuint16 pm_get_networku16(const pmbyte** aPtr)
{
	PM_ASSERT(aPtr != 0 && *aPtr != 0, TL("pm_getnetworku16(): Invalid parameter."));
	(*aPtr) += 2;
	return (*aPtr)[-1] + (((pmuint16)((*aPtr)[-2])) << 8);
}

// ---------------------------------------------------------------------------
pmuint32 pm_get_networku32(const pmbyte** aPtr)
{
	PM_ASSERT(aPtr != 0 && *aPtr != 0, TL("pm_getnetworku32(): Invalid parameter."));
	(*aPtr) += 4;
	return	(*aPtr)[-1] + (((pmuint32)((*aPtr)[-2])) << 8) + 
			(((pmuint32)((*aPtr)[-3])) << 16) + (((pmuint32)((*aPtr)[-4])) << 24);
}

// ---------------------------------------------------------------------------
void pm_set_networku16(pmbyte** aPtr, pmuint16 aValue)
{
	PM_ASSERT(aPtr != 0 && *aPtr != 0, TL("pm_setnetworku16(): Invalid parameter."));
	(*aPtr) += 2;
	(*aPtr)[-1] = (pmbyte)aValue;
	(*aPtr)[-2] = (pmbyte)(aValue >> 8);
}

// ---------------------------------------------------------------------------
void pm_set_networku32(pmbyte** aPtr, pmuint32 aValue)
{
	PM_ASSERT(aPtr != 0 && *aPtr != 0, TL("pm_setnetworku16(): Invalid parameter."));
	(*aPtr) += 4;
	(*aPtr)[-1] = (pmbyte)aValue;
	(*aPtr)[-2] = (pmbyte)(aValue >> 8);
	(*aPtr)[-3] = (pmbyte)(aValue >> 16);
	(*aPtr)[-4] = (pmbyte)(aValue >> 24);
}

// ---------------------------------------------------------------------------
//	DEFAULT PORTABLE IMPLEMENTATIONS
// ---------------------------------------------------------------------------
// string duplication, returns a newly allocated string (which is a copy of the
// original string), uses pm_malloc

char* pm_strdup_imp(const char* aCStr)
{	
	return ::pm_strcpy((char*)::pm_malloc(::pm_strlen(aCStr) + 1), aCStr);
}

// ---------------------------------------------------------------------------

int pm_strncasecmp_imp(const char *aString1, const char *aString2, size_t aMaxLen)
{
	#define LC(x) (x >= 'A' && x <= 'Z' ? x - 'A' + 'a' : x)

	while (LC(*aString1) == LC(*aString2) && aMaxLen > 0 && *aString1)
	{
 		aString1++, aString2++, aMaxLen--;
	}
	
	if (!aMaxLen)
		return 0;
		
	if (LC(*aString1) == LC(*aString2))
		return 0;
		
	if (LC(*aString1) < LC(*aString2))
		return -1;
		
	return 1;

	#undef LC
}

// ---------------------------------------------------------------------------

const char* pm_strstrncase_imp(const char *aString, const char *aSubString) 
{
	const char	*theStr, *thePtr;
	char		x, y;
 
	aString--;
	while (*++aString) 
	{
	 	if ((*aString == *aSubString) || ((*aString == (*aSubString ^ 0x20)) &&
	 		((*aString | 0x20) >= 'a') && ((*aString | 0x20) <= 'z')))
	 	{
	 	    theStr = aString;
	 	    thePtr = aSubString;
	 	    
	 	    while (((y = *++thePtr) != 0) && ((x = *++theStr) != 0))
	 	    {
			 	if (x == y)
			 		continue;
			 	
			 	if (x == (y ^ 0x20))
			 	{
					if ((x | 0x20) >= 'a' && (x | 0x20) <= 'z') 
			 			continue;
			 	}
				break;
			}
		 	    
		if (!*thePtr)
			return aString;
		}
	}

	return 0;
}

// ---------------------------------------------------------------------------

size_t pm_max_imp(size_t aValue1, size_t aValue2)
{	
	if (aValue1 > aValue2)
		return aValue1;
	return aValue2;
}

// ---------------------------------------------------------------------------

size_t pm_min_imp(size_t aValue1, size_t aValue2)
{	
	if (aValue1 < aValue2)
		return aValue1;
	return aValue2;
}

// ---------------------------------------------------------------------------
pmuint16 pm_network_to_hostu16_imp(pmuint16 aValue)
{
	pmbyte*	thePtr = (pmbyte*)&aValue;
	return (thePtr[0] << 8) + thePtr[1];
}

// ---------------------------------------------------------------------------
pmuint32 pm_network_to_hostu32_imp(pmuint32 aValue)
{
	pmbyte*	thePtr = (pmbyte*)&aValue;
	return (thePtr[0] << 24) + (thePtr[1] << 16) + (thePtr[2] << 8) + thePtr[3];
}

// ---------------------------------------------------------------------------
pmuint16 pm_host_to_networku16_imp(pmuint16 aValue)
{
	pmbyte*	thePtr = (pmbyte*)&aValue;
	return (thePtr[0] << 8) + thePtr[1];
}

// ---------------------------------------------------------------------------
pmuint32 pm_host_to_networku32_imp(pmuint32 aValue)
{
	pmbyte*	thePtr = (pmbyte*)&aValue;
	return (thePtr[0] << 24) + (thePtr[1] << 16) + (thePtr[2] << 8) + thePtr[3];
}
