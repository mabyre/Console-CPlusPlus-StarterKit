// ===========================================================================
//	PMDefVC4.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

/*
	This file provides mapping of various macros to the functions they
	must implement.
	Example the pm_strlen macro must be defined to the function that provide
	the functionnality of returning the length of a string.
	This function must have exactly the same prototype as defined in PMStub.h
	the line above the macro.
	If such a function does not exist natively of your system, your may define
	your macro to one of the standard function provided in PMStub.cpp, or write
	the function on your own and define your macro to this function. In that case
	a privilegied name for the function should be pm_strlen_imp for pm_strlen.
*/

// ---------------------------------------------------------------------------

#ifndef PMDefVC4_h
#define PMDefVC4_h

#ifndef PM_INTERNAL_INCLUDE_ALLOWED
	#error You cannot include 'PMDefVC4.h'. Include 'PMRC.h' instead.
#endif

// ---------------------------------------------------------------------------

#if !defined(_MSC_VER) || _MSC_VER < 1000
	// If you got this error and are using another compiler than VC++ 4.0 +
	// it is because these macros were checked only for this compiler.
	// You should write your own PMDefXXX.h using the information located in
	// PMStub.h and on top of this file and modify PMDefs.h to recognize your
	// compiler.
	#error This PMDefs.h is valid only for Microsoft Visual C++ 4.0 or later.
#endif

// ---------------------------------------------------------------------------
	/*	Disable warning about long template names being truncated	*/
#pragma warning(disable:4786)
	/*	Disable 'loop constant' warning	*/
#pragma warning(disable:4127)         
	/* 'void *__cdecl operator new(...)' : no matching operator delete found; memory will not be freed if initialization throws an exception */
#pragma warning(disable:4291)
// ---------------------------------------------------------------------------

// Including PMWinInc.h here will cause a compile-time bloat. So we just
// provide prototype for what is necessary.
//#include "PMWinInc.h"
extern "C" {
__declspec(dllimport) void		__stdcall Sleep(pmuint32);
__declspec(dllimport) pmuint32	__stdcall GetTickCount();
}

// ---------------------------------------------------------------------------
//	FUNCTIONS AND TYPES FOR MULTITHREADED REFERENCE COUNTING
// ---------------------------------------------------------------------------

typedef long pm_refcount;

#ifdef _MT
	#define PM_MT	1
#endif // _MT

#ifdef PM_MT
	#ifdef PMFinalRelease
	#include "PMWinInc.h"
	#endif

	#define PM_TS_INCREMENT(x) InterlockedIncrement(&(x))
	#define PM_TS_DECREMENT(x) InterlockedDecrement(&(x))
#else
	#define PM_TS_INCREMENT(x) (++(x))
	#define PM_TS_DECREMENT(x) (--(x))
#endif // PM_MT

// ---------------------------------------------------------------------------
#include <string.h>
#include <ctype.h>

// ---------------------------------------------------------------------------
//	FUNCTIONS DEFINED TO COMPILER OR OS PROVIDED FUNCTIONS
// ---------------------------------------------------------------------------

//void*		pm_memcpy(void *aDest, const void *aSrc, size_t aCount);
/**	Directly maps to ANSI memcpy.	*/
#define		pm_memcpy	memcpy

//void*		pm_memmove(void *aDest, const void *aSrc, size_t aCount);
/**	Directly maps to ANSI memmove.	*/
#define		pm_memmove	memmove

//void*		pm_memset(void *aDest, int v, size_t aCount);
/**	Directly maps to ANSI memset.	*/
#define		pm_memset	memset

//void*		pm_memchr(const void *aBuffer, int aChar, size_t aCount);
/**	Directly maps to ANSI memchr.	*/
#define		pm_memchr	memchr

//char*		pm_strcpy(char *aString1, const char *aString2);
/**	Directly maps to ANSI strcpy.	*/
#define		pm_strcpy	strcpy

//int		pm_strcmp(const char *aString1, const char *aString2);
/**	Directly maps to ANSI strcmp.	*/
#define		pm_strcmp	strcmp

//int		pm_strncmp(const char *aString1, const char *aString2, size_t aCount);
/**	Directly maps to ANSI strncmp.	*/
#define		pm_strncmp	strncmp

