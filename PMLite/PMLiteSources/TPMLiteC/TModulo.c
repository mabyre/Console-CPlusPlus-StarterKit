/*---------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 AbyreSoft. Written by Bruno Raby.
 *---------------------------------------------------------------------------*
 * TModulo.c - Tests de l'utiliation de l'operateur modulo
\*---------------------------------------------------------------------------*/

#include <conio.h>
#include "cMenu.h"
#include "cCore.h"
#include "cConsole.h"
#include "cInput.h"
#include "pmXDebug.h"

/*---------------------------------------------------------------------------*\
 * Utilisation de l'operateur modulo
 *---------------------------------------------------------------------------*
 * indice_1 suit indice et quand indice vaut 0 indice_1 vaut NB_MAX
\*---------------------------------------------------------------------------*/
void DoTestModulo1( void )
{
#define NB_MAX 3
    
    int i;
    static int indice = 0;
    static int indice_1 = 0;
    static int indice_2 = 0;

    for ( i = 0 ; i < 10 ; i++ )
    {
        indice = ( indice + 1 ) % NB_MAX;
        indice_1 = ( indice - 1 + NB_MAX ) % NB_MAX;
        indice_2 = ( indice - 2 + NB_MAX ) % NB_MAX;

        c_printf("===> %d\n", i);
        c_printf("i   : [%d]\n", indice);
        c_printf("i_1 : [%d]\n", indice_1);
        c_printf("i_2 : [%d]\n", indice_2);
        c_printf("--------------------\n");

    }

#undef NB_MAX
}

/*---------------------------------------------------------------------------*/

void DoTestModulo2( void )
{
#define NB_ECHANT 24
    
    int i;
    static int indice = 0;
    static int indice_345 = 0;

    for ( i = 0 ; i < 100 ; i++ )
    {
//        DEBUG0("i   : [%d]", indice);

        indice_345 = ( indice - 0 + NB_ECHANT) % NB_ECHANT;
        indice = ( indice + 1 ) % NB_ECHANT;

//        DEBUG0("i_1 : [%d]", indice_345);
//        DEBUG0("--------------------");
    }

#undef NB_ECHANT
}

/*---------------------------------------------------------------------------*/

void DoTestModulo3( void )
{
    
    int i;

    for ( i = 0 ; i < 40 ; i++ )
    {
        if ( (i % 4) == 0 )
        {
            PM_DEBUG0(TL( "[%d] : (i mod 4) == 0", i ));
        }
    }
}

/*---------------------------------------------------------------------------*/

PMMENU_BEGIN( Modulo, "Modulo - Utilisation de l'operateur modulo" )
   PMMENU_ITEM_EX( 1, "Utilisation du modulo", DoTestModulo1 )
   PMMENU_ITEM_EX( 2, "Modulo sur 24 echantillons", DoTestModulo2 )
   PMMENU_ITEM_EX( 3, "Comment ca demarre ?", DoTestModulo3 )
PMMENU_END()

/*---------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Modulo )
