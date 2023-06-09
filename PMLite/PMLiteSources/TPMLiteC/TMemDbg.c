/*--------------------------------------------------------------------------*\
 * TMemDbg.c
 * Copyright (C) 1996-98 AbyreSoft. All rights reserved.
\*--------------------------------------------------------------------------*/

#include "pmTypes.h"
#include "pmEnv.h"
#include "cMenu.h"
#include "cInput.h"

#include "cCore.h"
#include "pmXTrace.h"

/*--------------------------------------------------------------------------*
/* Allocate big static bloc before playing with memory allocation
*/
int gBigTableBloc[10000];
int *gBloc = &gBigTableBloc[0];

/*--------------------------------------------------------------------------*/

static void DoSetMaxMem(void)
{
   pmuint32 theValue;
   
   if (Input_UInt32("Enter the maximum memory to allocate", &theValue, pmfalse, 0, pmtrue))
      pm_xmemdbg_set_max( theValue );
}

/*--------------------------------------------------------------------------*/

static void DoAllocBlock(void)
{
   pmuint32 theValue;

   if ( Input_UInt32("Enter a block size", &theValue, pmfalse, 0, pmtrue) )
   {
        gBloc = pm_malloc( theValue );
        if ( gBloc == NULL )
            pm_printf("Allocation failed.\n");
        pm_xmemdbg_dump_state();
   }
}

/*--------------------------------------------------------------------------*/

static void DoFreeBlock(void)
{
    pm_free( gBloc );
    if ( gBloc == NULL )
        pm_printf("Freeing succed.\n");
    pm_xmemdbg_dump_state();
}

/*--------------------------------------------------------------------------*/

static void DoPerformMemoryCheck(void)
{
   char* theStr = (char*)pm_malloc( 100 );

   if (theStr != 0)
   {
      /* Write after the block end. */
      theStr[100] = 0;
      
      /* Check memory status. */
      pm_xmemdbg_check();

      pm_free( theStr );
   }
   else
      pm_printf("Not enough memory to perform the test\n");
}

/*--------------------------------------------------------------------------*/

static void DoDumpSate(void)
{
   pm_xmemdbg_dump_state();
}

/*--------------------------------------------------------------------------*/

static void DoDumpCheck(void)
{
   pm_xmemdbg_check(); 
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( MemDbg, "Memory debug test functions" )
   PMMENU_ITEM_EX( 1, "Set maximum memory", DoSetMaxMem )
   PMMENU_ITEM_EX( 2, "Allocate a block", DoAllocBlock )
   PMMENU_ITEM_EX( 3, "Free a bloc", DoFreeBlock )
   PMMENU_ITEM_EX( 4, "Perform memory check", DoPerformMemoryCheck )
   PMMENU_ITEM_EX( 5, "Dump memory state", DoDumpSate )
   PMMENU_ITEM_EX( 6, "Check memory", DoDumpCheck )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( MemDbg )
