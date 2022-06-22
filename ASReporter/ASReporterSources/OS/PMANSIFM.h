// ===========================================================================
//	PMANSIFM.h
//	Copyright (C) 1996 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMANSIFM_h
#define PMANSIFM_h

// ---------------------------------------------------------------------------

#include "PMMedia.h"

extern "C" 
{
	#include <stdio.h>
}

// ---------------------------------------------------------------------------

PMDEFINE(PMANSIFileMedia);

// ---------------------------------------------------------------------------
/**	Media on a file. The file is accessed using the ANSI file access API.
Use 'fopen' to open the file (or create it if necessary). Files are always opened
in binary mode.	*/

class PMANSIFileMedia : public PMMedia
{
public:
		/**	Returns temorary media (ie: media on a temporary file).	*/
	static PMMediaRef NewTemporaryANSIFileMedia();

		/**
		Returns media on an already existing (and opened) file. 
		The file is not closed when the object is destroyed.
		You must have opened the file with the correct permission in order to 
		perform your operation on the media (read or write)
		You may use stdin, stdout, and stderr as 'FILE*'.
		*/
	PMANSIFileMedia(FILE *aFile);
	
		/**
		Returns media on the file specified by the path 'aFilePath'.
		If 'afTemporary' is true the file will be physically deleted
		when the object is destroyed.
		Errors are not checked here, but later when the media is
		really used.
		The file will be created if it does not exist and you try to write to it.
		*/
	PMANSIFileMedia(PMStrRef aFilepath, pmbool afTemporary);

		/**	Destructor, physically deletes the file if it was temporary.
		one.	*/
	~PMANSIFileMedia();

		/**
		Returns the underlying file name. Returns an empty string
		when the 'FILE*' constructor was used.
		*/
	PMStrRef GetFilename();

		/**	See: \Ref{PMMedia::DoPrepareFill}.	*/
	virtual pmbool DoPrepareFill();

		/**	See: \Ref{PMMedia::DoWrite}.	*/
	virtual pmbool DoWrite(const char *aBuffer, size_t aWriteCount);

		/**	See: \Ref{PMMedia::DoEndFill}.	*/
	virtual pmbool DoEndFill();

		/**	See: \Ref{PMMedia::DoOpen}.	*/
	virtual pmbool DoOpen();

		/**	See: \Ref{PMMedia::DoClose}.	*/
	virtual pmbool DoClose();

		/**	See: \Ref{PMMedia::DoRead}.	*/
	virtual size_t DoRead(pmuint32 aPos, size_t aLen, char *aBuffer, pmbool *anErrorFlag);

protected:
		/**	The underlying file name (empty when the FILE* constructor was used).	*/
	PMStrRef	itsFilename;

		/**	The underlying file descriptor.	*/
	FILE		*itsFile;
	
		/**
		This value is accurate only if 'itsFile' is stdin, stdout, or stderr, which is only possible
		when the (FILE*) constructor was used.
		*/
	pmbool		itsfStd;

#ifndef PMFinalRelease
	static pmuint32	cOpenCount, cMaxOpenCount;
#endif
};

// ---------------------------------------------------------------------------

#endif	//	PMANSIFM_h


