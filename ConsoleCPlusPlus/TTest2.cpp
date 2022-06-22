/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * TTest2.c
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "pmDebug.h"
#include "pmXTrace.h"
#include <iostream>
using namespace std;

/*--------------------------------------------------------------------------*/

static void DoTrace( void )
{
    char format[] = "%s\0";
    string msg = "coucou";

    cout << "Hello there !\n";

    DEBUG0_TL(format, msg);
    PM_TRACE0(TL("%s", "Une trace de niveau 0"));
}

static void DoNothing( void ) {}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Test2, "Test 2 en cplus plus" )
   PMMENU_ITEM_EX(1, "Trace"    , DoTrace)
   PMMENU_ITEM_EX(2, "rien"    , DoNothing )
PMMENU_END()

/* ---------------------------------------------------------------------------   */

PMMENU_DEFAULT_HANDLER( Test2 )
