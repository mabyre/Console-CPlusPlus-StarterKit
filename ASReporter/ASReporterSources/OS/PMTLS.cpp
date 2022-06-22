// ===========================================================================
//	PMTLS.cpp
//	Copyright (C) 1996 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMTLS.h"

#include "PMCriSec.h"
#include "PMTrace.h"

#define PM_TRACEMODULE STRUCT

//-----------------------------------------------------------------------
static DWORD				gTLSId = 0xFFFFFFFF;
static pmuint32				gUniqueId = 1; // 0 means no id associated yet
static PMCriticalSection	gCritSect;

//-----------------------------------------------------------------------
pmbool PMTLS::Init(void*)
{
	if (gTLSId != 0xFFFFFFFF)
		return pmtrue;

	gTLSId = ::TlsAlloc();

	return gTLSId != 0xFFFFFFFF;
}

//-----------------------------------------------------------------------
void PMTLS::Close(void*)
{
	::TlsFree(gTLSId);
}

//-----------------------------------------------------------------------
pmuint32 PMTLS::GetUniqueID()
{
	PM_ASSERT(gTLSId != 0xFFFFFFFF, TL("TLS has not been inited"));

	pmuint32 theUniqueID = (pmuint32)::TlsGetValue(gTLSId);
	if (theUniqueID == 0)
	{
		if (::GetLastError() != NO_ERROR)
			PM_ASSERT_NOW(TL("PMTLS::GetUniqueID(): Error while accessing TLS"));
		
		{
			PMCriticalSectionLocker theLocker(&gCritSect);
			theUniqueID = gUniqueId++;
		}
		BOOL theResult = ::TlsSetValue(gTLSId, (void*)theUniqueID);
		PM_ASSERT(theResult, TL("Error while setting TLS"));
	}

	return theUniqueID;
}

/*
//-----------------------------------------------------------------------
PMTLS* PMTLS::Get()
{
	PM_ASSERT(gTLSId != 0xFFFFFFFF, TL("TLS has not been initialized"));

	PMTLS* theValue = (PMTLS*)::TlsGetValue(gTLSId);
	if (theValue == 0)
	{
		if (::GetLastError() != NO_ERROR)
			PM_ASSERT_NOW(TL("PMTLS::Get(): Error while accessing TLS"));
		
		theValue = new PMTLS;
		BOOL theResult = ::TlsSetValue(gTLSId, theValue);
		PM_ASSERT(theResult, TL("Error while setting TLS"));
		{
			PMCriticalSectionLocker theLocker(&gCritSect);
			theValue->itsUniqueID = gUniqueId++;
		}
	}

	return theValue;
}

//-----------------------------------------------------------------------
// This constructor is for initialization of default values of TLS
PMTLS::PMTLS()
{
	itsUniqueID = 0;
}
*/
