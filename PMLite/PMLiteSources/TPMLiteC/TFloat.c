/*--------------------------------------------------------------------------*\
 * TLang.c
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
\*--------------------------------------------------------------------------*/

#include <conio.h>

#include "cMenu.h"
#include "cCore.h"
#include "cConsole.h"
#include "cInput.h"

/* ---------------------------------------------------------------------------   */

double MultiplyD( double a, double b )
{
    return ( a * b );
}

/* ---------------------------------------------------------------------------   */

float MultiplyF( float a, float b )
{
    return ( a * b );
}

/* ---------------------------------------------------------------------------   */

float MultiplyFP( float *a, float *b )
{
    return ( *a * *b );
}

/* ---------------------------------------------------------------------------   */

void _DoSizeOf( void )
{
      c_printf("Taille d'un float  : [%d] oct.\n", sizeof( float ) ) ;
      c_printf("Taille d'un double : [%d] oct.\n", sizeof( double ) ) ;
}

/* ---------------------------------------------------------------------------   */

void _DoTestParameters( void )
{
    float a = (float)1.23456 ;
    float b = (float)1.123456 ;
    float c ;

    c = MultiplyF( a , b );
    c_printf("resultat : [%f]\n", c );

    c = MultiplyFP( &a , &b );
    c_printf("resultat : [%f]\n", c );

    c = (float)MultiplyD( (double)a , (double)b );
    c_printf("resultat : [%f]\n", c );
}

/* ---------------------------------------------------------------------------   */

PMMENU_BEGIN(Float, "Test Float")
   PMMENU_ITEM_EX(1, "Size of"    , _DoSizeOf)
   PMMENU_ITEM_EX(2, "Pass parameters" , _DoTestParameters)
PMMENU_END()

/* ---------------------------------------------------------------------------   */

PMMENU_DEFAULT_HANDLER(Float)
