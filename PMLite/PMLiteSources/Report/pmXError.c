/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------*
 * pmXError.c - Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * Trace Error functions.
\*--------------------------------------------------------------------------*/

#include "pmEnv.h"

/*--------------------------------------------------------------------------*/

#ifdef PM_USE_TRACES

/*--------------------------------------------------------------------------*/

#include "ASTrace.h" /* DLL de traces */
#include "pmTypes.h"

/*--------------------------------------------------------------------------*\
 * Constantes en relation avec les filtres du Reporter
\*--------------------------------------------------------------------------*/

#define MODULE_ERROR0 0x0100
#define MODULE_ERROR1 0x0101
#define MODULE_ERROR2 0x0102
#define MODULE_ERROR3 0x0103

/*--------------------------------------------------------------------------*\
 * T  for a line not terminated by a carriage return
 * TL for a line terminated by a carriage return
 * B  for a binary buffer dumped in hexadecimal
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
 * Fonctions de trace ERROR niveau 0
\*--------------------------------------------------------------------------*/

void ERROR0_T( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VT( MODULE_ERROR0, aFormatString, &theArgs );
   va_end( theArgs );
}

void ERROR0_TL( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VTL( MODULE_ERROR0, aFormatString, &theArgs );
   va_end( theArgs );
}

void ERROR0_B( pmbyte* aBuffer, size_t aBufferLen )
{
   as_trace_B( MODULE_ERROR0, aBuffer, aBufferLen );
}

/*--------------------------------------------------------------------------*\
 * Fonctions de trace debug niveau 1
\*--------------------------------------------------------------------------*/

void ERROR1_T( char* aFormatString, ... )
{
   va_list theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VT( MODULE_ERROR1, aFormatString, &theArgs );
   va_end( theArgs );
}

void ERROR1_TL( char* aFormatString, ... )
{
   va_list theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VTL( MODULE_ERROR1, aFormatString, &theArgs );
   va_end( theArgs );
}

void ERROR1_B( pmbyte* aBuffer, size_t aBufferLen )
{
   as_trace_B( MODULE_ERROR1, aBuffer, aBufferLen );
}

/*--------------------------------------------------------------------------*\
 * Fonctions de trace ERROR niveau 2
\*--------------------------------------------------------------------------*/

void ERROR2_T( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString);
   as_trace_VT( MODULE_ERROR2, aFormatString, &theArgs );
   va_end( theArgs );
}

void ERROR2_TL( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VTL( MODULE_ERROR2, aFormatString, &theArgs );
   va_end( theArgs );
}

void ERROR2_B( pmbyte* aBuffer, size_t aBufferLen )
{
   as_trace_B( MODULE_ERROR2, aBuffer, aBufferLen );
}

/*--------------------------------------------------------------------------*\
 * Fonctions de trace ERROR niveau 3
\*--------------------------------------------------------------------------*/

void ERROR3_T( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString );
   as_trace_VT( MODULE_ERROR3, aFormatString, &theArgs );
   va_end( theArgs );
}

void ERROR3_TL( char* aFormatString, ... )
{
   va_list  theArgs;

   va_start( theArgs, aFormatString);
   as_trace_VTL( MODULE_ERROR3, aFormatString, &theArgs );
   va_end( theArgs );
}

void ERROR3_B( pmbyte* aBuffer, size_t aBufferLen )
{
   as_trace_B( MODULE_ERROR3, aBuffer, aBufferLen );
}

/*--------------------------------------------------------------------------*/

#endif /* PM_USE_TRACES */

