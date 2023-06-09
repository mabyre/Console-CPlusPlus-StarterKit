/* ===================================================================
   Timer.c
   Copyright (C) 1996-99 Smartcode Technologie - All rights reserved.
   ===================================================================  */

/*
 *  Implementation of timer objects, WinNT version.
 */

#include "Timer.h"
#include "OS.h"
#include "GPRSTrac.h"

/*-----------------------------+
 |  Private constants, macros  |
 *-----------------------------*/

/*  Don't really use mutexex for now ...
 */
#define OS_GET_MUTEX(aMutex)
#define OS_RELEASE_MUTEX(aMutex)
#define OS_CREATE_MUTEX() 0
#define OS_DESTROY_MUTEX(aMutex)

/*  How long the checker thread sleeps (ms).
 */
#define CHECKER_SLEEP_TIME 50

/*  Comparison of two tick counts, t1 and t2.
 *  Works even if one of the tick counts has wrapped around
 *  the capacity of a 32-bit unsigned int. Returns:
 *    negative ... t1 <  t2
 *    zero ....... t1 == t2
 *    positive ... t1 >  t2
 */
#define COMPARE_TICKS(t1,t2) ((pmint32) (((pmuint32)t1)-((pmuint32)t2)))

#ifndef NDEBUG
#   define CHECK_INIT() CheckInit()
#else
#   define CHECK_INIT()
#endif

/*---------------------+
 |  Private variables  |
 +---------------------*/

static TDLList  sList;
static pmuint32 sLock;
static pmbool   sInitialized = 0;

/*-------------------+
 |  Private methods  |
 +-------------------*/

#ifndef NDEBUG
static void CheckInit( void )
{
   if ( !sInitialized ) {
      gprs_trace(TL(GPRSModule_kOS, "Timer: ERROR uninitialized"));
      TimerInit();
      sInitialized = 1;
   }
}
#endif

/*  Code for the thread that periodically checks for expired timers.
 */
static void TimerChecker( void* anArg )
{
   pmuint32 theNow;
   Timer*   theTimer;

   while ( sInitialized )
   {
      Sleep( CHECKER_SLEEP_TIME );
      if ( sList.itsHead )
      {
         theNow = c_xget_tick_count();
         OS_GET_MUTEX( sLock );
            for (
               theTimer = (Timer*)sList.itsHead;
               theTimer != 0;
               theTimer = (Timer*)theTimer->itsLinks.itsNext
            ) {
               if ( COMPARE_TICKS( theTimer->itsExpTime, theNow ) <= 0 ) {
                  DLL_RemElem( &sList, &theTimer->itsLinks );
                  gprs_trace(TL(GPRSModule_kOS,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" ));
                  gprs_trace(TL(GPRSModule_kOS, "TimerExpired: func=0x%x", (int)(theTimer->itsFunc)));
                  theTimer->itsFunc( theTimer->itsArg );
               }
            }
         OS_RELEASE_MUTEX( sLock );
      }
   }
}

/*------------------+
 |  Public methods  |
 +------------------*/

/*  Module initialization.
 */
void TimerInit( void )
{
   if ( sInitialized ) {
      gprs_trace(TL(GPRSModule_kOS, "TimerInit: ERROR already initialized"));
      return;
   }
   DLL_Init( &sList );
   sLock = OS_CREATE_MUTEX();
   sInitialized = 1;
   _beginthread( TimerChecker, 0, NULL )
}

/*  Module termination: cancel all outstanding timers.
 */
void TimerTerm( void )
{
   CHECK_INIT();
   TimerCancelAll();
   sInitialized = 0;
   OS_DESTROY_MUTEX( sLock );
}

/*  Start a timer.
 */
void TimerStart( Timer* self, pmuint32 aDelay, TimerCallback* aFunc, void* anArg )
{
   CHECK_INIT();
   gprs_trace(TL(GPRSModule_kOS, "TimerStart: func=0x%x delay=%d", (int)aFunc, aDelay));

   /*  Fill in the callback function and arg, and add this timer to the list.
    */
   self->itsFunc    = aFunc;
   self->itsArg     = anArg;
   self->itsExpTime = c_xget_tick_count() + aDelay;
   OS_GET_MUTEX( sLock );
      DLL_InsTail( &sList, &self->itsLinks );
   OS_RELEASE_MUTEX( sLock );
}

/*  Cancel a started timer.
 */
void TimerCancel( Timer* self )
{
   CHECK_INIT();
   gprs_trace(TL(GPRSModule_kOS, "TimerCancel: func=0x%x", (int)(self->itsFunc)));

   OS_GET_MUTEX( sLock );
      DLL_RemElem( &sList, &self->itsLinks );
   OS_RELEASE_MUTEX( sLock );
}

/*  Cancel all started timers.
 */
void TimerCancelAll( void )
{
   CHECK_INIT();
   while ( sList.itsHead != 0 ) {
      TimerCancel( (Timer*)(sList.itsHead) );
   }
}
