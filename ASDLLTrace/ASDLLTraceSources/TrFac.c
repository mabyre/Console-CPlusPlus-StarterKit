/*--------------------------------------------------------------------------*\
 * TrFac.c
 * Copyright (C) 1990 - 1996 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#include "TrFac.h"

#include "TrTFile.h"
#include "TrTStdEr.h"
#include "TrTTCP.h"

/*--------------------------------------------------------------------------*/

TTracer* __stdcall as_trace_create_instance( char* aName, char* aParam )
{
	TTracer*	theTracer = 0;

	if ( _stricmp( aName, "file" ) == 0 )
		theTracer = TrFile_Create( aParam );
	else
	if ( _stricmp( aName, "stderr" ) == 0)
		theTracer = TrStdErr_Create( aParam );
	else
	if ( _stricmp(aName, "tcp") == 0 )
		theTracer = TrTCP_Create( aParam );

	return theTracer;
}

/*--------------------------------------------------------------------------*/

void __stdcall as_trace_destroy_instance( TTracer* aTracer )
{
	aTracer->itsDestroyProc( aTracer );
}
