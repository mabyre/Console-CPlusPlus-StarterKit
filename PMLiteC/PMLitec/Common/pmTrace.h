/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------*
 * pmDebug.h - Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * Ajouter la trace pour des flottants
 * Tracing functions.
\*--------------------------------------------------------------------------*/

#ifndef _PM_TRACE_H_
#define _PM_TRACE_H_

/*--------------------------------------------------------------------------*/

#include "pmXTrace.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   extern "C" {
#endif

/*--------------------------------------------------------------------------*/

#ifdef PM_USE_TRACES

#pragma message("PM_USE_TRACES - Defined")

#  include "cStdArg.h"

   void _pm_trace0( char* aFormat, ... );
   void _pm_trace1( char* aFormat, ... );
   void _pm_trace2( char* aFormat, ... );
   void _pm_trace3( char* aFormat, ... );

#  define pm_trace0 _pm_trace0
#  define pm_trace1 _pm_trace1
#  define pm_trace2 _pm_trace2
#  define pm_trace3 _pm_trace3

#else

#pragma message("PM_USE_TRACES - NOT Defined")

/*--------------------------------------------------------------------------*\
 * Avoid traces functions if PM_USE_TRACES is NOT Defined
\*--------------------------------------------------------------------------*/

#  define pm_trace0(X)    ((void)0)
#  define pm_trace1(X)    ((void)0)
#  define pm_trace2(X)    ((void)0)
#  define pm_trace3(X)    ((void)0)

#endif /* PM_USE_TRACES */

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif

/*--------------------------------------------------------------------------*/

#endif /* _PM_TRACE_H_ */
