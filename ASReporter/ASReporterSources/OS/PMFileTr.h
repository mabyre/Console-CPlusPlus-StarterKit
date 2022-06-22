// ===========================================================================
//	PMFileTr.h
//	Copyright (C) 1996 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMFileTr_h
#define PMFileTr_h

// ---------------------------------------------------------------------------

#include "PMTrace.h"
#include "PMStr.h"
#include <stdio.h>

// ===========================================================================
//	PMANSIFileTracer
// ===========================================================================

PMDEFINE(PMANSIFileTracer);

// ---------------------------------------------------------------------------
/**	
This is a tracer that stores program traces in a file. The file may be accessed using the 
API for ANSI file access.
*/

class PMANSIFileTracer : public PMTracer
{
public:
		/** File path constructor. The file will be created when necessary
		(potenitially deleting a previously existing one).	*/
	PMANSIFileTracer(const char *aFilePath);

		/** 
		'FILE*' constructor. If 'afClose' is true the file will be closed
		when the object is destroyed.
		*/
	PMANSIFileTracer(FILE *aFile, pmbool afClose);
	
		/**	Destructor.	The file is closed (if it was specified). */
	virtual ~PMANSIFileTracer();

		/**	See: \Ref{PMTracer::B}.	*/
	virtual PMTraceSendParams*	B(const void* aBuffer, size_t aSize);

		/**	See: \Ref{PMTracer::Send}.	*/
	virtual void		Send(PMTraceSendParams* aParams);

		/**	See: \Ref{PMTracer::DisplayAssert}.	*/
	virtual void		DisplayAssert(const char* aTest, PMTraceSendParams* aMessage);

protected:
	FILE*		itsFile;
	pmbool		itsfClose, itsfMustIndentNext, itsfTryOpen;
	PMStrRef	itsFileName;

	virtual void	Indent(pmuint32 aLevel);
	virtual pmbool	DoOpen();

private:
	PMANSIFileTracer(const PMANSIFileTracer&);
	PMANSIFileTracer& operator=(const PMANSIFileTracer&);
};

// ---------------------------------------------------------------------------

#endif // PMFileTr_h
