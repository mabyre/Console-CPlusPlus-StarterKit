/*--------------------------------------------------------------------------*\
 * TStack.c
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "cXStack.h"
#include "pmXTrace.h"
#include "cXCore.h"

/*----------------------------------------------------------------------------*/

static void DoTestStack1(pmuint32 i)
{
   pmuint32 theDummy = i;

   PM_TRACE0(TL("DoTestStack1(%lu).", (unsigned long)i));
   pm_xstack_update_usage("DoTestStack1");
   if (i == 0)
      return;
   DoTestStack1(theDummy - 1);
}

/*----------------------------------------------------------------------------*/

static void DoTestStack(void)
{
   PM_TRACE0(TL("DoTestStack()."));
   pm_xstack_reset_usage();
   DoTestStack1(5);
}

/*----------------------------------------------------------------------------*/

PMMENU_BEGIN(Stack, "Stack Statistics")
   PMMENU_ITEM_EX(1, "Item 1", DoTestStack)
PMMENU_END()

PMMENU_DEFAULT_HANDLER(Stack)
