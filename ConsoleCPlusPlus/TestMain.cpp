/*--------------------------------------------------------------------------*\
 * TestMain.c
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "cXCore.h"
#include "cXStack.h"

/*--------------------------------------------------------------------------*/

PMMENU_DECLARE( Menu );
PMMENU_DECLARE( Trace );
PMMENU_DECLARE( Input );
PMMENU_DECLARE( Mem );
PMMENU_DECLARE( Test2 );
PMMENU_DECLARE( Cplusplus );
PMMENU_DECLARE( Containers );
PMMENU_DECLARE( Virtual );
PMMENU_DECLARE( Polymorphism );
PMMENU_DECLARE( Polymorphism2 );

/*--------------------------------------------------------------------------*/
void NT_TestMain(void)
{
   c_xstack_reset_usage();
   c_xthread_init();

   PMMenu_InitializeMainMenu();

   //PMMENU_USE( Menu );
   PMMENU_USE( Trace );
   //PMMENU_USE( Input );
   //PMMENU_USE( Mem );
   //PMMENU_USE( Test2 );
   PMMENU_USE( Cplusplus );
   //PMMENU_USE( Containers );
   //PMMENU_USE( Virtual );
   //PMMENU_USE( Polymorphism );
   //PMMENU_USE( Polymorphism2 );

   PMMenu_ExecuteMainMenu();

   PMMenu_DeleteMainMenu();

   c_xmemdbg_dump_state();
   c_xthread_term();
}
