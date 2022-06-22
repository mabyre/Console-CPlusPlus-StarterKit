/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * TTrace.c - Differentes facons de faire des traces
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "cInput.h"

#include "cXTrace.h"
#include "cCore.h"

#include "pmTrace.h"
#include "pmXError.h"
#include "pmXTrace.h"
#include "pmXDebug.h"

/*--------------------------------------------------------------------------*/

static void DoTrace( void )
{
    c_trace( TL( pmT_Core, "Verify you add message from PMLite Core"));
}

/*--------------------------------------------------------------------------*/

static void DoTraceError(void)
{
    PM_ERROR0(TL("ERROR 0 - Error tres grave"));
    PM_ERROR1(TL("ERROR 1 - Error grave"));
    PM_ERROR2(TL("ERROR 2 - Error faible"));
    PM_ERROR3(TL("ERROR 3 - Error tres faible"));
}

/*--------------------------------------------------------------------------*/

static void DoTraceTrace(void)
{
    PM_TRACE0(TL("TRACE 0 - Error tres grave"));
    PM_TRACE1(TL("TRACE 1 - Error grave"));
    PM_TRACE2(TL("TRACE 2 - Error faible"));
    PM_TRACE3(TL("TRACE 3 - Error tres faible"));
}

/*--------------------------------------------------------------------------*/

static void DoTraceDebug(void)
{
    size_t      i;
    pmbyte      theBuffer[ 256 ];

    PM_DEBUG0(TL("DEBUG 0 - Error tres grave"));
    PM_DEBUG1(TL("DEBUG 1 - Error grave"));
    PM_DEBUG2(TL("DEBUG 2 - Error faible"));
    PM_DEBUG3(TL("DEBUG 3 - Error tres faible"));

    for ( i = 0; i < 256; i++ )
        theBuffer[ i ] = (pmbyte)i;
    
    PM_DEBUG3(TL( "This is a buffer:" ));
    PM_DEBUG3(B(theBuffer, (size_t) 256));

}

/*--------------------------------------------------------------------------*/

static void DoTestPM_CODE( void )
{
    /* Macro pour inserer ou retirer du code de debug */
    PM_CODE0( PM_DEBUG0( TL( "Ce code est actif si DEBUG_LEVEL > 0" ) ); );
    PM_CODE1( PM_DEBUG0( TL( "Ce code est actif si DEBUG_LEVEL > 1" ) ); );
    PM_CODE2( PM_DEBUG0( TL( "Ce code est actif si DEBUG_LEVEL > 2" ) ); );
}

/*--------------------------------------------------------------------------*/

static void DoTestTrace(void)
{
    pmerror     theError = 0xFE12;
    pmuint32    theUInt32 = 0xFEDC1234;
    pmuint16    theUInt16 = 0xFE12;
    int         theInt = (int) 0xFE12FE12;
    
    pm_trace0("This is an int32:");
    pm_trace0("  unsigned [4275835444]=[%lu]", theUInt32);
    pm_trace0("  signed   [-19131852]=[%ld]", theUInt32);
    pm_trace0("  hexa     [fedc1234]=[%lx]", theUInt32);
    
    pm_trace0("This is an int16:");
    pm_trace0("  unsigned [65042]=[%hu]", theUInt16);
    pm_trace0("  signed   [-494]=[%hd]", theUInt16);
    pm_trace0("  hexa     [FE12]=[%hX]", theUInt16);
    
    pm_trace0("This is an int:");
    if (sizeof(int) == sizeof(pmint16))
    {
        pm_trace0("  unsigned [65042]=[%u]", theInt);
        pm_trace0("  signed   [-494]=[%d]", theInt);
        pm_trace0("  hexa     [fe12]=[%x]", theInt);
    }
    else
    {
        pm_trace0("  unsigned [4262657554]=[%u]", theInt);
        pm_trace0("  signed   [-32309742]=[%d]", theInt);
        pm_trace0("  hexa     [fe12fe12]=[%x]", theInt);
    }
    
    pm_trace0("This is a string [Hello world]=[%s]", "Hello world");
    pm_trace0("This is a char [A]=[%c]", 'A');

    pm_trace0("This is a string [Hello]=[%5s]", "Hello world");
    
    pm_trace0("This is a int16  [    0x41]=[%#8hx]", (pmuint16) 0x41);
    pm_trace0("This is an int32 [  0x4141]=[%#8lx]", (pmuint32) 0x4141);
    
    pm_trace0("This is a int16  [0x41    ]=[%#-8hx]", (pmuint16) 0x41);
    pm_trace0("This is an int32 [0x4141  ]=[%#-8lx]", (pmuint32) 0x4141);

    pm_trace0("This is an int32 [+20000]=[%#+2ld]", (pmuint32) 20000);
    
    pm_trace0("These are strange characters [\\t\\b\\n\\r]=[\t\b\n\r]");
    
    pm_trace0("This is a line");
    pm_trace0("break");
    
    pm_trace0("This is a percent sign [%%]=[%%]"); 
}

