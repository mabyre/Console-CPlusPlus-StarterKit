/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------*
 * pmXTrace.h - Written by Bruno Raby.
 * Tracing functions.
\*--------------------------------------------------------------------------*/

#ifndef _PM_XTRACE_H_
#define _PM_XTRACE_H_

/*--------------------------------------------------------------------------*/

#include "pmEnv.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   extern "C" {
#endif

#ifdef PM_USE_TRACES

#pragma message("PM_USE_TRACES - Defined in TRACE")

#   include "cStdArg.h"

    void TRACE0_T( char* aFormat, ... );
    void TRACE0_TL( char* aFormat, ... );
    void TRACE0_B( pmbyte* aBuffer, size_t aBufferLen );

    void TRACE1_T( char* aFormat, ... );
    void TRACE1_TL( char* aFormat, ... );
    void TRACE1_B( pmbyte* aBuffer, size_t aBufferLen );

    void TRACE2_T( char* aFormat, ... );
    void TRACE2_TL( char* aFormat, ... );
    void TRACE2_B( pmbyte* aBuffer, size_t aBufferLen );

    void TRACE3_T( char* aFormat, ... );
    void TRACE3_TL( char* aFormat, ... );
    void TRACE3_B( pmbyte* aBuffer, size_t aBufferLen );

#   define PM_TRACE0(X) TRACE0_ ## X
#   define PM_TRACE1(X) TRACE1_ ## X
#   define PM_TRACE2(X) TRACE2_ ## X
#   define PM_TRACE3(X) TRACE3_ ## X

#else

#pragma message("PM_USE_TRACES - NOT Defined in TRACE")

/*--------------------------------------------------------------------------*\
 * Avoid traces functions if PM_USE_TRACES is NOT Defined
\*--------------------------------------------------------------------------*/

#  define PM_TRACE0(X)    ((void) 0)
#  define PM_TRACE1(X)    ((void) 0)
#  define PM_TRACE2(X)    ((void) 0)
#  define PM_TRACE3(X)    ((void) 0)

#endif /* PM_USE_TRACES */

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif

/*--------------------------------------------------------------------------*/

#endif /* _PM_XTRACE_H_ */
