/*---------------------------------------------------------------------------*\
 * Copyright (C) 1996-98 AbyreSoft. All rights reserved.
 * cMenuDis.c - Menu Display
\*---------------------------------------------------------------------------*/

#include "pmEnv.h"
#include "cMenuDis.h"
#include "cInput.h"

/*---------------------------------------------------------------------------*/

#define kMenu_MaxIndentLevel  6
#define kMenu_PromptStr       "  Your choice"
#define kMenu_PromptStrLen    13
#define kMenu_PromptMaxLen    ((2 * kMenu_MaxIndentLevel) + kMenu_PromptStrLen + 1)

/*---------------------------------------------------------------------------*\
 * Called with aMenu != 0 and aLine == 0 for "0 - Exit"
\*---------------------------------------------------------------------------*/
   
void PMMenu_DisplayLine(PMMenu* aMenu, PMMenuLine* aLine, pmint32 aLevel)
{
   char  theIndent[kMenu_PromptMaxLen];

   if (aLevel > kMenu_MaxIndentLevel)
      aLevel = kMenu_MaxIndentLevel;
      
   pm_memset(theIndent, ' ', (size_t) (2 * aLevel));
   theIndent[2 * aLevel] = 0;

   if (aLine == 0)
      pm_printf("\n%s%2ld - %s\n", theIndent, (pmuint32) 0, "Exit");
   else
   {
      if (aLine->itsCode == kPMMenuSeparator)
         pm_printf("%s   - %s\n", theIndent, aLine->itsText);
      else
         pm_printf("%s%2ld - %s\n", theIndent, (pmuint32) aLine->itsCode, aLine->itsText);
   }
}

/*---------------------------------------------------------------------------*/

pmuint16 PMMenu_GetChoice(PMMenu* aMenu, pmint32 aLevel)
{
   pmuint16 theValue;
   char     thePrompt[kMenu_PromptMaxLen];
   
   if (aLevel > kMenu_MaxIndentLevel)
      aLevel = kMenu_MaxIndentLevel;
      
   pm_memset(thePrompt, ' ', (size_t) (2 * aLevel));
   pm_memcpy(&thePrompt[2 * aLevel], kMenu_PromptStr, kMenu_PromptStrLen);
   thePrompt[(2 * aLevel) + kMenu_PromptStrLen] = 0;

   pm_printf("\n");
   (void)Input_UInt16( thePrompt, &theValue, pmfalse, 0, pmfalse );
   pm_printf("\n");

   return theValue;
}

/*---------------------------------------------------------------------------*/

void PMMenu_BadChoice(PMMenu* aMenu, pmint32 aLevel)
{
   pm_printf("*** Unknown command ***\n");
}