/*---------------------------------------------------------------------------*/

static void DoTestTraceFloat( void )
{
    double theDouble;

    theDouble = 3.14159265358979;

    pm_trace0( 0, 356.3, 0 );
    pm_trace0( "%1.5f", theDouble, 0 );
    pm_trace0( "", theDouble, 1 );
    pm_trace0( "", -2.1, 0 );
    pm_trace0( "%1.1f", -3.0, 1 );
    pm_trace0( "", -theDouble, 1 );
    pm_trace0( "%020.10f", -theDouble, 1 );

    /* Affichage :
    356.3000003.141593.141593
    -2.100000-3.0
    -3.141593
    -00000003.1415926536
     */

}

/*--------------------------------------------------------------------------*/

static void DoTestStrings(void)
{
    char s_abcde[] = "ABCDE";
    char s_abcd[] = "ABCD";

    /* ----- strcmp ----- */
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : strcmp(\"AAA\", \"AA\") = %d\n", strcmp("AAA", "AA"));
#endif
    c_printf("PMLITE: c_strcmp_imp(\"AAA\", \"AA\") = %d\n", c_strcmp("AAA", "AA"));

#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : strcmp(\"AAA\", \"AAAA\") = %d\n", strcmp("AAA", "AAAA"));
#endif
    c_printf("PMLITE: c_strcmp_imp(\"AAA\", \"AAAA\") = %d\n", c_strcmp("AAA", "AAAA"));

#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : strcmp(\"AAA\", \"AAA\") = %d\n", strcmp("AAA", "AAA"));
#endif
    c_printf("PMLITE: c_strcmp_imp(\"AAA\", \"AAA\") = %d\n", c_strcmp("AAA", "AAA"));

    /* ----- strncmp ----- */
    c_printf("\n");
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : strncmp(\"AAA\", \"AA\", 2) = %d\n", strncmp("AAA", "AA", 2));
#endif
    c_printf("PMLITE: strncmp(\"AAA\", \"AA\", 2) = %d\n", c_strncmp("AAA", "AA", 2));
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : c_strncmp_imp(\"AAA\", \"AA\", 3) = %d\n", strncmp("AAA", "AA", 3));
#endif
    c_printf("PMLITE: strncmp(\"AAA\", \"AA\", 3) = %d\n", c_strncmp("AAA", "AA", 3));
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : c_strncmp_imp(\"AAA\", \"AAA\", 4) = %d\n", strncmp("AAA", "AAA", 4));
#endif
    c_printf("PMLITE: strncmp(\"AAA\", \"AAA\", 4) = %d\n", c_strncmp("AAA", "AAA", 4));
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : strncmp(\"A\", \"B\", 4) = %d\n", strncmp("A", "B", 0));
#endif
    c_printf("PMLITE: c_strncmp_imp(\"A\", \"B\", 4) = %d\n", c_strncmp("A", "B", 0));

    /* ----- strnicmp ----- */
    c_printf("\n");
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : _strnicmp(\"AAA\", \"aA\", 2) = %d\n", _strnicmp("AAA", "aA", 2));
#endif
    c_printf("PMLITE: c_xstrnicmp(\"AAA\", \"AA\", 2) = %d\n", c_xstrnicmp("AAA", "AA", 2));
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : _strnicmp(\"AAA\", \"AA\", 3) = %d\n", _strnicmp("AAA", "AA", 3));
#endif
    c_printf("PMLITE: c_xstrnicmp(\"AAA\", \"AA\", 3) = %d\n", c_xstrnicmp("AAA", "AA", 3));
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : _strnicmp(\"AAA\", \"aaa\", 4) = %d\n", _strnicmp("AAA", "aaa", 4));
#endif
    c_printf("PMLITE: c_xstrnicmp(\"AAA\", \"aaa\", 4) = %d\n", c_xstrnicmp("AAA", "aaa", 4));
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : _strnicmp(\"A\", \"B\", 4) = %d\n", _strnicmp("A", "B", 0));
#endif
    c_printf("PMLITE: c_xstrnicmp(\"A\", \"B\", 4) = %d\n", c_xstrnicmp("A", "B", 0));

    /* ----- strrev ----- */
    c_printf("\n");
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : _strrev(\"ABCDE\") = %s\n", _strrev( s_abcde ));
#endif
    c_printf("PMLITE: c_xstrrev_imp(\"ABCDE\") = %s\n", c_xstrrev( s_abcde ));
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : _strrev(\"ABCD\") = %s\n", _strrev( s_abcd ));
#endif
    c_printf("PMLITE: c_xstrrev_imp(\"ABCD\") = %s\n", c_xstrrev( s_abcd ));
}

