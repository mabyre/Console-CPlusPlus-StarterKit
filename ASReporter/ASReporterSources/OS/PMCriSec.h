// ===========================================================================
//	PMCriSec.h
//	Copyright (C) 1996 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMCriSec_h
#define PMCriSec_h

//-----------------------------------------------------------------------
#include "PMRC.h"
#include "PMWinInc.h"
#ifndef PMFinalRelease
	#include "PMStr.h"
#endif

//-----------------------------------------------------------------------
/** 
	Provides a Critical Section. 
	A critical section is used to prevent two different threads from executing
	the same code at the same time. When a critical section is locked by 
	one thread and another calls the Lock method, the latter will be blocked until
	the former calls Unlock.
	This object is NOT reference-counted by design. PMCriticalSection instances
	cannot be dynamic. They should be either automatic or static.
	See: \Ref{PMCriticalSectionLocker} to learn how to use them easily.
*/
class PMCriticalSection
{
public:
		/** Constructor. */
	PMCriticalSection();

#ifndef PMFinalRelease
		/** Constructor. */
	PMCriticalSection(const char* aName);
#endif

		/** Destructor. */
	virtual ~PMCriticalSection();

		/** Locks this critical section. */
	void Lock();
		/** Unlocks this critical section. */
	void Unlock();

		/** 
		This operator new will assert when called. It prevents you
		from creating dynamic PMCriticalSection instances.
		*/
    void* operator new(size_t);

protected:
	pmbool		itsfInitialized;
#ifndef PMFinalRelease
	PMStrRef	itsName;
#endif

	CRITICAL_SECTION itsCritSect;

	static PMCriticalSection&	GlobalCriticalSection();

	/**	Initializes all critical sections except Global. */
	void Initialize();
	/**	Initializes all critical sections .*/
	void _Initialize();

private:
	PMCriticalSection(const PMCriticalSection&);
	PMCriticalSection& operator=(const PMCriticalSection&);
};

//-----------------------------------------------------------------------
#ifndef PMFinalRelease
	/** 
		Debug version: Allows the critical section to know its name and then to trace when it is locked and unlocked.
	*/
	#define PMCRITICALSECTION(x) PMCriticalSection x(#x)
	#define PMCRITICALSECTION_MEMBER(cls, member) member(#cls "::" #member)
#else
	/** 
		Release version: Does nothing else than declaring x.
	*/
	#define PMCRITICALSECTION(x) PMCriticalSection x
	#define PMCRITICALSECTION_MEMBER(cls, member) member()
#endif

//-----------------------------------------------------------------------
/**
	This class is intended to make it easy to use PMCriticalSection.
	PMCriticalSectionLocker constructor calls the Lock method of the 
	specified Critical Section and the destructor calls the Unlock method.
	This provides the feature of making your code exception-safe: if an exception
	is raised inside the protected block of code by this critical section,
	the locker will be destroyed (automatic object) and then the critical section will
	be unlocked letting other threads execute it.
*/
class PMCriticalSectionLocker
{
public:
		/** Constructor: Locks aCriticalSection. */
	PMCriticalSectionLocker(PMCriticalSection* aCriticalSection);
		/** Destructor: Unlocks its Critical Section. */
	virtual ~PMCriticalSectionLocker();

protected:
	PMCriticalSection*	itsCritSect;

private:
	PMCriticalSectionLocker();
	PMCriticalSectionLocker(const PMCriticalSectionLocker&);
	PMCriticalSectionLocker& operator=(const PMCriticalSectionLocker&);
};

//-----------------------------------------------------------------------
#endif // PMCriSec_h
