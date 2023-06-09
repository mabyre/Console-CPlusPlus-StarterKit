/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * TStack.c - Differentes facons de faire des traces avec le Reporter
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "pmEnv.h"
#include "pmXError.h"
#include "pmXTrace.h"
#include "cXStack.h"

/*--------------------------------------------------------------------------*/

static void DoTestStack1(void)
{
    char buffer[34] = "Hello a coucou form the PM little";

    pm_xstack_update_usage( "DoTestStack1" );

    PM_TRACE0(TL("stack de 34 char au dessus de la pile"));
}

/*--------------------------------------------------------------------------*/

static void DoTestStack2(void)
{
    int buffer[40];

    pm_xstack_update_usage( "DoTestStack2" );

    PM_TRACE0(TL("stack de 40 int"));
}

/*--------------------------------------------------------------------------*/

static void DoTestStack21(void)
{
    int buffer[41];

    pm_xstack_update_usage( "DoTestStack21" );

    PM_TRACE0(TL("stack de 41 int"));
}

/*--------------------------------------------------------------------------*/

static void DoTestStack0(void)
{
    pm_xstack_update_usage( "DoTestStack0" );
}

/*--------------------------------------------------------------------------*/

static void DoUpdateUsage(void)
{
    pm_xstack_update_usage( "DoUpdateUsage" );
}

/*--------------------------------------------------------------------------*/

static void DoTestStack01(void)
{
    int buffer[1];
    pm_xstack_update_usage( "DoTestStack0" );
}

/*--------------------------------------------------------------------------*/

static void DoTestStackMax(void)
{
    DoTestStack1();
    DoTestStack2();
    DoTestStack21();
}

/*--------------------------------------------------------------------------*/

static void DoTestStackEach(void)
{
    pm_xstack_reset_usage();
    DoTestStack1();

    pm_xstack_reset_usage();
    DoTestStack2();

    pm_xstack_reset_usage();
    DoTestStack0();

    pm_xstack_reset_usage();
    DoTestStack0();

    pm_xstack_reset_usage();
    DoTestStack01();
}

/*--------------------------------------------------------------------------*/

static void DoResetStackUsage(void)
{
    pm_xstack_reset_usage();
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Stack, "Stack" )
    PMMENU_ITEM_EX( 1, "Donner le taille de la pile ici", DoUpdateUsage )
    PMMENU_ITEM_EX( 2, "Detecter l'utilisation max de la pile", DoTestStackMax )
    PMMENU_ITEM_EX( 3, "Donner le taille de la pile a chaque fois", DoTestStackEach )
    PMMENU_ITEM_EX( 4, "Rester l'usage de la pile", DoResetStackUsage )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Stack )
