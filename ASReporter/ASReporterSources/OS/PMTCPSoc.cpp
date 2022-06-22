// ===========================================================================
//	PMTCPSoc.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMTCPSoc.h"

// ---------------------------------------------------------------------------

#define PM_TRACEMODULE SOCKET
PM_DEFINE_TRACE_CODE( Info, 1 );

// ===========================================================================
//	PMTCPSocketFactory
// ===========================================================================

static PMTCPSocketFactoryRef gTCPFactory;
static pmbool gTCPFactoryRefRegistered;

// ---------------------------------------------------------------------------

PMTCPSocketFactoryRef PMTCPSocketFactory::GetDefaultFactory()
{	
	return gTCPFactory;
}

// ---------------------------------------------------------------------------

void PMTCPSocketFactory::SetDefaultFactory(const PMTCPSocketFactoryRef &aFactory)
{	
	if (!gTCPFactoryRefRegistered)
	{
		PMRC::RegisterRefCleanUp(&::gTCPFactory);
		gTCPFactoryRefRegistered = pmtrue;
	}
	
	gTCPFactory = aFactory;
}

// ---------------------------------------------------------------------------

PMTCPSocketRef PMTCPSocketFactory::NewDefaultSocketConnectedTo(const PMStrRef &aHost, pmuint16 aPort)
{	
	return gTCPFactory->NewSocketConnectedTo(aHost, aPort);
}

// ---------------------------------------------------------------------------

PMTCPSocketRef PMTCPSocketFactory::NewDefaultSocketConnectedTo(pmuint32 aPeerAddress, pmuint16 aPort)
{	
	return gTCPFactory->NewSocketConnectedTo(aPeerAddress, aPort);
}

// ---------------------------------------------------------------------------

PMTCPSocketAcceptorRef PMTCPSocketFactory::NewDefaultAcceptorFor(pmuint16 aPort)
{	
	return gTCPFactory->NewAcceptorFor(aPort);
}

// ---------------------------------------------------------------------------

PMStrRef PMTCPSocketFactory::GetHostName()
{	
	// If the subclass can do better (is is really necessary ?)
	return PMSTRSTATIC("localhost");
}

// ---------------------------------------------------------------------------

PMTCPSocketAcceptorRef PMTCPSocketFactory::NewAcceptorFor(pmuint16 )
{	
	//	Default implemementation: No way to accept
	return 0;	
}


// ===========================================================================
//	PMTCPSocketImplementation
// ===========================================================================

	//	Default read buffer size
const size_t PMTCPSocketImplementation_kDefaultBufferSize = 4096;	

// ---------------------------------------------------------------------------

size_t PMTCPSocketImplementation::GetBufferSize()
{	
	return PMTCPSocketImplementation_kDefaultBufferSize;
}

// ---------------------------------------------------------------------------

void PMTCPSocketImplementation::GetRealState(PMSocket_eState *, pmbool *, pmbool *)
{
}

// ---------------------------------------------------------------------------

pmbool PMTCPSocketImplementation::CancelBlockingCall()
{
	return Close(pmtrue);
}

// ===========================================================================
//	PMTCPSocket
// ===========================================================================

// ---------------------------------------------------------------------------
//	Already connected socket

PMTCPSocket::PMTCPSocket(const PMTCPSocketImplementationRef &anIMP):
		PMSocket(PMSocket_kConnected)
{	
	itsImplementation = anIMP;
	itsfNeedResolve = pmfalse;
	itsfNeedConnect = pmfalse;
}

// ---------------------------------------------------------------------------
//	To be connected socket

PMTCPSocket::PMTCPSocket(const PMTCPSocketImplementationRef &anIMP, pmuint32 aPeerAddress, pmuint16 aPort) :
		PMSocket(PMSocket_kNotConnected)
{	
	itsImplementation = anIMP;
	itsfNeedResolve = pmfalse;
	itsfNeedConnect = pmtrue;
	itsPeerAddress = aPeerAddress;
	itsDestinationPort = aPort;
}

// ---------------------------------------------------------------------------
//	To be resolved, to be connected socket

PMTCPSocket::PMTCPSocket(const PMTCPSocketImplementationRef &anIMP, const PMStrRef &aPeerName, pmuint16 aDestinationPort) :
		PMSocket(PMSocket_kNotConnected)
{	
	itsImplementation = anIMP;
	itsfNeedResolve = pmtrue;
	itsfNeedConnect = pmtrue;
	itsPeerName = aPeerName;
	itsDestinationPort = aDestinationPort;
}

// ---------------------------------------------------------------------------
//	Nothing to do at destruction

PMTCPSocket::~PMTCPSocket()
{
}

// ---------------------------------------------------------------------------
//	Startup of the socket
//	If needed resolve name and connect

pmbool PMTCPSocket::DoStartup()
{	
	if (itsfNeedResolve)
	{	
		MonitorOut(MakeTextInfo(PMSTRSTATIC("Resolving: ") + itsPeerName));
		if (!itsImplementation->Resolve(itsPeerName, &itsPeerAddress))
		{	
			itsError = kPMSocketDNSError;
			return pmfalse;
		}
	}
	
	if (itsfNeedConnect)
	{	
		MonitorOut(MakeTextInfo("Connecting"));
		if (!itsImplementation->Connect(itsPeerAddress, itsDestinationPort))
		{	
			itsError = kPMSocketConnectError;
			return pmfalse;
		}
	}

	return pmtrue;
}


