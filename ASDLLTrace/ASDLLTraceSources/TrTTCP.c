/*--------------------------------------------------------------------------*\
 * TrTTCP.c
 * Copyright (C) 1990 - 1997 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#include <winsock2.h>
#include "TrHelp.h"
#include "TrTTCP.h"

/*--------------------------------------------------------------------------*/

#define ATRACER	( (TTCPTracer*)aTracer )

struct _TWS
{
	HINSTANCE		itsHInst;
	int				itsRefCount;

	int				(__stdcall *_WSAStartup)(WORD, LPWSADATA);
	int				(__stdcall *_WSACleanup)(void);
	SOCKET			(__stdcall *_socket)(int, int, int);
	int				(__stdcall *_connect)(SOCKET, const struct sockaddr*, int);
	int				(__stdcall *_send)(SOCKET, const char*, int, int);
	int				(__stdcall *_closesocket)(SOCKET);
	struct hostent*	(__stdcall *_gethostbyname)(const char*);
	unsigned long	(__stdcall *_inet_addr)(const char*);
	unsigned short	(__stdcall *_htons)(unsigned short);
} gWS;

/*--------------------------------------------------------------------------*/

static int _DoLoadWinsock( void )
{
	WSADATA	theWD;

	if ( gWS.itsRefCount > 0 )
		goto _DoLoadWinsock_ok;

	gWS.itsHInst = LoadLibrary( "WSOCK32.DLL" );
	if (gWS.itsHInst == 0 )
		return 0;

	gWS._WSAStartup = (int (__stdcall*)(WORD, LPWSADATA))GetProcAddress(gWS.itsHInst, "WSAStartup");
	if (gWS._WSAStartup == 0)
		goto _DoLoadWinsock_failed;
	gWS._WSACleanup = (int (__stdcall*)(void))GetProcAddress(gWS.itsHInst, "WSACleanup");
	if (gWS._WSACleanup == 0)
		goto _DoLoadWinsock_failed;
	gWS._socket = (SOCKET (__stdcall *)(int, int, int))GetProcAddress(gWS.itsHInst, "socket");
	if (gWS._socket == 0)
		goto _DoLoadWinsock_failed;
	gWS._connect = (int	(__stdcall *)(SOCKET, const struct sockaddr*, int))GetProcAddress(gWS.itsHInst, "connect");
	if (gWS._connect == 0)
		goto _DoLoadWinsock_failed;
	gWS._send = (int (__stdcall *)(SOCKET, const char*, int, int))GetProcAddress(gWS.itsHInst, "send");
	if (gWS._send == 0)
		goto _DoLoadWinsock_failed;
	gWS._closesocket = (int (__stdcall *)(SOCKET))GetProcAddress(gWS.itsHInst, "closesocket");
	if (gWS._closesocket == 0)
		goto _DoLoadWinsock_failed;
	gWS._gethostbyname = (struct hostent* (__stdcall *)(const char*))GetProcAddress(gWS.itsHInst, "gethostbyname");
	if (gWS._gethostbyname == 0)
		goto _DoLoadWinsock_failed;
	gWS._inet_addr = (unsigned long (__stdcall *)(const char*))GetProcAddress(gWS.itsHInst, "inet_addr");
	if (gWS._inet_addr == 0)
		goto _DoLoadWinsock_failed;
	gWS._htons = (unsigned short (__stdcall *)(unsigned short))GetProcAddress(gWS.itsHInst, "htons");
	if (gWS._htons == 0)
		goto _DoLoadWinsock_failed;

	if (gWS._WSAStartup(MAKEWORD(2, 0), &theWD) != 0)
		goto _DoLoadWinsock_failed;

_DoLoadWinsock_ok:
	gWS.itsRefCount++;
	return 1;

_DoLoadWinsock_failed:
	FreeLibrary(gWS.itsHInst);
	memset(&gWS, 0, sizeof(gWS));
	return 0;
}

/*--------------------------------------------------------------------------*/

static void _DoUnloadWinsock( void )
{
	if ( --gWS.itsRefCount == 0 )
	{
		gWS._WSACleanup();
		FreeLibrary( gWS.itsHInst );
		memset( &gWS, 0, sizeof(gWS) );
	}
}

