// ===========================================================================
//	PMStream.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================
// Abstract Stream

#include "PMStream.h"
#include "PMStr.h"
#include "PMMemSt.h"

// ---------------------------------------------------------------------------

#define PM_TRACEMODULE STREAM
PM_DEFINE_TRACE_CODE(Info, 1);
PM_DEFINE_TRACE_CODE(Debug, 2);

// ===========================================================================
//	PMStream
// ===========================================================================

// ---------------------------------------------------------------------------
//	Constructor

PMStream::PMStream():
	itsError(PMStream_kNoError),
	itsFutureError(PMStream_kNoError)
{
	itsPushBack = new PMPushBack();
}

// ---------------------------------------------------------------------------
//	Destructor

PMStream::~PMStream()
{
}

// ---------------------------------------------------------------------------
//

pmbool PMStream::IsAtEOS()
{	
	return itsError == PMStream_kAtEOS;
}

// ---------------------------------------------------------------------------

pmbool PMStream::IsError()
{	
	return itsError != PMStream_kNoError;
}

// ---------------------------------------------------------------------------

PMStream_eError PMStream::ErrorCode()
{	
	return itsError;
}

// ---------------------------------------------------------------------------

void PMStream::SetAtEOS()
{	
	SetError(PMStream_kAtEOS);
}

// ---------------------------------------------------------------------------

void PMStream::SetError(PMStream_eError anError)
{	
	itsFutureError = anError;
}

// ---------------------------------------------------------------------------
//	Canonical read - Should be re-implemented by subclasses

pmbool PMStream::Read(size_t aCount, char *aBuffer, size_t *aReadCount)
{	
	PM_ASSERT(aCount, TL("PMStream::Read(): Cannot read 0 caracters"));
	PM_ASSERT(aBuffer, TL("PMStream::Read(): Cannot write to null pointer"));
	PM_ASSERT(aReadCount, TL("PMStream::Read(): Invalid parameter 3"));

		//	Checking for abort
	(*pm_busy_hook)();

	*aReadCount = 0;

	if (IsError())
		return pmfalse;

		//	Take pushbacked caracters first
	while (aCount && HasPushBack())
	{	
		size_t theCopyCount;

			//	Copy one pushbacked buffer
		itsPushBack->Read(aBuffer, aCount, &theCopyCount);
		aCount -= theCopyCount;
		*aReadCount += theCopyCount;
		aBuffer += theCopyCount;			
	}

		//	#### We may try a loop, here, as GNC cannot block any more
		//	We refuse to call GetNextChars if stream will be (ie:have been) in error
	if (!itsFutureError && aCount)
	{		//	Real read 
		size_t theReadCount = GetNextChars( aCount, aBuffer );
		*aReadCount += theReadCount;
		aCount -= theReadCount;
		aBuffer += theReadCount;

		if (*aReadCount==0)
		{		//	No caracters are avalaible, we propose to initiate a future read
				//	(To avoid busy looping, or too much read ahead)
			InitiateRead();
		}
	}

		//	Take push backed chars again if GetNextChars pushed some
	while (aCount && HasPushBack())
	{	
		size_t theCopyCount;

			//	Copy one pushbacked buffer
		itsPushBack->Read(aBuffer, aCount, &theCopyCount);
		aCount -= theCopyCount;
		*aReadCount += theCopyCount;
		aBuffer += theCopyCount;			
	}

		//	We promised to raise errors only when we read read zero caracters
	if (*aReadCount == 0)
		itsError = itsFutureError;

	return !IsError();
}

// ---------------------------------------------------------------------------
//	Non-canonical read methods

pmbool PMStream::BlockingRead(size_t aCount, char *aBuffer, size_t *aReadCount)
{	
	size_t theReadCount;
	size_t theDummyReadCount;

	if (!aReadCount)
		aReadCount = &theDummyReadCount;

	*aReadCount = 0;
	do
	{		//	Blocking read is guaranteed to return at least one char
		if (!Read(aCount, aBuffer, &theReadCount))
			return pmfalse;		//	Note that we can asume that the theReadCount is 0

		aCount -= theReadCount;
		aBuffer += theReadCount;
		*aReadCount += theReadCount;
	}	
	while (aCount);

	return pmtrue;
}

// ---------------------------------------------------------------------------
//	Exact read

pmbool PMStream::ExactRead(size_t aCount, char *aBuffer, pmbool *afTryAgain)
{	
	size_t	theReadCount;
	size_t 	theTotalReadCount = 0;
	char	*theBuffer = aBuffer;

	*afTryAgain = pmtrue;
	do
	{	
		if (!Read(aCount, theBuffer, &theReadCount))
		{	
			PushBack(theTotalReadCount, aBuffer);
			*afTryAgain = pmfalse;
			return pmfalse;
		}
		else
			if (theReadCount==0)
			{	
				PushBack(theTotalReadCount, aBuffer);
				*afTryAgain = pmfalse;
				return pmfalse;
			}
			else
			{	
				theBuffer += theReadCount;
				theTotalReadCount += theReadCount;
				aCount -= theReadCount;
			}
	}	
	while (aCount);

	return pmtrue;
}

// ---------------------------------------------------------------------------
//	If 'aCount' is not 0, this guarantees:
//		- next aCount chars returned by Read will be equal to content of aBuffer
//		- error is set to kNoError until aCount char will be read

void PMStream::PushBack(size_t aCount, const char *aBuffer)
{	
	if (!aCount)
		return;

	if (itsError)
	{		
			//	Pushing back clear the error flag
		itsFutureError = itsError;
		itsError = PMStream_kNoError;
	}

		//	Add this data to the push back buffer
	itsPushBack->AddBuffer(aBuffer, aCount, pmtrue);
}

// ---------------------------------------------------------------------------

void PMStream::InitiateRead()
{		
	//	Probably not a real problem
	//	PM_TRACE( Info, TL( "Initiate read on a stream where it has no meaning" ) );
}

// ---------------------------------------------------------------------------

pmbool PMStream::HasPushBack()
{	
	return itsPushBack->HasPushBack();
}

// ===========================================================================
//	PMEmptyStream
// ===========================================================================
// PMEmptyStream implementation

// ---------------------------------------------------------------------------

PMEmptyStream::PMEmptyStream()
{	
}

// ---------------------------------------------------------------------------

PMEmptyStream::~PMEmptyStream()
{	
}

// ---------------------------------------------------------------------------

size_t PMEmptyStream::GetNextChars(size_t ,char *)
{	
	SetAtEOS();
	return 0;
}

