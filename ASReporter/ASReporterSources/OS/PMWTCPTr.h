// ===========================================================================
//	PMWTCPTr.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMWTCPTr_h
#define PMWTCPTr_h

// ---------------------------------------------------------------------------

#include "PMWinInc.h"
#include "PMXTrace.h"
#include "PMStr.h"
#include <winsock.h>

PMDEFINE(PMData);

// ---------------------------------------------------------------------------

const pmuint16 PMWin32TCPTracer_kPort = 25999;

/** PMXTracer Opcode: Application name Opcode.	*/
const PMXTracerOpcode_eType PMXTracerOpcode_kAppName = 128;

// ---------------------------------------------------------------------------
/*
	When you are using a PMWin32TCPTracer, you must pass a pointer to this structure
	to DecodeBlock aUserData parameter. It is filled with addtionnal information
	found in the trace block. You can (and should) pass the same pointer for the 
	aParam parameter as the struct begins with a PMTraceSendParams struct.
*/
/** Internal Use. */

struct PMWin32TCPTracerInfo
{
	PMTraceSendParams	itsParam;
	DWORD				itsThreadID;
	DWORD				itsProcessID;
	PMStrRef			itsAppName;

	PMWin32TCPTracerInfo() : itsParam(), itsThreadID(0), itsProcessID(0), itsAppName(0) {}
};

// ---------------------------------------------------------------------------

PMDEFINE(PMWin32TCPTracer);

// ---------------------------------------------------------------------------
/**	
This is tracer that uses TCP. This class is different from \Ref{PMTCPTracer} because
it is for use on exclusively on Win32 platforms. It provides enhanced information and can
be used to dump memory leaks over TCP whereas PMTCPTracer cannot.
*/

class PMWin32TCPTracer : public PMXTracer
{
public:
		/**	Internet address constructor.	*/
	PMWin32TCPTracer(const PMStrRef& anAddress, pmuint16 aPort = PMWin32TCPTracer_kPort);

		/**	Destructor.	The TCP socket is closed. */
	virtual ~PMWin32TCPTracer();

		/** Returns the maximum number of times it will try to connect. */
	virtual pmuint16	GetMaxConnectRetryCount();

		/** Returns the number of times it will try again to connect. */
	virtual pmuint16	GetConnectRetryCount();

		/**	
		Returns true if data can be sent using the tracer.	
		*/
	virtual pmbool		CanSend();

		/** Inherited. See: \Ref{PMTracer::DisplayAssert}. */
	virtual void		DisplayAssert(const char* aTest, PMTraceSendParams* aParams);

protected:
	SOCKET		itsSocket;
	PMStrRef	itsHost;
	pmuint16	itsPort;
	pmbool		itsfWinsockLoaded, itsfTryConnect, itsfFirstTrace;
	pmuint32	itsLastConnectTry, itsConnectRetryDelay;
	pmuint16	itsConnectRetryCount, itsMaxConnectRetryCount;
	
		/** Creates the binary block to send. */
	virtual	PMDataRef	CreateData(PMTraceSendParams* aParams);

		/** 
		Inherited: See: \Ref{PMXTracer::DecodeParameter}.
		aUserData can be 0 if you don't want additional data used by a PMWin32TCPTracer.
		If aUserData is not null it must point to a \Ref{PMWin32TCPTracerInfo}.
		*/
	virtual pmbool	DecodeParameter(pmbyte anOpcode, const void* aPtr, size_t aLen, PMTraceSendParams* aParam, void* aUserData);

	virtual void	SendChunk(PMDataRef aData);

private:
	PMWin32TCPTracer(const PMWin32TCPTracer&);
	PMWin32TCPTracer& operator=(const PMWin32TCPTracer&);
};

// ---------------------------------------------------------------------------
#endif // PMWTCPTr_h
