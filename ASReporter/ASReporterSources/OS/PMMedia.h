// ===========================================================================
//	PMMedia.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMMedia_h
#define PMMedia_h

// ---------------------------------------------------------------------------

#include "PMStream.h"

// ===========================================================================
//	PMMedia
// ===========================================================================

	/** Type used for media state.	*/
typedef pmuint16 PMMedia_eState;

	/** Media is ready to be used. It is not opened. */
const PMMedia_eState PMMedia_kIdle = 1;

	/** Media is open for write.	*/
const PMMedia_eState PMMedia_kFilling = 2;

	/** Media is open for read.	*/
const PMMedia_eState PMMedia_kOpened = 3;

	/** Media encountered an error while accessing, reading, or writing.	*/
const PMMedia_eState PMMedia_kError = 4;

	/**	Value used when media size is not known.	*/
const pmuint32 PMMediaUnknownSize = (pmuint32) -1;

PMDEFINE(PMMedia);

// ---------------------------------------------------------------------------
/**
Base class for accessing (reading and writing) an ordered collection of bytes. 
Media helps store stream contents. When filling media (ie: writing), its current
content is always overwritten.
*/

class PMMedia : public PMRC
{
friend class PMMediaStream;

public:

	// -----------------------------------------------------------------------
	//	Factory
	// -----------------------------------------------------------------------

		/**	Creates new temporary media.	*/
	static PMMediaRef NewTemporaryMedia();

		/**	Creates new temporary media in memory.	*/
	static PMMediaRef NewTemporaryMemoryMedia();

		/**	Class variable that holds temporary media factory.	*/
	static PMMediaRef (*gTemporaryMediaFactory)();

	// -----------------------------------------------------------------------
	//	Construction / Destruction
	// -----------------------------------------------------------------------

		/**	Constructor. If 'afTemporary' is true, the media is temporary.	*/
	PMMedia(pmbool afTemporary);

		/**
		Destructor. If media is temporary, its storage is 
		physically deleted.
		*/
	virtual ~PMMedia();

	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------

		/** Returns current media state.	*/
	PMMedia_eState GetState();

		/**	Returns if media is temporary.	*/
	virtual pmbool IsTemporary();

		/**
		If 'afTemporary' is true, media storage will be physically deleted 
		at destruction.
		*/
	virtual void SetTemporary(pmbool afTemporary);

		/**
		Returns media's total size. If this size cannot be computed or is unknown, this method 
		returns 'PMMediaUnknownSize'.
		*/
	virtual pmuint32 GetSize();

		/**
		Returns a stream on media content.
		It is possible to get several streams on the same media.
		This method returns a NULL reference (for example if the associated storage 
		does not exist, or the media is busy being filled).
		*/
	virtual PMStreamRef GetStream();

	// -----------------------------------------------------------------------
	//	Filling
	// -----------------------------------------------------------------------

		/**
		Fills the media with the contents of 'aStream'. This is the easiest way
		to move a stream into media.
		Returns true if successful.
		*/
	pmbool SyncFillFromStream(PMStreamRef aStream);

		/**
		Prepares the media to be filled with the contents of 'aStream'.
		Returns true if successful.
		*/
	virtual pmbool PrepareFill(PMStreamRef aStream);

		/**
		Move part of the stream into media. 'PrepareFill' should have been called 
		sucessfully prior to using this method.
		Returns true if you should continue calling 'ContinueFill'.
		If false is returned, you need to call 'GetState()' to get the media state 
		and find out if everything went correctly.
		*/
	pmbool ContinueFill();

		/**
		Prepares media for a sequence of write operations. Media is typically open
		for write access.
		*/
	virtual pmbool DoPrepareFill()	
		{ return pmtrue; }
	
		/**	Terminates a sequence of write operations. Media is typically closed.	*/
	virtual pmbool DoEndFill()		
		{ return pmtrue; }

		/**
		Prepares media for a sequence of read operations. Media is typically 
		open for read access.
		*/
	virtual pmbool DoOpen()		
		{ return pmtrue; }
	
		/**	Terminates a sequence of read operations. Media is typically closed.	*/
	virtual pmbool DoClose()	
		{ return pmtrue; }

		/**
		Writes 'aBuffer' of 'aSize' bytes into media. Returns true
		if successful.
		*/
	virtual pmbool DoWrite(const char *aBuffer, size_t aSize);
	
		/**
		Tries to read 'aLen' bytes into 'aBuffer' from media starting at the 
		offset 'aPos'. Returns the amount of bytes read. '*anErrorFlag' is set to
		false if an error occurred.
		*/
	virtual size_t DoRead(pmuint32 aPos, size_t aLen, char *aBuffer, pmbool *anErrorFlag);

protected:		

		// -----------------------------------------------------------------------
		//	Implementation
		// -----------------------------------------------------------------------
		
		/**	Number of streams currently on media.	*/
	size_t	itsNumberOfStreams;		

		/**	Input stream.	*/
	PMStreamRef itsInputStream;

		/**	Media state.	*/
	PMMedia_eState	itsState;

		/**	If pmtrue, media is temporary.	*/
	pmbool itsfTemporary;

