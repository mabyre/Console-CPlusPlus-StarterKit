// ===========================================================================
//	PMWFilTr.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMWFilTr_h
#define PMWFilTr_h

// ---------------------------------------------------------------------------

#include "PMFileTr.h"

// ===========================================================================
//	PMWin32FileTracer
// ===========================================================================

PMDEFINE(PMWin32FileTracer);

// ---------------------------------------------------------------------------
/**	
This is a tracer that stores traces in a file. The file is accessed using the 
ANSI file access API. This Windows version overrides PMANSIFileTracer just
for displaying assertions.
*/

class PMWin32FileTracer : public PMANSIFileTracer
{
public:
		/** See: \Ref{PMANSIFileTracer} */
	PMWin32FileTracer(const char *aFilePath);

		/** See: \Ref{PMANSIFileTracer} */
	PMWin32FileTracer(FILE *aFile, pmbool afClose);
	
		/** See: \Ref{PMANSIFileTracer::~PMANSIFileTracer} */
	virtual ~PMWin32FileTracer();

		/**	See: \Ref{PMTracer::DisplayAssert}.	*/
	virtual void		DisplayAssert(const char* aTest, PMTraceSendParams* aMessage);

protected:
private:
	PMWin32FileTracer(const PMWin32FileTracer&);
	PMWin32FileTracer& operator=(const PMWin32FileTracer&);
};

// ---------------------------------------------------------------------------

#endif // PMWFilTr_h