/*--------------------------------------------------------------------------*/

#define ITOA_RESULT(x)                                 \
if (x == 0) c_printf("[%s] returned [0].\n", #x);      \
else c_printf("[%s] returned [%s].\n", #x, theBuffer); \

static void DoTestIToA(void)
{
    char    theBuffer[12];

    ITOA_RESULT(c_xuint16toa(1, theBuffer, 12));
    ITOA_RESULT(c_xuint16toa(65535, theBuffer, 12));
    ITOA_RESULT(c_xuint16toa(65535, theBuffer, 3));
    ITOA_RESULT(c_xuint16toa(65535, theBuffer, 5));
    ITOA_RESULT(c_xuint16toa(65535, theBuffer, 6));
    ITOA_RESULT(c_xint16toa(-32768, theBuffer, 6));
    ITOA_RESULT(c_xint16toa(-32768, theBuffer, 7));
    ITOA_RESULT(c_xuint32toa(4294967295, theBuffer, 11));
    ITOA_RESULT(c_xint32toa(-1000000000, theBuffer, 11));
    ITOA_RESULT(c_xint32toa(-1000000000, theBuffer, 12));
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Trace, "Trace - Tests" )
PMMENU_ITEM_EX( 1, "c_trace", DoTrace )
PMMENU_ITEM_EX( 2, "module Error", DoTraceError )
PMMENU_ITEM_EX( 3, "module Trace", DoTraceTrace )
PMMENU_ITEM_EX( 4, "module Debug", DoTraceDebug )
PMMENU_ITEM_SEPARATOR( "-----------------" )
PMMENU_ITEM_EX( 5, "trace a float", DoTestTraceFloat )
PMMENU_ITEM_EX( 6, "PM CODE", DoTestPM_CODE )
PMMENU_ITEM_SEPARATOR( "-----------------" )
PMMENU_ITEM_EX( 7, "Formated traces test", DoTestTrace )
PMMENU_ITEM_EX( 8, "Strings", DoTestStrings )
PMMENU_ITEM_EX( 9, "itoa", DoTestIToA )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER(Trace)
