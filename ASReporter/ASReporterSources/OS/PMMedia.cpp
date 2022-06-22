// ===========================================================================
//	PMMedia.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMMedia.h"
#include "PMBuffer.h"
#include "PMOSHelp.h"

// ---------------------------------------------------------------------------

#define PM_TRACEMODULE MEDIA
PM_DEFINE_TRACE_CODE(Info, 1);

// ===========================================================================
//	PMMedia
// ===========================================================================

// ---------------------------------------------------------------------------

void PMMedia::SetTemporary(pmbool afTemporary)
{	
	itsfTemporary = afTemporary;
}

// ---------------------------------------------------------------------------

pmbool PMMedia::IsTemporary()
{	
	return itsfTemporary;
}

// ---------------------------------------------------------------------------

pmuint32 PMMedia::GetSize()
{	
	return PMMediaUnknownSize;
}

// ---------------------------------------------------------------------------

PMStreamRef PMMedia::GetStream()
{
	return new PMMediaStream(this);
};

// ---------------------------------------------------------------------------

PMMedia_eState PMMedia::GetState()
{	
	return itsState;
}

// ---------------------------------------------------------------------------

void PMMedia::SetState(PMMedia_eState aState)
{	
	itsState = aState;
}

// ---------------------------------------------------------------------------

void PMMedia::StreamAdded()
{	
	if (!itsNumberOfStreams++)
		_OpenMedia();
}

// ---------------------------------------------------------------------------

void PMMedia::StreamRemoved()
{	
	if (!--itsNumberOfStreams)
		_CloseMedia();
}

// ---------------------------------------------------------------------------

pmbool PMMedia::PrepareFill(PMStreamRef aStream)
{	
	if (GetState() == PMMedia_kIdle)
	{
		PM_ASSERT(itsInputStream.IsNull(), TL("PMMedia::PrepareFill(): The media is still filling something and this method is not reentrant."));
		itsInputStream = aStream;
		itsWritePos = 0;
		if (DoPrepareFill())
		{	
			SetState(PMMedia_kFilling);
			return pmtrue;
		}
	}
	return pmfalse;
}

// ---------------------------------------------------------------------------

pmbool PMMedia::ContinueFill()
{	
	size_t		theBufferSize = PMOSHelper::GetCurrent()->GetTemporaryBufferSize();
	PMBuffer	theBuffer(theBufferSize);
	size_t		theReadCount;
	
	while (1)
	{	
		if (!itsInputStream->Read(theBufferSize, (char*)theBuffer.buffer(), &theReadCount))
		{		
				//	End of read
			if (!itsInputStream->IsAtEOS() && (itsInputStream->ErrorCode() != PMStream_kSolveFailed))
			{
					// #### No, SolveFailed should generate an error but this will stop the mime parsing
				PM_TRACE(Info, TL("PMMedia::ContinueFill() Read error (code %ld)", (long) itsInputStream->ErrorCode()));
				AbortFill();
			}
			else
				EndFill();
				
			return pmfalse;
		}
		
		if (!theReadCount)
			return pmtrue;

			//	Write data
		if (!DoWrite((char*)theBuffer.buffer(), theReadCount))
		{	
				//	There was an error writing in the media
			PM_TRACE(Info, TL("PMMedia::ContinueFill(): Error while writing to media"));
			
			AbortFill();
			return pmfalse;
		}

		itsWritePos += theReadCount;
	}
	
	// NOT REACHED
	return pmfalse;
}

// ---------------------------------------------------------------------------

pmbool PMMedia::DoWrite(const char *, size_t ) 
{
	return pmfalse; 
}
	

// ---------------------------------------------------------------------------

size_t PMMedia::DoRead(pmuint32 , size_t , char *, pmbool *) 
{
	return 0; 
}

// ---------------------------------------------------------------------------

void PMMedia::AbortFill()
{
	itsInputStream = 0;
	SetState(PMMedia_kError);
	DoEndFill();
}

// ---------------------------------------------------------------------------

void PMMedia::EndFill()
{
	itsInputStream = 0;
	SetState(PMMedia_kIdle);
	if (!DoEndFill())
		SetState(PMMedia_kError);
}

