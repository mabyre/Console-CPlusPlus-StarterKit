/*===========================================================================*\
 * TestMain.c
 * (C) 1997 AbyreSoft - All rights reserved.
\*===========================================================================*/

#include "cMenu.h"

/*---------------------------------------------------------------------------*/

PMMENU_DECLARE( Report );

/*---------------------------------------------------------------------------*/
void NT_TestMain(void)
{
   PMMenu_InitializeMainMenu();

   PMMENU_USE( Report );

   PMMenu_ExecuteMainMenu();
   PMMenu_DeleteMainMenu();
}
