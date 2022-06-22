/* ===========================================================================
   TMenu.c
   Copyright (C) 1996-98 AbyreSoft. All rights reserved.
   ===========================================================================   */

#include "cMenu.h"
#include "cConsole.h"

/* ---------------------------------------------------------------------------   */

static void DoItem1(void)
{
   c_printf("Menu Item 1.\n");
}

/* ---------------------------------------------------------------------------   */

static void DoItem2(void)
{
   c_printf("Menu Item 2.\n");
}

/* ---------------------------------------------------------------------------   */

static void DoItem3(char* aParam)
{
   c_printf("Menu Item 3 (%s).\n", aParam);
}

/* ---------------------------------------------------------------------------   */

static void DoInit(void)
{
   c_printf("Menu Init.\n");
}

/* ---------------------------------------------------------------------------   */

static void DoClose(void)
{
   c_printf("Menu Close.\n");
}

/* ---------------------------------------------------------------------------   */

PMMENU_BEGIN(Menu, "Menu Test")
   PMMENU_ITEM_EX(1, "Item 1", DoItem1)
   PMMENU_ITEM_SEPARATOR("-----------------")
   PMMENU_ITEM(2, "Item 2")
   PMMENU_ITEM_SEPARATOR("-----------------")
   PMMENU_ITEM(3, "Item 3")
PMMENU_END()

/* ---------------------------------------------------------------------------   */

PMMENU_HANDLER(Menu)
{
   PMMENU_CHOICE_BEGIN()

      PMMENU_CASE_INIT(DoInit);
      PMMENU_CASE_CLOSE(DoClose);

      PMMENU_CASE_CHOICE(2, DoItem2);
      PMMENU_CASE_CHOICE_1(3, DoItem3, "Test");

   PMMENU_CHOICE_END()
}