		/**	Current write pointer (may change).	*/
	size_t itsWritePos;
		/**
		Low-level media open (read) operation.
		Sets media state accordingly.
		*/
	void _OpenMedia();

		/**
		Low-level media close (read) operation.
		Sets media state accordingly.
		*/
	void _CloseMedia();

		/**	Changes media state.	*/
	void SetState(PMMedia_eState aState);
	
	void AbortFill();
	
	void EndFill();
		
		// -----------------------------------------------------------------------
		//	Implementation, used by PMMediaStream
		// -----------------------------------------------------------------------

	void StreamAdded();
	
	void StreamRemoved();

		/**
		Low-level media access.
		Read aLen data from aPos position in aBuffer.
		Returns number of bytes read.
		#### 4th parameter should be an error code ?
		*/
	size_t Read(pmuint32 aPos, size_t aLen, char *aBuffer, pmbool *anErrorFlag);
};


// ===========================================================================
//	PMEmptyMedia
// ===========================================================================
/**	Empty media. (ie: media that has no content).	*/

class PMEmptyMedia : public PMMedia
{
public:
	// -----------------------------------------------------------------------
	//	Construction / Destruction
	// -----------------------------------------------------------------------

		/**	Default constructor.	*/
	PMEmptyMedia();
	
		/**	Destructor.	*/
	virtual ~PMEmptyMedia();
	
	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------

		/**	Returns an empty stream.	*/
	virtual PMStreamRef GetStream();
};


// ===========================================================================
//	PMMemoryMedia
// ===========================================================================

PMDEFINE(PMMemoryMedia);

// ---------------------------------------------------------------------------
/**	Media that keeps its contents in memory.	*/

class PMMemoryMedia : public PMMedia
{
public:

	// -----------------------------------------------------------------------
	//	Construction / Destruction
	// -----------------------------------------------------------------------

		/**
		Default constructor. Media will allocate and destroy memory 
		on its own.
		*/
	PMMemoryMedia();

		/**
		Buffer size constructor. Media will allocate and destroy memory 
		on its own.
		*/
	PMMemoryMedia(size_t aLen);

		/**
		Buffer constructor.
		If 'afCopy' is true, a new buffer is allocated and the buffer is copied.
		If 'afCopy' is false, no new buffer is allocated and a pointer to the supplied 
		buffer is kept internally. Therefore direct changes to the buffer will change
		memory media as well.
		*/
	PMMemoryMedia(size_t aLen, char *aBuffer, pmbool afCopy = pmfalse);

		/**
		Const buffer constructor.
		If 'afCopy' is true, a new buffer is allocated and the buffer is copied.
		If 'afCopy' is false, no new buffer is allocated and a pointer to the supplied
		buffer is kept internally. Therefore direct changes to the buffer will change
		memory media as well.
		*/
	PMMemoryMedia(size_t aLen, const char *aBuffer, pmbool afCopy = pmfalse);

		/**
		Created on a zero-terminated buffer (typically a C string).
		If 'afCopy' is true, a new buffer is allocated and the buffer is copied.
		If 'afCopy' is false, no new buffer is allocated and a pointer to the supplied
		buffer is kept internally. Therefore direct changes to the buffer will change
		memory media as well.
		*/
	PMMemoryMedia(const char *aBuffer, pmbool afCopy = pmfalse);

		/**	Destructor.	*/
	~PMMemoryMedia();

	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------

		/**	Returns memory media's total size.	*/
	virtual pmuint32 GetSize();


	// -----------------------------------------------------------------------
	//	Advanced API (for internal use)
	// -----------------------------------------------------------------------

		/**	Internal use.	*/
	char *GetRawData()		
		{ return itsBuffer; }
	
		/**	Internal use.	*/
	size_t GetRawLength()	
		{ return itsLength; }

protected:

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------
	//	#### Problem (?) nothing prevent user to write in read-only media
	//	Should return an error [We need a flag to read-only...]

		/**	Pointer to data	*/
	char 	*itsBuffer;

		/**	Length of data	*/
	size_t	itsLength;

		/**	Allocated size	*/
	size_t 	itsAllocatedSize;

	virtual pmbool DoWrite(const char *aBuffer, size_t aWriteCount);
	
	virtual size_t DoRead(pmuint32 aPos, size_t aLen, char *aBuffer, pmbool *anErrorFlag);

	void _CheckForSize(size_t aNeededLen);

	virtual pmbool	PrepareFill(PMStreamRef aStream);
};


// ===========================================================================
//	PMMediaStream
// ===========================================================================

PMDEFINE(PMMediaStream);

// ---------------------------------------------------------------------------
/**	Stream on media. Lets you use media as if it were a stream.	*/

class PMMediaStream : public PMStream
{
public:

	// -----------------------------------------------------------------------
	//	Construction / Destruction
	// -----------------------------------------------------------------------

		/**	PMMedia constructor.	*/
	PMMediaStream(const PMMediaRef& aMedia);
	
		/**	Destructor.	*/
	virtual ~PMMediaStream();

protected:

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------
		
	PMMediaRef	itsMedia;
	pmuint32	itsOffset;

	virtual size_t GetNextChars(size_t aCount, char *aBuffer);
};

// ---------------------------------------------------------------------------

#endif // PMMedia_h

