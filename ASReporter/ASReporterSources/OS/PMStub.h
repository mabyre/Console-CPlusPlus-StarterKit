// ===========================================================================
//	PMStub.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================
/*
	Important note:
	--------------
	Many functions are just ANSI C redefinitions or very standard function
	redefinition. This is to provide platform independent source as some
	platform / compiler are not strictly ANSI compliant or do not provide
	some very used but non ANSI functions (strdup, strnicmp, ...)
	Taking the example of strlen, we defined a function pm_strlen that was
	just calling strlen. This was penalizing in term of CPU and stack.
	To remove this overhead, we now define pm_strlen as a macro to strlen.
	All those associations are done in PMDef.h
	PMStub.h / .cpp is providing implementations in standard C/C++ for 
	some standard functions. Those functions are named as their standard
	counter-part but postfixed with _imp to avoid name clashes.
	You will have to provide or change PMDef.h depending on your platform /
	compiler to map macros to the right thing: either to one of your compiler
	built-in method or to the standard implementation.
	Taking the example of pm_strncasecmp that compares n-th first characters
	of 2 strings case insensitive: Visual C++ provides _strnicmp that does
	exactly the same thing so in PMDef.h for VC++ you will find:
	#define pm_strncasecmp _strnicmp
	For other platform where such a method is not provided you will find:
	#define pm_strncasecmp pm_strncasecmp_imp
	where pm_strncasecmp_imp is the method to do this we provide in PMStub.cpp
	We provide in comments the prototypes of such macro-functions.
	After preprocessing the function must respect this prototype.
	Example: // void* pm_memcpy(void*, const void*, size_t)
	That means that your define of pm_memcpy (like #define pm_memcpy memcpy)
	must replace pm_memcpy with a function that has EXACTLY the same prototype.
	If not you may use casts for parameters like:
	If your memcpy is void* memcpy(void*, void*, size)
	#define pm_memcpy(d,s,t) memcpy(d, (void*)(s), t)
	But you cannot use cast to change the return value
	#define pm_memcpy (void*)memcpy (if your memcpy is returning char*) will
	not work because pm_memcpy may be used with ::pm_memcpy preprocessed to
	::(void*)memcpy that will not compile. In that case you must use a function (:-()
	inline void* pm_memcpy_imp(void* d, const void* s, size_t t)
	{
		return (void*)memcpy(d,s,t);
	}
	#define pm_memcpy pm_memcpy_imp
	If your compiler is not too bad and inline it you may not be too much penalized.

	All macro definitions should be in a file named PMDefs.h. 
	PMDefs.h is included by this file.
	You should take care that only one PMDefs.h can be found on your disk
	by your compiler for a given project. You must use the same PMDefs.h file
	for all the different libs used for a given project.
*/

#ifndef PM_INTERNAL_INCLUDE_ALLOWED
	#error You cannot include 'PMStub.h'. Include 'PMRC.h' instead.
#endif

// ---------------------------------------------------------------------------

#ifndef PMStub_h
#define PMStub_h

// ---------------------------------------------------------------------------

#include "PMTypes.h"

// ===========================================================================
//	Constants
// ===========================================================================

	/**	Value used for a non valid offset.	*/
const size_t pmnpos = (size_t)-1;

// ===========================================================================
//	For not used function parameters
// ===========================================================================

/**
	Use this macro when a formal parameter is not used in a function.
	It will prevent the compiler to issue a warning about not using the parameter.
	Example:
	void PMXTracer::SendChunk(PMDataRef PM_NOTUSED(aData)) 	{}
*/
#define PM_NOTUSED(x) 

// ===========================================================================
//	Make a pmuint32 given 2 pmuint16 (used for token like 'BINA' for win16)
// ===========================================================================

/**
	Makes a pmuint32 given 2 pmuint16. 
	Its prototype is:.
	pmuint32 PM_MAKEU32(pmuint16 aLow, pmuint16 aHigh);
*/
#define PM_MAKEU32(low, high)	((pmuint32)(((pmuint16)(low)) | ((pmuint32)((pmuint16)(high))) << 16))

