/*--------------------------------------------------------------------------*\
 * TFcq.c - Test driver for the FCQueues stuff.
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------
 *
 * Portability : 
 * Windows 95/98 The lpThreadId parameter of CreateThread() cannot be NULL.
 * Windows 95/98 CreateThread succeeds only when it is called in the 
 * context of a 32-bit program. A 32-bit DLL cannot create an additional 
 * thread when that DLL is being called by a 16-bit program.
 *
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "cXTrace.h"
#include "cInput.h"
#include "cXcore.h"
#include "ASDebug.h"
#include "cTrace.h"

#include "FCQueue.h"

/*==========================================================================*\
 * Various helper routines
\*==========================================================================*/

/*--------------------------------------------------------------------------*\
 * Return string representation of the given int value in a static buffer
\*--------------------------------------------------------------------------*/
static char* Int2String( pmint32 anInt )
{
   static char sBuf[16];
   char*       thePtr = &sBuf[15];
   pmbool      theNegFlag = 0;

   if ( anInt < 0 ) {
      theNegFlag = 1;
      anInt = -anInt;
   }
   *thePtr-- = 0;
   do {
      *thePtr-- = '0' + (anInt % 10);
      anInt /= 10;
   } while ( anInt != 0 );

   return thePtr + 1;
}

/*--------------------------------------------------------------------------*\
 * Message manipulation routines.
\*--------------------------------------------------------------------------*/

#define MAXMSGLEN 200

static void MsgAppendStr( TPacket* aMsg, char* aStr )
{
   pmuint32 theLen = c_strlen( aStr );
   pmuint32 theCap = aMsg->itsBufferLen - aMsg->itsDataLen;

   if ( theLen > theCap ) {
      theLen = theCap;
   }
   if ( theLen == 0 ) {
      return;
   }
   c_memcpy( aMsg->itsData + aMsg->itsDataLen - 1, aStr, theLen );
   aMsg->itsDataLen += theLen;
   aMsg->itsData[ aMsg->itsDataLen - 1 ] = 0;
}

/*--------------------------------------------------------------------------*/

static TPacket* MsgCreate( char *aStr )
{
   TPacket* theMsg = PacketAllocate( 0, MAXMSGLEN );

   if ( theMsg ) {
      theMsg->itsData[0] = 0;
      theMsg->itsDataLen = 1;
      MsgAppendStr( theMsg, aStr );
   }
   return theMsg;
}

/*--------------------------------------------------------------------------*/

static void MsgAppendInt( TPacket* aMsg, pmint32 anInt )
{
   MsgAppendStr( aMsg, Int2String( anInt ) );
}

/*--------------------------------------------------------------------------*\
 * Put N number of messages to the given queue.
 * Verify that we could put N without waiting, and that we would have to 
 * wait to put one more.
\*--------------------------------------------------------------------------*/
static pmbool PutN( FCQueue* aQ, pmbyte aN )
{
   pmint32  theI;
   TPacket* theMsg;
   char     theStr[] = "This is message number X";

   for ( theI = 0; theI < aN; theI++ ) 
   {
      if ( !FcqCanPut( aQ ) ) {
         DEBUG0("FCQTEST ERROR: CAN'T PUT, aN=%d theI=%d", aN, theI);
         return 0;
      } else {
         theStr[23] = (char) ('0' + theI);
         theMsg = MsgCreate( theStr );
         if ( theMsg ) {
            FcqPutMsg( aQ, theMsg );
         } else {
            DEBUG0("FCQTEST ERROR: PutN: NO MORE MEMORY, aN=%d theI=%d", aN, theI);
            return 0;
         }
      }
   }

   if ( FcqCanPut( aQ ) ) {
      DEBUG0("FCQTEST ERROR: CAN PUT, aN=%d", aN);
      return 0;
   }
   return 1;
}

/*--------------------------------------------------------------------------*\
 * Get aN number of messages from the aQ.
 * Verify that we could really get that many.
\*--------------------------------------------------------------------------*/
static pmbool GetN( FCQueue* aQ, pmbyte aN )
{
   pmint32  theI;
   TPacket* theMsg;

   for ( theI = 0; theI < aN; theI++ ) 
   {
      theMsg = (TPacket*) FcqGetMsg( aQ );
      if ( theMsg ) {
         DEBUG0("FCQTEST Got packet : \"%s\"", theMsg->itsData);
         PacketRelease( theMsg );
      } else {
         DEBUG0("FCQTEST ERROR: CAN'T GET, aN=%d theI=%d", aN, theI);
         return 0;
      }
   }
   return 1;
}

