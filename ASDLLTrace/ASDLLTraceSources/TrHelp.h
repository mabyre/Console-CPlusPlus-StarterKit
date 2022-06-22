/*--------------------------------------------------------------------------*\
 * TrHelp.h
 * Copyright (C) 1990 - 2000 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#ifndef TrHelp_h
#define TrHelp_h

/*--------------------------------------------------------------------------*/

#include "OSWinInc.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------*/

typedef	WORD	TraceModule;
typedef	WORD	TraceCode;
typedef	DWORD	TraceIdent;

typedef	BYTE	TraceKind;
#define TraceKind_Normal			0
#define TraceKind_EOL				1
#define TraceKind_Binary			2 

typedef WORD TraceOpcode;
#define TraceOpcode_End				0

#define TraceOpcode_Attributes		1
#define TraceOpcode_IndentLevel		2
#define TraceOpcode_ProcessID		3
#define TraceOpcode_ThreadID		4
#define TraceOpcode_Data			5
#define TraceOpcode_TimeStamp		6
#define TraceOpcode_AppName			128

/*--------------------------------------------------------------------------*/

#define SPRINTF_BUFFER_SIZE	1024

/*--------------------------------------------------------------------------*/

typedef struct _TTraceSendParams
{
	TraceKind	itsKind;
	TraceIdent	itsIdent;
	BYTE*		itsData;
	size_t		itsDataLen;
	BYTE		itsIndentLevel;
	DWORD		itsTimeStamp;
} 
TTraceSendParams;

/*--------------------------------------------------------------------------*/

typedef struct _TTracer
{
	BOOL		itsfOpened;
	BOOL		itsfFirstTrace;
	DWORD		itsOpenRetryCount;
	DWORD		itsMaxOpenRetryCount;
	DWORD		itsLastOpenTryTime;
	DWORD		itsOpenRetryDelay;
	/* 
	itsKind must be set to:
		TraceKind_Normal if this tracer does not need binary marshalling.
		TraceKind_Binary if this tracer needs binary marshalling.
	*/
	TraceKind	itsKind;
	BOOL		(*itsOpenProc)(struct _TTracer* aTracer);
	void		(*itsCloseProc)(struct _TTracer* aTracer);
	void		(*itsSendProc)(struct _TTracer* aTracer, TTraceSendParams* aParams);
	void		(*itsDestroyProc)(struct _TTracer* aTracer);
} 
TTracer;

/*--------------------------------------------------------------------------*/

extern TTracer*			gTracer;
extern int				gfTracesActivated;
extern int				gTracesInited;
extern CRITICAL_SECTION	gSendCS;

/*--------------------------------------------------------------------------*/

void Trace_SendText( TTraceSendParams* aParams, char* aFormat, va_list* anArgs );
void Trace_SendString( TTraceSendParams *aParams, char *aString );

/*--------------------------------------------------------------------------*/

void Trace_Send(TTraceSendParams* aParams);
void Trace_Close(TTracer* aTracer);

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
	}
#endif /* __cplusplus */

/*--------------------------------------------------------------------------*/
#endif /* TrHelp_h */
