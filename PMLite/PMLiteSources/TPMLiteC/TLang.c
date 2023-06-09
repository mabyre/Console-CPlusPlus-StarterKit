/*---------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 AbyreSoft. Written by Bruno Raby.
 *---------------------------------------------------------------------------*
 * TLang.c - Tests du langage C
\*---------------------------------------------------------------------------*/

#include <conio.h>

#include "cMenu.h"
#include "cCore.h"
#include "cConsole.h"
#include "cInput.h"

/*---------------------------------------------------------------------------*\
 * Quel est l'effet de Break et Continue dans une boucle
 *---------------------------------------------------------------------------*
 * The continue keyword passes control to the next iteration of the smallest 
 * enclosing do, for, or while statement in which it appears.
 *---------------------------------------------------------------------------*
 * The break keyword terminates the smallest enclosing do, for, switch, or 
 * while statement in which it appears.
\*---------------------------------------------------------------------------*/
void DoTestBreakContinue( void )
{
    int i ;

    c_printf("Break dans un For\n");

    for ( i = 0 ; i < 5 ; i++ )
    {
        /* Traitement */
        c_printf("indice [%d]\n", i );

        if ( i == 3 ) break; /* Sortie de boucle */
    }

    c_printf("sortie avec i == [%d]\n\n", i );


    c_printf("Continue dans un For\n");

    for ( i = 0 ; i < 5 ; i++ )
    {
        /* Traitement 1 */
        c_printf("Traitement 1 - indice [%d]\n", i );

        if ( i == 3 ) continue; /* Aller a la prochaine boucle */

        c_printf("Traitement 2 - indice [%d]\n", i );
    }

/*  Resultat de l'execution : */
/*
    Break dans un For
    indice [0]
    indice [1]
    indice [2]
    indice [3]
    sortie avec i == [3]

    Continue dans un For
    Traitement 1 - indice [0]
    Traitement 2 - indice [0]
    Traitement 1 - indice [1]
    Traitement 2 - indice [1]
    Traitement 1 - indice [2]
    Traitement 2 - indice [2]
    Traitement 1 - indice [3] <== Le Traitement 2 n'est pas effectue
    Traitement 1 - indice [4]
    Traitement 2 - indice [4]
*/
}

/*---------------------------------------------------------------------------*\
 * Test du passage de parametres par valeur
\*---------------------------------------------------------------------------*/

double _MultiplyD( double a, double b )
{
    return ( a * b );
}

/*---------------------------------------------------------------------------*/

float _MultiplyF( float a, float b )
{
    return ( a * b );
}

/*---------------------------------------------------------------------------*/

float _MultiplyFP( float *a, float *b )
{
    return ( *a * *b );
}

/*---------------------------------------------------------------------------*/

void DoTestParameters( void )
{
    float a = (float)1.23456 ;
    float b = (float)1.123456 ;
    float c ;

    c = _MultiplyF( a , b );
    c_printf("resultat : [%f]\n", c );

    c = _MultiplyFP( &a , &b );
    c_printf("resultat : [%f]\n", c );

    c = (float)_MultiplyD( a , b );
    c_printf("resultat : [%f]\n", c );
}

/*----------------------------------------------------------------------------*\
 * Taille des objets C
\*----------------------------------------------------------------------------*/

void DoSizeOf( void )
{
    /* Le short est-il signe ? non signe ? */
    short a = -2;
    unsigned short b = -2;
    char ca = -2;
    unsigned char cb = -2;

    c_printf("short a = -2; => [%d] [%x]\n", a , a );
    c_printf("unsigned short b = -2; => [%d] [%x]\n", b , b );

    c_printf("char ca = -2; => [%d] [%x]\n", ca , ca );
    c_printf("unsigned char cb = -2; => [%d] [%x]\n", cb , cb );

    c_printf( "Taille d'un char   : [%d] oct.\n", sizeof( char ) );
    c_printf( "Taille d'un int    : [%d] oct.\n", sizeof( int ) );
    c_printf( "Taille d'un double : [%d] oct.\n", sizeof( double ) );
    c_printf( "Taille d'un float  : [%d] oct.\n", sizeof( float ) );
    c_printf( "Taille d'un short  : [%d] oct.\n", sizeof( short ) );
    c_printf( "unsigned int  : [%d] oct.\n", sizeof( unsigned int ) );
    c_printf( "unsigned long : [%d] oct.\n", sizeof( unsigned long ) );

/**** Affichage
short a = -2; => [-2] [fffffffe]
unsigned short b = -2; => [65534] [fffe]
Taille d'un char   : [1] oct.
Taille d'un int    : [4] oct.
Taille d'un double : [8] oct.
Taille d'un float  : [4] oct.
Taille d'un short  : [2] oct.
unsigned int  : [4] oct.
unsigned long : [4] oct.
****/
}

/*---------------------------------------------------------------------------*\
 * Utilisation de l'operateur module (%) et de la division entiere (/) pour 
 * determiner sur combien de mots de 32 bits (4 octets) tient un objet dont 
 * la taille est donnee en octets (mot de 8bits)
\*---------------------------------------------------------------------------*/

int NumberOfWords( int i )
{
    int r;

    r = i/4;

    if ( i % 4 ) r = r + 1;

    return r;
}

void DoDivisonEntiere( void )
{
    c_printf(" 1/4 : %d -> now : %d\n", 1/4, NumberOfWords(1) );
    c_printf(" 2/4 : %d -> now : %d\n", 2/4, NumberOfWords(2) );
    c_printf(" 3/4 : %d -> now : %d\n", 3/4, NumberOfWords(3) );
    c_printf(" 4/4 : %d -> now : %d\n", 4/4, NumberOfWords(4) );
    c_printf(" 5/4 : %d -> now : %d\n", 5/4, NumberOfWords(5) );
    c_printf(" 6/4 : %d -> now : %d\n", 6/4, NumberOfWords(6) );
    c_printf(" 7/4 : %d -> now : %d\n", 7/4, NumberOfWords(7) );
    c_printf(" 8/4 : %d -> now : %d\n", 8/4, NumberOfWords(8) );
    c_printf(" 9/4 : %d -> now : %d\n", 9/4, NumberOfWords(9) );
    c_printf("10/4 : %d -> now : %d\n", 10/4, NumberOfWords(10) );
    c_printf("11/4 : %d -> now : %d\n", 11/4, NumberOfWords(11) );
    c_printf("12/4 : %d -> now : %d\n", 12/4, NumberOfWords(12) );

/* Affichage :
 1/4 : 0 -> now : 1
 2/4 : 0 -> now : 1
 3/4 : 0 -> now : 1
 4/4 : 1 -> now : 1
 5/4 : 1 -> now : 2
 6/4 : 1 -> now : 2
 7/4 : 1 -> now : 2
 8/4 : 2 -> now : 2
 9/4 : 2 -> now : 3
10/4 : 2 -> now : 3
11/4 : 2 -> now : 3
12/4 : 3 -> now : 3
*/
}

/*---------------------------------------------------------------------------*/

PMMENU_BEGIN(Lang, "TLang : Test du Langage C")
   PMMENU_ITEM_EX(1, "Taille des elements avec sizeof()", DoSizeOf)
   PMMENU_ITEM_EX(2, "Passage de parametres par valeurs", DoTestParameters)
   PMMENU_ITEM_EX(3, "Sortie de boucle - test \"break\" et \"continue\" ", DoTestBreakContinue)
   PMMENU_ITEM_EX(4, "Division entiere", DoDivisonEntiere)
PMMENU_END()

/*---------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Lang )