/*--------------------------------------------------------------------------*/

static void NullWakeupMethod( void )
{
   DEBUG0("FCQTEST NullWakeupMethod");
}

/*==========================================================================*\
 * Multiple Run Test
\*==========================================================================*/

/*--------------------------------------------------------------------------*\
 * N application threads, N layer 3 entities, 1 layer 2 entity, and 1 driver.
 * Each application thread sends packets to the corresponding layer 3 entity.
 * Each layer 3 entity forwards the packet to the layer 2 entity.
 * The layer 2 entity just prints traces of the packets it receives.
 * The driver thread toggles the runnable state of the layer 2 entity from 
 * time to time.
\*--------------------------------------------------------------------------*/

/*--------------------+
 |  Constants, types  |
 *--------------------*/

#define MAXMPX 10  /* max multiplex factor */

/*  Generic context structure. 
 */
typedef struct
{
   pmuint32 itsIndex;
   pmuint32 itsCount;
}
MRContext;

/*--------+
 |  Data  |
 *--------*/

/*  Simulation parameters.
 */
static pmuint32 sMpxDegree   = 5;    /* i.e. number of layer 3 entities     */
static pmuint32 sTotalTime   = 1000; /* total simulation time (msec)        */
static pmuint16 sL3LoMark    = 1;    /* layer 3 lo-water mark               */
static pmuint16 sL3HiMark    = 3;    /* layer 3 lo-water mark               */
static pmuint16 sL2LoMark    = 3;    /* layer 2 lo-water mark               */
static pmuint16 sL2HiMark    = 6;    /* layer 2 lo-water mark               */
static pmuint32 sDrivNapTime = 20;   /* delay between enable/disable (msec) */
static pmbool   sProperSched = 1;    /* use separate thread for scheduler?  */

/*  Application contexts, thread handles, event object handles.
 */
static MRContext    sApplCtx   [MAXMPX];  /* contexts            */
static HANDLE       sApplThread[MAXMPX];  /* handles on threads  */
static OS_EventMask sApplEvtObj[MAXMPX];  /* event mask objects  */

/*  Driver thread handle.
 */
static HANDLE       sDrivThread;

/*  FCQueue scheduler thread handle, event mask object.
 */
static HANDLE       sSchedThread;
static OS_EventMask sSchedEvtObj;

/*  Layer 3 contexts, queues.
 */
static MRContext    sLay3Ctx[MAXMPX];
static FCQueue*     sLay3Q  [MAXMPX];

/*  Layer 2 context, queue.
 */
static MRContext    sLay2Ctx;
static FCQueue*     sLay2Q;

/*--------+
 |  Code  |
 *--------*/

/*--------------------------------------------------------------------------*\
 * Proper queue scheduler thread's code.
\*--------------------------------------------------------------------------*/
static DWORD WINAPI MRSchedThreadProc( void* aCtx )
{
   for (;;) 
   {
      OS_EventMaskWait( &sSchedEvtObj );
      FcqLock();
      while ( FcqRunOneQueue() ) {}
      FcqUnlock();
   }
}

/*--------------------------------------------------------------------------*\
 * Proper queue scheduler's wakeup method.
\*--------------------------------------------------------------------------*/
static void MRSchedWakeup( void ) 
{
   OS_EventMaskSignal( &sSchedEvtObj, 1 );
}

/*--------------------------------------------------------------------------*\
 * Clever queue scheduler wakeup method.
\*--------------------------------------------------------------------------*/
static void MRCleverSchedWakeup( void ) 
{
   while ( FcqRunOneQueue() ) {}
}

/*--------------------------------------------------------------------------*\
 * Application threads' code.
\*--------------------------------------------------------------------------*/
static DWORD WINAPI MRApplThreadProc( void* aCtx )
{
   MRContext* theCtx = (MRContext*) aCtx;
   pmuint32   theI   = theCtx->itsIndex;
   TPacket*   theMsg;

   for (;;) 
   {
      FcqLock();
      while ( FcqCanPut( sLay3Q[ theI ] ) ) 
      {
         theMsg = MsgCreate( "Appli " );
         if ( ! theMsg ) {
            DEBUG0("FCQTEST ERROR: MRApplThreadProc: NO MORE MEMORY, theI=%d", theI);
            return 1;
         }
         MsgAppendInt( theMsg, theI );
         MsgAppendStr( theMsg, " message " );
         MsgAppendInt( theMsg, ++theCtx->itsCount );
         FcqPutMsg( sLay3Q[ theI ], theMsg );
         if ( !sProperSched ) {
            FcqUnlock(); /* Let someone else run */
            FcqLock();
         }
      }
      FcqUnlock();
      OS_EventMaskWait( &sApplEvtObj[ theI ] );
   }
   return 0; /* NOTREACHED */
}

