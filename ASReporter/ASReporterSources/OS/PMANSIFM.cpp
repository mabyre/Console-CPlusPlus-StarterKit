// ===========================================================================
//	PMANSIFM.cpp
//	Copyright (C) 1996 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMANSIFM.h"
#include "PMBuffer.h"

// ---------------------------------------------------------------------------
#ifndef SEEK_SET
		//	#### This was a problem on sun OS
	#error "Your compiler does not look ANSI : SEEK_SET is not defined (0)"
#endif

// ---------------------------------------------------------------------------

#define PM_TRACEMODULE MEDIA
PM_DEFINE_TRACE_CODE(Info, 1);

// ---------------------------------------------------------------------------
#ifndef PMFinalRelease
pmuint32 PMANSIFileMedia::cOpenCount;
pmuint32 PMANSIFileMedia::cMaxOpenCount;
#endif

// ---------------------------------------------------------------------------

PMANSIFileMedia::PMANSIFileMedia(FILE* aFile) : 
	PMMedia(pmfalse)
{	
	PM_TRACE(Info, TL("PMANSIFileMedia::PMANSIFileMedia(FILE*)"));
	itsFile = aFile;
	itsFilename = PMStr::Empty();
	itsfStd = (aFile == stdin) || (aFile == stdout) || (aFile == stderr);
}

// ---------------------------------------------------------------------------

PMANSIFileMedia::PMANSIFileMedia(PMStrRef aFilename, pmbool afTemporary) : 
	PMMedia(afTemporary)
{	
	PM_TRACE(Info, TL("PMANSIFileMedia::PMANSIFileMedia(%s%s)", aFilename->c_str(), afTemporary?" (TEMP)":""));
	itsFile = 0;
	itsFilename = aFilename;
	itsfStd = pmfalse;
}

// ---------------------------------------------------------------------------

PMANSIFileMedia::~PMANSIFileMedia()
{	
	PM_TRACE(Info, TL("PMANSIFileMedia::~PMANSIFileMedia(%s)", itsFilename->c_str()));
	DoClose();
	if (itsfTemporary && !itsfStd)
	{
		PM_TRACE(Info, TL("PMANSIFileMedia::~PMANSIFileMedia(): Removing file [%s]", itsFilename->c_str()));
		if (::remove(itsFilename->c_str()) == -1)
			PM_TRACE(Info, TL("PMANSIFileMedia::~PMANSIFileMedia(): Error while removing file [%s]", itsFilename->c_str()));
	}
}

// ---------------------------------------------------------------------------

PMStrRef PMANSIFileMedia::GetFilename()
{	
	return itsFilename;
}

// ---------------------------------------------------------------------------

pmbool PMANSIFileMedia::DoPrepareFill()
{	
	if (itsFile == 0)
	{
		itsFile = ::fopen(itsFilename, "wb");
		if (itsFile == 0)
			PM_TRACE(Info, TL("PMANSIFileMedia::DoPrepareFill(): Error while opening: %s", itsFilename->c_str()));
#ifndef PMFinalRelease
		else
			if (++cOpenCount > cMaxOpenCount)
			{
				cMaxOpenCount = cOpenCount;
				PM_TRACE(Info, TL("PMANSIFileMedia::DoPrepareFill(): New ANSI file max. open count: %lu", cMaxOpenCount));
			}
#endif // PMFinalRelease
	}

	return itsFile != 0;
}

// ---------------------------------------------------------------------------

pmbool PMANSIFileMedia::DoEndFill()
{	
	if ((itsFile != 0) && (!itsfStd))
	{
#ifndef PMFinalRelease
		cOpenCount--;
#endif
		if (::fclose(itsFile) == EOF)
			return pmfalse;
		itsFile = 0;
	}
	
	return pmtrue;
}

// ---------------------------------------------------------------------------

pmbool PMANSIFileMedia::DoWrite( const char *aBuffer, size_t aWriteCount )
{	
	return ::fwrite( aBuffer, aWriteCount, 1, itsFile )==1;
}

// ---------------------------------------------------------------------------

pmbool PMANSIFileMedia::DoOpen()
{	
	if (itsFile == 0)
	{
		itsFile = ::fopen(itsFilename->c_str(), "rb");
		if (itsFile == 0)
			PM_TRACE(Info, TL("PMANSIFileMedia::DoOpen(): Error while opening: %s", itsFilename->c_str()));
#ifndef PMFinalRelease
		else
			if (++cOpenCount > cMaxOpenCount)
			{
				cMaxOpenCount = cOpenCount;
				PM_TRACE(Info, TL("PMANSIFileMedia::DoOpen(): New ANSI file max. open count: %lu", cMaxOpenCount));
			}
#endif // PMFinalRelease
	}
	
	return itsFile != 0;
}

// ---------------------------------------------------------------------------

pmbool PMANSIFileMedia::DoClose()
{	
	return DoEndFill();
}

// ---------------------------------------------------------------------------

size_t PMANSIFileMedia::DoRead(pmuint32 aPos, size_t aLen, char *aBuffer, pmbool *anErrorFlag)
{	
	*anErrorFlag = pmtrue;
	if ((!itsfStd) && (::fseek( itsFile, (long)aPos, SEEK_SET ) != 0))
	{	
		PM_TRACE( Info, TL("PMANSIFileMedia::DoRead(): Cannot seek") );
		return 0;
	}

	size_t theReadLen = ::fread( aBuffer, 1, aLen, itsFile );
	if ((theReadLen != aLen) && (!feof(itsFile)))
	{	
		PM_TRACE(Info, TL("PMANSIFileMedia::DoRead(): I/O error while reading"));
		return theReadLen;
	}
	
	*anErrorFlag = pmfalse;
	return theReadLen;
}

// ---------------------------------------------------------------------------

PMMediaRef PMANSIFileMedia::NewTemporaryANSIFileMedia()
{
	static	pmint32 sID = 0;
	char	theBuffer[32];
	FILE*	theFile;

	if (sID == 0)
		sID = ::pm_rand(0,999999);

	do		
	{		//	Finding an unused file
		::sprintf(theBuffer, "TMP%06ld", (long)sID++);
		theFile = ::fopen(theBuffer, "r");
		if (theFile != NULL)
			::fclose(theFile);
		else
		{
			theFile = ::fopen(theBuffer, "wb");
			::fclose(theFile);
			theFile = 0;
		}
	}	while (theFile);

	PM_TRACE(Info, TL("PMANSIFileMedia::NewTemporaryANSIFileMedia(): Using temporary file [%s]", theBuffer));
	return new PMANSIFileMedia(theBuffer, pmtrue);
}
