// ===========================================================================
//	PMSocket.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMSocket.h"

// ---------------------------------------------------------------------------

#define PM_TRACEMODULE SOCKET
PM_DEFINE_TRACE_CODE( Info, 1 );
PM_DEFINE_TRACE_CODE( Debug, 2 );
PM_DEFINE_TRACE_CODE( Warn, 3 );

// ===========================================================================
//	PMSocketFactory
// ===========================================================================

// ---------------------------------------------------------------------------

pmbool PMSocketFactory::Init( void* )
{
	// Default implemementation: all is ok
	return pmtrue;
}

// ---------------------------------------------------------------------------

void PMSocketFactory::Close( void* )
{
	// Default implemementation: nothing to do
}

// ===========================================================================
//	PMSocket
// ===========================================================================

pmuint32 PMSocket::cID;

// ---------------------------------------------------------------------------
//	Create a socket

PMSocket::PMSocket() :
	PMMonitorable(PM_MAKEU32('ck','So'))
{	
	PM_API( TL("PMSocket::PMSocket()") );
	
	itsState = PMSocket_kNotConnected;
	itsfOkRead = pmfalse;
	itsfOkWrite = pmfalse;
	itsfCancelled = pmfalse;
	itsID = ++cID;
}

// ---------------------------------------------------------------------------
//	Create a socket in certain state

PMSocket::PMSocket( PMSocket_eState aState ) :
	PMMonitorable(PM_MAKEU32('ck', 'So')),
	itsState(aState)
{	
	PM_API( TL("PMSocket::PMSocket(PMSocket_eState)") );
	
	itsfOkRead = pmtrue;
	itsfOkWrite = pmtrue;
	itsfCancelled = pmfalse;
	itsID = ++cID;
}

// ---------------------------------------------------------------------------
//	Return state of the socket
//	If socket is connected, ask the underlying implementation to update status

PMSocket_eState PMSocket::GetState()
{	
	//	PM_API( "PMSocket::GetState()" );
	if (itsfCancelled)
		return PMSocket_kCancelled;
		
	if (itsState==PMSocket_kConnected)
		DoUpdateState();
		
	return itsState;
}

// ---------------------------------------------------------------------------
//	Do whatever is necessary to go to connected state

pmbool PMSocket::Startup()
{	
	PM_API( TL("PMSocket::Startup()") );
	
	if (itsfCancelled)
		return pmfalse;
		
	if (itsState == PMSocket_kNotConnected)
	{
		if (DoStartup())
		{	
			_SetState( PMSocket_kConnected );
		}
		else
		{	
			_SetState( PMSocket_kError );
			return pmfalse;
		}
	}
	
	return pmtrue;
}

// ---------------------------------------------------------------------------

pmbool PMSocket::Close()
{	
	PM_API( TL("PMSocket::Close()") );
	
	if (itsfCancelled)
	{
		ImmediateClose();
		return pmfalse;
	}
	
	if (itsState == PMSocket_kConnected)
	{	
		if (DoClose())
		{	
			_SetState( PMSocket_kClosed );
			return pmtrue;
		}
		_SetState( PMSocket_kError );
	}
	
	return pmfalse;
}

// ---------------------------------------------------------------------------

void PMSocket::ImmediateClose()
{	
	PM_API( TL("PMSocket::ImmediateClose()") );
	
	DoImmediateClose();	//	Should not _SetState(...) that calls ImmediateClose
	itsState = PMSocket_kError;
}

// ---------------------------------------------------------------------------

pmbool PMSocket::IsOkRead()
{
	//	PM_API( "PMSocket::IsOkRead()" );
	if (itsfCancelled)
		return pmfalse;
		
	DoUpdateState();	
	return itsState==PMSocket_kConnected && itsfOkRead;
}

// ---------------------------------------------------------------------------

pmbool PMSocket::IsOkWrite()
{	
	PM_API( TL("PMSocket::IsOkWrite()") );
	
	if (itsfCancelled)
		return pmfalse;
		
	DoUpdateState();
	return itsState==PMSocket_kConnected && itsfOkWrite;
}