// ---------------------------------------------------------------------------

pmbool PMTCPSocket::DoClose()
{	
	if (itsImplementation->Close(pmfalse))
	{	
		_SetState(PMSocket_kClosed);
		return pmtrue;
	}
	
	_SetState(PMSocket_kError);
	itsError = kPMSocketUnknownError;
	return pmtrue;
}

// ---------------------------------------------------------------------------

void PMTCPSocket::DoImmediateClose()
{	
	itsImplementation->Close(pmtrue);
	_SetState(PMSocket_kClosed);		//	#### Or should it be error ?
}

// ---------------------------------------------------------------------------

pmbool PMTCPSocket::DoIsDataAvalaible()
{	
	return itsImplementation->IsDataAvalaible();
}

// ---------------------------------------------------------------------------

PMDataRef PMTCPSocket::DoReadAppend(const PMDataRef &aData, size_t aSizeMax)
{		
	size_t theDataSize = aData->size();
	if ((theDataSize >= aSizeMax) && (aSizeMax != 0))
		return aData;

		//	Prepare fresh space for data
	size_t theImpBufSize = itsImplementation->GetBufferSize(), theBytesCount;
	if (aSizeMax)
	{
		// If a size max was specified, use it, but never overrun the implementation buffer size
		theBytesCount = aSizeMax - theDataSize;
		if (theBytesCount > theImpBufSize)
			theBytesCount = theImpBufSize;
	}
	else
		theBytesCount = theImpBufSize;

	aData->SetSize(theDataSize + theBytesCount);
	char *thePtr = (char *)aData->buffer();
	thePtr += theDataSize;

		//	Blocking read of at most theByteCount
	size_t theRealRead = itsImplementation->Read(thePtr, theBytesCount, pmfalse);

	if (theRealRead == pmnpos)
	{
		_SetState(PMSocket_kError);
		itsError = kPMSocketReadError;
		theRealRead = 0;
	}
	else
		if (theRealRead == 0)
			//	We'll return by the normal way...
			itsfOkRead = pmfalse;

	aData->SetSize(theDataSize + theRealRead);

	if (theRealRead == 0)
		return 0;

	return aData;
}

// ---------------------------------------------------------------------------

size_t PMTCPSocket::DoRead(void *aBuffer, size_t aLen)
{	
	size_t theRealRead = itsImplementation->Read((char *)aBuffer, aLen, pmfalse);

	if (theRealRead == pmnpos)
	{	
		_SetState(PMSocket_kError);
		itsError = kPMSocketReadError;
		return 0;
	}

	if(theRealRead==0)
	{	
		itsfOkRead = pmfalse;
		return 0;
	}

	return theRealRead;
}

// ---------------------------------------------------------------------------

pmbool PMTCPSocket::DoWrite(const void *aData, size_t aLen)
{	
	if (itsImplementation->Write((const char *)aData, aLen))
		return pmtrue;
		
	_SetState(PMSocket_kError);
	itsError = kPMSocketWriteError;
	return pmfalse;
}

// ---------------------------------------------------------------------------

void PMTCPSocket::DoUpdateState()
{	
	if (itsState == PMSocket_kConnected)
	{
		itsImplementation->GetRealState(&itsState, &itsfOkRead, &itsfOkWrite);
		if (!itsfOkRead && !itsfOkWrite)
			Close();
	}
}

// ---------------------------------------------------------------------------

pmbool PMTCPSocket::DoCancelBlockingCall()
{	
	return itsImplementation->CancelBlockingCall();
}

// ---------------------------------------------------------------------------

pmuint16 PMTCPSocket::GetPort()
{	
	return itsImplementation->GetPort();
}

// ---------------------------------------------------------------------------

PMStrRef PMTCPSocket::GetDotAddress()
{	
	return itsImplementation->GetDotAddress();
}

// ---------------------------------------------------------------------------

PMStrRef PMTCPSocket::GetRemoteDotAddress()
{	
	return itsImplementation->GetRemoteDotAddress();
}

// ---------------------------------------------------------------------------

size_t PMTCPSocket::GetBufferSize()
{	
	return itsImplementation->GetBufferSize();
}

// ---------------------------------------------------------------------------

PMTCPSocketImplementationRef PMTCPSocket::GetImplementation()
{
	return itsImplementation;
}

// ===========================================================================
//	PMTCPSocketAcceptor
// ===========================================================================

PMTCPSocketAcceptor::PMTCPSocketAcceptor(const PMTCPSocketImplementationRef &aIMP)
{	
	itsImplementation = aIMP;
}

// ---------------------------------------------------------------------------

PMSocketRef PMTCPSocketAcceptor::WaitForConnection(pmuint32 aMsTimeout)
{	
	PMTCPSocketImplementationRef theImp = itsImplementation->WaitForConnection(aMsTimeout);
	if (!theImp.IsNull())
		return new PMTCPSocket(theImp);
	
	return 0;
}

// ---------------------------------------------------------------------------

void PMTCPSocketAcceptor::Cancel()
{	
	itsImplementation->CancelBlockingCall();
}

// ---------------------------------------------------------------------------

pmuint16 PMTCPSocketAcceptor::GetPort()
{	
	return itsImplementation->GetPort();
}


