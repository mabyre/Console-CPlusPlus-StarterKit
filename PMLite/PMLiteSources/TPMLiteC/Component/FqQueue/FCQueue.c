/*--------------------------------------------------------------------------*\
 * FCQueue.c - Implementation of FCQueue objects.
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------
 *
 *      Implementation Notes:
 *
 *      1) Messages are of type TPacket.
 *
 *      2) A queue's message list is implemented as a TDLList.
 *
 *      3) For each priority level, there is a list of ready queues.
 *      At any given time, a given queue is either on the ready list
 *      corresponding to its priority, or it is on no ready list at all.
 *      The ready lists are implemented as SLLists.
 *
 *      4) When any operation causes a non-ready queue to become ready, it
 *      is immediately put onto the ready list corresponding to its priority.
 *      However, when an operation causes a ready queue to become non-ready,
 *      it is NOT immediately removed from the ready list; we just wait
 *      until it propagates to the front of the list and remove it then.
 *
 *      5) Each queue maintains a list of queues waiting on it to desaturate. 
 *      A given queue can be on at most one such waiting list, i.e. a queue
 *      cannot wait on multiple queues. The waiting lists are implemented as
 *      SLLists.
 *
\*--------------------------------------------------------------------------*/
 
#include "cXCore.h"
#include "cXDLList.h"
#include "SLList.h"
#include "cTrace.h"
#include "OS.h"
#include "FCQueue.h"

/*--------------------------------------------------------------------------*\
 * Private constants, types, macros
\*--------------------------------------------------------------------------*/

/* Maximum number of queues */
#define MAXNQ 28

/* Number of priority levels */
#define NPRI  (Fcq_PRI_MAX + 1 - Fcq_PRI_MIN)

/* Given a pointer to a queue, return the index to that queue in sQTab[] */
#define QINDEX( aQ )  ((aQ) - sQTab)

/* Is this queue the running queue ? */
#define IS_RUNNING( aQ ) ((aQ) == sRunningQ)

/* Is this queue on a ready list ? */
#define IS_ON_READY_LIST( aQ ) ((aQ)->itsReadyNext != 0)

/* Is this queue truly "ready", i.e. runnable, not waiting, and not empty ? */
#define IS_READY( aQ ) (                \
    (((aQ)->itsFlags) & FcqRUNNABLE) && \
    (aQ)->itsBlocker  == 0           && \
    (aQ)->itsMsgCount != 0              \
)

/* Offsets to link fields within the FCQueue structure */
#define OFFSET_READY_NEXT    SLL_OFFSET_TO_FIELD( FCQueue, itsReadyNext   )
#define OFFSET_WAITING_NEXT  SLL_OFFSET_TO_FIELD( FCQueue, itsWaitingNext )

/*--------------------------------------------------------------------------*\
 * Public globals
\*--------------------------------------------------------------------------*/

pmuint32 gFcqMutexObject; /* for exclusive access to queueing system */

/*--------------------------------------------------------------------------*\
 * Private globals
\*--------------------------------------------------------------------------*/

static FCQueue          sQTab[MAXNQ];      /* where the queues live                        */
static FCQueue*         sReadyLists[NPRI]; /* a list of ready queues for each priority     */
static pmuint32         sReadyOrRunningK;  /* counts queues on ready lists + running queue */
static FCQueue*         sRunningQ;         /* the queue whose run method is active         */
static FcqWakeupMethod* sWakeupMethod;     /* the user's wakeup method                     */
static pmbool           sInitialized;      /* has FcqInit() been called ?                  */

/*--------------------------------------------------------------------------*\
 * Private methods
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
 * The default run method: remove one message and free it
\*--------------------------------------------------------------------------*/
static void DefaultRunMethod( FCQueue* self )
{
    TPacket* theMsg;

    PM_BEGIN_PROC( DefaultRunMethod );

    if ( (theMsg = FcqGetMsg( self )) != 0 ) 
    {
        PacketRelease( theMsg );
    }
}

/*--------------------------------------------------------------------------*\
 * If this queue is ready, and it's not on a ready list, and it's not running,
 * then put it at the tail of the ready list indexed by its priority.
 * If there were previously no queues on any ready lists, and no running queue,
 * call user's wakeup method.
\*--------------------------------------------------------------------------*/
static void PutOnReadyListIfAppropriate( FCQueue* self )
{
   PM_BEGIN_PROC( PutOnReadyListIfAppropriate );

   if ( !IS_ON_READY_LIST(self) && !IS_RUNNING(self) && IS_READY(self) )
   {
      c_trace(TL(M_FC_QUEUE, "FCQ putting q %s on ready list %d", FcqGetName(self), self->itsPriority));
      SLL_InsTail( (SLL_List*)&sReadyLists[self->itsPriority], OFFSET_READY_NEXT, (SLL_Node*)self ); 
      if ( ++sReadyOrRunningK == 1 && sWakeupMethod ) 
      {
         c_trace(TL(M_FC_QUEUE, "FCQ Wakeup!"));
         sWakeupMethod();
      }
   }
}