// ---------------------------------------------------------------------------

pmbool PMSocket::IsDataAvalaible()
{
	//	PM_API( "PMSocket::IsDataAvalaible()" );

	if (!Startup())
		return pmfalse;
		
	if (itsState==PMSocket_kConnected)
		return DoIsDataAvalaible();
		
	return pmtrue;
}

// ---------------------------------------------------------------------------

PMDataRef PMSocket::Read(PMDataRef aData, size_t aSizeMax)
{	
	//	PM_API( "PMSocket::Read()" );
	PM_TRACE(Debug, TL("PMSocket::Read(): sizeMax = %lu", (unsigned long)aSizeMax));

		//	Start eventually
	if (!Startup())
	{	
		PM_TRACE(Debug, TL("PMSocket::Read(): Cannot start"));
		return 0;
	}

		//	Prepare the buffer
	PMDataRef theData = aData;
	if (theData.IsNull())
		theData = new PMData();	//	We may allocate data from a shared pool

	return _ReadAppend( theData, aSizeMax );
}

// ---------------------------------------------------------------------------

size_t PMSocket::Read( void *aBuffer, size_t aLen )
{	
		//	Start eventually
	if (!Startup())
		return 0;

	return DoRead( aBuffer, aLen );
}

// ---------------------------------------------------------------------------

PMDataRef PMSocket::ReadAll(PMDataRef aData )
{	
	PM_API( TL("PMSocket::ReadAll()") );
	
		//	Start eventually	
	if (!Startup())
		return 0;

		//	Prepare the buffer
	PMDataRef theData = aData;
	if (theData.IsNull())
		theData = new PMData();	//	We may allocate data from a shared pool
	theData->SetSize(0);

	while (IsOkRead())
		_ReadAppend( theData );

	return theData;
}

// ---------------------------------------------------------------------------

pmbool PMSocket::Write(const PMDataRef &aData )
{	
	PM_API( TL("PMSocket::Write()") );
	
		//	Start eventually
	if (!Startup())
		return pmfalse;

	return Write( aData->buffer(), aData->size() );
}
// ---------------------------------------------------------------------------

pmbool PMSocket::Write(const void *aData, size_t aLen )
{	
	PM_API( TL("PMSocket::Write(<buffer>)") );
	
		//	Start eventually
	if (!Startup())
		return pmfalse;

		//	Special case, we should _never_ write zero bytes
	if (aLen == 0)
		return pmtrue;

	return DoWrite( aData, aLen );
}

// ---------------------------------------------------------------------------

void PMSocket::Cancel()
{	
	PM_API( TL("PMSocket::Cancel()") );
	
	if (itsfCancelled || itsState == PMSocket_kError || itsState == PMSocket_kClosed)
		return;
	
		// before CancelBlockingCall for another thread not restarting a call ignoring cancelled
	itsfCancelled = pmtrue; 
	DoCancelBlockingCall();
	
		// #### See if must not wait for real cancelling of the call
}

// ---------------------------------------------------------------------------

PMError PMSocket::GetLastError()
{	
	return itsError;
}

// ---------------------------------------------------------------------------
//	Private implementation 
//	Change state, close if goes in error

void PMSocket::_SetState( PMSocket_eState aState )
{	
	if (itsState != aState && aState == PMSocket_kError)
		ImmediateClose();
		
	itsState = aState;	
}

// ---------------------------------------------------------------------------
//	Read data, append it to aData

PMDataRef PMSocket::_ReadAppend( const PMDataRef &aData, size_t aSizeMax )
{
		//	#### An optimisation should be to try several times
		//	if IsDataAvalaible() after the first read...
	return DoReadAppend( aData, aSizeMax );
}

// ---------------------------------------------------------------------------
//	#### Je ne sais pas si cela doit etre appelle avant ou apres un changement d'etat

void PMSocket::DoUpdateState()
{	
	//	Default empty implementation
}

// ---------------------------------------------------------------------------

pmuint32 PMSocket::GetID()
{
	return itsID;
}
