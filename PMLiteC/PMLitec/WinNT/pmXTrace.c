/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------*
 * pmXTrace.c - Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * Tracing functions.
\*--------------------------------------------------------------------------*/

#include "pmXEnv.h"

#ifdef PM_USE_TRACES

/*--------------------------------------------------------------------------*/

#include "ASTrace.h" /* DLL de traces */

/*--------------------------------------------------------------------------*\
 * Constantes en relation avec les filtres du Reporter
\*--------------------------------------------------------------------------*/

#define MODULE_TRACE0 0x1000
#define MODULE_TRACE1 0x1001
#define MODULE_TRACE2 0x1002
#define MODULE_TRACE3 0x1003

/*--------------------------------------------------------------------------*\
 * T  for a line not terminated by a carriage return
 * TL for a line terminated by a carriage return
 * B  for a binary buffer dumped in hexadecimal
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
 * Fonctions de trace TRACE niveau 0
\*--------------------------------------------------------------------------*/

void TRACE0_T( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VT( MODULE_TRACE0, aFormatString, &theArgs );
   va_end( theArgs );
}

void TRACE0_TL( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VTL( MODULE_TRACE0, aFormatString, &theArgs );
   va_end( theArgs );
}

void TRACE0_B( pmbyte* aBuffer, size_t aBufferLen )
{
   as_trace_B( MODULE_TRACE0, aBuffer, aBufferLen );
}

/*--------------------------------------------------------------------------*\
 * Fonctions de trace TRACE niveau 1
\*--------------------------------------------------------------------------*/

void TRACE1_T( char* aFormatString, ... )
{
   va_list theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VT( MODULE_TRACE1, aFormatString, &theArgs );
   va_end( theArgs );
}

void TRACE1_TL( char* aFormatString, ... )
{
   va_list theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VTL( MODULE_TRACE1, aFormatString, &theArgs );
   va_end( theArgs );
}

void TRACE1_B( pmbyte* aBuffer, size_t aBufferLen )
{
   as_trace_B( MODULE_TRACE1, aBuffer, aBufferLen );
}

/*--------------------------------------------------------------------------*\
 * Fonctions de trace TRACE niveau 2
\*--------------------------------------------------------------------------*/

void TRACE2_T( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString);
   as_trace_VT( MODULE_TRACE2, aFormatString, &theArgs );
   va_end( theArgs );
}

void TRACE2_TL( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VTL( MODULE_TRACE2, aFormatString, &theArgs );
   va_end( theArgs );
}

void TRACE2_B( pmbyte* aBuffer, size_t aBufferLen )
{
   as_trace_B( MODULE_TRACE2, aBuffer, aBufferLen );
}

/*--------------------------------------------------------------------------*\
 * Fonctions de trace TRACE niveau 3
\*--------------------------------------------------------------------------*/

void TRACE3_T( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VT( MODULE_TRACE3, aFormatString, &theArgs );
   va_end( theArgs );
}

void TRACE3_TL( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString);
   as_trace_VTL( MODULE_TRACE3, aFormatString, &theArgs );
   va_end( theArgs );
}

void TRACE3_B( pmbyte* aBuffer, size_t aBufferLen )
{
   as_trace_B( MODULE_TRACE3, aBuffer, aBufferLen );
}

/*--------------------------------------------------------------------------*/

#endif /* PM_USE_TRACES */

