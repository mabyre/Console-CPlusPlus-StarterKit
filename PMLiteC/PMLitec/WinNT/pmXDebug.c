/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------*
 * pmXDebug.c - Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * Debugging functions.
\*--------------------------------------------------------------------------*/

#include "pmXEnv.h"

#ifdef PM_USE_TRACES

/*--------------------------------------------------------------------------*/

#include "ASTrace.h" /* DLL de traces */

/*--------------------------------------------------------------------------*\
 * Constantes en relation avec les filtres du Reporter
\*--------------------------------------------------------------------------*/

#define MODULE_DEBUG0 0x2000
#define MODULE_DEBUG1 0x2001
#define MODULE_DEBUG2 0x2002
#define MODULE_DEBUG3 0x2003

/*--------------------------------------------------------------------------*\
 * T  for a line not terminated by a carriage return
 * TL for a line terminated by a carriage return
 * B  for a binary buffer dumped in hexadecimal
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
 * Fonctions de trace debug niveau 0
\*--------------------------------------------------------------------------*/

void DEBUG0_T( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VT( MODULE_DEBUG0, aFormatString, &theArgs );
   va_end( theArgs );
}

void DEBUG0_TL( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VTL( MODULE_DEBUG0, aFormatString, &theArgs );
   va_end( theArgs );
}

void DEBUG0_B( pmbyte* aBuffer, size_t aBufferLen )
{
   as_trace_B( MODULE_DEBUG0, aBuffer, aBufferLen );
}

/*--------------------------------------------------------------------------*\
 * Fonctions de trace debug niveau 1
\*--------------------------------------------------------------------------*/

void DEBUG1_T( char* aFormatString, ... )
{
   va_list theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VT( MODULE_DEBUG1, aFormatString, &theArgs );
   va_end( theArgs );
}

void DEBUG1_TL( char* aFormatString, ... )
{
   va_list theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VTL( MODULE_DEBUG1, aFormatString, &theArgs );
   va_end( theArgs );
}

void DEBUG1_B( pmbyte* aBuffer, size_t aBufferLen )
{
   as_trace_B( MODULE_DEBUG1, aBuffer, aBufferLen );
}

/*--------------------------------------------------------------------------*\
 * Fonctions de trace debug niveau 2
\*--------------------------------------------------------------------------*/

void DEBUG2_T( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString);
   as_trace_VT( MODULE_DEBUG2, aFormatString, &theArgs );
   va_end( theArgs );
}

void DEBUG2_TL( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VTL( MODULE_DEBUG2, aFormatString, &theArgs );
   va_end( theArgs );
}

void DEBUG2_B( pmbyte* aBuffer, size_t aBufferLen )
{
   as_trace_B( MODULE_DEBUG2, aBuffer, aBufferLen );
}

/*--------------------------------------------------------------------------*\
 * Fonctions de trace debug niveau 3
\*--------------------------------------------------------------------------*/

void DEBUG3_T( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VT( MODULE_DEBUG3, aFormatString, &theArgs );
   va_end( theArgs );
}

void DEBUG3_TL( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString);
   as_trace_VTL( MODULE_DEBUG3, aFormatString, &theArgs );
   va_end( theArgs );
}

void DEBUG3_B( pmbyte* aBuffer, size_t aBufferLen )
{
   as_trace_B( MODULE_DEBUG3, aBuffer, aBufferLen );
}

/*--------------------------------------------------------------------------*/

#endif /* PM_USE_TRACES */

