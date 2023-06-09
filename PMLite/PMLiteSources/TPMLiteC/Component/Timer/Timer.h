/* ===================================================================
   Timer.h
   Copyright (C) 1996-99 Smartcode Technologie - All rights reserved.
   ===================================================================  */

/*
 *  Public header file for timer objects.
 *
 *  A timer object allows you to schedule a callback after a specified delay.
 *  Time is measured in milliseconds. The longest allowed delay is 2^31 - 1
 *  milliseconds, which is approximately 24.8 days.
 *  It's up to you to manage the memory occupied by a timer object.
 *
 *  TimerInit ....... module initialization: must be called before anything else
 *  TimerTerm ....... module termination
 *  TimerStart ...... start a timer -- arrange for a callback after a given delay
 *  TimerCancel ..... cancel a started timer
 *  TimerCancelAll .. cancel all pending timers
 */

#ifndef Timer_h
#define Timer_h

#include "TimerH.h"

#ifdef __cplusplus
   extern "C" {
#endif

/*---------+
 |  Types  |
 +---------*/

/*  The opaque timer object.
 */
typedef struct _Timer Timer;

/*  The type of a callback function.
 */
typedef void TimerCallback( void* anArg );

/*-----------+
 |  Methods  |
 +-----------*/

/*  Module initialization.
 */
void TimerInit( void );

/*  Module termination.
 */
void TimerTerm( void );

/*  Start a timer.
 */
void TimerStart(
   Timer*         self,   /* the timer object           */
   pmuint32       aDelay, /* the delay, in milliseconds */
   TimerCallback* aFunc,  /* the function to be called  */
   void*          anArg   /* the function's argument    */
);

/*  Cancel a started timer.
 */
void TimerCancel( Timer* self );

/*  Cancel all started timers.
 */
void TimerCancelAll( void );



#ifdef __cplusplus
   }
#endif

#endif /* Timer_h */
