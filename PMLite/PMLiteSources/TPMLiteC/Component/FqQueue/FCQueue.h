/*--------------------------------------------------------------------------*\
 * FCQueue.h - Public header file for FCQueue objects.
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
 *--------------------------------------------------------------------------
 *
 *      An FCQueue is a flow-controlled queue of messages.
 *
\*--------------------------------------------------------------------------*/

#ifndef FCQueue_h
#define FCQueue_h

#include "FCQueueH.h"

#ifdef __cplusplus
   extern "C" {
#endif


/*-------------+
 |  Constants  |
 +-------------*/

/* A queue's priority of service */
#define Fcq_PRI_MIN  ((pmbyte)0)  /* lowest priority  */
#define Fcq_PRI_NORM ((pmbyte)1)  /* normal priority  */
#define Fcq_PRI_MAX  ((pmbyte)2)  /* highest priority */

/*---------+
 |  Types  |
 +---------*/

/*  The opaque flow-controlled queue object */
typedef struct _FCQueue FCQueue;

/*  The type of the run method associated with a queue.
 *  The run method of a queue is invoked periodically by the scheduler.
 *  You supply your own version of the run method via FcqSetRunMethod().
 *  When the run method is invoked, there is at least one message on the queue.
 */
typedef void FcqRunMethod( FCQueue* self );

/*  The type of the callback function that you might associate with a queue.
 *  This function is invoked whenever the queue goes from blocked to unblocked.
 *  It is typically used to wakeup a thread "outside" the queueing system.
 *  You associate your callback function with a queue via FcqSetCallback().
 */
typedef void FcqCallbackFunc( void* anArg );

/*  The type of the "wakeup" method that you supply to FcqInit().
 *  Your wakeup method will be called whenever the entire system of queues
 *  goes from the idle state (no ready queues), to the a state where at least
 *  one queue is ready to run. Your wakeup method should arrange for a thread
 *  to repeatedly call FcqRunOneQueue() until the system again becomes idle.
 */
typedef void FcqWakeupMethod( void );

/*-----------+
 |  Methods  |
 +-----------*/

/*  Module initialization/termination.
 *  When initializing the module, you supply a "wakeup" method that will be
 *  called whenever the entire system of queues goes from idle (no ready queues)
 *  to ready (at least one ready queue). 
 */
void FcqInit( FcqWakeupMethod *aWakeupMethod );
void FcqTerm( void );

/*  Instance creation/destruction.
 *  By default, a newly created queue has the following attributes:
 *    - user context ... null
 *    - priority ....... Fcq_PRI_NORM
 *    - lo water mark .. 0
 *    - hi water mark .. 1
 *    - run method ..... just frees one message
 */
FCQueue* FcqNew ( void );
void     FcqFree( FCQueue* self );

/*  Instance parameterization (usually called just after instance creation).
 */
void  FcqSetName     ( FCQueue* self, char*         aName      );
void  FcqSetContext  ( FCQueue* self, void*         aContext   );
void  FcqSetPriority ( FCQueue* self, pmbyte        aPriority  );
void  FcqSetLoMark   ( FCQueue* self, pmbyte        aMark      );
void  FcqSetHiMark   ( FCQueue* self, pmbyte        aMark      );
void  FcqSetRunMethod( FCQueue* self, FcqRunMethod* aRunMethod );

/*  For associating a callback function and argument with a queue.
 *  The callback function is called when the queue goes from blocked to unblocked.
 */
void  FcqSetCallback ( FCQueue* self, FcqCallbackFunc *aFunc, void* anArg );

/*  Get the name of this queue.
 */
#if !Fcq_INLINE
char* FcqGetName( FCQueue* self );
#endif

/*  Methods called from the run method of a queue and applied to that SAME queue:
 */
#if !Fcq_INLINE
void*    FcqGetContext( FCQueue* self );              /* get associated context    */
TPacket* FcqPeekMsg   ( FCQueue* self );              /* peek at message at head   */
#endif
TPacket* FcqGetMsg    ( FCQueue* self );              /* remove message at head    */
void     FcqWaitOnQ   ( FCQueue* self, FCQueue* aQ ); /* wait for aQ to unsaturate */

/*  Methods called from the run method of a queue and applied to some OTHER queue
 *  (also called from outside any run method ...).
 */
#if !Fcq_INLINE
pmbool FcqCanPut( FCQueue* self );                    /* i.e. not saturated ?      */
#endif
void   FcqPutMsg( FCQueue* self, TPacket* aMsg );     /* put message at tail       */

/*  Other queue methods.
 */
void FcqSetRunnable( FCQueue* self, pmbool aBool );   /* explicitly enable/disable */
void FcqFlush      ( FCQueue* self );                 /* free all messages         */

/*  Locking methods. There is one system-wide lock.
 *  Locking/unlocking is done for you before/after your run method is called.
 *  However, when using queues from outside any run method, you must do the
 *  locking/unlocking yourself.
 */
#if !Fcq_INLINE
void FcqLock( void );
void FcqUnlock( void );
#endif

/*  Scheduler entry point.
 *  Run the least-recently run, highest priority, ready queue.
 *  Returns 1 on success, 0 if there were no ready queues.
 *  You must insure exclusive access to the queueing system by calling FcqLock()
 *  beforehand, and FcqUnlock() afterwards. 
 */
pmbool FcqRunOneQueue( void );


#ifdef __cplusplus
   }
#endif

#endif /* FCQueue.h */ 
