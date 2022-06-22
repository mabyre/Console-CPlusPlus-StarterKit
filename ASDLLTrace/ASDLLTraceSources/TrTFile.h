/*	===================================================================
	TrFile.h
	Copyright (C) 1990 - 2000 AbyreSoft - All rights reserved.
	===================================================================	*/

#ifndef TrFile_h
#define TrFile_h

/*	------------------------------------------------------------------- */
#include "OSWinInc.h"
#include <stdio.h>

/*	------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*	------------------------------------------------------------------- */
typedef struct _TFileTracer
{
	BOOL		itsfOpened;
	BOOL		itsfFirstTrace;
	DWORD		itsOpenRetryCount;
	DWORD		itsMaxOpenRetryCount;
	DWORD		itsLastOpenTryTime;
	DWORD		itsOpenRetryDelay;
	TraceKind	itsKind;
	BOOL		(*itsOpenProc)(TTracer* aTracer);
	void		(*itsCloseProc)(TTracer* aTracer);
	void		(*itsSendProc)(TTracer* aTracer, TTraceSendParams* aParams);
	void		(*itsDestroyProc)(struct _TTracer* aTracer);

	char		itsFileName[MAX_PATH];
	FILE*		itsFile;
} TFileTracer;

/*	------------------------------------------------------------------- */
TTracer*	TrFile_Create(void* aParam);

/*	------------------------------------------------------------------- */
int			TrFile_OpenProc(TTracer* aTracer);
void		TrFile_CloseProc(TTracer* aTracer);
void		TrFile_SendProc(TTracer* aTracer, TTraceSendParams* aParams);
void		TrFile_DestroyProc(TTracer* aTracer);

/*	------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/*	------------------------------------------------------------------- */
#endif /* TrFile_h */
