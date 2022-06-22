// ===========================================================================
//	PMRC.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMRC.h"
#include "PMArray.h"
#include "PMTrace.h"
#include "PMCriSec.h"

// ---------------------------------------------------------------------------

#define PM_TRACEMODULE RC

#ifdef PMF_RC_ALLOCATION_TRACE
	PM_DEFINE_TRACE_CODE(Allocation, 1);
#endif

#ifdef PMF_RC_INITCLOSE_TRACE
	PM_DEFINE_TRACE_CODE(Init, 2);
#endif

#ifdef PMF_RC_DESTRUCTION_TRACE
	PM_DEFINE_TRACE_CODE(VerboseDestruction, 3);
#endif

// ---------------------------------------------------------------------------
//	Static variables
// ---------------------------------------------------------------------------

static pmbool						gPMRCInited;	//	If pmtrue, PMRC is inited
static PMRef<PMArray<void(*)()> >	gCleanUp;		//	Array of clean-up functions
static PMRef<PMArray<PMRefAny*> >	gRefCleanUp;	//	Array of reference to clean-up

#if defined(PMF_RC_ALLOCATION_TRACE) || defined(PMF_RC_INITCLOSE_TRACE) || defined(PMF_RC_DESTRUCTION_TRACE)
	static pmuint32 gRecuringTraces;
#endif

// ===========================================================================
//	PMRC
// ===========================================================================

// ---------------------------------------------------------------------------
// Constructor

PMRC::PMRC()
{
#ifdef PMF_RC_INITCLOSE_TRACE
	if (!gPMRCInited)
		if (!gRecuringTraces)
		{	
			gRecuringTraces++;
			{	
				PM_TRACE(Init, TL("PMRC::PMRC(%p) Created object before _Init() or after _Close()", (void *)this));
			}
			gRecuringTraces--;
		}
#endif	// PMF_RC_INITCLOSE_TRACE

	itsRefCount = 0;
	::CreationObjectHook(this);

#ifdef PMF_RC_ALLOCATION_TRACE
	if (!gRecuringTraces)
	{	
		gRecuringTraces++;
		{	
			PM_TRACE(Allocation, TL("PMRC::PMRC(%p)", (void *)this));
		}
		gRecuringTraces--;
	}
#endif	// PMF_RC_ALLOCATION_TRACE
}

// ---------------------------------------------------------------------------
// Destructor

PMRC::~PMRC()
{
	PM_ASSERT(!itsRefCount, TL("Deleting an RC object with non-zero RefCount"));

#ifdef PMF_RC_INITCLOSE_TRACE
	if (!gPMRCInited)
		if (!gRecuringTraces)
		{	
			gRecuringTraces++;
			{	
				PM_TRACE(Init, TL("PMRC::~PMRC(%p) Deleted object before _Init() or after _Close()", (void *)this));
			}
			gRecuringTraces--;
		}
#endif	// PMF_RC_INITCLOSE_TRAC

	::DestructionObjectHook(this);

#ifdef PMF_RC_ALLOCATION_TRACE
	if (!gRecuringTraces)
	{	
		gRecuringTraces++;
		{	
			PM_TRACE(Allocation, TL("PMRC::~PMRC(%p)", (void *)this));
		}
		gRecuringTraces--;
	}
#endif // PMF_RC_ALLOCATION_TRACE
}

// ---------------------------------------------------------------------------
#ifndef PMFinalRelease
void PMRC::Retain() const
{
	PM_TS_INCREMENT(((PMRC*) this)->itsRefCount);
	PM_ASSERT(((PMRC*)this)->itsRefCount != -2, TL("PMRC::Retain(): Ref count overflow."));
}
#endif /* PMFinalRelease */

// ---------------------------------------------------------------------------

void PMRC::Release() const
{
	PMIN("PMRC::Release()");
	
	if (!PM_TS_DECREMENT(((PMRC *)this)->itsRefCount))
	{
#ifdef PMF_RC_DESTRUCTION_TRACE
		if (!gRecuringTraces)
		{	
			gRecuringTraces++;
			{	
			#ifndef PMF_NO_TYPEINFO
				PM_TRACE(VerboseDestruction, T("Releasing: %s, ", typeid(*this).name()));
			#endif
			}
			gRecuringTraces--;
		}
#endif // PMF_RC_DESTRUCTION_TRACE

		((PMRC *)this)->NotifyRefCountIsNull();
	}
	
	PMOUT;
}

