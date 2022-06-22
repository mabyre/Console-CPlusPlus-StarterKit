/*--------------------------------------------------------------------------*\
 * ASTrace.c
 * Copyright (C) 1990 - 1996 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#include "ASTrace.h"
#include "TrHelp.h"
#include "TrInit.h"

#include "mmsystem.h"
#pragma comment(lib, "WINMM")

/*--------------------------------------------------------------------------*/

ASTRACEAPI void __stdcall as_trace_init( HINSTANCE anInstance )
{
	if ( ++gTracesInited == 1 )
		InitTraces( anInstance );
}

/*--------------------------------------------------------------------------*/

ASTRACEAPI void __stdcall as_trace_close( void )
{
	if (--gTracesInited == 0)
		CloseTraces();
}

/*--------------------------------------------------------------------------*/

ASTRACEAPI void __stdcall as_trace_VT( WORD aModule, char* aFormatString, va_list* anArgs )
{
	TTraceSendParams	theParams;

	if ( gTracer == 0 )
		return;

	theParams.itsKind = TraceKind_Normal;
	theParams.itsIdent = aModule << 16;
	Trace_SendText(&theParams, aFormatString, anArgs);
}

/*--------------------------------------------------------------------------*/

ASTRACEAPI void __stdcall as_trace_T( WORD aModule, char* aFormatString, ... )
{
	va_list theList;
	va_start(theList, aFormatString);

	as_trace_VT(aModule, aFormatString, &theList);

	va_end(theList);
}

/*--------------------------------------------------------------------------*/

ASTRACEAPI void __stdcall as_trace_VTL( WORD aModule, char* aFormatString, va_list* anArgs )
{
	TTraceSendParams theParams;

	if ( gTracer == 0 )
		return;

	theParams.itsKind = TraceKind_EOL;
	theParams.itsIdent = aModule << 16;
	Trace_SendText( &theParams, aFormatString, anArgs );
}

/*--------------------------------------------------------------------------*/

ASTRACEAPI void __stdcall as_trace_TL( WORD aModule, char* aFormatString, ... )
{
	va_list theList;
	va_start( theList, aFormatString );

	as_trace_VTL( aModule, aFormatString, &theList );

	va_end( theList );
}

/*--------------------------------------------------------------------------
** in C charp we can not use va_start
*/

ASTRACEAPI void __stdcall as_trace_STR( WORD aModule, char *aString )
{
	TTraceSendParams theParams;

	if ( gTracer == 0 )
		return;

	theParams.itsKind = TraceKind_EOL;
	theParams.itsIdent = aModule << 16;
	Trace_SendString( &theParams, aString );
}

/*--------------------------------------------------------------------------*/

ASTRACEAPI void __stdcall as_trace_B( WORD aModule, BYTE* aBuffer, size_t aBufferLen )
{
	TTraceSendParams theParams;

	if ( gTracer == 0 )
		return;

	if ( aBufferLen > 0 )
	{
		theParams.itsData = aBuffer;
		theParams.itsKind = TraceKind_Binary;
		theParams.itsIdent = aModule << 16;
		theParams.itsDataLen = aBufferLen;
		theParams.itsIndentLevel = 0;
		theParams.itsTimeStamp = timeGetTime();
		Trace_Send( &theParams );
	}
}
