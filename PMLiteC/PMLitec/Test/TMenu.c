/*===========================================================================*\
 * Copyright (C) 1996-2023 SoDevLog. All rights reserved.
 *===========================================================================*
 * TMenu.c - Menu's Module Test
\*===========================================================================*/

#include "cMenu.h"
#include "cConsole.h"

/*---------------------------------------------------------------------------*/

static void DoItem1( void )
{
    c_printf( "Do Menu Item 1.\n" );
}

/*---------------------------------------------------------------------------*/

static void DoItem2( void )
{
    c_printf( "Do Menu Item 2.\n" );
}

/*---------------------------------------------------------------------------*/

static void DoItem3( char *aParam )
{
    c_printf( "Do Menu Item 3 with param (%s).\n", aParam );
}

/*---------------------------------------------------------------------------*/

static void DoInit( void )
{
    c_printf( "DoInit: Executed at Init.\n" );
}

/*---------------------------------------------------------------------------*/

static void DoClose( void )
{
    c_printf( "DoClose: Executed while Closing.\n" );
}


/*---------------------------------------------------------------------------*\
 * Sub menu functions
\*---------------------------------------------------------------------------*/

static void DoExecuteSubMenu( void )
{
    PMMENU_SUBEXECUTE( SubMenu1 );
}

/*---------------------------------------------------------------------------*/

static void DoSubMenuItem1()
{
    c_printf( "Do Sub menu 1 - item 1.\n" );
}

/*---------------------------------------------------------------------------*/

static void DoSubMenuItem2()
{
    c_printf( "Do Sub menu 1 - item 2.\n" );
}

/*---------------------------------------------------------------------------*/

static void DoSubMenuItem3( void )
{
    c_printf( "Do Sub menu 1 - item 3.\n" );
}

/*---------------------------------------------------------------------------*\
 * Main menu definition
\*---------------------------------------------------------------------------*/

PMMENU_BEGIN( Menu, "Menu Test" )
PMMENU_ITEM_EX( 1, "Item 1", DoItem1 )
PMMENU_ITEM_SEPARATOR( "-----------------" )
PMMENU_ITEM( 2, "Item 2" )
PMMENU_ITEM_SEPARATOR( "-----------------" )
PMMENU_ITEM( 3, "Item 3 : Execution with param" )
PMMENU_ITEM_EX( 4, "Sub menu 1", DoExecuteSubMenu )
PMMENU_END()

/*---------------------------------------------------------------------------*/

PMMENU_HANDLER( Menu )
{
    PMMENU_CHOICE_BEGIN()

        PMMENU_CASE_INIT( DoInit );
        PMMENU_CASE_CLOSE( DoClose );

        PMMENU_CASE_CHOICE( 2, DoItem2 );
        PMMENU_CASE_CHOICE_1( 3, DoItem3, "Test" );

    PMMENU_CHOICE_END()
}

/*---------------------------------------------------------------------------*\
 * Sub menu 1 definition
\*---------------------------------------------------------------------------*/

PMMENU_BEGIN( SubMenu1, "Sub menu 1" )
PMMENU_ITEM( 1, "SM Item 1" )
PMMENU_ITEM( 2, "SM Item 2" )
PMMENU_ITEM_EX( 3, "SM Item 3", DoSubMenuItem3 )
PMMENU_END()

PMMENU_HANDLER( SubMenu1 )
{
    PMMENU_CHOICE_BEGIN()

        PMMENU_CASE_CHOICE_1( 1, DoSubMenuItem1, "Sub menu 1 : Item 1" );
        PMMENU_CASE_CHOICE_1( 2, DoSubMenuItem2, "Sub menu 1 : Item 2" );

    PMMENU_CHOICE_END()
}

/*---------------------------------------------------------------------------*/
