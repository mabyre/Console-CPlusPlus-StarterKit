/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * TCplusplus.c
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include <iostream>
using namespace std;

/*--------------------------------------------------------------------------*/

void DoTest( void )
{
    cout << "Hello there !\n";
}

void DoNothing( void ) {}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Cplusplus, "Test Cplus Plus" )
   PMMENU_ITEM_EX(1, "Test"    , DoTest )
   PMMENU_ITEM_EX(2, "rien"    , DoNothing )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Cplusplus )