// ---------------------------------------------------------------------------

PMMedia::PMMedia(pmbool afTemporary) : 
	itsInputStream(0)
{	
	itsfTemporary = afTemporary;
	itsNumberOfStreams = 0;
	itsState = PMMedia_kIdle;
}

// ---------------------------------------------------------------------------

PMMedia::~PMMedia()
{
	if (!itsInputStream.IsNull())
	{
		PM_TRACE(Info, TL("PMMedia::~PMMedia(): Destroying a media with a filling operation in progress."));
//		AbortFill();
		DoEndFill();
	}
	PM_ASSERT(!itsNumberOfStreams, TL("PMMedia::~PMMedia(): Destroying media in use."));
}

// ---------------------------------------------------------------------------

void PMMedia::_OpenMedia()
{	
	if (DoOpen())
		SetState(PMMedia_kOpened);
	else
		SetState(PMMedia_kError);
}

// ---------------------------------------------------------------------------

void PMMedia::_CloseMedia()
{	
	if (GetState() == PMMedia_kOpened)
		if (!DoClose())
			PM_TRACE(Info, TL("PMMedia::_CloseMedia(): Cannot close media"));
	
		//	Always set, cause what would it mean to refuse to close ?
	SetState(PMMedia_kIdle);
}

// ---------------------------------------------------------------------------

size_t PMMedia::Read(pmuint32 aPos, size_t aLen, char *aBuffer, pmbool *anErrorFlag)
{	
	*anErrorFlag = pmfalse;
	if (GetState() != PMMedia_kOpened)
	{
		*anErrorFlag = pmtrue;
		return 0;
	}

	return DoRead(aPos, aLen, aBuffer, anErrorFlag);
}

// ---------------------------------------------------------------------------

pmbool PMMedia::SyncFillFromStream(PMStreamRef aStream)
{	
	if (!PrepareFill(aStream))
		return pmfalse;

	while (ContinueFill())
			;

	return GetState() == PMMedia_kIdle;
}

// ---------------------------------------------------------------------------

PMMediaRef PMMedia::NewTemporaryMedia()
{	
	return (*gTemporaryMediaFactory)();
}

// ---------------------------------------------------------------------------

PMMediaRef (*PMMedia::gTemporaryMediaFactory)() = NewTemporaryMemoryMedia;

// ---------------------------------------------------------------------------

PMMediaRef PMMedia::NewTemporaryMemoryMedia()
{	
	return new PMMemoryMedia();
}


// ===========================================================================
//	PMEmptyMedia
// ===========================================================================

PMEmptyMedia::PMEmptyMedia() : 
	PMMedia(pmtrue)
{ 
}

// ---------------------------------------------------------------------------

PMEmptyMedia::~PMEmptyMedia()
{ 
}

// ---------------------------------------------------------------------------

PMStreamRef PMEmptyMedia::GetStream()
{ 
	return new PMEmptyStream(); 
}

// ===========================================================================
//	PMMemoryMedia
// ===========================================================================

// ---------------------------------------------------------------------------


PMMemoryMedia::PMMemoryMedia() :
		PMMedia(pmtrue)
{	
	itsBuffer = 0;
	itsLength = 0;
	itsAllocatedSize = 0;
}

// ---------------------------------------------------------------------------

PMMemoryMedia::PMMemoryMedia(size_t aLen) :
		PMMedia(pmtrue)
{	
	if (aLen != PMMediaUnknownSize)
	{
		itsBuffer = (char *) ::pm_malloc(aLen);
		itsLength = aLen;
		itsAllocatedSize = aLen;
	}
	else
	{
		itsBuffer = 0;
		itsLength = 0;
		itsAllocatedSize = 0;
	}
}

// ---------------------------------------------------------------------------

PMMemoryMedia::PMMemoryMedia(size_t aLen, char *aBuffer, pmbool afCopy) :
	PMMedia(afCopy)
{	
	if (afCopy)
	{
		itsBuffer = (char *) ::pm_malloc(aLen);
		::pm_memcpy(itsBuffer, aBuffer, aLen);
	}
	else
		itsBuffer = aBuffer;

	itsLength = aLen;
	itsAllocatedSize = aLen;
}

