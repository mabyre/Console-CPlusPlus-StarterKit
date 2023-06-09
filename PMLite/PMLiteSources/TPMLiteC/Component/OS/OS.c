/*--------------------------------------------------------------------------*\
 * OS.c
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------
 * 99/09/16 HOu : OS Modification for LGB (Alcatel) Simulation 
 * 21/09/99 HOu : Delete Modifcations for Alcatel Simulation
 * 21/09/99 BRa : aDest is 32 bits long
 * 18/11/99 TKe : Added OS_EventMask stuff
\*--------------------------------------------------------------------------*/

#include "OS.h"
#include "cXcore.h"
#include "cXTrace.h"
#include "cTrace.h"
#include "Packets.h"

/*--------------------------------------------------------------------------*\
 * Event Mask Objects
\*--------------------------------------------------------------------------*/

void OS_EventMaskInit( OS_EventMask* self )
{
   OS_BEGIN_PROC( OS_EventMaskInit  );

   self->itsMask    = 0;
   self->itsPrimEvt = CreateEvent( 0, FALSE, FALSE, 0 );
   self->itsMutex   = OS_CreateMutex();
}

/*--------------------------------------------------------------------------*/

void OS_EventMaskTerm( OS_EventMask* self )
{
   OS_BEGIN_PROC( OS_EventMaskTerm );

   CloseHandle( self->itsPrimEvt );
   OS_DestroyMutex( self->itsMutex );
}

/*--------------------------------------------------------------------------*/

pmuint32 OS_EventMaskWait( OS_EventMask* self )
{
   pmuint32 theMask;

   OS_BEGIN_PROC( OS_EventMaskWait );

   /* Block until non-zero mask */
   while ( self->itsMask == 0 ) 
   {
      c_trace(TL(M_OS,"OS_EventMask %d: begin wait",(int)(self->itsPrimEvt)));
      WaitForSingleObject( self->itsPrimEvt, INFINITE );
      c_trace(TL(M_OS,"OS_EventMask %d: end wait",(int)(self->itsPrimEvt)));
   }

   /* Read and reset the mask */
   OS_GetMutex( self->itsMutex );
      theMask = self->itsMask;
      self->itsMask = 0;
   OS_ReleaseMutex( self->itsMutex );

   return theMask; /* return original (non-zero) value */
}

/*--------------------------------------------------------------------------*/

void OS_EventMaskSignal( OS_EventMask* self, pmuint32 aMask )
{
   OS_BEGIN_PROC( OS_EventMaskSignal );

   OS_GetMutex( self->itsMutex );
      self->itsMask |= aMask;
   OS_ReleaseMutex( self->itsMutex );

   c_trace(TL(M_OS,"OS_EventMask %d: signalling",(int)(self->itsPrimEvt)));
   SetEvent( self->itsPrimEvt );
}

/*--------------------------------------------------------------------------*/

pmuint32 OS_CreateMutex( void )
{
   pmuint32 theMutex = (unsigned long) CreateSemaphore( 0, 1, 1, 0 );
   
   c_trace(TL(M_OS, "OS : creating mutex (%lu)",theMutex));

   return theMutex;
}

/*--------------------------------------------------------------------------*/

void OS_DestroyMutex( pmuint32 aMutex )
{
   c_trace(TL(M_OS, "OS : destroying mutex (%lu)",aMutex));

   CloseHandle( (HANDLE)aMutex );
}

/*--------------------------------------------------------------------------*/

void OS_GetMutex( pmuint32 aMutex )
{
   c_trace(TL(M_OS, "OS : Waiting for mutex (%lu)", aMutex));
   WaitForSingleObject( (HANDLE)aMutex, INFINITE );
   c_trace(TL(M_OS, "OS : Mutex (%lu) acquired", aMutex));
}

/*--------------------------------------------------------------------------*/

void OS_ReleaseMutex( pmuint32 aMutex )
{
   c_trace(TL(M_OS, "OS : Releasing mutex (%lu)", aMutex));
   ReleaseSemaphore((HANDLE)aMutex, 1, 0);
}

