// ===========================================================================
//	PMTCPTr.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMTCPTr_h
#define PMTCPTr_h

// ---------------------------------------------------------------------------

#include "PMXTrace.h"
#include "PMStr.h"

// ===========================================================================
//	PMTCPTracer
// ===========================================================================

	/**	TCP port number used by the TCP tracer.	*/
const pmuint16 PMTCPTracer_kPort = 25999;

// ---------------------------------------------------------------------------

PMDEFINE(PMData);
PMDEFINE(PMTCPTracer);
PMDEFINE(PMTCPSocket);

// ---------------------------------------------------------------------------
/**	A tracer using TCP.	*/

class PMTCPTracer : public PMXTracer
{
public:
		/**	Internet address constructor.	*/
	PMTCPTracer(const PMStrRef &anAddress, pmuint16 aPort = PMTCPTracer_kPort);
	
		/**	Destructor.	The TCP socket is closed. */
	virtual ~PMTCPTracer();

		/**	
		Returns true if data can be sent using the tracer (ie: returns true 
		if the TCP connection is up).	
		*/
	virtual pmbool	CanSend();

protected:
	PMTCPSocketRef	itsSocket;
	PMStrRef		itsHost;
	pmuint16		itsPort;
	
	virtual void	SendChunk(PMDataRef aData);

private:
	PMTCPTracer(const PMTCPTracer&);
	PMTCPTracer& operator=(const PMTCPTracer&);
};

// ---------------------------------------------------------------------------
#endif // PMTCPTr_h