// ---------------------------------------------------------------------------

PMMemoryMedia::PMMemoryMedia(size_t aLen, const char *aBuffer, pmbool afCopy) :
	PMMedia(afCopy)
{
	if (afCopy)
	{
		itsBuffer = (char *) ::pm_malloc(aLen);
		::pm_memcpy(itsBuffer, aBuffer, aLen);
	}
	else
		itsBuffer = (char*) aBuffer;

	itsLength = aLen;
	itsAllocatedSize = aLen;		//	NO
}

// ---------------------------------------------------------------------------

PMMemoryMedia::PMMemoryMedia(const char *aBuffer, pmbool afCopy) :
	PMMedia(afCopy)
{	
	if (afCopy)
		itsBuffer = (char *) ::pm_strdup(aBuffer);
	else
		itsBuffer = (char *) aBuffer;

	itsLength = ::pm_strlen(aBuffer);
	itsAllocatedSize = itsLength;		//	NO
}

// ---------------------------------------------------------------------------

PMMemoryMedia::~PMMemoryMedia()
{	
	if (itsfTemporary)
		::pm_free(itsBuffer);
}

// ---------------------------------------------------------------------------

pmbool PMMemoryMedia::PrepareFill(PMStreamRef aStream)
{	
	if (PMMedia::PrepareFill(aStream))
	{
		itsLength = 0;
		return pmtrue;
	}	
	
	return pmfalse;
}

// ---------------------------------------------------------------------------

pmuint32 PMMemoryMedia::GetSize()
{	
	return (pmuint32) itsLength;
}

// ---------------------------------------------------------------------------

void PMMemoryMedia::_CheckForSize(size_t aNeededLen)
{	
		//	#### WE SHOULD NOT DO THAT IF WE HAVE NOT ALLOCATED THE MEMORY OURSELFS
	if (itsAllocatedSize<aNeededLen)
		if (!itsAllocatedSize)
		{	
			itsAllocatedSize = ::pm_max(2048, aNeededLen);
			itsBuffer = (char *) ::pm_malloc(itsAllocatedSize);
		}
		else
		{
			// itsAllocatedSize *= 2; // This was buggy...
			itsAllocatedSize = ::pm_max(itsAllocatedSize * 2, aNeededLen);
			itsBuffer = (char *) ::pm_realloc(itsBuffer, itsAllocatedSize);			
		}
}

// ---------------------------------------------------------------------------

pmbool PMMemoryMedia::DoWrite(const char *aBuffer, size_t aWriteCount)
{	
	_CheckForSize(itsWritePos + aWriteCount);
	
	::pm_memcpy(itsBuffer + itsWritePos, aBuffer, aWriteCount);
	itsLength += aWriteCount;
	
	return pmtrue;
}

// ---------------------------------------------------------------------------

size_t PMMemoryMedia::DoRead(pmuint32 aPos, size_t aLen, char *aBuffer, pmbool *anErrorFlag)
{	
	size_t	theCopySize = ::pm_min(aLen, itsLength - aPos);
	
	*anErrorFlag = pmfalse;
	::pm_memcpy(aBuffer, itsBuffer + aPos, theCopySize);
	
	return theCopySize;
}


// ===========================================================================
//	PMMediaStream
// ===========================================================================

// ---------------------------------------------------------------------------

PMMediaStream::PMMediaStream(const PMMediaRef &aMedia)
{
	itsMedia = aMedia;
	itsMedia->StreamAdded();
	itsOffset = 0;
}

// ---------------------------------------------------------------------------

PMMediaStream::~PMMediaStream()
{	
	itsMedia->StreamRemoved();
}

// ---------------------------------------------------------------------------

size_t PMMediaStream::GetNextChars(size_t aCount, char *aBuffer)
{
	pmbool	theErrorFlag = pmfalse;
	size_t	theReadLen = itsMedia->Read(itsOffset, aCount, aBuffer, &theErrorFlag);

	if (theErrorFlag)
	{
		SetError(PMStream_kIOError);
		return 0;
	}
	
	if (theReadLen != aCount)
		SetAtEOS();

	itsOffset += theReadLen;

	return theReadLen;
}




