/*===========================================================================*\
 * TestMain.c
 * (C) 1997 AbyreSoft - All rights reserved.
\*===========================================================================*/

#include "pmEnv.h"
#include "cMenu.h"
#include "cXCore.h"

/*---------------------------------------------------------------------------*/

PMMENU_DECLARE( DLList );
PMMENU_DECLARE( Stack );
PMMENU_DECLARE( XCore );
PMMENU_DECLARE( MemDbg );
PMMENU_DECLARE( Core );
PMMENU_DECLARE( BOrder );
PMMENU_DECLARE( Report );
PMMENU_DECLARE( PMDebug );
PMMENU_DECLARE( PMTrace );
PMMENU_DECLARE( Lang );
PMMENU_DECLARE( Menu );

/*---------------------------------------------------------------------------*/
void NT_TestMain(void)
{
   pm_xstack_reset_usage();
   pm_xstack_dump_usage();
   pm_xthread_init();

   PMMenu_InitializeMainMenu();

   PMMENU_USE( DLList );
   PMMENU_USE( Menu );
   PMMENU_USE( Stack );
   PMMENU_USE( XCore );
   PMMENU_USE( Core );
   PMMENU_USE( MemDbg );
   PMMENU_USE( BOrder );
   PMMENU_USE( Report );
   PMMENU_USE( PMDebug );
   PMMENU_USE( PMTrace );
   PMMENU_USE( Lang );
   PMMENU_USE( Menu );

   PMMenu_ExecuteMainMenu();
   PMMenu_DeleteMainMenu();

   pm_xstack_dump_usage();
   pm_xmemdbg_dump_state();
   pm_xthread_term();
}