//char*		pm_strchr(const char *aString, int aChar);
/**	Directly maps to ANSI strchr.	*/
#define		pm_strchr	strchr

//char*		pm_strstr(const char *aString, const char *aSubString);
/**	Directly maps to ANSI strstr.	*/
#define		pm_strstr	strstr

//size_t	pm_strlen(const char *aString);
/**	Directly maps to ANSI strlen.	*/
#define		pm_strlen	strlen

//int		pm_strncasecmp(const char *aString1, const char *aString2, size_t aMaxLen = pmnpos);
/**	
	The pm_strncasecmp function lexicographically compares, at most, the first aMaxLen 
	characters of aString1 and aString2. The comparison is performed without regard 
	to case; pm_strncasecmp is a case-insensitive version of pm_strncmp. The comparison ends 
	if a terminating null character is reached in either string before aMaxLen 
	characters are compared. If the strings are equal when a terminating null character 
	is reached in either string before count characters are compared, the shorter 
	string is lesser.
	Using VC++ it maps to _strnicmp.
	Its prototype is:.
	int		pm_strncasecmp(const char *aString1, const char *aString2, size_t aMaxLen);
*/
#define		pm_strncasecmp	_strnicmp

//int		pm_toupper(int aChar);
/**	
	Directly maps to ANSI toupper. 
	If the compiler reports an error when using this function it may be because you use
	the global scope operator with it like this: ::pm_toupper. On some compilers toupper is 
	a macro	that generates this error. In that case, simply remove the ::.
*/
#define		pm_toupper		toupper

//int		pm_tolower(int aChar);
/**	
	Directly maps to ANSI tolower. 
	If the compiler reports an error when using this function it may be because you use
	the global scope operator with it like this: ::pm_toupper. On some compilers toupper is 
	a macro	that generates this error. In that case, simply remove the ::.
*/
#define		pm_tolower		tolower

/**
	Returns the count of milliseconds elapsed from a fixed origin.
	This period must not be a "by-process" period, but an absolute "system" period.
*/
#define		pm_system_time	GetTickCount

	/**	Sleeps for aMsDelay milliseconds.	*/
#define		pm_sleep		Sleep

// ---------------------------------------------------------------------------
//	FUNCTIONS DEFINED TO STANDARD PMSTUB IMPLEMENTED FUNCTIONS
// ---------------------------------------------------------------------------
// We cannot use _strdup as pmfree is using pm_free_hook.
// As long as pm_free_hook is calling free we would have no problem with _strdup
// But is pm_free_hook is changed, then a string allocated with malloc inside
// _strdup would have been releases with something else. :-(
//char*		pm_strdup(const char *aPtr);
/**
	Duplicates a C-String using pm_malloc to allocate memory.
	You must call pm_free on the returned char* or pass it to a PMBuffer that will
	handle the memory deallocation. 
	Its prototype is:.
	char*		pm_strdup(const char *aPtr);
	See also: \Ref{pm_strdup_imp}.
*/
#define		pm_strdup	pm_strdup_imp

//char*		pm_strstrncase(const char *aString, const char *aSubString);
/**
	This method is the same as ANSI strstr except that the comparison is performed 
	without regard to case.
	Its prototype is:.
	char*		pm_strstrncase(const char *aString, const char *aSubString);
	See also: \Ref{pm_strstrncase_imp}.
*/
#define		pm_strstrncase	pm_strstrncase_imp

//size_t	pm_max_imp(size_t aValue1, size_t aValue2);
/**
	Returns the max between two size_t.
	Its prototype is:.
	size_t	pm_max_imp(size_t aValue1, size_t aValue2);
	See also: \Ref{pm_max_imp}.
*/
#define		pm_max			pm_max_imp

//size_t	pm_min_imp(size_t aValue1, size_t aValue2);
/**
	Returns the min between two size_t.
	Its prototype is:.
	size_t	pm_min_imp(size_t aValue1, size_t aValue2);
	See also: \Ref{pm_min_imp}.
*/
#define		pm_min			pm_min_imp

//pmuint16	pm_network_to_hostu16_imp(pmuint16 aValue);
/** Macro mapping to \Ref{pm_network_to_hostu16_imp}. */
#define		pm_network_to_hostu16	pm_network_to_hostu16_imp