/*--------------------------------------------------------------------------*\
 * Unblock all queues waiting on this queue.
 * Put each unblocked queue on a ready list if appropriate.
 * Also: if there's a user callback function, call it.
\*--------------------------------------------------------------------------*/
static void UnblockWaitingQueues( FCQueue* self )
{
   FCQueue* theQ;

   PM_BEGIN_PROC( UnblockWaitingQueues );

   while ( (theQ = SLL_RemHead( (SLL_List*)&self->itsWaitingList, OFFSET_WAITING_NEXT)) != 0 ) 
   {
      c_trace(TL(M_FC_QUEUE, "FCQ q %s stops waiting on q %s", FcqGetName(theQ), FcqGetName(self)));
      theQ->itsBlocker = 0;
      PutOnReadyListIfAppropriate( theQ );
   }

   if ( self->itsCallbackFunc ) 
   {
      (self->itsCallbackFunc)( self->itsCallbackArg );
   }
}

/*--------------------------------------------------------------------------*\
 * Public methods
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
 * Module initialization.
\*--------------------------------------------------------------------------*/
void FcqInit( FcqWakeupMethod* aWakeupMethod )
{
   PM_BEGIN_PROC( FcqInit );

   c_memset( sQTab,       0, sizeof(sQTab) );
   c_memset( sReadyLists, 0, sizeof(sReadyLists) );
   sReadyOrRunningK = 0;
   sRunningQ        = 0;
   sWakeupMethod    = aWakeupMethod;
   gFcqMutexObject  = OS_CreateMutex();
   sInitialized     = 1;
}

/*--------------------------------------------------------------------------*\
 * Module termination.
\*--------------------------------------------------------------------------*/
void FcqTerm( void )
{
   FCQueue* theQ;

   PM_BEGIN_PROC( FcqTerm );

   for ( theQ = &sQTab[0]; theQ < &sQTab[MAXNQ]; theQ++ ) 
   {
      if ( theQ->itsFlags & FcqALLOCATED ) {
         FcqFree( theQ );
      }
   }
   OS_DestroyMutex( gFcqMutexObject );
   sInitialized = 0;
}

/*--------------------------------------------------------------------------*\
 * Constructor
\*--------------------------------------------------------------------------*/
FCQueue* FcqNew()
{
   FCQueue* theQ;
   pmuint32 theIndex;
   char     theName[3];

   PM_BEGIN_PROC( FcqNew );

   if ( ! sInitialized ) {
      c_trace(TL(M_FC_QUEUE, "FCQ ### NOT INITIALIZED!"));
      return 0; /* no, no, no */
   }

   for ( theQ = &sQTab[0]; theQ < &sQTab[MAXNQ]; theQ++ ) {
      if ( !(theQ->itsFlags & FcqALLOCATED) ) {
         theIndex = QINDEX( theQ );
         c_trace(TL(M_FC_QUEUE, "FCQ allocating q %d", theIndex));
         theQ->itsFlags = FcqALLOCATED | FcqRUNNABLE;
         FcqSetPriority( theQ, Fcq_PRI_NORM );
         FcqSetLoMark( theQ, 0 );
         FcqSetHiMark( theQ, 1 );
         FcqSetRunMethod( theQ, DefaultRunMethod );
         theName[2] = '\0';
         theName[1] = '0' + (char) (theIndex % 10);
         theName[0] = '0' + (char) ((theIndex / 10) % 10);
         FcqSetName( theQ, theName );
         return theQ;
      }
   }

   c_trace(TL(M_FC_QUEUE, "FCQ ### NO MORE QUEUES!"));
   return 0; /* no unallocated queues */
}

