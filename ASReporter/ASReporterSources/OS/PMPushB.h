// ===========================================================================
//	PMPushB.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMPushB_h
#define PMPushB_h

// ---------------------------------------------------------------------------

#include "PMArray.h"

// ===========================================================================
//	PMPushBack
// ===========================================================================

PMDEFINE(PMPushBack);
PMDEFINE(PMPushBackBuffer);

// ---------------------------------------------------------------------------
/**
Handles a list of pushback buffers. Pushbacks are used to store data that
has been read but is not necessary for current processing. This data 
is pushed back, and will be used later.
*/

class PMPushBack : public PMRC
{	
public:
	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**	Constructor. There are no pushback buffers.	*/
	PMPushBack();
	
		/**	Destructor. All remaining pushback buffers are freed.	*/
	~PMPushBack();
	
	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------

		/**	Returns true if data has been pushed back and is available to 
		be read.	*/
	pmbool	HasPushBack();

		/**
		Adds specified buffer to the pushback. The buffer is copied.
		If 'afAtEnd' is true, the buffer is appended to the list of 
		pushed-back buffers.
		*/
	void	AddBuffer(const char *aBuffer, size_t aCount, pmbool afAtEnd);

		/**
		Reads data from pushback buffers. 'aCopyCount' is the amount of data
		read.
		*/
	void	Read(char *aBuffer, size_t aCount, size_t *aCopyCount);
	
protected:
	
	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

	typedef PMArray<PMPushBackBufferRef>	PMPushBackArray;
	typedef PMRef<PMPushBackArray>			PMPushBackArrayRef;

	PMPushBackArrayRef	itsArray;
	pmuint32			itsID;
	
	void Dump();
};

// ===========================================================================
//	PMPushBackBuffer
// ===========================================================================
/**	A buffer used by 'PMPushBack'.	*/

class PMPushBackBuffer : public PMRC
{	
friend class PMPushBack;

protected:
	
	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

	size_t		itsLeft;
	size_t		itsOffset;
	char		*itsData;
	pmuint32	itsID;
	
		/**	Buffer constructor. Data is copied.	*/
	PMPushBackBuffer(const char *aBuffer, size_t aCount);
	
		/**	Destructor. Data is freed.	*/
	~PMPushBackBuffer();
	
		/**	Returns true if buffer is empty.	*/
	pmbool IsEmpty();

		/**
		Copies 'aCount' chars from pushback buffer into 'aBuffer', then frees
		the buffer if it is empty.
		*/
	void	Copy(char *aBuffer, size_t aCount, size_t *aCopyCount);
	
		/**	Destroys allocated pushed-back data.	*/
	void	FreeBuffer();

		/**	Internal use.	*/
	void	Dump();

private:
	PMPushBackBuffer();
};

// ---------------------------------------------------------------------------

#endif // PMPushB_h

