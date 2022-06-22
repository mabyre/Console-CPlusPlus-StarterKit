/*--------------------------------------------------------------------------*\
 * TestMain.c
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "cXCore.h"
#include "cXStack.h"

/*--------------------------------------------------------------------------*/

PMMENU_DECLARE( Menu );
PMMENU_DECLARE( Test2 );
PMMENU_DECLARE( Cplusplus );

/*--------------------------------------------------------------------------*/
void NT_TestMain(void)
{
   c_xstack_reset_usage();
   c_xthread_init();

   PMMenu_InitializeMainMenu();

   PMMENU_USE( Menu );
   PMMENU_USE( Cplusplus );
   PMMENU_USE( Test2 );

   PMMenu_ExecuteMainMenu();

   PMMenu_DeleteMainMenu();

   c_xmemdbg_dump_state();
   c_xthread_term();
}
