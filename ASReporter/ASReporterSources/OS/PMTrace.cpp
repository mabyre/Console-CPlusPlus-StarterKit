// ===========================================================================
//	PMTrace.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMTrace.h"
#include "PMStr.h"
#include "PMArray.h" 		// For new(void*, size_t)
#include "PMPrintf.h"

#define PM_TRACEMODULE STRUCT
PM_DEFINE_TRACE_CODE(Trace, 1);

// ---------------------------------------------------------------------------
// I dont want to have a static object here.
// It's because it may be used before being initialized

static char			cCurrentTracerObject[sizeof(PMTracerRef)];
PMTracerRef*		PMTracer::cCurrentTracer;
pmint32				PMTracer::cfActive;
PMCriticalSection	PMTracer::cCritSect;

pmuint32		PMTraceIndenter::cLevel;

// ===========================================================================
//	PMTraceSendParams
// ===========================================================================

// ---------------------------------------------------------------------------

PMTraceSendParams::PMTraceSendParams()
{
	itsKind = PMTrace_kKindEOT;
	itsIdent = 0;
	itsFile = 0;
	itsLine = 0;
	itsData = 0;
	itsDataLen = 0;
	itsIndentLevel = 0;
}

// ---------------------------------------------------------------------------

PMTraceSendParams::~PMTraceSendParams()
{
	::pm_free(itsData);
}

// ===========================================================================
//	PMTraceIndenter
// ===========================================================================

// ---------------------------------------------------------------------------

PMTraceIndenter::PMTraceIndenter()
{
	cLevel++;
}

// ---------------------------------------------------------------------------

PMTraceIndenter::~PMTraceIndenter()
{
	cLevel--;
}

// ---------------------------------------------------------------------------

pmuint32 PMTraceIndenter::GetLevel()
{
	return cLevel;
}

// ===========================================================================
//	PMNullTracer
// ===========================================================================
// A PMTracer subclass that does nothing

PMDEFINE (PMNullTracer);

class PMNullTracer : public PMTracer
{
public:
	PMNullTracer() {}
	virtual ~PMNullTracer() {}

protected:
	virtual PMTraceSendParams*	T(const char* , ...)			{ return 0; }
	virtual PMTraceSendParams*	TL(const char* , ...)			{ return 0; }
	virtual PMTraceSendParams*	B(const void* , size_t )		{ return 0; }

	virtual PMTraceSendParams*	VT(const char* , va_list )		{ return 0; }
	virtual PMTraceSendParams*	VTL(const char* , va_list )		{ return 0; }

	virtual void Send(PMTraceSendParams * )						{}

private:
	PMNullTracer(const PMNullTracer&);
	PMNullTracer& operator=(const PMNullTracer&);
};

// ===========================================================================
//	PMTracer
// ===========================================================================

// ---------------------------------------------------------------------------

PMTracer::PMTracer() : 
	PMRC()
{
}

// ---------------------------------------------------------------------------

PMTracer::~PMTracer()
{
}

// ---------------------------------------------------------------------------

const char *PMTracer::PlatformEOL()
{
	static char		sEOL[3];
	static pmbool	sEOLInited;
	
	if (!sEOLInited)
	{
		size_t	theSize = ::pm_min(PMStr::PlatformEOL()->size(), 2);
		::pm_memcpy(sEOL, PMStr::PlatformEOL()->c_str(), theSize + 1);
		sEOL[2] = 0;
		sEOLInited = pmtrue;
	}
	
	return sEOL;
}

// ---------------------------------------------------------------------------

void PMTracer::SetupTracer()
{
	if (cCurrentTracer == 0)
	{
		cCurrentTracer = (PMTracerRef*) cCurrentTracerObject;
		new (cCurrentTracer) PMRef<PMTracer>;
		
			//	We cannot delete this object, because a tracer must be available
			//	till the complete end of the application
			//	So a default tracer is always recreated when needed
	}
}

