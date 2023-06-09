/*===========================================================================*\
 * TestMain.c
 * (C) 1997 AbyreSoft - All rights reserved.
\*===========================================================================*/

#include "cMenu.h"
#include "cXCore.h"
#include "cXStack.h"

/*---------------------------------------------------------------------------*/

PMMENU_DECLARE( BOrder );
PMMENU_DECLARE( Core );
PMMENU_DECLARE( DLList );
PMMENU_DECLARE( Menu );
PMMENU_DECLARE( Input );
PMMENU_DECLARE( Stack );
PMMENU_DECLARE( XCore );

/*---------------------------------------------------------------------------*/
void NT_TestMain( void )
{
    /* c_xstack_reset_usage(); */
    c_xthread_init();

    PMMenu_InitializeMainMenu();

    PMMENU_USE( BOrder );
    PMMENU_USE( Core );
    PMMENU_USE( DLList );
    PMMENU_USE( Menu );
    PMMENU_USE( Input );
    PMMENU_USE( Stack );
    PMMENU_USE( XCore );

    PMMenu_ExecuteMainMenu();

    PMMenu_DeleteMainMenu();

    c_xmemdbg_dump_state();
    c_xthread_term();
}