//pmuint32	pm_network_to_hostu32_imp(pmuint32 aValue);
/** Macro mapping to \Ref{pm_network_to_hostu32_imp}. */
#define		pm_network_to_hostu32	pm_network_to_hostu32_imp

//pmuint16	pm_host_to_networku16_imp(pmuint16 aValue);
/** Macro mapping to \Ref{pm_host_to_networku16_imp}. */
#define		pm_host_to_networku16	pm_host_to_networku16_imp

//pmuint32	pm_host_to_networku32_imp(pmuint32 aValue);
/** Macro mapping to \Ref{pm_host_to_networku32_imp}. */
#define		pm_host_to_networku32	pm_host_to_networku32_imp

// ---------------------------------------------------------------------------
//	FUNCTIONS DEFINED TO SPECIFIC IMPLEMENTED FUNCTIONS
// ---------------------------------------------------------------------------

// Include protoypes of other implementations
#include "PMSVC4.h"

//pmuint32	pm_atouint32(const char *aString);
/**
	Converts aString to pmuint32. 
	Returns 0 if the string is not containing digits.
	In the string begins with digits and then contains alphabetic values it
	returns the conversion of the leading digits.
	Its prototype is:.
	pmuint32	pm_atouint32(const char *aString);
*/
#define		pm_atouint32	pm_atouint32_imp

//pm_uint32toa_imp(pmuint32 anUInt32, char* aString, size_t aLen)
/**
	Converts anInt to aString. 
	aLen is the total size of buffer aString. It will assert if the 
	provided buffer is not large enough to handle the converted string.
	Its prototype is:.
	pmuint32	pm_uint32toa_imp(pmuint32 anUInt32, char* aString, size_t aLen);
*/
#define		pm_uint32toa	pm_uint32toa_imp

//pmint32	pm_rand(pmint32 aMin, pmint32 aMax);
/**
	Returns a "random" number between aMin and aMax, both included.
	It uses the ANSI rand function so the same limitations and conditions
	apply when using it. (You must initialize it before using it. Check your
	compiler documentation about this.)
	Take care that it uses signed ints.
	Its prototype is:.
	pmint32	pm_rand(pmint32 aMin, pmint32 aMax);
*/
#define		pm_rand			pm_rand_imp

//void*	pm_malloc(size_t aSize);
/**
	Allocates a memory block of size aSize.
	You can change the pm_malloc_hook function pointer to change the default
	allocator.
	Its prototype is:.
	void*	pm_malloc(size_t aSize);
	See also: \Ref{pm_malloc_hook} in this doc and in the overview in Word format.
*/
#define		pm_malloc		pm_malloc_hook

//void*	pm_realloc(void *memblock, size_t aSize);
/**
	Reallocates an existing memory block allocated with \Ref{pm_malloc} to grow or 
	shrink it to a new size aSize.
	You can change the pm_realloc_hook function pointer to change the default
	allocator.
	Its prototype is:.
	void*	pm_realloc(void *aBlock, size_t aSize);
	See also: \Ref{pm_malloc_hook} in this doc and in the overview in Word format.
*/
#define		pm_realloc		pm_realloc_hook

//void	pm_free(void* aPtr);
/**
	Frees a memory block allocated or reallocated with \Ref{pm_malloc} or \Ref{pm_realloc}.
	You can change the pm_free_hook function pointer to change the default
	allocator.
	Its prototype is:.
	void	pm_free(void* aPtr);
	See also: \Ref{pm_malloc_hook} in this doc and in the overview in Word format.
*/
#define		pm_free			pm_free_hook

// ---------------------------------------------------------------------------

#ifndef PMFinalRelease
	#ifndef PM_INTERNAL_INCLUDE_ALLOWED
		#define PM_INTERNAL_INCLUDE_ALLOWED
			#include "PMMemVC4.h"
		#undef PM_INTERNAL_INCLUDE_ALLOWED
	#else
		#include "PMMemVC4.h"
	#endif
	#define PMNEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define PMNEW new
#endif // PMFinalRelease

// ---------------------------------------------------------------------------

#endif // PMDefVC4_h
