/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------*
 * TPMDebug.c - Written by Bruno Raby.
 * Test des fonctions de debug du module pmDebug
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "cInput.h"

#include "pmXDebug.h"

/*--------------------------------------------------------------------------*/

static void DoTestPM_CODE( void )
{
    PM_CODE0( PM_DEBUG0(TL("Ce code est actif si DEBUG_LEVEL > 0")); );
    PM_CODE1( PM_DEBUG0(TL("Ce code est actif si DEBUG_LEVEL > 1")); );
    PM_CODE2( PM_DEBUG0(TL("Ce code est actif si DEBUG_LEVEL > 2")); );
}

/*--------------------------------------------------------------------------*/

static void DoTestBEGIN_PROC( void )
{
    /* La pile doit etre importante lors de l'appel de cette fonction */
    int grosTableau[ 1024 ];

    PM_BEGIN_PROC( DoTestBEGIN_PROC );
    grosTableau[ 0 ] = 1; /* avoid compilation warnig */
    /* Affichage :
    ** c_stack_dump_usage(): Stack maximum depth reached: (4476) in [DoTestBEGIN_PROC]
    */

}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( PMDebug, "PMDebug")
	PMMENU_ITEM_EX(2, "Debug code test", DoTestPM_CODE)
	PMMENU_ITEM_EX(3, "PM_BEGIN_PROC", DoTestBEGIN_PROC)
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( PMDebug )
