/*--------------------------------------------------------------------------*\
 * Copyright (c) 1999-2000 AbyreSoft - All rights reserved.
 *--------------------------------------------------------------------------*
 * pmDebug.c - Written by Bruno Raby.
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
#include "pmXDebug.h"

/*---------------------------------------------------------------------*/

void _pm_debug0( char *aFormatString, ... )
{
    va_list  theArgs;
    char     theBuffer[ 256 ];

    va_start( theArgs, aFormatString );
    vsprintf( theBuffer, aFormatString, theArgs );
    va_end( theArgs );

    PM_DEBUG0(TL( "%s", theBuffer ));
}

/*---------------------------------------------------------------------*/

void _pm_debug1( char *aFormatString, ... )
{
    va_list  theArgs;
    char     theBuffer[ 256 ];

    va_start( theArgs, aFormatString );
    vsprintf( theBuffer, aFormatString, theArgs );
    va_end( theArgs );

    PM_DEBUG1(TL( "%s", theBuffer ));
}

/*---------------------------------------------------------------------*/

void _pm_debug2( char *aFormatString, ... )
{
    va_list  theArgs;
    char     theBuffer[ 256 ];

    va_start( theArgs, aFormatString );
    vsprintf( theBuffer, aFormatString, theArgs );
    va_end( theArgs );

    PM_DEBUG2(TL( "%s", theBuffer ));
}

/*---------------------------------------------------------------------*/

void _pm_debug3( char* aFormatString, ... )
{
    va_list  theArgs;
    char     theBuffer[ 256 ];

    va_start( theArgs, aFormatString );
    vsprintf( theBuffer, aFormatString, theArgs );
    va_end( theArgs );

    PM_DEBUG3(TL( "%s", theBuffer ));
}

/*---------------------------------------------------------------------*/

#endif /* PMLITE_TRACE */
