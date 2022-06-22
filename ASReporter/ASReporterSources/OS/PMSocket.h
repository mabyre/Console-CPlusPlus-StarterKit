// ===========================================================================
//	PMSocket.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMSocket_h
#define PMSocket_h

// ---------------------------------------------------------------------------

#include "PMRC.h"
#include "PMData.h"
#include "PMStr.h"
#include "PMError.h"
#include "PMMonit.h"

// ===========================================================================
//	PMSocketFactory
// ===========================================================================

PMDEFINE(PMSocketFactory);

// ---------------------------------------------------------------------------
/**	Base class for socket factories.	*/

class PMSocketFactory : public PMRC
{
public:
		/**
		Initializes the socket factory and returns true if all is Ok.
		Sub-classes should add their code here to perform operating  
		system-dependant initialization.
		*/
	virtual pmbool	Init(void *aParam = 0);
	
		/**
		Finalizes the socket factory.
		Sub-classes should add their code here to perform operating  
		system-dependant finalization.
		*/
	virtual void	Close(void *aParam = 0);
};

// ===========================================================================
//	PMSocket
// ===========================================================================

	/**	Socket: Error class code.	*/
const pmuint16 kPMSocketErrorClass = 0x10;

	/**	Socket: Unknown error.	*/
const pmuint32 kPMSocketUnknownError	= 0x100001;

	/**	Socket: DNS resolution error.	*/
const pmuint32 kPMSocketDNSError		= 0x100002;

	/**	Socket: Cannot connect to the remote peer.	*/
const pmuint32 kPMSocketConnectError	= 0x100003;

	/**	Socket: Read error. The connection may be down.	*/
const pmuint32 kPMSocketReadError		= 0x100004;

	/**	Socket: Write error. The connection may be down.	*/
const pmuint32 kPMSocketWriteError		= 0x100005;

// ---------------------------------------------------------------------------

	/**	Type for the socket state.	*/
typedef pmuint16 PMSocket_eState;

	/** Socket state: Not connected.	*/
const PMSocket_eState PMSocket_kNotConnected	= 1;

	/** Socket state: Connected, ready for read and write operations.	*/
const PMSocket_eState PMSocket_kConnected		= 2;

	/** Socket state: An error has occurred.	*/
const PMSocket_eState PMSocket_kError			= 3;

	/** Socket state: Closed.	*/
const PMSocket_eState PMSocket_kClosed			= 4;

	/** Socket state: A call to the socket has been cancelled. 	*/
const PMSocket_eState PMSocket_kCancelled		= 5;

// ---------------------------------------------------------------------------

PMDEFINE(PMSocket);

// ---------------------------------------------------------------------------
/**
A socket is a communication channel between two endpoints.
A socket can have several states.
kNotConnected : The socket is not connected (with whatever consequences for an implementation).
Startup() has to be called for the socket to be usable. Startup() may be called several times without harm.
Startup() can block, so any non-blocking routine called while state is 'kNotConnected'
may block.
kConnected :	The socket is connected and usable.
kError :		The socket is in error. Many functions return "error state defaults"
This state cannot be cleared, the socket is not usable again.
A socket has two communication channels (in/out), and each channel has its own state.
For now those states are limited to boolean functions, called 'IsOkRead()'
and 'IsOkWrite()'. These functions return false if there is no way for the associated
operation (read and write) to succeed.
*/

class PMSocket : public PMRC, public PMMonitorable
{
public:
		/**
		Reads all data until an error occurs.
		Always returns a 'PMData' (which can be empty).
		*/
	virtual PMDataRef ReadAll(PMDataRef aData = 0);

	// ---------------------------------------------------------------------------
	//	Public interface
	// ---------------------------------------------------------------------------

		/**	Default constructor. The socket is not connected.	*/
	PMSocket();

		/** Returns socket state.	*/
	virtual PMSocket_eState GetState();

		/**
		Finishes socket startup (ie: connection) if needed.
		Returns true if successful (state is 'PMSocket_kConnected').
		It is safe to call 'Startup' several times.
		If not called, it will be called when needed.
		*/
	virtual pmbool Startup();

		/**
		Closes the socket. Returns true if succesful.
		May block. May be called even if '!IsOkRead' or/and '!IsOkWrite'.
		Operates only in the state 'PMSocket_kConnected'.
		If socket is currently cancelled it "aborts" (ie: calls 'ImmediateClose')
		*/
	virtual pmbool Close();								

