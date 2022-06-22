/*--------------------------------------------------------------------------*\
 * TrHelp.c
 * Copyright (C) 1990 - 1996 AbyreSoft - All rights reserved.
 *--------------------------------------------------------------------------*
 * Add the user function : Trace_SendString()
 * to avoid any uses of _vsnprintf and so one. Let the user do the work 
 * usefull on the new Developement plateform .NET FrameWork
\*--------------------------------------------------------------------------*/

#include "TrHelp.h"
#include "TrInit.h"

#include <stdio.h>

/*--------------------------------------------------------------------------*/

TTracer*			gTracer;
int					gfTracesActivated;
int					gTracesInited;
CRITICAL_SECTION	gSendCS;

/*--------------------------------------------------------------------------*\
    Message format:

    vvXXXXXXXXXXXXXX
    vv Version of trace protocol (2 bytes: Minor, Major)

    ================================================
    For version 1.0 (0001XX...XX)
    XX...XX = NNNNp0p1...pn
    NNNN : total size of the message (including vvNNNN) (4 bytes)
    pi :	Message parameters
	    Format:	cnndd..dd
			    c	: Parameter opcode (1 byte)
			    n	: nn parameter data len (excluding cnn)
			    d..d: parameter data

    Parameters valid opcodes:
	    1 : Attributes: xxxxt
		    xxxx = module<<16 + code - 32 bits
		    t = kind (0 = normal, 1 = EOT, 2 = binary) - 8 bits
	    2 : Indentation level: i (8 bits)
	    3 :	Process ID
	    4 : Thread ID
	    5 : Trace Data
	    6 :	Time Stamp.

	    128 : App Name.

	    0 : End of message. Every message must end with this parameter
		    As parameter data len is 0, the message MUST end with 3 0 bytes

    Take care that 16 bits and 32 bits values are transmitted in network 
    byte order (big-endian) over the network.

    -----------------------------------------------------------------------
    Create data will not append the trailing null parameter.
    That will allow to subclass it, call the base class implementation,
    and then append more parameters. The final null parameter is added by 
    Send just before calling SendChunk.
    Tough, the total size of the message is already set to take into account
    the trailing 3 bytes that will be added so you will have to only add your
    parameter size to the message size.

    -----------------------------------------------------------------------
    Version matching is done on the major only so increment the major
    when a change made this incompatible with the reporter.

\*--------------------------------------------------------------------------*/

#define TRACE_VERSION	0x0100

/*--------------------------------------------------------------------------*/

static void Trace_PrepareBuffer( TTraceSendParams* aParams )
{
	BYTE	theByte;
	WORD	theU16;
	DWORD	theU32;
	BYTE	*thePtr, *theDataPtr;
	size_t	theSize, theDataLen;

	#define ADD_BYTE(b)	theByte = (b); *thePtr++ = theByte;
	#define ADD_U16(s)	theU16 = (s); *thePtr++ = (BYTE)(theU16 >> 8); *thePtr++ = (BYTE)theU16;
	#define ADD_U32(l)	theU32 = (l); *thePtr++ = (BYTE)(theU32 >> 24); *thePtr++ = (BYTE)(theU32 >> 16); *thePtr++ = (BYTE)(theU32 >> 8); *thePtr++ = (BYTE)theU32;

	theDataPtr = aParams->itsData;
	theDataLen = aParams->itsDataLen;

	theSize = 2 + 4 + 8 + 4 + 7 + 7 + 7 + 3 + 3 + aParams->itsDataLen;
	/* Code + Len + strlen + 1 */
	if (gTracer->itsfFirstTrace)
		theSize += 4 + strlen(sAppName);

	thePtr = aParams->itsData = (BYTE*)malloc(theSize);
	aParams->itsDataLen = theSize;

	// Version
	ADD_U16(TRACE_VERSION);

	// Reserve place for length
	ADD_U32(aParams->itsDataLen);

	// Attributes
	ADD_BYTE(TraceOpcode_Attributes); ADD_U16(5);
	ADD_U32(aParams->itsIdent);
	ADD_BYTE(aParams->itsKind);

	// Indentation level
	ADD_BYTE(TraceOpcode_IndentLevel); ADD_U16(1);
	ADD_BYTE(aParams->itsIndentLevel);

	// Time stamp
	ADD_BYTE(TraceOpcode_TimeStamp); ADD_U16(4);
	// So in the tracer we can assume that a null value means value not present.
	// The bias is 1 millisec only if the system timer is 0 (one millec every 49.7 days)
	if (aParams->itsTimeStamp == 0)
		aParams->itsTimeStamp++;
	ADD_U32(aParams->itsTimeStamp);

	// Thread ID and Process ID
	ADD_BYTE(TraceOpcode_ProcessID); ADD_U16(4);
	ADD_U32(GetCurrentProcessId());

	ADD_BYTE(TraceOpcode_ThreadID); ADD_U16(4);
	ADD_U32(GetCurrentThreadId());

	if (gTracer->itsfFirstTrace)
	{
		size_t theLen = strlen(sAppName) + 1;
		ADD_BYTE(TraceOpcode_AppName); ADD_U16(theLen);
		memcpy(thePtr, sAppName, theLen);
		thePtr += theLen;
	}

	// Trace Data. The trailing 0 will be transmitted over the network...
	ADD_BYTE(TraceOpcode_Data); ADD_U16(theDataLen);
	memcpy(thePtr, theDataPtr, theDataLen);
	thePtr += theDataLen;

	memset(thePtr, 0, 3);
}