// ---------------------------------------------------------------------------

PMTracerRef PMTracer::GetTracer()
{
	SetupTracer();
	if (cCurrentTracer->IsNull())
		(*cCurrentTracer) = new PMNullTracer();

	return (*cCurrentTracer);
}

// ---------------------------------------------------------------------------

void PMTracer::SetTracer(const PMTracerRef &aTracer)
{
	SetupTracer();

		//	Don't remove next line. It keeps the old tracer in memory to avoid operator= to crash
	PMTracerRef theTracer = *cCurrentTracer; 
	(*cCurrentTracer) = aTracer;
}

// ---------------------------------------------------------------------------

pmbool PMTracer::SetActive(pmbool afActive)
{
	if (afActive)
		cfActive++;
	else
		cfActive--;

	return (cfActive > 0);
}

// ---------------------------------------------------------------------------

pmbool PMTracer::IsActive()
{
	return (cfActive > 0);
}

// ---------------------------------------------------------------------------

void PMTracer::DisplayAssert(const char* PM_NOTUSED(aTest), PMTraceSendParams* aParams)
{
	delete aParams;

	Assert();
}

// ---------------------------------------------------------------------------

void PMTracer::Assert()
{
	::pm_throw_assert();
}

// ---------------------------------------------------------------------------

PMTraceSendParams* PMTracer::T(const char* aFormat, ...)
{
	va_list theArgs;
	va_start(theArgs, aFormat);

	PMTraceSendParams* theParams = VT(aFormat, theArgs);

	va_end(theArgs);

	return theParams;
}

// ---------------------------------------------------------------------------

PMTraceSendParams* PMTracer::TL(const char* aFormat, ...)
{
	va_list theArgs;
	va_start(theArgs, aFormat);

	PMTraceSendParams* theParams = VTL(aFormat, theArgs);

	va_end(theArgs);

	return theParams;
}

// ---------------------------------------------------------------------------

PMTraceSendParams* PMTracer::B(const void* aBuffer, size_t aSize)
{
	PMTraceSendParams* theParams = new PMTraceSendParams;
	theParams->itsKind = PMTrace_kKindBinary;
	
	theParams->itsData = ::pm_malloc(aSize);
	::pm_memcpy(theParams->itsData, aBuffer, aSize);
	theParams->itsDataLen = aSize;

	return theParams;
}

// ---------------------------------------------------------------------------

PMTraceSendParams* PMTracer::VT(const char* aFormat, va_list anArgs)
{
	PMTraceSendParams* theParams = new PMTraceSendParams;
	theParams->itsKind = PMTrace_kKindNormal;
	
	// The leading 0 belongs to the trace data
	char* theData = (char*)::pm_vxsprintf(aFormat, anArgs)->TakeOwnership();
	theParams->itsData = theData;
	theParams->itsDataLen = ::pm_strlen(theData) + 1;

	return theParams;
}

// ---------------------------------------------------------------------------

PMTraceSendParams* PMTracer::VTL(const char* aFormat, va_list anArgs)
{
	PMTraceSendParams* theParams = new PMTraceSendParams;
	theParams->itsKind = PMTrace_kKindEOT;

	// The leading 0 belongs to the trace data
	char* theData = (char*)::pm_vxsprintf(aFormat, anArgs)->TakeOwnership();
	theParams->itsData = theData;
	theParams->itsDataLen = ::pm_strlen(theData) + 1;

	return theParams;
}

// ===========================================================================
//	PMTraceActiver
// ===========================================================================

// ---------------------------------------------------------------------------

PMTraceActiver::PMTraceActiver(pmbool afActive) :
	itsfActive(afActive)
{
	PMTracer::SetActive(afActive);
}

// ---------------------------------------------------------------------------

PMTraceActiver::~PMTraceActiver()
{
	PMTracer::SetActive(!itsfActive);
}