/*--------------------------------------------------------------------------*\
 * Applications' wakeup method.
\*--------------------------------------------------------------------------*/
static void MRApplWakeup( void* aCtx )
{
   pmuint32 theI = ((MRContext*)aCtx)->itsIndex;

   OS_EventMaskSignal( &sApplEvtObj[ theI ], 1 ); 
}

/*--------------------------------------------------------------------------*\
 * Layer 3 entities' run method.
\*--------------------------------------------------------------------------*/
static void MRLay3RunMethod( FCQueue* aQ )
{
   MRContext* theCtx  = (MRContext*) FcqGetContext( aQ );
   TPacket*   theMsg;

   while ( (theMsg = FcqPeekMsg( aQ )) != 0 ) 
   {
      if ( ! FcqCanPut( sLay2Q ) ) {
         FcqWaitOnQ( aQ, sLay2Q );
         return;
      }
      FcqGetMsg( aQ );
      MsgAppendStr( theMsg, " : L3 entity " );
      MsgAppendInt( theMsg, theCtx->itsIndex );
      FcqPutMsg( sLay2Q, theMsg );
   }
}

/*--------------------------------------------------------------------------*\
 * Layer 2 entity's run method.
\*--------------------------------------------------------------------------*/
static void MRLay2RunMethod( FCQueue* aQ )
{
   MRContext* theCtx = (MRContext*) FcqGetContext( aQ );
   TPacket*   theMsg;

   while ( (theMsg = FcqPeekMsg( aQ )) != 0 ) 
   {
      DEBUG0("FCQTEST Got packet : \"%s\"", theMsg->itsData);
      theCtx->itsCount++;
      PacketRelease( FcqGetMsg( aQ ) );
   }
}

/*--------------------------------------------------------------------------*\
 * Driver thread's code.
\*--------------------------------------------------------------------------*/
static DWORD WINAPI MRDrivThreadProc( void* aCtx )
{
   for (;;) 
   {
      FcqLock(); 
      FcqSetRunnable( sLay2Q, pmfalse ); 
      FcqUnlock(); 
      
      Sleep( sDrivNapTime );

      FcqLock(); 
      FcqSetRunnable( sLay2Q, pmtrue  ); 
      FcqUnlock(); 
      
      Sleep( sDrivNapTime );
   }
   return 0;
}

/*--------------------------------------------------------------------------*\
 * Start the whole thing.
\*--------------------------------------------------------------------------*/
static void MRTestStart()
{
   pmuint32 theI;
   char     theName[30];
   DWORD    ThreadId; /* for Windows only 98 : lpThreadId can't be NULL */
   LPDWORD  lpThreadId = &ThreadId;

   /*  Initialize the FCQueue module.
    *  Create the layer 3 queues, and the layer 2 queue.
    */
   FcqInit( sProperSched ? MRSchedWakeup : MRCleverSchedWakeup );
   for ( theI=0; theI < sMpxDegree; theI++ ) 
   {
      sLay3Ctx[theI].itsIndex = theI;
      sLay3Ctx[theI].itsCount = 0;
      sLay3Q[theI] = FcqNew();
      c_strcpy( theName, "L3E" );
      c_strcat( theName, Int2String(theI) );
      FcqSetName     ( sLay3Q[theI], theName );
      FcqSetContext  ( sLay3Q[theI], &sLay3Ctx[theI] );
      FcqSetLoMark   ( sLay3Q[theI], (pmbyte)sL3LoMark );
      FcqSetHiMark   ( sLay3Q[theI], (pmbyte)sL3HiMark );
      FcqSetRunMethod( sLay3Q[theI], MRLay3RunMethod );
   }
   sLay2Ctx.itsIndex = 0;
   sLay2Ctx.itsCount = 0;
   sLay2Q = FcqNew();
   FcqSetName     ( sLay2Q, "L2E0" );
   FcqSetContext  ( sLay2Q, &sLay2Ctx );
   FcqSetLoMark   ( sLay2Q, (pmbyte)sL2LoMark );
   FcqSetHiMark   ( sLay2Q, (pmbyte)sL2HiMark );
   FcqSetRunMethod( sLay2Q, MRLay2RunMethod );

   /*  Create event objects.
    */
   if ( sProperSched ) 
   {
      OS_EventMaskInit( &sSchedEvtObj );
   }
   for ( theI=0; theI < sMpxDegree; theI++ ) 
   {
      OS_EventMaskInit( &sApplEvtObj[theI] );
   }

   /*  Create scheduler thread, driver thread, application threads.
    */
   if ( sProperSched )
      sSchedThread = CreateThread( 0, 0, MRSchedThreadProc, 0, 0, lpThreadId );
   
   sDrivThread = CreateThread( 0, 0, MRDrivThreadProc,  0, 0, lpThreadId );
   TRACE3( if ( sDrivThread == NULL ) DEBUG3("MRTestStart : ERROR : Driver thread is not created!"); );
   SetThreadPriority( sDrivThread, THREAD_PRIORITY_ABOVE_NORMAL );

   for ( theI=0; theI < sMpxDegree; theI++ ) 
   {
      FcqSetCallback( sLay3Q[theI], MRApplWakeup, &sApplCtx[ theI ] );
      sApplCtx[ theI ].itsIndex = theI;
      sApplCtx[ theI ].itsCount = 0;
      sApplThread[ theI ] = CreateThread( 0, 0, MRApplThreadProc, &sApplCtx[ theI ], 0, lpThreadId );
   }
}