		/**
		Closes the socket "ungracefully". Will not block.
		This is automatically called when the reference count falls to zero.
		(Which is not exactly the same as called at destruction...)
		This method is called when a socket gets an error.
		*/
	virtual void ImmediateClose();						

		/**	
		Returns if a read can be issued on the socket. (ie: state is at 
		least 'PMSocket_kConnected').
		*/
	virtual pmbool IsOkRead();							

		/**	Returns if a write can be issued on the socket.	*/
	virtual pmbool IsOkWrite();							

		/**
		Returns if data is available.
		If socket is not in connected state, then it connects first.
		If socket is in error state, this method returns true, avoiding code which could  
		loop indefinitely under certain conditions:
		while (!theSocket->IsDataAvalaible())
		::DoSomething();
		theSocket->Read(...)
		*/
	virtual pmbool IsDataAvalaible();					

		/**
		Appends data to a 'PMData' or creates new one (if 'aData' is 0).
		'aSizeMax' is the maximum size of the returned 'PMData' or 0 (no limit).
		Returns a NULL reference if an error occurs.
		Will block if no data is avalaible.
		*/
	virtual PMDataRef Read(PMDataRef aData = 0, size_t aSizeMax = 0);

		/**	Blocking read of at most 'aLen' bytes in 'aBuffer'.	*/
	virtual size_t Read(void *aBuffer, size_t aLen);

		/**	Writes the content of 'aData'. Returns true if OK.	*/
	virtual pmbool Write(const PMDataRef &aData);

		/**	Writes 'aLen' bytes from 'aBuffer'. Returns true if OK.	*/
	virtual pmbool Write(const void *aBuffer, size_t aLen);

		/**	Cancels any pending blocking calls.	*/
	virtual void Cancel();

		/** Returns specific error: DNS, connect, aborted, etc. */
	virtual PMError GetLastError();

protected:

	// ---------------------------------------------------------------------------
	//	Implementation Interface
	// ---------------------------------------------------------------------------

	PMError itsError;

	PMSocket_eState	itsState;
	pmbool			itsfOkRead;
	pmbool			itsfOkWrite;
	pmbool			itsfCancelled;
	pmuint32		itsID;

	static pmuint32	cID;

		/**	Constructor.	*/
	PMSocket(PMSocket_eState aState);

		/**	Change state.	*/
	virtual void _SetState(PMSocket_eState aState);

		/**	Blockread what is avalaible.	*/
	virtual PMDataRef _ReadAppend(const PMDataRef &aData, size_t aSizeMax = 0);

	// ---------------------------------------------------------------------------
	//	To be implemented by subclasses
	// ---------------------------------------------------------------------------

	virtual pmbool		DoStartup() = 0;
	
	virtual pmbool		DoClose() = 0;
	virtual void		DoImmediateClose() = 0;
	
	virtual pmbool		DoIsDataAvalaible() = 0;
	
	virtual PMDataRef	DoReadAppend(const PMDataRef &, size_t aSizeMax = 0) = 0;
	virtual size_t		DoRead(void *aBuffer, size_t aLen) = 0;
	
	virtual pmbool		DoWrite(const void *aData, size_t aLen) = 0;
	
	virtual void		DoUpdateState();		
	
	virtual pmbool		DoCancelBlockingCall() = 0;

public:
		/**
		Return a pmuint32 identifying the socket.
		This is just a helper for debugging multi-threaded multi-socket applications.
		*/
	virtual pmuint32 GetID();
};

// ===========================================================================
//	PMSocketAcceptor
// ===========================================================================

PMDEFINE(PMSocketAcceptor);

// ---------------------------------------------------------------------------
/**	A factory used to accept incoming connections.	*/

class PMSocketAcceptor : public PMRC
{
public:
		/**
		Waits for an incoming connection. Returns a NULL reference if no socket can 
		be created.
		'aMSTimeout' is given in milliseconds, but there is no guarantee that
		the precision will be honored. The default timeout value is infinite.
		*/
	virtual PMSocketRef WaitForConnection(pmuint32 aMSTimeout = (pmuint32) -1) = 0;

		/**	Cancel a pending 'WaitForConnection' call.	*/
	virtual void Cancel() = 0;

		/**	Returns the last error that occured.	*/
	virtual PMError GetLastError() { return 0; }
};

// ---------------------------------------------------------------------------

#endif	// PMSocket_h

