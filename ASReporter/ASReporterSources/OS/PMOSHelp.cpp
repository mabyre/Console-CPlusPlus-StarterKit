// ===========================================================================
//	PMOSHelp.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMOSHelp.h"
#include "PMTrace.h"
#include "PMMedia.h"
#include "PMANSIFM.h"
#include "PMCriSec.h"

// ---------------------------------------------------------------------------

#define PM_TRACEMODULE STRUCT
PM_DEFINE_TRACE_CODE(Info, 1);

// ===========================================================================
//	PMOSHelper
// ===========================================================================

static PMOSHelperRef	sHelper;
static pmbool			sfCleanUpRegistered;

// ---------------------------------------------------------------------------

PMOSHelperRef PMOSHelper::GetCurrent()
{
	if (sHelper.IsNull())
	{
		PMCriticalSectionLocker theLocker(PMRC::GetGlobalCriticalSection());
		if (sHelper.IsNull())
			SetCurrent(new PMOSHelper);
	}

	return sHelper;
}

// ---------------------------------------------------------------------------

PMOSHelperRef PMOSHelper::SetCurrent(const PMOSHelperRef& anHelper)
{
	PMCriticalSectionLocker theLocker(PMRC::GetGlobalCriticalSection());
	if (!sfCleanUpRegistered)
	{
		sfCleanUpRegistered = pmtrue;
		PMRC::RegisterRefCleanUp(&::sHelper);
	}

	PMOSHelperRef theOld = sHelper;
	sHelper = anHelper;

	return theOld;
}

// ---------------------------------------------------------------------------

PMStrRef PMOSHelper::CreateRandomFileName()
{
	PMStrRef theName = PMStr::NewRandomString("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 8);

	return ValidateFileName(theName);
}

// ---------------------------------------------------------------------------

void PMOSHelper::SetTemporaryMediaType(PMMediaTemporary_eType aType)
{
	PM_ASSERT(aType == PMMediaTemporary_kMemory || aType == PMMediaTemporary_kFile, TL("PMOSHelper::SetTemporaryMediaType(): Invalid argument"));
	
	if (aType == PMMediaTemporary_kMemory)
		PMMedia::gTemporaryMediaFactory = PMMedia::NewTemporaryMemoryMedia;
	else
		PM_ASSERT_NOW(TL("PMOSHelper::SetTemporaryMediaType: Unknown media type. Should be implemented by sublass."));
}

// ---------------------------------------------------------------------------

PMCharacterSet_eCode PMOSHelper::GetPlatformCharset()
{
	return PMCharacterSet_kISO_8859_1;
}

// ---------------------------------------------------------------------------

PMEndOfLine_eCode PMOSHelper::GetPlatformEndOfLine()
{
	return PMEndOfLine_kLF;
}

// ---------------------------------------------------------------------------

PMStrRef PMOSHelper::GetHostName()
{
	return PMStr::Empty();
}

// ---------------------------------------------------------------------------

size_t PMOSHelper::GetTemporaryBufferSize()
{
	return (size_t) 32767;
}

// ---------------------------------------------------------------------------

pmbool PMOSHelper::HasInfiniteMemory()
{
	return pmtrue;
}

// ---------------------------------------------------------------------------

PMStrRef PMOSHelper::GetErrorString(const PMError& /*anError*/)
{
	return PMStr::Empty();
}
