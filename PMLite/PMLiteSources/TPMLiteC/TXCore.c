/* ===================================================================
   TXCore.c
   Copyright (C) 1996-98 AbyreSoft - All rights reserved.
   ===================================================================  */

#include <math.h>
#include "pmEnv.h"
#include "cMenu.h"
#include "cInput.h"
#include "cXCore.h"

/* ------------------------------------------------------------------- */
#define RAND_TEST_RANGE 10
#define RAND_TEST_COUNT 1000000

static void DoTestRand(void)
{
   static pmuint32   sArray[RAND_TEST_RANGE];
   size_t         i;
   pmuint32    theU32;

   memset(sArray, 0, sizeof(sArray));

   for (i = 0; i < RAND_TEST_COUNT; i++)
      sArray[ pm_xrand(RAND_TEST_RANGE) ]++;

   pm_printf("Mean is: %lu\n", RAND_TEST_COUNT / RAND_TEST_RANGE);
   if (RAND_TEST_RANGE <= 20)
   {
      for (i = 0; i < RAND_TEST_RANGE; i++)
         pm_printf("%02lu: %lu\n", (pmuint32)i, sArray[i]);
   }
   theU32 = 0;
   for (i = 0; i < RAND_TEST_RANGE; i++)
      if (sArray[i] < RAND_TEST_COUNT / RAND_TEST_RANGE)
         theU32 += (RAND_TEST_COUNT / RAND_TEST_RANGE - sArray[i]) * (RAND_TEST_COUNT / RAND_TEST_RANGE - sArray[i]);
      else
         theU32 += (sArray[i] - RAND_TEST_COUNT / RAND_TEST_RANGE) * (sArray[i] - RAND_TEST_COUNT / RAND_TEST_RANGE);
   
   theU32 = (pmuint32)sqrt(theU32 / RAND_TEST_RANGE);
   pm_printf("Standard deviation: %lu\n", theU32);
}


/* ------------------------------------------------------------------- */

PMMENU_BEGIN(XCore, "XCore Test")
   PMMENU_ITEM_EX(1, "Rand", DoTestRand)
   PMMENU_ITEM_SEPARATOR("-----------------")
PMMENU_END()

/* ------------------------------------------------------------------- */

PMMENU_DEFAULT_HANDLER(XCore)
