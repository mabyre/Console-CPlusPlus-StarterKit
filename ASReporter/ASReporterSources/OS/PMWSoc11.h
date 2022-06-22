// ===========================================================================
//	PMWSoc11.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMWSoc11_h
#define PMWSoc11_h

//-----------------------------------------------------------------------
#include "PMTCPSoc.h"
#include "PMStr.h"

//-----------------------------------------------------------------------
/**	TCP socket factory for Win32 platforms using WinSock DLL. */

class PMWinSock11Factory : public PMTCPSocketFactory
{
public:
	/** Inherited. See: \Ref{PMTCPSocketFactory::Init}. */
	virtual pmbool			Init(void* = 0);
	/** Inherited. See: \Ref{PMTCPSocketFactory::Close}. */
	virtual void			Close(void* = 0);

	/** Inherited. See: \Ref{PMTCPSocketFactory::NewSocketConnectedTo}. */
	virtual PMTCPSocketRef	NewSocketConnectedTo(const PMStrRef &aHost, pmuint16 aPort);
	/** Inherited. See: \Ref{PMTCPSocketFactory::NewSocketConnectedTo}. */
	virtual PMTCPSocketRef	NewSocketConnectedTo(pmuint32 aPeerAddress, pmuint16 aPort);
	/** Inherited. See: \Ref{PMTCPSocketFactory::GetHostName}. */
	virtual PMStrRef		GetHostName();

	/** Inherited. See: \Ref{PMTCPSocketFactory::NewAcceptorFor}. */
	virtual PMTCPSocketAcceptorRef NewAcceptorFor(pmuint16 aPort);
};

//-----------------------------------------------------------------------
#endif // PMWSoc11_h
