// ===========================================================================
//	PMTCPSoc.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMTCPSoc_h
#define PMTCPSoc_h

// ---------------------------------------------------------------------------

#include "PMSocket.h"
#include "PMStr.h"

// ===========================================================================
//	PMTCPSocketFactory
// ===========================================================================

PMDEFINE(PMTCPSocketFactory);
PMDEFINE(PMTCPSocket);
PMDEFINE(PMTCPSocketAcceptor);
PMDEFINE(PMTCPSocketImplementation);

/**	The TCP socket factory.	*/

class PMTCPSocketFactory : public PMSocketFactory
{
public:

	// -----------------------------------------------------------------------
	//	Factory
	// -----------------------------------------------------------------------

		/**
		Returns a socket that will connect to the specified host and port 
		before any other action. Uses the default socket factory.
		*/
	static PMTCPSocketRef			NewDefaultSocketConnectedTo(const PMStrRef &aHost, pmuint16 aPort);

		/**
		Returns a socket that will connect to the specified TCP/IP address 
		and port before any other action. Uses the default socket factory.
		*/
	static PMTCPSocketRef 			NewDefaultSocketConnectedTo(pmuint32 aPeerAddress, pmuint16 aPort);

		/**
		Returns a socket acceptor that will generate new sockets for incoming connections.
		Accept is not possible if a NULL reference is returned, 
		Uses the default socket factory.
		*/
	static PMTCPSocketAcceptorRef	NewDefaultAcceptorFor(pmuint16 aPort);

		/**	Returns program's default factory.	*/
	static PMTCPSocketFactoryRef GetDefaultFactory();

		/**	Sets program's default factory.	*/
	static void SetDefaultFactory(const PMTCPSocketFactoryRef &aFactory);

		/**
		Returns a socket that will connect to the specified host and port 
		before any other action. Implemented by sub-classes.
		*/
	virtual PMTCPSocketRef NewSocketConnectedTo(const PMStrRef &aHost, pmuint16 aPort) = 0;

		/**
		Returns a socket that will connect to the specified TCP/IP address 
		and port before any other action. Implemented by sub-classes.
		*/
	virtual PMTCPSocketRef NewSocketConnectedTo(pmuint32 aPeerAddress, pmuint16 aPort) = 0;

		/**
		Returns a socket acceptor that will generate new socket as clients 
		connect themselves. If a NULL reference is returned, accept is not possible.
		*/
	virtual PMTCPSocketAcceptorRef NewAcceptorFor(pmuint16 aPort);

	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------

		/**	Returns the TCP/IP-compliant hostname.	*/
	virtual PMStrRef GetHostName();
};

// ===========================================================================
//	PMTCPSocketImplementation
// ===========================================================================
/**	This is the base class for implementating TCP/IP sockets.	*/


class PMTCPSocketImplementation : public PMRC
{
public:
		/**
		Class (static) method accessed via instance.
		[in order to be able to change implementations easily].
		*/
	virtual pmbool Resolve(const PMStrRef &aPeerName, pmuint32 *aPeerAddress) = 0;

		/**	Open the connection	*/
	virtual pmbool Connect(pmuint32 aPeerAddress, pmuint16 aDestinationPort) = 0;

		/**
		Close the connection
		If flag is true, the close should be as fast as possible (even if dirty).
		*/
	virtual pmbool Close(pmbool afAbort) = 0;

		/**	Returns true if data is avalaible	*/
	virtual pmbool IsDataAvalaible() = 0;

		/**
		Read method. Should block until data is avalaible if 'afBlock' is true.
		Returns 'pmnpos' if an error occured.
		*/
	virtual size_t Read(char *aBuffer, size_t aMaxSize, pmbool afBlock) = 0;

		/**
		Write method. Will block until data is sent to the lower layer.
		Will not wait for ACK from peer.
		*/
	virtual pmbool Write(const char *aBuffer, size_t aLen) = 0;

		/**	Information about buffer size. DEFAULT_BUF_SIZE  is 4 Kb by default.	*/
	virtual size_t GetBufferSize();

		/**
		Gets real state of the underlying socket.
		Values are pre-filled.
		*/
	virtual void GetRealState(PMSocket_eState *aState, pmbool *afCanRead, pmbool *afCanWrite);

		/**
		Cancels pending blocking code.
		Returns true if cancelled, no if not cancelled.
		In any case, the socket is unusable after that call,
		so the implementation can be a call to close()...
		*/
	virtual pmbool CancelBlockingCall();

		/**	Experimental for accepting connections	*/
		//	-1 means block
	virtual PMTCPSocketImplementationRef WaitForConnection(pmuint32 PM_NOTUSED(aMsTimeout))
		{ return 0; }

