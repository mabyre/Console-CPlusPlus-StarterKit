/*--------------------------------------------------------------------------*\
 * TBool.c
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
\*--------------------------------------------------------------------------*/

#include <conio.h>

#include "cMenu.h"
#include "cCore.h"
#include "cConsole.h"
#include "cInput.h"
#include "proper.h"

/*--------------------------------------------------------------------------*/

int type = 0;

/*--------------------------------------------------------------------------*/

void DoTestProper( void )
{
   type = P_PROPERTIE1_TYPE1 | P_PROPERTIE1_TYPE2;

   if ( P_IS_PROPERTIE1_TYPE1( type ) )
   {
      c_printf("OK - P_IS_PROPERTIE1_TYPE1\n");
   }
   else
   {
      c_printf("KO - P_IS_PROPERTIE1_TYPE1\n");
   }

   if ( P_IS_PROPERTIE1_TYPE2( type ) )
   {
      c_printf("OK - P_IS_PROPERTIE1_TYPE2\n");
   }
   else
   {
      c_printf("KO - P_IS_PROPERTIE1_TYPE2\n");
   }

   if ( P_COMPONENT_HAS_PROPERTIE2( type ) )
   {
      c_printf("KO - P_COMPONENT_HAS_PROPERTIE2\n");
   }
   else
   {
      c_printf("OK - P_COMPONENT_HAS_ NO PROPERTIE2\n");
   }

}

/*--------------------------------------------------------------------------*/

void DoNothing( void )
{
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN(Proper, "Test Proper")
   PMMENU_ITEM_EX(1, "Test Proper" , DoTestProper)
   PMMENU_ITEM_EX(2, "Do Nothing"  , DoNothing)
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER(Proper)