/*--------------------------------------------------------------------------*\
 * Destructor.
 * Remove and free all queued messages (this unblocks any queues waiting 
 * on this one).
 * If this queue is on a ready list, remove it from that list.
 * If this queue is on a waiting list, remove it from that list.
\*--------------------------------------------------------------------------*/
void FcqFree( FCQueue* self )
{
   PM_BEGIN_PROC( FcqFree );

   c_trace(TL(M_FC_QUEUE, "FCQ freeing q %s", FcqGetName(self)));
   FcqFlush( self );
   if ( SLL_RemElem( (SLL_List*)&sReadyLists[self->itsPriority], OFFSET_READY_NEXT, (SLL_List*)self ) ) {
      sReadyOrRunningK--;
   }
   if ( self->itsBlocker ) {
      SLL_RemElem( (SLL_List*)&self->itsBlocker->itsWaitingList, OFFSET_WAITING_NEXT, (SLL_List*)self );
   }
   c_memset( self, 0, sizeof(FCQueue) );
}

/*--------------------------------------------------------------------------*\
 * Instance parameterization methods.
\*--------------------------------------------------------------------------*/

void FcqSetContext  ( FCQueue* self, void*         aCtx  ) { self->itsContext   = aCtx;  }
void FcqSetPriority ( FCQueue* self, pmbyte        aPri  ) { self->itsPriority  = aPri;  }
void FcqSetLoMark   ( FCQueue* self, pmbyte        aMrk  ) { self->itsLoMark    = aMrk;  }
void FcqSetHiMark   ( FCQueue* self, pmbyte        aMrk  ) { self->itsHiMark    = aMrk;  }
void FcqSetRunMethod( FCQueue* self, FcqRunMethod* aRM   ) { self->itsRunMethod = aRM;   }

void FcqSetName( FCQueue* self, char* aName )
{
   pmuint32 theLen;

   PM_BEGIN_PROC( FcqSetName );

   theLen = c_strlen(aName);
   if ( FcqMAXNAMEL < theLen )
      theLen = FcqMAXNAMEL;
   c_memcpy( self->itsName, aName, theLen );
   self->itsName[ theLen ] = '\0';
}

/*--------------------------------------------------------------------------*\
 * Install a callback function to invoke when the queue becomes unblocked
\*--------------------------------------------------------------------------*/
void FcqSetCallback( FCQueue* self, FcqCallbackFunc *aFunc, void* anArg )
{
   self->itsCallbackFunc = aFunc;
   self->itsCallbackArg  = anArg;
}

/*--------------------------------------------------------------------------*\
 * Return this queue's name
\*--------------------------------------------------------------------------*/
#if !Fcq_INLINE
void* FcqGetName( FCQueue* self ) { return self->itsName; }
#endif

/*--------------------------------------------------------------------------*\
 * Return the user context associated with this queue 
\*--------------------------------------------------------------------------*/
#if !Fcq_INLINE
void* FcqGetContext( FCQueue* self ) { return self->itsContext; }
#endif

/*--------------------------------------------------------------------------*\
 * Return the message at the head of this queue (or 0 if the queue is empty).
 * The message is NOT removed from the queue.
\*--------------------------------------------------------------------------*/
#if !Fcq_INLINE
TPacket* FcqPeekMsg( FCQueue* self ) { return (TPacket*)self->itsMsgList.itsHead; }
#endif

/*--------------------------------------------------------------------------*\
 * Remove the message at the head of this queue and return it (or 0 if empty).
 * If this queue becomes unsaturated, note that fact and unblock queues 
 * waiting on it.
\*--------------------------------------------------------------------------*/
TPacket* FcqGetMsg( FCQueue* self )
{
   TPacket* theMsg;

   PM_BEGIN_PROC( FcqGetMsg );

   theMsg = (TPacket*) DLL_RemHead( &self->itsMsgList );
   if ( theMsg ) {
      c_trace(TL(M_FC_QUEUE, "FCQ getting msg from q %s", FcqGetName(self)));
      if ( --self->itsMsgCount == self->itsLoMark && (self->itsFlags & FcqSATURATED) ) {
         self->itsFlags &= ~FcqSATURATED;
         UnblockWaitingQueues( self );
      }
   }
   return theMsg; 
}

/*--------------------------------------------------------------------------*\
 * If this queue is not already blocked, note that aQ is blocking this queue,
 * and put this queue at the tail of the list of queues waiting on aQ.
\*--------------------------------------------------------------------------*/
void FcqWaitOnQ( FCQueue* self, FCQueue* aQ )
{
   PM_BEGIN_PROC( FcqWaitOnQ );

    if ( self->itsBlocker == 0 ) {
        c_trace(TL(M_FC_QUEUE, "FCQ q %s waiting on q %s", FcqGetName(self), FcqGetName(aQ)));
        self->itsBlocker = aQ;
        SLL_InsTail( (SLL_List*)&aQ->itsWaitingList, OFFSET_WAITING_NEXT, (SLL_List*)self );
    } else {
        c_trace(TL(M_FC_QUEUE, "FCQ q %d wants to wait on q %d, already waiting on q %d!",
            FcqGetName(self), FcqGetName(aQ), FcqGetName(self->itsBlocker)));
    }
}

