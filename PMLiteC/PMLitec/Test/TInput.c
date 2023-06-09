/*---------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 - AbyreSoft. All rights reserved.
 *---------------------------------------------------------------------------*
 * TInput.c - Written by Bruno Raby.
\*---------------------------------------------------------------------------*/

#include <conio.h>
#include <ctype.h>

#include "pmXTrace.h"
#include "pmTrace.h"
#include "cMenu.h"
#include "cCore.h"
#include "cConsole.h"
#include "cInput.h"

/*---------------------------------------------------------------------------*\
 * int isalnum( int c );
/*---------------------------------------------------------------------------*/
static void DoTestIsalnum( void )
{
    char *c1 = "a";
//    char *c2;
    int result;

    result  = isalnum( *c1 );
    c_printf("result : %d , c1 : %c\n", result, *c1 );

    /* Crash c2 n'est pas definit 
    result  = isalnum( (int)(*c2) );
    c_printf("result : %d , c2 : %c\n", result, *c2 );
    */
}

/*---------------------------------------------------------------------------*/

void DoEnterAChar( void )
{
    char c = 'Q';

    c_printf("Touch a key ? - <0> to exit\n");
    while ( c != '0' )
    {
        c = c_getc();
        c_printf("c:[%c], x:[%02X], d:[%d]\n", c , c , c) ;
    }
}

/*---------------------------------------------------------------------------*/

static void DoGetInt32( void )
{
    int val;

    c_printf("Get an int32 ?");
    val = c_get_int32();
    c_printf("val [%d]", val );
}

/*---------------------------------------------------------------------------*/

static void DoInput_UInt32( void )
{
    pmbool   theResult;
    pmuint32 theValue;
    pmuint32 theDefaultValue = 0xffff;

    //pmbool Input_UInt32
    //(
    //    char* aPrompt, 
    //    pmuint32* aValue, 
    //    pmbool afDefault, 
    //    pmuint32 aDefaultValue, 
    //    pmbool afCancel
    //);

    theResult = Input_UInt32
    ( 
        "Entrer un UINT32 ", 
        &theValue, 
        pmtrue, 
        theDefaultValue, 
        pmtrue
    );

    c_printf("result : [%d]\n" , theResult );
    c_printf("valeur : [%d]"   , theValue );
}

/*---------------------------------------------------------------------------*/

static void DoInput_String( void )
{
    pmbool   theResult;
    char     theString[80];
    double   theDouble;

    //pmbool Input_String
    //(
    //    char* aPrompt, 
    //    char* aBuffer, 
    //    char* aDefaultValue, 
    //    pmbool afCancel, 
    //    pmbool afEmpty
    //);

    theResult = Input_String("Entrer une chaine ", theString, "Hello", pmfalse, pmfalse );
    theDouble= atof( theString );

    c_printf("result : [%d]\n" , theResult );
    c_printf("string : [%s]\n" , theString );
    c_printf("double : [%f]\n" , theDouble );
}

/*---------------------------------------------------------------------------*/

static void DoInput_NewString( void )
{
    int     theResult;
//    char    theString1[80];
    char    theString2[80] = "the string";

    /* Crash la fonction Input_NewString car theString1 doit etre initialisee */
//    theResult = Input_NewString("Entrer une chaine ", theString1, pmfalse );
//    c_printf("string : [%s]\n" , theString1 );

    theResult = Input_NewString("Entrer une chaine ", theString2, pmtrue );
    c_printf("string : [%s]\n" , theString2 );

    theResult = Input_NewString("Entrer une chaine ", theString2, pmfalse );
    c_printf("string : [%s]\n" , theString2 );
}

/*---------------------------------------------------------------------------*/

static void DoAfficherTraceFloat( void )
{
    char   theBuffer[200];
    char   theString[80];
    double theDouble;
    int    theResult;
    int    taille;

    theResult = Input_String("Entrer un flottant ", theString, "Hello", pmfalse, pmfalse );
    theDouble = atof( theString );

    c_printf("Resultat de Input_String : %d\n", theResult);

    /* Format and print various data */
    taille  = sprintf( theBuffer, "%f", theDouble );
    PM_TRACE0(TL("%s", theBuffer ));
}

/*---------------------------------------------------------------------------*/

static DoTestTraceFloat( void )
{
    double theDouble;

    theDouble = 3.14159265358979;

    pm_trace0( 0, 356.3,  0 );
    pm_trace0( "%1.5f", theDouble,  0 );
    pm_trace0( "", theDouble,  1 );
    pm_trace0( "", -2.1,  0 );
    pm_trace0( "%1.1f", -3.0,  1 );
    pm_trace0( "", -theDouble,  1 );
    pm_trace0( "%020.10f", -theDouble,  1 );

/* Affichage :
356.3000003.141593.141593
-2.100000-3.0
-3.141593
-00000003.1415926536
 */

}

/*---------------------------------------------------------------------------*/

PMMENU_BEGIN( Input, "Inputs - Test Chars" )
    PMMENU_ITEM_EX(1, "isalnum"                          , DoTestIsalnum )
    PMMENU_ITEM_EX(2, "c_get_int32"                      , DoGetInt32 )
    PMMENU_ITEM_EX(3, "c_getc"                           , DoEnterAChar )
    PMMENU_ITEM_EX(4, "Input_UInt32()"                   , DoInput_UInt32 )
    PMMENU_ITEM_EX(5, "Input_String()"                   , DoInput_String )
    PMMENU_ITEM_EX(6, "Input_NewString()"                , DoInput_NewString )
    PMMENU_ITEM_EX(7, "Afficher un float dans la trace " , DoAfficherTraceFloat )
    PMMENU_ITEM_EX(8, "pm_trace_float()"                 , DoTestTraceFloat )
PMMENU_END()

/*---------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Input )
