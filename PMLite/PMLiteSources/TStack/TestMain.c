/*===========================================================================*\
 * TestMain.c
 * (C) 1997 AbyreSoft - All rights reserved.
\*===========================================================================*/

#include "cMenu.h"
//#include "cXCore.h"
#include "cXStack.h"

/*---------------------------------------------------------------------------*/

PMMENU_DECLARE( Stack );

/*---------------------------------------------------------------------------*/

void NT_TestMain(void)
{
   c_xstack_reset_usage();
   c_xstack_dump_usage();

   PMMenu_InitializeMainMenu();

   PMMENU_USE( Stack );

   PMMenu_ExecuteMainMenu();
   PMMenu_DeleteMainMenu();

   c_xstack_dump_usage();
}
