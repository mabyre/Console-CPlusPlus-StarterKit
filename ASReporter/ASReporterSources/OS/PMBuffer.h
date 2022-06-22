// ===========================================================================
//	PMBuffer.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMBuffer_h
#define	PMBuffer_h

// ---------------------------------------------------------------------------

#include "PMRC.h"

// ===========================================================================
//	PMBuffer
// ===========================================================================

PMDEFINE(PMBuffer);

// ---------------------------------------------------------------------------
/**	
Handles a temporary memory buffer in an exception-safe manner.	
Whatever the outcome (exception or normal destruction) the buffer is always deleted.
'PMBuffer' should always be instanciated as an automatic object.
*/

class PMBuffer : public PMRC
{
public:

	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**	Constructor. Allocates a buffer of 'aSize' bytes.	*/
	PMBuffer(size_t aSize = 0);

		/**
		Destructor. Deallocates the buffer unless 'TakeOwnership' has been called 
		previously.
		*/
	~PMBuffer();

	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------
	
		/**
		Reallocates the buffer to 'aNewSize' bytes.
		Note: This changes the buffer's address.
		If you call this method after having called 'TakeOwnership', this method
		will create a new buffer.
		*/
	void		Realloc(size_t aNewSize);

		/**
		Gives ownsership of the buffer to the caller, meaning that it
		becomes the caller's responsibility to deallocate the returned
		buffer. The returned buffer must be deallocated using '::pm_free'.
		You can create a new buffer using 'Realloc'.
		*/
	void		*TakeOwnership();

		/**	Returns the buffer address, for read and write access.	*/
	void		*buffer() const 
		{ return itsPtr; }

		/**	Returns the buffer's allocated size.	*/
	size_t		size() const 
		{ return itsSize; }
	
protected:
	void*	itsPtr;
	size_t	itsSize;
};

// ---------------------------------------------------------------------------

#endif // PMBuffer_h

