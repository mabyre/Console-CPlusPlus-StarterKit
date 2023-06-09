/*--------------------------------------------------------------------------*\
 * Copyright (c) 1999-2000 AbyreSoft - All rights reserved.
 *--------------------------------------------------------------------------*
 * pmTrace.c - Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * Utiliser va_start(), va_end() et vsprintf() pour ajouter des traces pour 
 * les flottants. On laisse pas le choix, il y a un RetourChariot a chaque 
 * fin de ligne.
\*--------------------------------------------------------------------------*/

#include "pmEnv.h"

#ifdef PM_USE_TRACES

/*---------------------------------------------------------------------*/

#include <stdarg.h>
#include <stdio.h>
#include "pmTypes.h"
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