// ===========================================================================
//	PM Initialization/Finalization
// ===========================================================================

	/**	Initialize PM. Returns false if failed.	*/
pmbool	pm_init(void *aParam = 0);

	/**	Finalize PM.	*/
void	pm_close(void *aParam = 0);

	/**	Initialization hook.	*/
extern pmbool	(*pm_init_hook)(void *aParam);

	/**	Finalization hook.	*/
extern void		(*pm_close_hook)(void *aParam);

// ===========================================================================
//	PM Memory management
// ===========================================================================

	/** Allocates memory. */
void*	pm_malloc_imp(size_t aSize);

	/** Reallocates previously allocated memory. */
void*	pm_realloc_imp(void *memblock, size_t aSize);

	/** Frees memory allocated using pm_malloc or pm_realloc. */
void	pm_free_imp(void* aPtr);

	/** 
	Hook used to allocate memory. Default \Ref{pm_malloc} implementation uses this hook. 
	Memory allocated by this hook is freed by the \Ref{pm_free_hook}.
	This hook mechanism lets you customize the way memory is managed. Default 
	implementation uses ANSI memory management.
	This hook should be implemented in the platform stub.	
	*/
extern void* (*pm_malloc_hook)(size_t aSize);

	/** 
	Hook used to free memory allocated by \Ref{pm_malloc_hook} or \Ref{pm_realloc_hook}.
	Default implementations of \Ref{pm_free} and \Ref{pm_forget} use this hook.
	This hook mechanism lets you customize the way memory is managed. Default 
	implementation uses ANSI memory management.
	This hook should be implemented in the platform stub.	
	*/
extern void (*pm_free_hook)(void *aPtr);

	/** 
	Hook used to reallocate memory allocated by \Ref{pm_malloc_hook} or by 
	Default implementations of \Ref{pm_realloc_hook} itself. 'pm_realloc' uses this hook.
	This hook mechanism lets you customize the way memory is managed. Default 
	implementation uses ANSI memory management.
	This hook should be implemented in the platform stub.	
	*/
extern void* (*pm_realloc_hook)(void *aPtr, size_t aSize);


	/** Helper function to deallocate *aPtr and set it to 0. It uses pm_free. */
void		pm_forget(void** aPtr);

// ===========================================================================
//	Busy hook. Can be used to throw exceptions.
// ===========================================================================

	/** 
	Hook called repeateadly when ObjectSet performs lengthy tasks. This hook can be 
	used to throw an exception to abort the current thread.
	The busy hook should be implemented in the platform stub.	
	*/
extern void (*pm_busy_hook)();

// ===========================================================================
//	Asserts and trace hooks
// ===========================================================================

	/** Called when an assertion is raised. In turn call \Ref{pm_throw_assert_hook}. */
void		pm_throw_assert();

	/** 
	Hook used to throw an exception when an assertion is raised. This mechanism lets
	you choose the exception class to throw. Default implementation is to throw a
	\Ref{PMAssertException}.
	\Ref{pm_throw_assert} uses this hook.
	*/
extern void (*pm_throw_assert_hook)();

// ===========================================================================
//	Miscelleanous routines NOT defined as macros.
// ===========================================================================
	/**	
	Gets a pmuint16 at the address contained in *aPtr assuming it is represented 
	in the network byte order (big-endian). *aPtr is incremented 16 bits.
	*/
pmuint16	pm_get_networku16(const pmbyte** aPtr);
	/**	
	Gets a pmuint32 at the address contained in *aPtr assuming it is represented 
	in the network byte order (big-endian). *aPtr is incremented 32 bits.
	*/
pmuint32	pm_get_networku32(const pmbyte** aPtr);
	/**
	Sets a pmuint16 at the address contained in *aPtr using the network byte order
	(big-endian). *aPtr is incremented 16 bits.
	*/
