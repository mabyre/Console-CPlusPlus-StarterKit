/*--------------------------------------------------------------------------*\
 * TTableVerite.c
 * Copyright (c) - Written by Bruno Raby.
\*--------------------------------------------------------------------------*/

#include <conio.h>

#include "cMenu.h"
#include "cCore.h"
#include "cConsole.h"
#include "cInput.h"

/*----------------------------------------------------------------------------*/

void DoPrintTable( void )
{
    unsigned int a,b;

    /*------------------------------*/
    printf("\ntable &\n");
    printf("-------\n");
    a=0;
    b=0;
    printf("0 0 | %d\n", a&b);
    a=0;
    b=1;
    printf("0 1 | %d\n", a&b);
    a=1;
    b=0;
    printf("1 0 | %d\n", a&b);
    a=1;
    b=1;
    printf("1 1 | %d\n", a&b);
    /*------------------------------*/
    printf("\ntable |\n");
    printf("-------\n");
    a=0;
    b=0;
    printf("0 0 | %d\n", a|b);
    a=0;
    b=1;
    printf("0 1 | %d\n", a|b);
    a=1;
    b=0;
    printf("1 0 | %d\n", a|b);
    a=1;
    b=1;
    printf("1 1 | %d\n", a|b);
    /*------------------------------*/
    printf("\ntable ^\n");
    printf("-------\n");
    a=0;
    b=0;
    printf("0 0 | %d\n", a^b);
    a=0;
    b=1;
    printf("0 1 | %d\n", a^b);
    a=1;
    b=0;
    printf("1 0 | %d\n", a^b);
    a=1;
    b=1;
    printf("1 1 | %d\n", a^b);
    /*------------------------------*/
    printf("\ntable !^\n");
    printf("-------\n");
    a=0;
    b=0;
    printf("0 0 | %d\n", !(a^b));
    a=0;
    b=1;
    printf("0 1 | %d\n", !(a^b));
    a=1;
    b=0;
    printf("1 0 | %d\n", !(a^b));
    a=1;
    b=1;
    printf("1 1 | %d\n", !(a^b));
}

/*----------------------------------------------------------------------------*/

PMMENU_BEGIN(TableVerite, "Afficher les Tables de verite")
   PMMENU_ITEM_EX(1, "Table de verite", DoPrintTable)
PMMENU_END()

/*----------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER(TableVerite)
