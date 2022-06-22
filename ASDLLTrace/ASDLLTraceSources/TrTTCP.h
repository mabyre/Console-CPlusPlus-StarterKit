/*	===================================================================
	TrTTCP.h
	Copyright (C) 1990 - 2000 AbyreSoft - All rights reserved.
	===================================================================	*/

#ifndef TrTTCP_h
#define TrTTCP_h

/*	------------------------------------------------------------------- */
#include "OSWinInc.h"
#include <stdio.h>
#include <winsock.h>

/*	------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*	------------------------------------------------------------------- */
#define	TCPTracer_Port		25999

/*	------------------------------------------------------------------- */
typedef struct _TTCPTracer
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

	DWORD		itsIP;
	WORD		itsPort;
	SOCKET		itsSocket;
} TTCPTracer;

/*	------------------------------------------------------------------- */
TTracer*	TrTCP_Create(void* aParam);

/*	------------------------------------------------------------------- */
int		TrTCP_OpenProc(TTracer* aTracer);
void	TrTCP_CloseProc(TTracer* aTracer);
void	TrTCP_SendProc(TTracer* aTracer, TTraceSendParams* aParams);
void	TrTCP_DestroyProc(TTracer* aTracer);

/*	------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/*	------------------------------------------------------------------- */
#endif /* TrTTCP_h */
