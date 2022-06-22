/*--------------------------------------------------------------------------*\
 * Copyright (c) 1999-2000 AbyreSoft - All rights reserved.
 *--------------------------------------------------------------------------*
 * pmTrace.c - Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * Reutiliser va_start() et va_end() pour ajouter la trace pour les 
 * flottants. On laisse pas le choix il y a un RetourChariot a chaque fin
 * de ligne
\*--------------------------------------------------------------------------*/

#include "pmXEnv.h"

#ifdef PM_USE_TRACES

/*---------------------------------------------------------------------*/

#include <stdarg.h>
#include <stdio.h>
#include "pmXTrace.h"

/*---------------------------------------------------------------------*/

void _pm_trace0( char *aFormatString, ... )
{
    va_list  theArgs;
    char     theBuffer[ 256 ];

    va_start( theArgs, aFormatString );
    vsprintf( theBuffer, aFormatString, theArgs );
    va_end( theArgs );

    PM_TRACE0(TL( "%s", theBuffer ));
}

/*---------------------------------------------------------------------*/

void _pm_trace1( char *aFormatString, ... )
{
    va_list  theArgs;
    char     theBuffer[ 256 ];

    va_start( theArgs, aFormatString );
    vsprintf( theBuffer, aFormatString, theArgs );
    va_end( theArgs );

    PM_TRACE1(TL( "%s", theBuffer ));
}

/*---------------------------------------------------------------------*/

void _pm_trace2( char *aFormatString, ... )
{
    va_list  theArgs;
    char     theBuffer[ 256 ];

    va_start( theArgs, aFormatString );
    vsprintf( theBuffer, aFormatString, theArgs );
    va_end( theArgs );

    PM_TRACE2(TL( "%s", theBuffer ));
}

/*---------------------------------------------------------------------*/

void _pm_trace3( char* aFormatString, ... )
{
    va_list  theArgs;
    char     theBuffer[ 256 ];

    va_start( theArgs, aFormatString );
    vsprintf( theBuffer, aFormatString, theArgs );
    va_end( theArgs );

    PM_TRACE3(TL( "%s", theBuffer ));
}

/*---------------------------------------------------------------------*/

#endif /* PM_USE_TRACES */
