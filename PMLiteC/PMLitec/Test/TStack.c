/* ===================================================================
   TStack.c
   Copyright (C) 1996-98 AbyreSoft - All rights reserved.
   ===================================================================  */

#include "cMenu.h"
#include "cXStack.h"
#include "cXTrace.h"
#include "cXCore.h"

/* ------------------------------------------------------------------- */

static void DoTestStack1(pmuint32 i)
{
   pmuint32 theDummy = i;

   c_trace(TL(pmT_Core, "DoTestStack1(%lu).", (unsigned long)i));
   c_xstack_update_usage("DoTestStack1");
   if (i == 0)
      return;
   DoTestStack1(theDummy - 1);
}

/* ------------------------------------------------------------------- */

static void DoTestStack(void)
{
   c_trace(TL(pmT_Core, "DoTestStack()."));
   c_xstack_reset_usage();
   DoTestStack1(5);
}

/* ------------------------------------------------------------------- */

PMMENU_BEGIN(Stack, "Stack Statistics")
   PMMENU_ITEM_EX(1, "Item 1", DoTestStack)
PMMENU_END()

PMMENU_DEFAULT_HANDLER(Stack)