/*--------------------------------------------------------------------------*\
 *  Is it ok to put a message on this queue ?
\*--------------------------------------------------------------------------*/
#if !Fcq_INLINE
pmbool FcqCanPut( FCQueue* self ) { return !(self->itsFlags & FcqSATURATED); }
#endif

/*--------------------------------------------------------------------------*\
 * Append a message to the tail of this queue.
 * If that saturates this queue, note that fact.
 * Put this queue on a ready list if appropriate.
\*--------------------------------------------------------------------------*/
void FcqPutMsg( FCQueue* self, TPacket* aMsg )
{
   PM_BEGIN_PROC( FcqPutMsg );

    c_trace(TL(M_FC_QUEUE, "FCQ putting msg onto q %s", FcqGetName(self)));
    DLL_InsTail( &self->itsMsgList, (TDLListElem*)aMsg );

    if ( ++self->itsMsgCount == self->itsHiMark ) 
    {
        self->itsFlags |= FcqSATURATED;
    }

    if ( self->itsMsgCount == 1 ) 
    {
        PutOnReadyListIfAppropriate( self );
    }
}

/*--------------------------------------------------------------------------*\
 * Explicitly enable/disable this queue.
 * If enabling, put this queue on a ready list if appropriate
\*--------------------------------------------------------------------------*/
void FcqSetRunnable( FCQueue* self, pmbool aBool )
{
   PM_BEGIN_PROC( FcqSetRunnable );

   if ( aBool ) 
   {
      c_trace(TL(M_FC_QUEUE, "FCQ enabling q %s", FcqGetName(self)));
      self->itsFlags |= FcqRUNNABLE;
      PutOnReadyListIfAppropriate( self );
   } 
   else 
   {
      c_trace(TL(M_FC_QUEUE, "FCQ disabling q %s", FcqGetName(self)));
      self->itsFlags &= ~FcqRUNNABLE;
   }
}

/*--------------------------------------------------------------------------*\
 * Remove all messages from this queue and free them
\*--------------------------------------------------------------------------*/
void FcqFlush( FCQueue* self )
{
    TPacket* theMsg;

   PM_BEGIN_PROC( FcqFlush );

    while ( (theMsg = FcqGetMsg( self )) != 0 ) 
    {
        PacketRelease( theMsg );
    }
}

/*--------------------------------------------------------------------------*\
 * Get/release exclusive access to the whole system of queues
\*--------------------------------------------------------------------------*/
#if !Fcq_INLINE
void FcqLock  ( void ) { OS_GetMutex    ( gFcqMutexObject ); }
void FcqUnlock( void ) { OS_ReleaseMutex( gFcqMutexObject ); }
#endif


/*--------------------------------------------------------------------------*\
 *  Run the least-recently run, highest-priority, ready queue
 *  Returns zero if and only if there were no ready queues
\*--------------------------------------------------------------------------*/
pmbool FcqRunOneQueue( void )
{
   pmint32 thePriority;

   PM_BEGIN_PROC( FcqRunOneQueue );

   for ( thePriority = Fcq_PRI_MAX; thePriority >= Fcq_PRI_MIN; thePriority-- ) 
   {
      FCQueue** theReadyList = &sReadyLists[thePriority];
      FCQueue*  theQ;

      while ( (theQ = (FCQueue*) SLL_RemHead( (SLL_List*)theReadyList, OFFSET_READY_NEXT )) != 0 )
      {
         if ( IS_READY( theQ ) )
         {
             /* Run this queue */
             c_trace(TL(M_FC_QUEUE, "FCQ running q %s priority %d", FcqGetName(theQ), thePriority));
             sRunningQ = theQ;
             theQ->itsRunMethod( theQ );
             sRunningQ = 0;

             /* If it's still ready, put back onto its ready list, at the tail */
             if ( IS_READY( theQ ) ) 
             {
                 c_trace(TL(M_FC_QUEUE, "FCQ putting q %s back on ready list", FcqGetName(theQ)));
                 SLL_InsTail( (SLL_List*)theReadyList, OFFSET_READY_NEXT, (SLL_List*)theQ );
             } 
             else 
             {
                 sReadyOrRunningK--;
             }
             return 1; /* ran one */
         }
         c_trace(TL(M_FC_QUEUE, "FCQ removing q %s from ready list %d", FcqGetName(theQ), thePriority));
         sReadyOrRunningK--;
      }
   }
   return 0; /* nothing to do */
}
