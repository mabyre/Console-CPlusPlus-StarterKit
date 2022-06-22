// ===========================================================================
//	PMStream.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMStream_h
#define PMStream_h

// ---------------------------------------------------------------------------

#include "PMStr.h"
#include "PMPushB.h"

// ===========================================================================
//	PMStream
// ===========================================================================

	/**	Type used for stream errors.	*/
typedef pmuint16 PMStream_eError;

	/**	No error.	*/
const PMStream_eError PMStream_kNoError = 0;

	/**	End of stream has been reached.	*/
const PMStream_eError PMStream_kAtEOS = 1;

	/**	Read error.	*/
const PMStream_eError PMStream_kIOError = 2;

	/**	The stream was not solved.	*/
const PMStream_eError PMStream_kSolveFailed = 3;

// ---------------------------------------------------------------------------

PMARRAYDEFINE( PMStream );

// ---------------------------------------------------------------------------
/**
Streams are used to read data.
NOTE: A stream cannot be re-initialized when in error condition.
*/

class PMStream : public PMRC
{	
friend class PMLinkStream;
friend class PMHeadStream;

public:

	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**	Default constructor.	*/
	PMStream();

		/**	Destructor.	*/
	virtual ~PMStream();

		/**	Returns true if the stream is at end.	*/
	pmbool IsAtEOS();

		/**
		Returns true if next call to 'Read' is guaranteed to fail.
		This also returns true when the stream end has been reached.
		*/
	pmbool IsError();

		/** Returns the stream's current error code.	*/
	PMStream_eError ErrorCode();

	// -----------------------------------------------------------------------
	//	Read from stream
	// -----------------------------------------------------------------------

		/**
		Reads at most 'aCount' characters into 'aBuffer'. Fills 'aReadCount' with the number
		of characters read (which will generally be less than requested).
		Returns true if successful, false if an error occured (i.e. 'IsError'  
		returns true). Cannot fail and read characters at the same time (i.e. if 'Read' 
		returns false, '*aReadCount' will be 0).
		*/
	virtual pmbool Read(size_t aCount, char *aBuffer, size_t *aReadCount);

		/**
		Reads 'aCount' bytes in blocking mode, meaning that the function will return
		when at least one byte is read or an error occurs.
		Returns true if successful (i.e. 'aCount' bytes have been read).
		Returns false if there are any errors ('*aReadCount' contains the count of bytes read).
		*/
	pmbool BlockingRead(size_t aCount, char *aBuffer, size_t *aReadCount);

		/**
		Tries to read exacly 'aCount' bytes from the stream (and will not read less).
		Returns true if 'aCount' bytes were read. If the read operation failed and 
		'*afTryAgain' is true, you may issue a new read.
		*/
	virtual pmbool ExactRead(size_t aCount, char *aBuffer, pmbool *afTryAgain);

		/**
		Pushes back 'aCount' bytes. If 'aCount' is 0, this method does nothing.
		You can freely call 'PushBack' several times and/or interleave with 'Read' calls.
		'PushBack' is not designed to be used on many small buffers. Data is copied.
		*/
	 void PushBack(size_t aCount, const char *aBuffer);

protected:

	// -----------------------------------------------------------------------
	//	Implementation. Reserved for implementors
	// -----------------------------------------------------------------------

		/**
		When a stream returns 0 caracters to a read operation, it asks itself
		to initiate a read operation. Streams should avoid returning 0 
		caracters from Read.
		*/
	virtual void InitiateRead();

	PMStream_eError		itsError;		//	Error to return to client
	PMStream_eError		itsFutureError;	//	Error to return when pushback is empty
	PMPushBackRef		itsPushBack;	//	List of PushBacks

		/**
		This is the canonical function to implement data retrieval.
		'aCount' is the number of characters requested.
		'aBuffer' is a character buffer guaranteed to have enough place to hold
		'aCount' characters.
		The function should return the number of caracters actually read and
		placed in 'aBuffer'.
		*/

		/**
		If 'canBlock' is false, the routine MUST NOT block for any reason,
		and is allowed to return 0 bytes without error.
		*/

		/**
		If 'canBlock' is true, the routine IS REQUIRED to return either an
		error, or at least one character.
		*/

		/**
		Errors and EOS conditions should be notified by using a SetError() call.
		Those errors would only be visible by the client until the next read.
		It is legal to return characters and to set error conditions at the same
		time. Clients will see the error condition after those caracters have
		been read.
		*/

		/**
		For making it easier to use complex encoding/decoding algorithms, you may use 
		a PushBack from inside the GetNextChars() function. They will be
		delivered to the client before the next GetNextChars().
		*/

		/**	GetNextChars() will never get called after it has set an error condition.	*/

		//	-- THIS IS NOT IMPLEMENTED YET (AND WILL BE CHANGED) ---------------------------
		/**
		If the implemented stream is a subclass of 'PMStreamModule', several more functions
		are avalaible to it:
		*/

		/**
		A way to set errors in a compatible way with the peer.
		PeerError()	Should alway be called when the peer is in error
		as usual; client will only see it when data exhausted.
		A way to require that peer is a pushback-able stream.
		SetPeerPushBack()Can change itsPeer value.
		*/
	virtual size_t GetNextChars( size_t aCount, char *aBuffer ) = 0;

		/**	Stream is definitely at EOS.	*/
	void SetAtEOS();

		/**	Sets the stream error code.	*/
	void SetError(PMStream_eError anError);

		/**	Returns if there are any characters in pushback buffers.	*/
	pmbool HasPushBack();			
};


// ===========================================================================
//	PMEmptyStream
// ===========================================================================
/**
An empty stream. (ie: a stream that has no content). The first (and each following)
time you read data from it, it will read 0 characters. After the 
first read, 'IsAtEOS' will always returns true.
*/

class PMEmptyStream : public PMStream
{
public:

	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**	Default constructor. The stream is ready to read.	*/
	PMEmptyStream();
	
		/**	Destructor.	*/
	virtual ~PMEmptyStream();
	
protected:
	virtual size_t GetNextChars(size_t aCount, char *aBuffer);
};

// ---------------------------------------------------------------------------

#endif // PMStream_h

