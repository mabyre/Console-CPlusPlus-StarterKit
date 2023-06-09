/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------*
 * TFormatPrintf.c - Written by Bruno Raby
 * Regarder les differentes possibilites du format de printf()
\*--------------------------------------------------------------------------*/

#include <conio.h>

#include "cMenu.h"
#include "cCore.h"
#include "cConsole.h"
#include "cInput.h"

/*---------------------------------------------------------------------------*\
    A format specification, which consists of optional and required fields, 
    has the following form:
    %[flags] [width] [.precision] [{h | l | I64 | L}]type
    Flag Directives
    ===============
    Flag Meaning                                              Default 
    -    Left align the result within the given field width.  Right align. 
    +    Prefix the output value with a sign (+ or -) if 
         the output value is of a signed type. Sign appears only for negative signed values (-). 
    0    If width is prefixed with 0, zeros are added until the minimum width is reached. 
         If 0 and - appear, the 0 is ignored. If 0 is specified with an integer format 
         (i, u, x, X, o, d) the 0 is ignored.                 No padding. 
    blank (' ') Prefix the output value with a blank if the output value is signed and 
         positive; the blank is ignored if both the blank and + flags appear.      No blank appears. 
    #    When used with the o, x, or X format, the # flag prefixes any nonzero output value 
         with 0, 0x, or 0X, respectively.                     No blank appears. 
         When used with the e, E, or f format, the # flag forces the output value to contain 
         a decimal point in all cases.                        Decimal point appears only if digits follow it. 
         When used with the g or G format, the # flag forces the output value to contain 
         a decimal point in all cases and prevents the truncation of trailing zeros. 
         Ignored when used with c, d, i, u, or s.
\*---------------------------------------------------------------------------*/


static void DoTest( void )
{
    float  f = (float)123456.123456789;
    double d1 = 123456.123456789;
    double d = 1.2;

    double fp = 251.7366;
       
    // printf 
    c_printf("%f [%-20.4f]\n", f , f ) ;
    c_printf("%f [%g]\n", f , f ) ;
    c_printf("%f [%g]\n", d , d ) ;
    c_printf("%f [%E]\n", d1 , d1 ) ;



   /* Display real numbers. */
   printf( "Real numbers:\n%f %.2f %e %E\n", fp, fp, fp, fp );
}

/*---------------------------------------------------------------------------*/

void DoNothing( void ){}

/*---------------------------------------------------------------------------*/

PMMENU_BEGIN( FormatPrintf, "FPf - Test format de printf" )
   PMMENU_ITEM_EX(1, "Test"    , DoTest)
   PMMENU_ITEM_EX(2, "Nothing" , DoNothing)
PMMENU_END()

/*---------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( FormatPrintf )