		/**	Returns the port to which the socket is bound.	*/
	virtual pmuint16 GetPort() = 0;

		/**	Returns the address of the socket or 0.	*/
	virtual PMStrRef GetDotAddress() = 0;

		/**	Returns the address of the remote server or 0.	*/
	virtual PMStrRef GetRemoteDotAddress() = 0;
};

// ===========================================================================
//	PMTCPSocket
// ===========================================================================
/**	This is the class that the socket factory should instantiate.	*/

class PMTCPSocket : public PMSocket
{
public:

	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**	Constructor for an already-connected socket.	*/
	PMTCPSocket(const PMTCPSocketImplementationRef &anIMP);

		/**	Constructor for a socket that is not yet connected.	*/
	PMTCPSocket(const PMTCPSocketImplementationRef &anIMP, pmuint32 aPeerAddress, pmuint16 aDestinationPort);

		/**	Constructor for a socket that is not yet resolved, not yet connected.	*/
	PMTCPSocket(const PMTCPSocketImplementationRef &anIMP, const PMStrRef &aPeerName, pmuint16 aDestinationPort);

		/**	Aborts socket at destruction.	*/
	virtual ~PMTCPSocket();

	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------

		/**	See: \Ref{PMSocket::DoStartup}.	*/
	virtual pmbool DoStartup();
	
		/**	See: \Ref{PMSocket::DoClose}.	*/
	virtual pmbool DoClose();
	
		/**	See: \Ref{PMSocket::DoImmediateClose}.	*/
	virtual void DoImmediateClose();
	
		/**	See: \Ref{PMSocket::DoIsDataAvalaible}.*/
	virtual pmbool DoIsDataAvalaible();
	
		/**	See: \Ref{ PMSocket::DoReadAppend}.	*/
	virtual PMDataRef DoReadAppend(const PMDataRef &aData, size_t aSizeMax = 0);
	
		/**	See: \Ref{ PMSocket::DoRead}.	*/
	virtual size_t DoRead(void *aBuffer, size_t aLen);
	
		/**	See: \Ref{ PMSocket::DoWrite}.	*/
	virtual pmbool DoWrite(const void *aData, size_t aLen);
	
		/**	See: \Ref{ PMSocket::DoUpdateState}.	*/
	virtual void DoUpdateState();
	
		/**	See: \Ref{ PMSocket::DoCancelBlockingCall}.	*/
	virtual pmbool DoCancelBlockingCall();

		/**	Returns the port used by the socket. Returns 0 on error.	*/
	virtual pmuint16 GetPort();
	
		/**	Returns the TCP/IP address used by the socket (in the form www.xxx.yyy.zzz) .	*/
	virtual PMStrRef GetDotAddress();

		/**	Returns the address of the remote server or 0.	*/
	virtual PMStrRef GetRemoteDotAddress();

		/**	Returns the best buffer size to use when reading or writing data.	*/
	virtual size_t GetBufferSize();

	// -----------------------------------------------------------------------
	//	Special
	// -----------------------------------------------------------------------

		/**
		Grants access to the underlying implementation.
		May return a NULL reference if the implementation
		is unavailable.
		*/
	virtual	PMTCPSocketImplementationRef GetImplementation();
	
protected:

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

	PMStrRef	itsPeerName;			//	Peer name
	pmbool		itsfNeedResolve;		//	If name needs to be resolved
	pmbool		itsfNeedConnect;		//	If socket needs to be connected
	pmuint32	itsPeerAddress;			//	IP address of peer
	pmuint16	itsDestinationPort;		//	Destination port

	PMTCPSocketImplementationRef itsImplementation;
};

// ===========================================================================
//	PMTCPSocketAcceptor
// ===========================================================================
/**
A socket acceptor for incoming TCP connections.
(i.e. using a socket to accept other socket connections).
*/

class PMTCPSocketAcceptor:public PMSocketAcceptor
{
public:

	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**	Constructor.	*/
	PMTCPSocketAcceptor(const PMTCPSocketImplementationRef &aIMP);

	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------

		/**	See: \Ref{PMSocketAcceptor::WaitForConnection}	*/
	virtual PMSocketRef	WaitForConnection(pmuint32 aMsTimeout = (pmuint32) -1);
	
		/**	See: \Ref{PMSocketAcceptor::Cancel}	*/
	virtual void		Cancel();

		/**	Returns the port used by the accepting socket. Returns 0 on error.	*/
	virtual pmuint16	GetPort();

protected:

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

	PMTCPSocketImplementationRef itsImplementation;
};

// ---------------------------------------------------------------------------

#endif // PMTCPSoc_h