/*--------------------------------------------------------------------------*\
 *  Stop the whole thing.
\*--------------------------------------------------------------------------*/
static void MRTestStop()
{
   pmuint32 theI;

   /* Kill the applications threads */
   for ( theI=0; theI < sMpxDegree; theI++ ) 
   {
      TerminateThread( sApplThread[theI], 0 );
      CloseHandle( sApplThread[theI] );
   }

   /* Kill the driver thread */
   TerminateThread( sDrivThread, 0 );
   CloseHandle( sDrivThread );

   /* Kill the scheduler thread */
   if ( sProperSched ) {
      TerminateThread( sSchedThread, 0 );
      CloseHandle( sSchedThread );
   }

   /* Destroy event objects */
   for ( theI=0; theI < sMpxDegree; theI++ ) 
   {
      OS_EventMaskTerm( &sApplEvtObj[theI] );
   }
   if ( sProperSched ) {
      OS_EventMaskTerm( &sSchedEvtObj );
   }

   /*  Terminate the FCQueue module */
   FcqTerm();
}

/*--------------------------------------------------------------------------*/
static void DoMultiRunTest( void )
{
      DEBUG0("BEGIN %d", sMpxDegree);
      MRTestStart();
      Sleep( sTotalTime );
      MRTestStop();
      DEBUG0("  END %d", sMpxDegree);
      DEBUG0("FCQTEST %d total messages received", sLay2Ctx.itsCount);
      c_printf( "\nMulti Run Test DONE, %d total messages received\n", sLay2Ctx.itsCount );
}

/*--------------------------------------------------------------------------*/
static void DoVariableMultiRunTest( void )
{
   static char sFmt[] = "FCQTEST ---------- %s MultiRunTest degree = %d ----------";

   if (    Input_UInt32( "Degree of multiplexing", &sMpxDegree,   pmtrue, sMpxDegree,   pmtrue ) 
        && Input_UInt32( "Total simu time (msec)", &sTotalTime,   pmtrue, sTotalTime,   pmtrue ) 
        && Input_UInt16( "Layer 3 lo-water mark",  &sL3LoMark,    pmtrue, sL3LoMark,    pmtrue ) 
        && Input_UInt16( "Layer 3 hi-water mark",  &sL3HiMark,    pmtrue, sL3HiMark,    pmtrue ) 
        && Input_UInt16( "Layer 2 lo-water mark",  &sL2LoMark,    pmtrue, sL2LoMark,    pmtrue ) 
        && Input_UInt16( "Layer 2 hi-water mark",  &sL2HiMark,    pmtrue, sL2HiMark,    pmtrue ) 
        && Input_UInt32( "Driver Nap Time (msec)", &sDrivNapTime, pmtrue, sDrivNapTime, pmtrue )
      ) 
   {
      sProperSched = Input_Char( "Separate scheduler thread? (y/n)", "YN" ) == 'Y';
      if ( sMpxDegree < 1 ) {
         sMpxDegree = 1;
      } else if ( sMpxDegree > MAXMPX ) {
         sMpxDegree = MAXMPX;
      }

      DEBUG0("BEGIN", sMpxDegree);
      MRTestStart();
      Sleep( sTotalTime );
      MRTestStop();
      DEBUG0(" END ", sMpxDegree);
      DEBUG0("FCQTEST %d total messages received", sLay2Ctx.itsCount);
      c_printf( "\nMulti Run Test DONE, %d total messages received\n", sLay2Ctx.itsCount );
   }
}

