/*---------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 AbyreSoft. Written by Bruno Raby.
 *---------------------------------------------------------------------------*
 * TTableau.c 
\*---------------------------------------------------------------------------*/

#include <conio.h>
#include "cMenu.h"
#include "cCore.h"
#include "cConsole.h"
#include "cInput.h"
#include "pmTrace.h"

/*---------------------------------------------------------------------------*/

#define NB_ACQUISITION      10
#define NB_ANALOG_CHANNELS  3

/*---------------------------------------------------------------------------*/

int SamplesPeriode[ NB_ACQUISITION ][ NB_ANALOG_CHANNELS ] =
{
    1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,3
};

/*---------------------------------------------------------------------------*/

void DoTestTableau( void )
{
    int i,j;

    for ( i = 0; i < NB_ANALOG_CHANNELS; i++ )
    {
        for ( j = 0; j < NB_ACQUISITION; j++ )
        {
            c_printf("%d - %d\n", j, SamplesPeriode[j][i] );
        }
    }
}

/*---------------------------------------------------------------------------*/

static void DoNothing( void ) {}

/*---------------------------------------------------------------------------*/

PMMENU_BEGIN( Tableau, "Tableau - Utilisation de tableau 2D" )
   PMMENU_ITEM_EX( 1, "Test", DoTestTableau )
   PMMENU_ITEM_EX( 2, "Rien", DoNothing)
PMMENU_END()

/*---------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Tableau )
