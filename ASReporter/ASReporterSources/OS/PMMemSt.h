// ===========================================================================
//	PMMemSt.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMMemSt_h
#define PMMemSt_h

// ---------------------------------------------------------------------------

#include "PMStream.h"

// ===========================================================================
//	PMMemoryStream
// ===========================================================================

PMDEFINE(PMMemoryStream);

// ---------------------------------------------------------------------------
/**	Stream on a memory buffer.	*/

class PMMemoryStream : public PMStream
{
public:

	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**	Constructor. The buffer is copied.	*/
	PMMemoryStream(const char *aBuffer, size_t aCount);
	
		/**	Destructor.	*/
	virtual ~PMMemoryStream();

protected:

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

	size_t GetNextChars(size_t aCount, char *aBuffer);

private:
	PMMemoryStream();
};

// ---------------------------------------------------------------------------

#endif	// PMMemSt_h