/*==========================================================================*\
 * Simple Run Test
\*==========================================================================*/

typedef struct { pmbyte itsHiMark; pmbyte itsErr; } RunTestContext;

/*--------------------------------------------------------------------------*/

static void RunTestRunMethod( FCQueue* aQ ) 
{
   RunTestContext* ctx = (RunTestContext*) FcqGetContext( aQ );

   if ( ! GetN( aQ, ctx->itsHiMark ) ) {
   }
}

/*--------------------------------------------------------------------------*/

static pmbool DoRunTest2( pmbyte aHiMark )
{
   RunTestContext ctx;
   FCQueue*       theQ = FcqNew();

   ctx.itsHiMark = aHiMark;
   ctx.itsErr    = 0;

   FcqSetHiMark   ( theQ, aHiMark );
   FcqSetContext  ( theQ, &ctx );
   FcqSetRunMethod( theQ, RunTestRunMethod );

   if ( ! PutN( theQ, aHiMark ) )
      return 0;

   FcqSetRunnable( theQ, 0 ); while ( FcqRunOneQueue() ) {}
   FcqSetRunnable( theQ, 1 ); while ( FcqRunOneQueue() ) {}
   FcqSetRunnable( theQ, 1 ); while ( FcqRunOneQueue() ) {}

   FcqFree( theQ );
   return !ctx.itsErr;
}

/*--------------------------------------------------------------------------*/

static void DoRunTest( void )
{
   pmbyte theHiMark;

   FcqInit( NullWakeupMethod );

   for ( theHiMark = 1; theHiMark <= 3; theHiMark++ ) 
   {
      if ( ! DoRunTest2( theHiMark ) ) {
         c_printf( "Simple Run Test FAILED.\n" );
         return;
      }
   }

   FcqTerm();

   c_printf( "Simple Run Test OK.\n" );
}

/*==========================================================================*\
 * Simple Put/Get Test
\*==========================================================================*/

static pmbool DoPutGetTest2( pmbyte aHiMark )
{
   FCQueue* theQ = FcqNew();

   FcqSetHiMark( theQ, aHiMark );

   if ( ! PutN( theQ, aHiMark ) )
      return 0;

   if ( ! GetN( theQ, aHiMark ) )
      return 0;

   FcqFree( theQ );
   return 1;
}

/*--------------------------------------------------------------------------*/

static void DoPutGetTest( void )
{
   pmbyte theHiMark;

   FcqInit( NullWakeupMethod );
   for ( theHiMark = 1; theHiMark <= 3; theHiMark++ ) 
   {
      if ( ! DoPutGetTest2( theHiMark ) ) {
         c_printf( "Simple Put/Get Test FAILED.\n" );
         return;
      }
   }
   FcqTerm();
   c_printf( "Simple Put/Get Test OK.\n" );
}

/*------ Constructor Test --------------------------------------------------*/

static void DoConstructorTest( void )
{
   pmint32 theN;

   FcqInit( NullWakeupMethod );
   for ( theN = 0; theN < 1000; theN++ ) 
   {
      if ( FcqNew() == 0 ) 
         break;
   }
   FcqTerm();
   c_printf( "Constructed %d queues.\n", theN );
}

/*--------------------------------------------------------------------------*/

static void DoTFcqInit( void )
{
}

/*--------------------------------------------------------------------------*/

static void DoTFcqTerm( void )
{
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Fcq, "FCQueue Tests" )
   PMMENU_ITEM_EX( 1, "Constructor Test",    DoConstructorTest )
   PMMENU_ITEM_EX( 2, "Simple Put/Get Test", DoPutGetTest      )
   PMMENU_ITEM_EX( 3, "Simple Run Test",     DoRunTest         )
   PMMENU_ITEM_EX( 4, "Multi Run Test",      DoMultiRunTest    )
PMMENU_END()

PMMENU_HANDLER( Fcq )
{
   PMMENU_CHOICE_BEGIN()
      PMMENU_CASE_INIT ( DoTFcqInit );
      PMMENU_CASE_CLOSE( DoTFcqTerm );
   PMMENU_CHOICE_END()
}
