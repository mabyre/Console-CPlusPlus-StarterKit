/*--------------------------------------------------------------------------*\
 * TBOrder.c
 * Copyright (C) 1996-98 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "pmEnv.h"
#include "cXBOrder.h"
#include "cInput.h"

/*--------------------------------------------------------------------------*/

static void DoTestByteOrder(void)
{
    pmuint16 theUInt16;
    pmuint32 theUInt32;
    pmbyte   theStoreUInt16[2];
    pmbyte   theStoreUInt32[4];
    pmbyte   theStorage[5];

    pm_printf("\nTest: PM_MEMORY_NON_ALIGNED\n");
    pm_printf("\nThe following tests should not crash your computer.\n");
    pm_printf("If a crash occurs it's probably because the value of\n");
    pm_printf("the PM_MEMORY_NON_ALIGNED option is not correct.\n");

    pm_memset(theStorage, 0, sizeof(theStorage));
    c_memcpy_16(&theUInt16, &theStorage[1]);
    c_memcpy_32(&theUInt32, &theStorage[1]);

    pm_printf("Test completed : OK all ok it's fine\n");

    pm_printf("\nTest: PM_IN_LITTLE_ENDIAN\n");
    pm_printf("\nIf The following tests fail it's probably because the value of\n");
    pm_printf("PM_IN_LITTLE_ENDIAN option is not correct.\n\n");

    (void)Input_UInt16( "Enter an UInt16", &theUInt16, pmfalse, 0, pmfalse );
    pm_printf("  Before swap: 0x%hx\n", theUInt16);
    pm_printf("   After swap: 0x%hx\n", c_swap_u16(theUInt16));

    pm_printf("Network order: 0x%hx\n", c_ntoh16(theUInt16));

    pm_printf("      Storing: 0x%hx\n", theUInt16);
    c_store_u16(&theStoreUInt16, theUInt16);
    pm_printf("       Stored: 0x%hx 0x%hx\n", (pmuint16)theStoreUInt16[0], (pmuint16)theStoreUInt16[1]);
    c_get_u16(&theUInt16, &theStoreUInt16);
    pm_printf("      Getting: 0x%hx\n", theUInt16);
    pm_printf("      Getting: 0x%hx\n", c_read_u16(theStoreUInt16));

    pm_printf("\n");
    (void) Input_UInt32("Enter an UInt32", &theUInt32, pmfalse, 0, pmfalse);
    pm_printf("  Before swap: 0x%lx\n", theUInt32);
    pm_printf("   After swap: 0x%lx\n", c_swap_u32(theUInt32));

    pm_printf("Network order: 0x%lx\n", c_ntoh32(theUInt32));

    pm_printf("      Storing: 0x%lx\n", theUInt32);
    c_store_u32(&theStoreUInt32, theUInt32);
    pm_printf("       Stored: 0x%hx 0x%hx 0x%hx 0x%hx\n", (pmuint16)theStoreUInt32[0], (pmuint16)theStoreUInt32[1], (pmuint16)theStoreUInt32[2], (pmuint16)theStoreUInt32[3]);
    c_get_u32(&theUInt32, &theStoreUInt32);
    pm_printf("      Getting: 0x%lx\n", theUInt32);
    pm_printf("      Getting: 0x%hx\n", c_read_u32(theStoreUInt32));
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( BOrder, "Byte ordering" )
   PMMENU_ITEM_EX( 1, "Byte ordering", DoTestByteOrder )
PMMENU_END()

PMMENU_DEFAULT_HANDLER( BOrder )
