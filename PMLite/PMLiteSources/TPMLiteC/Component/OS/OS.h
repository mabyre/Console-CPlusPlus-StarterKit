/*--------------------------------------------------------------------------*\
 * OS.h
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------
 * 16/09/1999  HOu : OS Modification for LGB (Alcatel) Simulation 
 * 18/11/1999  TKe : Mods for Fcq stuff
\*--------------------------------------------------------------------------*/

#ifndef OS_h
#define OS_h

#include "OSWinInc.h"
#include <process.h>

#include "cXTypes.h"

#ifdef __cplusplus
   extern "C" {
#endif

/*--------------------------------------------------------------------------*\
 * Event Mask Objects
 *--------------------------------------------------------------------------
 *
 * An EventMask object provides a wait/signal protocol for synchronizing 
 * threads:
 *
 * A "handler" thread loops, calling EventMaskWait(), which blocks that 
 * thread until one or more events are signaled. The value returned from 
 * EventMaskWait() is a bit mask indicating the set of events that need 
 * to be handled.
 *
 * Other threads signal events by calling EventMaskSignal(), specifying a bit
 * mask for the events that need handling. Multiple calls to EventMaskSignal()
 * have a cumulitive effect -- adding more events to the event set.
 *
\*--------------------------------------------------------------------------*/

typedef struct _OS_EventMask
{
   pmuint32  itsMask;
   HANDLE    itsPrimEvt;
   pmuint32  itsMutex;
}
OS_EventMask;

void     OS_EventMaskInit  ( OS_EventMask* self );
void     OS_EventMaskTerm  ( OS_EventMask* self );
pmuint32 OS_EventMaskWait  ( OS_EventMask* self );
void     OS_EventMaskSignal( OS_EventMask* self, pmuint32 aBitMask );

/*--------------------------------------------------------------------------*\
 * Mutex Objects Functions
\*--------------------------------------------------------------------------*/

pmuint32 OS_CreateMutex( void );
void     OS_DestroyMutex( pmuint32 aMutex );
void     OS_GetMutex( pmuint32 aMutex );
void     OS_ReleaseMutex( pmuint32 aMutex );

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif

#endif
