/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------*
 * pmDebug.h - Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * Ajouter la trace pour des flottants
 * Debugging functions with Unix's style.
\*--------------------------------------------------------------------------*/

#ifndef _PM_DEBUG_H_
#define _PM_DEBUG_H_

/*--------------------------------------------------------------------------*/

#include "pmXDebug.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   extern "C" {
#endif

/*--------------------------------------------------------------------------*/

#ifdef PM_USE_TRACES

#pragma message("PM_USE_TRACES - Defined")

#  include "cStdArg.h"

   void _pm_debug0( char* aFormat, ... );
   void _pm_debug1( char* aFormat, ... );
   void _pm_debug2( char* aFormat, ... );
   void _pm_debug3( char* aFormat, ... );

#  define pm_debug0 _pm_debug0
#  define pm_debug1 _pm_debug1
#  define pm_debug2 _pm_debug2
#  define pm_debug3 _pm_debug3

#else

#pragma message("PM_USE_TRACES - NOT Defined")

/*--------------------------------------------------------------------------*\
 * Avoid traces functions if PM_USE_TRACES is NOT Defined
\*--------------------------------------------------------------------------*/

#  define pm_debug0(X)    ((void)0)
#  define pm_debug1(X)    ((void)0)
#  define pm_debug2(X)    ((void)0)
#  define pm_debug3(X)    ((void)0)

#endif /* PM_USE_TRACES */

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif

/*--------------------------------------------------------------------------*/

#endif /* _PM_DEBUG_H_ */