void		pm_set_networku16(pmbyte** aPtr, pmuint16 aValue);
	/**
	Sets a pmuint32 at the address contained in *aPtr using the network byte order
	(big-endian). *aPtr is incremented 32 bits.
	*/
void		pm_set_networku32(pmbyte** aPtr, pmuint32 aValue);

// ===========================================================================
//	Miscelleanous routines defined as macros (PMDefs.h)
//	See in PMDefs.h how they are mapped.
// ===========================================================================

//	All these functions must now been defined as macros in PMDefs.h
//	The functions these macros are defined to must have exactly the
//	prototype given below.
//
//		========================================
//		Functions with no default implementation:
//		========================================
//
//void*		pm_memcpy(void *aDest, const void *aSrc, size_t aCount);
//void*		pm_memmove(void *aDest, const void *aSrc, size_t aCount);
//void*		pm_memset(void *aDest, int v, size_t aCount);
//void*		pm_memchr(const void *aBuffer, int aChar, size_t aCount);

//char*		pm_strcpy(char *aString1, const char *aString2);
//int		pm_strcmp(const char *aString1, const char *aString2);
//int		pm_strncmp(const char *aString1, const char *aString2, size_t aCount);
//char*		pm_strchr(const char *aString, int aChar);
//char*		pm_strstr(const char *aString, const char *aSubString);
//size_t	pm_strlen(const char *aString);
//char*		pm_strstrncase(const char *aString, const char *aSubString);
//int		pm_strncasecmp(const char *aString1, const char *aString2, size_t aMaxLen = pmnpos);

//pmuint32	pm_atouint32(const char *aString);
//char*		pm_uint32toa(pmuint32 anInt, char *aString, size_t aLen);

//int		pm_toupper(int aChar);
//int		pm_tolower(int aChar);

//pmint32	pm_rand(pmint32 aMin, pmint32 aMax);

//	Returns the count of milliseconds elapsed from a fixed origin.
//	This period must not be a "by-process" period, but an absolute "system" period.
//pmuint32	pm_system_time();

//	Sleeps for aMsDelay milliseconds.
//void		pm_sleep(pmuint32 aMsDelay);

//		========================================
//		Functions with default implementation provided in PMStub.cpp:
//		========================================

/** 
	Duplicate the string pointed to by aPtr.
	Default implementation.
	Be careful that memory allocated by pm_strdup must be deallocated by
	pm_free. Remap pm_strdup macro to a native _strdup only if your are sure 
	that pm_free is mapped to the deallocating function needed by your native
	_strdup. 
*/
char*		pm_strdup_imp(const char *aPtr);

	/** Default implementation for pm_strstrncase. */
const char*	pm_strstrncase_imp(const char *aString, const char *aSubString);
	/** Default implementation for pm_strncasecmp. */
int			pm_strncasecmp_imp(const char *aString1, const char *aString2, size_t aMaxLen = pmnpos);

	/** Default implementation of pm_max. */
size_t		pm_max_imp(size_t aValue1, size_t aValue2);
	/** Default implementation of pm_min. */
size_t		pm_min_imp(size_t aValue1, size_t aValue2);

	/**	Transforms 'aValue' represented in network byte order to host byte order. */
pmuint16	pm_network_to_hostu16_imp(pmuint16 aValue);
	/**	Transforms 'aValue' represented in network byte order to host byte order. */
pmuint32	pm_network_to_hostu32_imp(pmuint32 aValue);
	/**	Transforms 'aValue' represented in host byte order to network byte order. */
pmuint16	pm_host_to_networku16_imp(pmuint16 aValue);
	/**	Transforms 'aValue' represented in host byte order to network byte order. */
pmuint32	pm_host_to_networku32_imp(pmuint32 aValue);

// ===========================================================================
//	Include the macro mapping file
// ===========================================================================

#include "PMDefs.h"

// ---------------------------------------------------------------------------

#ifndef PMNEW
	#define PMNEW new
#endif /* PMNEW */

// ---------------------------------------------------------------------------

#endif // PMStub_h
