// ===========================================================================
//	PMCriSec.cpp
//	Copyright (C) 1996 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMCriSec.h"

#include "PMTrace.h"
#ifndef PMFinalRelease
	#include "PMTLS.h"
#endif

//-----------------------------------------------------------------------
#define PM_TRACEMODULE STRUCT
PM_DEFINE_TRACE_CODE(Info, 1);

//-----------------------------------------------------------------------
PMCriticalSection::PMCriticalSection() :
	itsfInitialized(pmfalse)
{
}

#ifndef PMFinalRelease
//-----------------------------------------------------------------------
PMCriticalSection::PMCriticalSection(const char* aName) :
	itsfInitialized(pmfalse)
{
	if (aName != 0)
		itsName = PMStr::NewFromCString(aName, PMStr_kRef, PMStr_kUnknown);
}
#endif // PMFinalRelease

//-----------------------------------------------------------------------
PMCriticalSection::~PMCriticalSection()
{	
	if (itsfInitialized)
		::DeleteCriticalSection(&itsCritSect);
}

//-----------------------------------------------------------------------
void PMCriticalSection::Initialize()
{
	GlobalCriticalSection().Lock();
	_Initialize();
	GlobalCriticalSection().Unlock();
}

//-----------------------------------------------------------------------
void PMCriticalSection::_Initialize()
{
	if (!itsfInitialized)
	{	
		::InitializeCriticalSection(&itsCritSect);
		itsfInitialized = pmtrue;
	}
}

//-----------------------------------------------------------------------
void PMCriticalSection::Lock()
{	
	if (!itsfInitialized)
		Initialize();

#ifndef PMFinalRelease
	if (!itsName.IsNull())
		// Which one is better to use ? PMTLS::GetUniqueID or ::GetCurrentThreadId ? 
		PM_TRACE(Info, TL("PMCriticalSection::Lock(): Thread [%lx] trying to lock [%s]", ::GetCurrentThreadId(), itsName->c_str()));
#endif

	::EnterCriticalSection(&itsCritSect);

#ifndef PMFinalRelease
	if (!itsName.IsNull())
		PM_TRACE(Info, TL("PMCriticalSection::Lock(): [%s] locked by thread [%lx]", itsName->c_str(), ::GetCurrentThreadId()));
#endif
}

//-----------------------------------------------------------------------
void PMCriticalSection::Unlock()
{	
#ifndef PMFinalRelease
	if (!itsName.IsNull())
		PM_TRACE(Info, TL("PMCriticalSection::Unlock(): [%s] unlocked by thread [%lx]", itsName->c_str(), ::GetCurrentThreadId()));
#endif
	::LeaveCriticalSection(&itsCritSect);
}

//-----------------------------------------------------------------------
//	This create (eventually) and returns the global mutex
PMCriticalSection& PMCriticalSection::GlobalCriticalSection()
{		//	First time, global mutex is not inited
	static pmbool sfInited = pmfalse;
	static char sBuffer[ sizeof PMCriticalSection ];
	static PMCriticalSection *gCritSect = (PMCriticalSection *)sBuffer;
	
	if (!sfInited)
	{	//	The global mutex should be the only one unsecurely created
		gCritSect->PMCriticalSection::PMCriticalSection();
		gCritSect->_Initialize();
		sfInited = pmtrue;
	}

	return *gCritSect;
}

//-----------------------------------------------------------------------
void* PMCriticalSection::operator new(size_t)
{
	PM_ASSERT_NOW(TL("PMCriticalSection::operator new(): Cannot create a Critical section dynamically."));
	return 0;
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PMCriticalSectionLocker::PMCriticalSectionLocker(PMCriticalSection* aCriticalSection) :
	itsCritSect(aCriticalSection)
{
	PM_ASSERT(itsCritSect != 0, TL("PMCriticalSectionLocker::PMCriticalSectionLocker(): Null CriticalSection"));
	itsCritSect->Lock();
}

//-----------------------------------------------------------------------
PMCriticalSectionLocker::~PMCriticalSectionLocker()
{
	itsCritSect->Unlock();
}
