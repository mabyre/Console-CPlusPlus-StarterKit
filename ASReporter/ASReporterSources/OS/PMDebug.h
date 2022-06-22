// ===========================================================================
//	PMDebug.h
//	Copyright (C) 1996 Smartcode Software, Inc. All rights reserved.
// ===========================================================================
/*	Here are many macros useful for debugging.	*/

#ifndef PM_INTERNAL_INCLUDE_ALLOWED
	#error You cannot include PMDebug.h. Include PMRC.h instead.
#endif

// ----------------------------------------------------------------------------

#ifndef PMDebug_h
#define PMDebug_h

// ===========================================================================
//	Special macros to tailor PMRC for specific uses.
//	Generally used for porting and/or implementing ObjectSet-wide modifications
// ===========================================================================

	/**
	If PMF_RC_ALLOCATION_TRACE is defined, allocation/deallocation is traced.
	This is useful for validating new ports.
	*/
#define noPMF_RC_ALLOCATION_TRACE				
	
	/**
	If PMF_RC_INITCLOSE_TRACE is defined, ObjectSet asserts if an object 
	is created/destroyed after Close() has been called (or before Init()).
	*/
#define	noPMF_RC_INITCLOSE_TRACE					
										
	/**	If PMF_RC_DESTRUCTION_TRACE is defined, ObjectSet traces object 
	classes at destruction.
	*/
#define noPMF_RC_DESTRUCTION_TRACE		

	/**	If PMF_RC_DEBUG is defined, ObjectSet traces manipulation of every object
	(which means that there is a lot of output).
	*/
#define noPMF_RC_DEBUG

	/**	If PMF_RC_THROW is defined, an exception will be thrown after 
	exactly 'n' object manipulations.
	*/
#define noPMF_RC_THROW		

	/**
	If PMF_RC_REGISTER is defined, ObjectSet registers each creation/destruction 
	of reference-counted objects. On exit, display a summary. This is useful 
	for tracking memory leaks.
	*/
#define noPMF_RC_REGISTER

	/**
	If PMF_RC_VERBOSE is defined, ObjectSet displays (using stdin and stdout) 
	each entry and exit of functions where PMIN and PMOUT macros are used.
	*/
#define noPMF_RC_VERBOSE

	/**	If PMF_PMAPI is defined, PMAPI macros generate traces.	*/
#define noPMF_PMAPI

	/**	If PMF_RELEASE_ASSERT is defined, less code is generated 
	for managing assertions.
	*/
#define noPMF_RELEASE_ASSERT

	/**	If PMF_NO_TRACE is defined, no trace code is generated.	*/
#define noPMF_NO_TRACE

	/**	If PMF_NO_TYPEINFO is defined, no RTTI-related functions will be used.	*/
#define noPMF_NO_TYPEINFO

// ===========================================================================
//	PMFinalRelease
// ===========================================================================

#ifdef PMFinalRelease

	#define PMF_RELEASE_ASSERT
	#define PMF_NO_TRACE

#endif

// ===========================================================================
//	Debug strings
// ===========================================================================

// Default is: defined in debug, not defined in release.
#ifdef PMFinalRelease
	#undef PM_DEBUG_STR
#else
	#ifndef noPM_DEBUG_STR
		#define PM_DEBUG_STR
	#else
		#undef PM_DEBUG_STR
	#endif
#endif

// ===========================================================================
//	Includes
// ===========================================================================

#include "PMStub.h"

#ifndef PMF_NO_TYPEINFO
	#include <typeinfo.h>
#endif

// ===========================================================================
//	PMF_RC_DEBUG, PMF_RC_THROW
// ===========================================================================

	/**	Default: log nothing.	*/
#define PM_INTERNAL_LOG(S)

#ifdef PMF_RC_DEBUG
	#include <stdio.h>
		
	#undef PM_INTERNAL_LOG
	#ifdef PMF_NO_TYPEINFO
		#define PM_INTERNAL_LOG(S)	do { ::printf("| %s [%x]\n", S, (void*) this); fflush(stdout); } while(0)
	#else
		#define PM_INTERNAL_LOG(S)	do { ::printf("| %s [%x]\n", S, typeid(this).name()); fflush(stdout); } while(0)
	#endif

#endif

#ifdef PMF_RC_THROW
	extern pmuint32 _gThrowCount;
	
	#undef PM_INTERNAL_LOG
	#define PM_INTERNAL_LOG(S)  { if (--_gThrowCount == 0) throw("ABORT TEST"); } 
#endif

// ===========================================================================
//	PMF_RC_REGISTER
// ===========================================================================

class PMRC;

	/**
	These functions are called every time reference-counted objects are created
	or deleted.
	This mechanism is used when PMF_RC_REGISTER is defined to track 
	creation and destruction of objects.
	In addition, this is an open mechanism that lets hooks be defined for 
	subclasses; these hooks will be called each time an object of this class 
	is created or deleted.
	*/
void CreationObjectHook(PMRC* anObject);
void DestructionObjectHook(PMRC* anObject);

	/**	Compares two PMRC* objects; comparison is made on object addresses.	*/
int	Compare(PMRC *anObject1, PMRC *anObject2);

// ===========================================================================
//	PMF_RC_VERBOSE
// ===========================================================================

#ifdef PMF_RC_VERBOSE

	#include <stdio.h>
	
	#define PMIN(X)															\
		do																	\
		{																	\
			::printf("-->" << "[%ld]%s\n", (void *) this, X);				\
		}																	\
		while (0)
        
	#define PMOUT						\
		do								\
		{								\
			::printf("<--\n");			\
		}								\
		while (0)

#else	// PMF_RC_VERBOSE

	#define PMIN(X)
	#define PMOUT
	
#endif	// PMF_RC_VERBOSE

// ===========================================================================
//	PMCOUNTER
// ===========================================================================
/**
Used to count hits for a specific line of code without having to run the profiler.
This class is used via the macro 'PMCOUNTER'.
Typical usage of the macro:
if (aCondition)
{       
	PMCOUNTER( "Condition true" );
	...
}
*/

class PMCounter
{
public:
		/**	Internal Use. Constructor.	*/
	PMCounter(const char *aName, const char *aSource, int aSourcePos, pmuint32 *aCounter);
	
		/**	Internal Use. Destructor.	*/
	~PMCounter();

private:
	const char	*itsName;
	const char	*itsSource;
	int			itsSourcePos;
	pmuint32	*itsCounter;
};

// ----------------------------------------------------------------------------

#define PMCOUNTER(name)																		\
	do																						\
	{																						\
		static pmuint32		sCnt##__LINE__;													\
		static PMCounter	sCntObj##__LINE__(name, __FILE__, __LINE__, &sCnt##__LINE__);	\
		sCnt##__LINE__++;																	\
	}																						\
	while (0)

// ----------------------------------------------------------------------------

#endif  // PMDebug_h