/*--------------------------------------------------------------------------*/

#define kChunkSize	3900

void Trace_Send(TTraceSendParams* aParams)
{
	size_t		theSize, theOrgSize;
	BYTE		*theData, *theOrgData;
	TraceKind	theKind;

	EnterCriticalSection(&gSendCS);
	if (!gTracer->itsfOpened)
	{
		if (gTracer->itsOpenRetryCount == 0)
			goto Trace_Send_end;
		if (GetTickCount() - gTracer->itsLastOpenTryTime < gTracer->itsOpenRetryDelay)
			goto Trace_Send_end;
		gTracer->itsOpenRetryCount--;
		gTracer->itsLastOpenTryTime = GetTickCount();
		if (!gTracer->itsOpenProc(gTracer))
			goto Trace_Send_end;
	}

	if (gTracer->itsKind == TraceKind_Normal)
	{
		gTracer->itsSendProc(gTracer, aParams);
		goto Trace_Send_end;
	}

	theSize = theOrgSize = aParams->itsDataLen;
	theData = theOrgData = aParams->itsData;
	theKind = aParams->itsKind;
	
	while (theSize != 0)
	{
		if (theSize > kChunkSize)
		{
			aParams->itsDataLen = kChunkSize;
			// If the trace is a EOT, only the last chunk must be sent this way
			if (theKind == TraceKind_EOL)
				aParams->itsKind = TraceKind_Normal;
			theSize -= kChunkSize;
		}
		else
		{
			aParams->itsDataLen = theSize;
			// Reset the original kind in case it has been changed.
			aParams->itsKind = theKind;
			theSize = 0;
		}

		aParams->itsData = theData;
		theData += aParams->itsDataLen;

		Trace_PrepareBuffer(aParams);
		gTracer->itsSendProc(gTracer, aParams);

		free(aParams->itsData);
	}
	aParams->itsData = theOrgData;
	aParams->itsDataLen = theOrgSize;

Trace_Send_end:
	LeaveCriticalSection(&gSendCS);
}

/*--------------------------------------------------------------------------*/

void Trace_Close( TTracer* aTracer )
{
	aTracer->itsCloseProc(aTracer);
	aTracer->itsOpenRetryCount = aTracer->itsMaxOpenRetryCount;
	aTracer->itsLastOpenTryTime = GetTickCount() - aTracer->itsOpenRetryDelay;
	aTracer->itsfFirstTrace = TRUE;
}

/*----------------------------------------------------------------------------*/

void Trace_SendText( TTraceSendParams* aParams, char* aFormat, va_list* anArgs )
{
	int	theLen;

	aParams->itsData = (BYTE*)malloc(SPRINTF_BUFFER_SIZE);
	aParams->itsIndentLevel = 0;
	aParams->itsTimeStamp = timeGetTime();

	theLen = _vsnprintf((char*)aParams->itsData, SPRINTF_BUFFER_SIZE, aFormat, *anArgs);
	if (theLen > 0)
	{
		aParams->itsDataLen = (size_t)theLen;
		Trace_Send(aParams);
	}
	free(aParams->itsData);
}

/*--------------------------------------------------------------------------*/

void Trace_SendString( TTraceSendParams *aParams, char *aString )
{
	int	theLen;

	aParams->itsData = (BYTE*)malloc( SPRINTF_BUFFER_SIZE );
	aParams->itsIndentLevel = 0;
	aParams->itsTimeStamp = timeGetTime();

    theLen = strlen( aString ) + 1;

	if ( theLen > 1 )
	{
        strcpy( (char *)aParams->itsData, aString );
		aParams->itsDataLen = (size_t)theLen;
		Trace_Send( aParams );
	}
	free( aParams->itsData );
}

