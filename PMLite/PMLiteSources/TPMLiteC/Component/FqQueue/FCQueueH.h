/*--------------------------------------------------------------------------*\
 * FCQueueH.h - Internal header file for FCQueue objects.
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
\*--------------------------------------------------------------------------*/

#ifndef FCQueueH_h
#define FCQueueH_h

#include "cXEnv.h"
#include "OS.h"
#include "Packets.h"

/* Bits used in itsFlags */
#define FcqALLOCATED  0x01
#define FcqRUNNABLE   0x02
#define FcqSATURATED  0x04

/* Max length of a queue's name, not counting terminating null */
#define FcqMAXNAMEL 10

/* The real flow-controlled queue object */
struct _FCQueue
{
   struct _FCQueue*  itsReadyNext;    /* next queue in ready list                 */  
   struct _FCQueue*  itsBlocker;      /* the queue that's blocking me             */
   struct _FCQueue*  itsWaitingNext;  /* next queue blocked by queue blocking me  */
   struct _FCQueue*  itsWaitingList;  /* list of queues I'm blocking              */ 
   TDLList           itsMsgList;      /* the list of messages                     */
   void*             itsContext;      /* user's context associated with this q    */
   void (*itsRunMethod)( struct _FCQueue* ); /* user's code to processes this q   */
   void (*itsCallbackFunc)( void * ); /* callback function invoked when unblocked */
   void*             itsCallbackArg;  /* argument to callback function            */
   pmbyte            itsPriority;     /* priority of service (0 is lowest)        */
   pmbyte            itsLoMark;       /* low-water mark                           */
   pmbyte            itsHiMark;       /* high-water mark                          */
   pmbyte            itsFlags;        /* allocated, runnable, saturated, ...      */
   pmbyte            itsMsgCount;     /* the number of queued messages            */
   char              itsName[FcqMAXNAMEL+1]; /* this queue's name                 */
};

/*  Macro versions of trivial public functions */
#define Fcq_INLINE 1
#if Fcq_INLINE
    extern pmuint32 gFcqMutexObject;
#   define FcqGetName( self )    ((self)->itsName)
#   define FcqGetContext( self ) ((self)->itsContext)
#   define FcqPeekMsg( self )    ((TPacket*)(self)->itsMsgList.itsHead)
#   define FcqCanPut( self )     (!((self)->itsFlags & FcqSATURATED))
#   define FcqLock()             OS_GetMutex(gFcqMutexObject)
#   define FcqUnlock()           OS_ReleaseMutex(gFcqMutexObject)
#endif

#endif /* FCQueueH_h */
