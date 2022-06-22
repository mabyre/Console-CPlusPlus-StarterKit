/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * TTrace.c - Differentes facons de faire des traces
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "cInput.h"

#include "cCore.h"
#include "cTrace.h"

#include "pmXError.h"
#include "pmXTrace.h"
#include "pmXDebug.h"
#include "pmTrace.h"

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
    PM_DEBUG0(TL("DEBUG 0 - Error tres grave"));
    PM_DEBUG1(TL("DEBUG 1 - Error grave"));
    PM_DEBUG2(TL("DEBUG 2 - Error faible"));
    PM_DEBUG3(TL("DEBUG 3 - Error tres faible"));
}

/*--------------------------------------------------------------------------*/

static void DoSetMaxMem(void)
{
    pmuint32 theValue;

    if (Input_UInt32("Enter max memory", &theValue, pmfalse, 0, pmtrue))
        c_xmemdbg_set_max(theValue);
}

/*--------------------------------------------------------------------------*/

static void DoAllocBlock(void)
{
    pmuint32 theValue;

    if ( Input_UInt32("Enter block size", &theValue, pmfalse, 0, pmtrue) )
    {
        if (c_malloc(theValue) == 0)
            c_printf("Allocation failed.\n");
    }
}

/*--------------------------------------------------------------------------*/

static void DoTestCheckMem(void)
{
    char* theStr = (char*) c_malloc(100);

    if (theStr != 0)
    {
            /*  Write after the block end.  */
        theStr[100] = 0;
        
            /*  Check memory status.    */
        c_xmemdbg_check();

        c_free(theStr);
    }
    else
        c_printf("Not enough memory to perform the test\n");
}

/*--------------------------------------------------------------------------*/

static void DoTestTrace(void)
{
    pmerror     theError = 0xFE12;
    pmuint32    theUInt32 = 0xFEDC1234;
    pmuint16    theUInt16 = 0xFE12;
    int         theInt = (int) 0xFE12FE12;
    pmbyte      theBuffer[256];
    size_t      i;
    
    // !!! some not supported
    c_trace(TL(pmM_Test, "This is an int32:"));
    c_trace(TL(pmM_Test, "  unsigned [4275835444]=[%lu]", theUInt32));
    c_trace(TL(pmM_Test, "  signed   [-19131852]=[%ld]", theUInt32));
    c_trace(TL(pmM_Test, "  hexa     [fedc1234]=[%lx]", theUInt32));
    
    c_trace(TL(pmM_Test, "This is an int16:"));
    c_trace(TL(pmM_Test, "  unsigned [65042]=[%hu]", theUInt16));
    c_trace(TL(pmM_Test, "  signed   [-494]=[%hd]", theUInt16));
    c_trace(TL(pmM_Test, "  hexa     [FE12]=[%hX]", theUInt16));
    
    c_trace(TL(pmM_Test, "This is an int:"));
    if (sizeof(int) == sizeof(pmint16))
    {
        c_trace(TL(pmM_Test, "  unsigned [65042]=[%u]", theInt));
        c_trace(TL(pmM_Test, "  signed   [-494]=[%d]", theInt));
        c_trace(TL(pmM_Test, "  hexa     [fe12]=[%x]", theInt));
    }
    else
    {
        c_trace(TL(pmM_Test, "  unsigned [4262657554]=[%u]", theInt));
        c_trace(TL(pmM_Test, "  signed   [-32309742]=[%d]", theInt));
        c_trace(TL(pmM_Test, "  hexa     [fe12fe12]=[%x]", theInt));
    }
    
    c_trace(TL(pmM_Test, "This is a string [Hello world]=[%s]", "Hello world"));
    c_trace(TL(pmM_Test, "This is a char [A]=[%c]", 'A'));

    c_trace(TL(pmM_Test, "This is a string [Hello]=[%5s]", "Hello world"));
    
    c_trace(TL(pmM_Test, "This is a int16  [    0x41]=[%#8hx]", (pmuint16) 0x41));
    c_trace(TL(pmM_Test, "This is an int32 [  0x4141]=[%#8lx]", (pmuint32) 0x4141));
    
    c_trace(TL(pmM_Test, "This is a int16  [0x41    ]=[%#-8hx]", (pmuint16) 0x41));
    c_trace(TL(pmM_Test, "This is an int32 [0x4141  ]=[%#-8lx]", (pmuint32) 0x4141));

    c_trace(TL(pmM_Test, "This is an int32 [+20000]=[%#+2ld]", (pmuint32) 20000));
    
    c_trace(TL(pmM_Test, "These are strange characters [\\t\\b\\n\\r]=[\t\b\n\r]"));
    
    c_trace(TL(pmM_Test, "This is a line"));
    c_trace(TL(pmM_Test, "break"));
    
    c_trace(TL(pmM_Test, "This is a percent sign [%%]=[%%]"));
    
    for (i = 0 ; i < 256 ; i++)
        theBuffer[i] = (pmbyte) i;
    c_trace(TL(pmM_Test, "This is a buffer:"));
    c_trace(B(pmM_Test, theBuffer, (size_t) 256));
}