/*--------------------------------------------------------------------------*/

TTracer* TrTCP_Create( void* aParam )
{
	TTCPTracer*		theTracer;
	char*			theHost;
	WORD			thePort;
	DWORD			theIP;

	if ( !_DoLoadWinsock() )
		return 0;
	
	thePort = TCPTracer_Port;
	if ( aParam == 0 )
		theHost = "localhost";
	else
	{
		char* thePtr;
		
		theHost = (char*)aParam;
		thePtr = strchr( theHost, ':' );
		if ( thePtr != 0 )
		{
			*thePtr++ = 0;
			thePort = atoi( thePtr );
			if ( thePort == 0 )
				thePort = TCPTracer_Port;
		}
	}
	theIP = gWS._inet_addr( theHost );
	if ( theIP == INADDR_NONE )
	{
		struct hostent*	theHE;

		theHE = gWS._gethostbyname( theHost );
		if ( theHE == 0)
		{
			_DoUnloadWinsock( );
			return 0;
		}
		theIP = *(DWORD*)theHE->h_addr;
	}

	theTracer = malloc( sizeof( TTCPTracer ) );
	theTracer->itsfOpened = FALSE;
	theTracer->itsKind = TraceKind_Binary;
	theTracer->itsOpenProc = TrTCP_OpenProc;
	theTracer->itsCloseProc = TrTCP_CloseProc;
	theTracer->itsSendProc = TrTCP_SendProc;
	theTracer->itsDestroyProc = TrTCP_DestroyProc;

	theTracer->itsIP = theIP;
	theTracer->itsPort = gWS._htons( thePort );
	theTracer->itsSocket = INVALID_SOCKET;

	return (TTracer*)theTracer;
}

/*--------------------------------------------------------------------------*/

BOOL TrTCP_OpenProc( TTracer* aTracer )
{
	struct sockaddr_in	theSin;

	if ( aTracer->itsfOpened )
		return TRUE;

	ATRACER->itsSocket = gWS._socket( AF_INET, SOCK_STREAM, 0 );	
	if ( ATRACER->itsSocket == INVALID_SOCKET )
		return FALSE;

	theSin.sin_family = AF_INET;
	theSin.sin_port = ATRACER->itsPort;
	theSin.sin_addr.s_addr = ATRACER->itsIP;
	if ( gWS._connect( ATRACER->itsSocket, (struct sockaddr*)&theSin, sizeof( struct sockaddr_in ) ) == -1 )
	{
		gWS._closesocket( ATRACER->itsSocket );
		ATRACER->itsSocket = INVALID_SOCKET;
		return FALSE;
	}

	aTracer->itsfOpened = TRUE;
	return TRUE;
}

/*--------------------------------------------------------------------------*/

void TrTCP_CloseProc( TTracer* aTracer )
{
	if ( !aTracer->itsfOpened )
		return;

	gWS._closesocket( ATRACER->itsSocket );
	ATRACER->itsSocket = INVALID_SOCKET;
	
	aTracer->itsfOpened = FALSE;
}

/*--------------------------------------------------------------------------*/

void TrTCP_SendProc( TTracer* aTracer, TTraceSendParams* aParams )
{
	int		theResult;
	BYTE*	thePtr;
	size_t	theLen;

	if ( !aTracer->itsfOpened )
		return;

	thePtr = aParams->itsData;
	theLen = aParams->itsDataLen;
	while ( theLen > 0 )
	{
		theResult = gWS._send( ATRACER->itsSocket, thePtr, theLen, 0 );
		if ( theResult == SOCKET_ERROR )
			break;
		theLen -= theResult;
		thePtr += theResult;
	}

	aTracer->itsfFirstTrace = FALSE;

}

/*--------------------------------------------------------------------------*/

void TrTCP_DestroyProc( TTracer* aTracer )
{
	if ( aTracer->itsfOpened )
		TrTCP_CloseProc( aTracer );
	free( aTracer );

	_DoUnloadWinsock();
}
