// ===========================================================================
//	PMDebug.cpp
//	Copyright (C) 1996 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#define PM_INTERNAL_INCLUDE_ALLOWED
	#include "PMDebug.h"
#undef PM_INTERNAL_INCLUDE_ALLOWED

#include "PMHTbl.h"

// ----------------------------------------------------------------------------

#define PM_TRACEMODULE STATS
PM_DEFINE_TRACE_CODE(Info, 1);

// ----------------------------------------------------------------------------

#ifdef PMF_RC_THROW		
		// This counter is used in exception debug
		/**	Internal use.	*/
	pmuint32 _gThrowCount;					
#endif

// ===========================================================================
//	PMF_RC_REGISTER
// ===========================================================================

#ifndef PMF_RC_REGISTER

void CreationObjectHook(PMRC* )	{}
void DestructionObjectHook(PMRC* )	{}

#else // PMF_RC_REGISTER

// ----------------------------------------------------------------------------
// 	This object instantiated only one time as a global variable
//	is used to compute ref counted objects statistics

class PMWatcher
{	
public:
	PMWatcher();
	~PMWatcher();
};

// ----------------------------------------------------------------------------

static size_t		sWatcherCount = 0;
static PMWatcher	sWatcher;

// ----------------------------------------------------------------------------

static void ProcBeforeRCCreation();
static void ProcAfterRCDestruction();

// ----------------------------------------------------------------------------

PMWatcher::PMWatcher()
{	
	if (!sWatcherCount)
		::ProcBeforeRCCreation();
	sWatcherCount++;
}

// ----------------------------------------------------------------------------

PMWatcher::~PMWatcher()
{	
	sWatcherCount--;
	if (!sWatcherCount)
		::ProcAfterRCDestruction();
}

// ----------------------------------------------------------------------------

static pmint32	sCount;			// Actual count of objects
static pmint32	sMaxCount;		// Maximal count of objects
static pmint32	sTotalCount;	// Total of allocations

static pmbool	sfBusy;			// pmtrue if Counter is busy counting

static PMMap<PMRC *, PMRC *> *sMap = 0;

void Add(PMRC * );
void Sub(PMRC * );

// ----------------------------------------------------------------------------

void CreationObjectHook(PMRC* anObject)
{	
	::Add(anObject);
}

// ----------------------------------------------------------------------------

void DestructionObjectHook(PMRC* anObject)
{	
	::Sub(anObject);
}

// ----------------------------------------------------------------------------
//	#### This compare is not signed

int Compare(PMRC *anObject1, PMRC *anObject2)
{	
	return (void *) anObject1 != (void *) anObject2;
}

// ----------------------------------------------------------------------------

static pmuint32 Hash(PMRC *anObject)
{	
	return (pmuint32) anObject;
}

// ----------------------------------------------------------------------------

void Add(PMRC * anObject)
{
	// #### Beware, not multi-threaded...
	if (sfBusy) return;              

//	PMIN("PMRCCounter::Add(...)");

	sTotalCount++;
	sCount++;
	if (sCount>sMaxCount) sMaxCount=sCount;

	sfBusy = pmtrue;
	if (!sMap)
	{	
			// #### The map is definitely leaked
		sMap = new PMMap<PMRC *,PMRC *>;
//			sMap->Retain();
	}
	sMap->SetObjectForKey(anObject, anObject);
	sfBusy = pmfalse;

//	PMOUT;
}

// ----------------------------------------------------------------------------

void Sub(PMRC * anObject)
{
		// #### Beware, not multi-threaded...
	if (sfBusy)
		return;              

	sCount--;
        
	sfBusy = pmtrue;
	sMap->RemoveKey(anObject);
	sfBusy = pmfalse;
}

// ----------------------------------------------------------------------------

static void ProcBeforeRCCreation()
{	
	sfBusy = pmfalse;
	sCount = sMaxCount = sTotalCount = 0;
}

// ----------------------------------------------------------------------------
// Dump ref count statistics

static void ProcAfterRCDestruction()
{	
	if (sMap != 0)
	{	
		sfBusy = pmtrue;
		PM_TRACE(Info, TL("--------------- RC Object Statistics --------------"));
		PM_TRACE(Info, TL("Number of RC objects remaining in application:%ld", (long) sCount));
		PM_TRACE(Info, TL("  Maximal number of objects allocated at once:%ld", (long) sMaxCount));
		PM_TRACE(Info, TL(" Total number of objects allocated during run:%ld", (long) sTotalCount));
		PM_TRACE(Info, TL("---------------------------------------------------"));

		PMRef<PMArray<PMRC *> > theKeys = sMap->GetKeyArray();
		PM_TRACE(Info, TL("-------------- RC Hash Table content --------------"));
		for (size_t i = 0 ; i != theKeys->GetCount() ; i++)
		{	
			PMRC *theObject = theKeys->At(i);
			
			#ifndef PMF_NO_TYPEINFO
				PM_TRACE(Info, TL("%p [%s]", (void *) theObject, typeid(*theObject).name()));
			#else
				PM_TRACE(Info, TL("%p", (void *) theObject));
			#endif
		}
		
		PM_TRACE(Info, TL("---------------------------------------------------"));

		delete sMap;
	}
}

#endif // PMF_RC_REGISTER


// ===========================================================================
//	PMCounter
// ===========================================================================

#undef PM_TRACEMODULE
#define PM_TRACEMODULE PROFILE
PM_DEFINE_TRACE_CODE(Info, 1);

// ----------------------------------------------------------------------------

PMCounter::PMCounter(const char *aName, const char *aSource, int aSourcePos, pmuint32 *aCounter)
{	
	itsName = aName;
	itsSource = aSource;
	itsSourcePos = aSourcePos;
	itsCounter = aCounter;
}

// ----------------------------------------------------------------------------

PMCounter::~PMCounter()
{	
	//	We cannot emit trace there because this will generate traces after pm_close has been 
	//	called. It is rather hard to work around this
		
	//	PM_TRACE(Counter, TL("COUNTER %s (%s:%ld) HITS:%ld", itsName, itsSource, (long)itsSourcePos, (long)*itsCounter));
}
