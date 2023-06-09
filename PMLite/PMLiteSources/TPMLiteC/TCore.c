/*--------------------------------------------------------------------------*\
 * TCore.c
 * Copyright (C) 1996-98 AbyreSoft. All rights reserved.
\*--------------------------------------------------------------------------*/

#include "pmTypes.h"
#include "pmEnv.h"
#include "cMenu.h"
#include "cInput.h"

#include "cCore.h"
#include "pmXTrace.h"

/*--------------------------------------------------------------------------*/

#define TEST_USE_LIBC
char gString[80] = "ABCDE";

/*--------------------------------------------------------------------------*/

static void DoTestCheckMem(void)
{
   char* theStr = (char*)pm_malloc( 100 );

   if (theStr != 0)
   {
      /* Write after the block end. */
      theStr[100] = 0;
      
      /* Check memory status. */
      pm_xmemdbg_check();

      pm_free(theStr);
   }
   else
      pm_printf("Not enough memory to perform the test\n");
}

/*--------------------------------------------------------------------------*\
 * Comparer l'execution des fonctions de la LIBC et les implementation
 * de la PM
\*--------------------------------------------------------------------------*/
static void DoTestStringsFunctions( void )
{

   /* ----- strcmp ----- */
#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : strcmp(\"AAA\", \"AA\") = %d\n", strcmp("AAA", "AA"));
#endif
   pm_printf("PMLITE: c_strcmp(\"AAA\", \"AA\") = %d\n", c_strcmp("AAA", "AA"));

#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : strcmp(\"AAA\", \"AAAA\") = %d\n", strcmp("AAA", "AAAA"));
#endif
   pm_printf("PMLITE: c_strcmp(\"AAA\", \"AAAA\") = %d\n", c_strcmp("AAA", "AAAA"));

#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : strcmp(\"AAA\", \"AAA\") = %d\n", strcmp("AAA", "AAA"));
#endif
   pm_printf("PMLITE: c_strcmp(\"AAA\", \"AAA\") = %d\n", c_strcmp("AAA", "AAA"));

   /* ----- strncmp ----- */
   pm_printf("\n");
#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : strncmp(\"AAA\", \"AA\", 2) = %d\n", strncmp("AAA", "AA", 2));
#endif
   pm_printf("PMLITE: strncmp(\"AAA\", \"AA\", 2) = %d\n", c_strncmp("AAA", "AA", 2));
#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : c_strncmp(\"AAA\", \"AA\", 3) = %d\n", strncmp("AAA", "AA", 3));
#endif
   pm_printf("PMLITE: strncmp(\"AAA\", \"AA\", 3) = %d\n", c_strncmp("AAA", "AA", 3));
#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : c_strncmp(\"AAA\", \"AAA\", 4) = %d\n", strncmp("AAA", "AAA", 4));
#endif
   pm_printf("PMLITE: strncmp(\"AAA\", \"AAA\", 4) = %d\n", c_strncmp("AAA", "AAA", 4));
#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : strncmp(\"A\", \"B\", 4) = %d\n", strncmp("A", "B", 0));
#endif
   pm_printf("PMLITE: c_strncmp(\"A\", \"B\", 4) = %d\n", c_strncmp("A", "B", 0));

   /* ----- strnicmp ----- */
   pm_printf("\n");
#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : _strnicmp(\"AAA\", \"aA\", 2) = %d\n", strnicmp("AAA", "aA", 2));
#endif
   pm_printf("PMLITE: c_strnicmp(\"AAA\", \"AA\", 2) = %d\n", c_strnicmp("AAA", "AA", 2));
#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : _strnicmp(\"AAA\", \"AA\", 3) = %d\n", strnicmp("AAA", "AA", 3));
#endif
   pm_printf("PMLITE: c_strnicmp(\"AAA\", \"AA\", 3) = %d\n", c_strnicmp("AAA", "AA", 3));
#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : _strnicmp(\"AAA\", \"aaa\", 4) = %d\n", strnicmp("AAA", "aaa", 4));
#endif
   pm_printf("PMLITE: c_strnicmp(\"AAA\", \"aaa\", 4) = %d\n", c_strnicmp("AAA", "aaa", 4));
#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : _strnicmp(\"A\", \"B\", 4) = %d\n", strnicmp("A", "B", 0));
#endif
   pm_printf("PMLITE: c_strnicmp(\"A\", \"B\", 4) = %d\n", c_strnicmp("A", "B", 0));

   /* ----- strrev ----- */
   pm_printf("\n");
#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : _strrev(\"ABCDE\") = %s\n", strrev( gString ));
#endif
   pm_printf("PMLITE: c_strrev(\"ABCDE\") = %s\n", c_strrev( gString ));
#ifdef TEST_USE_LIBC
   pm_printf("LIBC  : _strrev(\"ABCD\") = %s\n", strrev( gString ));
#endif
   pm_printf("PMLITE: c_strrev(\"ABCD\") = %s\n", c_strrev( gString ));
}

/*--------------------------------------------------------------------------*/

#define ITOA_RESULT(x) if (x == 0) pm_printf("[%s] returned [0].\n", #x); else pm_printf("[%s] returned [%s].\n", #x, theBuffer);
static void DoTestIToA(void)
{
   char  theBuffer[12];

   ITOA_RESULT(c_uint16toa(1, theBuffer, 12));
   ITOA_RESULT(c_uint16toa(65535, theBuffer, 12));
   ITOA_RESULT(c_uint16toa(65535, theBuffer, 3));
   ITOA_RESULT(c_uint16toa(65535, theBuffer, 5));
   ITOA_RESULT(c_uint16toa(65535, theBuffer, 6));
   ITOA_RESULT(c_int16toa(-32768, theBuffer, 6));
   ITOA_RESULT(c_int16toa(-32768, theBuffer, 7));
   ITOA_RESULT(c_uint32toa(4294967295, theBuffer, 11));
   ITOA_RESULT(c_int32toa(-1000000000, theBuffer, 11));
   ITOA_RESULT(c_int32toa(-1000000000, theBuffer, 12));
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Core, "Core Test" )
   PMMENU_ITEM_EX( 1, "Strings functions", DoTestStringsFunctions )
   PMMENU_ITEM_EX( 2, "itoa", DoTestIToA )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Core )
