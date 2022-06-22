/*--------------------------------------------------------------------------*\
 * TrTStdEr.c
 * Copyright (C) 1990 - 1997 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#include "TrHelp.h"
#include "TrTStdEr.h"
#include "TrTFile.h"

/*--------------------------------------------------------------------------*/

TTracer* TrStdErr_Create(void* aParam)
{
	TFileTracer*	theTracer;

	theTracer = malloc(sizeof(TFileTracer));
	theTracer->itsfOpened = FALSE;
	theTracer->itsKind = TraceKind_Normal;
	theTracer->itsOpenProc = TrStdErr_OpenProc;
	theTracer->itsCloseProc = TrStdErr_CloseProc;
	theTracer->itsSendProc = TrFile_SendProc;
	theTracer->itsDestroyProc = TrStdErr_DestroyProc;

	theTracer->itsFile = stderr;
	theTracer->itsFileName[0] = 0;
	theTracer->itsfOpened = TRUE;

	return (TTracer*)theTracer;
}

/*--------------------------------------------------------------------------*/

BOOL TrStdErr_OpenProc(TTracer* aTracer)
{
	return TRUE;
}

/*--------------------------------------------------------------------------*/

void TrStdErr_CloseProc(TTracer* aTracer)
{
}

/*--------------------------------------------------------------------------*/

void TrStdErr_DestroyProc(TTracer* aTracer)
{
	free(aTracer);
}
