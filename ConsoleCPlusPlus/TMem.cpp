/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * TMem.c - Test memory functions
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "cInput.h"

#include "cCore.h"
#include "pmEnv.h"

/*--------------------------------------------------------------------------*/

static void DoSetMaxMem( void )
{
    pmuint32 theValue;

    if ( Input_UInt32( "Enter max memory", &theValue, pmfalse, 0, pmtrue ) )
        c_xmemdbg_set_max( theValue );
}

/*--------------------------------------------------------------------------*/

static void DoAllocBlock( void )
{
    pmuint32 theValue;

    if ( Input_UInt32( "Enter block size", &theValue, pmfalse, 0, pmtrue ) )
    {
        if ( c_malloc( theValue ) == 0 )
            c_printf( "Allocation failed.\n" );
    }
}

/*--------------------------------------------------------------------------*/

static void DoTestCheckMem( void )
{
    char* theStr = (char*)c_malloc( 100 );

    if ( theStr != 0 )
    {
        /*  Write after the block end.  */
        theStr[ 100 ] = 0;

        /*  Check memory status.    */
        c_xmemdbg_check();

        c_free( theStr );
    }
    else
        c_printf( "Not enough memory to perform the test\n" );
}

/*--------------------------------------------------------------------------*\
 * Reminder PM_BEGIN_PROC is to use at the beginning of each function
 * if you want to have a real information on stack management
\*--------------------------------------------------------------------------*/
static void DoTestBEGIN_PROC( void )
{
    /* La pile doit etre importante lors de l'appel de cette fonction */
    int bigTab[ 1024 ];

    PM_BEGIN_PROC( DoTestBEGIN_PROC );
    bigTab[ 0 ] = 1; /* avoid compilation warnig */
    /* Affichage :
    ** c_stack_dump_usage(): Stack maximum depth reached: (4476) in [DoTestBEGIN_PROC]
    */

}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Mem, "Memory" )
    PMMENU_ITEM_EX( 1, "Set max mem", DoSetMaxMem )
    PMMENU_ITEM_EX( 2, "Allocate block", DoAllocBlock )
    PMMENU_ITEM_EX( 3, "Test CheckMem", DoTestCheckMem )
    PMMENU_ITEM_EX( 4, "Test PM_BEGIN_PROC", DoTestBEGIN_PROC )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Mem )