// ---------------------------------------------------------------------------
//	Called at initialisation

void PMRC::_Init()
{	
	PM_ASSERT(!gPMRCInited, TL("PMRC::_Init() called twice!"));

	gPMRCInited = pmtrue;
	gCleanUp = new PMArray<void(*)()>;
	gRefCleanUp = new PMArray<PMRefAny*>;
}

// ---------------------------------------------------------------------------
//	Called at finalisation

void PMRC::_Close()
{	
	PM_ASSERT(gPMRCInited, TL("PMRC::_Close() called when not inited!"));

	gPMRCInited = pmfalse;
}

// ---------------------------------------------------------------------------
//	Register for cleanup

void PMRC::RegisterCleanUp(void (*aFunction)())
{	
	PMCriticalSectionLocker theLocker(PMRC::GetGlobalCriticalSection());
	gCleanUp->AppendObject(aFunction);
}

// ---------------------------------------------------------------------------
//	Register a reference for cleanup

void PMRC::RegisterRefCleanUp(PMRefAny *aRefPtr)
{	
	PMCriticalSectionLocker theLocker(PMRC::GetGlobalCriticalSection());
	gRefCleanUp->AppendObject(aRefPtr);
}

// ---------------------------------------------------------------------------
//	Called just before closing

void PMRC::_CleanUp()
{		
		//	If a cleanup throws an exception, we abort all the clean-ups
		//	(By design), but we free the clean-up 

	PMRef<PMArray<void(*)()> > theCleanUp = gCleanUp;
	PMRef<PMArray<PMRefAny*> > theRefCleanUp = gRefCleanUp;
	gRefCleanUp = 0;
	gCleanUp = 0;

	size_t i;
		//	First call cleanup functions
		//	Using (i+1) to end the loop when i reaches 0
	for (i = theCleanUp->GetCount(); i > 0 ; i--)
	{	
		void (*f)() = theCleanUp->At(i-1);
		(*f)();
	}
	
		//	then deallocate static ref
	for (i = theRefCleanUp->GetCount(); i > 0 ; i--)
	{	
		PMRefAny *theRefPtr = theRefCleanUp->At(i-1);
		(*theRefPtr) = 0;
	}
}

// ---------------------------------------------------------------------------
// Initialized before main starts.
static PMCriticalSection gGlobalCriticalSection;

PMCriticalSection* PMRC::GetGlobalCriticalSection()
{
	return &gGlobalCriticalSection;
}

// ===========================================================================
//	PMRefAny
// ===========================================================================

PMRefAny &PMRefAny::operator=(const PMRefAny &anotherObject)
{	
	if (&anotherObject != this)
	{	
		if (itsPointer != 0)
			itsPointer->Release();
		if ((itsPointer = anotherObject.itsPointer) != 0)
			itsPointer->Retain();
	}
	
	return *this;
}

// ---------------------------------------------------------------------------

void PMRefAny::NullPointerAccess() const
{
	PM_ASSERT_NOW(TL("PMRefAny: NULL pointer access"));
}

// ---------------------------------------------------------------------------

void PMRefAny::BadInitValue() const
{
	PM_ASSERT_NOW(TL("PMRefAny: Only 0 (zero) can be used to build a PMRef"));
}

// ===========================================================================

// ---------------------------------------------------------------------------
//	#### Should be a real hash

pmuint32 Hash(int anInt)
{	
	return anInt;
}

// ===========================================================================

// ---------------------------------------------------------------------------

int Compare(pmuint16 a, pmuint16 b)
{	
	if (a<b) return -1;
	if (a>b) return 1;
	return 0;
}

// ---------------------------------------------------------------------------

int Compare(pmint16 a, pmint16 b)
{	
	if (a<b) return -1;
	if (a>b) return 1;
	return 0;
}

// ---------------------------------------------------------------------------

int Compare(pmuint32 a, pmuint32 b)
{	
	if (a<b) return -1;
	if (a>b) return 1;
	return 0;
}

// ---------------------------------------------------------------------------

int Compare(pmint32 a, pmint32 b)
{	
	if (a<b) return -1;
	if (a>b) return 1;
	return 0;
}

// ---------------------------------------------------------------------------

void ThrowCompare()
{
	PM_ASSERT_NOW(TL("Template Compare called. This must never happen"));
}