/*--------------------------------------------------------------------------*/

static void DoTestStrings(void)
{
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
    c_printf("LIBC  : _strnicmp(\"AAA\", \"aA\", 2) = %d\n", strnicmp("AAA", "aA", 2));
#endif
    c_printf("PMLITE: c_xstrnicmp(\"AAA\", \"AA\", 2) = %d\n", c_xstrnicmp("AAA", "AA", 2));
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : _strnicmp(\"AAA\", \"AA\", 3) = %d\n", strnicmp("AAA", "AA", 3));
#endif
    c_printf("PMLITE: c_xstrnicmp(\"AAA\", \"AA\", 3) = %d\n", c_xstrnicmp("AAA", "AA", 3));
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : _strnicmp(\"AAA\", \"aaa\", 4) = %d\n", strnicmp("AAA", "aaa", 4));
#endif
    c_printf("PMLITE: c_xstrnicmp(\"AAA\", \"aaa\", 4) = %d\n", c_xstrnicmp("AAA", "aaa", 4));
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : _strnicmp(\"A\", \"B\", 4) = %d\n", strnicmp("A", "B", 0));
#endif
    c_printf("PMLITE: c_xstrnicmp(\"A\", \"B\", 4) = %d\n", c_xstrnicmp("A", "B", 0));

    /* ----- strrev ----- */
    c_printf("\n");
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : _strrev(\"ABCDE\") = %s\n", strrev("ABCDE"));
#endif
    c_printf("PMLITE: c_xstrrev_imp(\"ABCDE\") = %s\n", c_xstrrev("ABCDE"));
#ifdef PMLITE_USE_LIBC_FOR_TESTS
    c_printf("LIBC  : _strrev(\"ABCD\") = %s\n", strrev("ABCD"));
#endif
    c_printf("PMLITE: c_xstrrev_imp(\"ABCD\") = %s\n", c_xstrrev("ABCD"));
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

PMMENU_BEGIN(Trace, "Trace - Tests")
    PMMENU_ITEM_EX(1, "Test du module Error", DoTraceError )
    PMMENU_ITEM_EX(2, "Test du module Trace", DoTraceTrace )
    PMMENU_ITEM_EX(3, "Test du module Debug", DoTraceDebug )
    PMMENU_ITEM_SEPARATOR("-----------------")
    PMMENU_ITEM_EX(4, "Set max mem", DoSetMaxMem)
    PMMENU_ITEM_EX(5, "Allocate block", DoAllocBlock)
    PMMENU_ITEM_EX(6, "Test CheckMem", DoTestCheckMem)
    PMMENU_ITEM_SEPARATOR("-----------------")
    PMMENU_ITEM_EX(7, "Traces test", DoTestTrace)
    PMMENU_ITEM_EX(8, "Strings", DoTestStrings)
    PMMENU_ITEM_EX(9, "itoa", DoTestIToA)
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER(Trace)
