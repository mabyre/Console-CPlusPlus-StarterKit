/*==========================================================================*\
 * Main.c
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
\*==========================================================================*/

#include "OSWinInc.h"
#include "ASTrace.h"

#include "pmXEnv.h"
#include "cXTypes.h"

/*--------------------------------------------------------------------------*/

void NT_TestMain(void);

#define APP_TITLE "Console Test application"

/*--------------------------------------------------------------------------*/

int main()
{
/*--------------------------------------------------------------------------*\
 * Console management. Change the console buffer, size and position.
\*--------------------------------------------------------------------------*/
#ifndef NDEBUG
   static const char szTitle[] = APP_TITLE " - Debug";
#else
   static const char szTitle[] = APP_TITLE " - Release";
#endif
   COORD       theCoord = { 80, 1000 };
   SMALL_RECT  theRect  = { 0, 0, 150, 24 };
   HWND        theHWnd  = 0;
   int         i = 0;

   SetConsoleTitle(szTitle);
   SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), theCoord);
   theCoord = GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));
   while (1)
   {
      theHWnd = FindWindow("ConsoleWindowClass", szTitle);
      if (theHWnd == 0)
      {
         i++;
         if (i == 20)
            break;
         Sleep(100);
      }
      else
         break;
   }
   if (theHWnd != 0)
   {
      SetWindowPos(theHWnd, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
      if (theRect.Bottom >= 2)
      {
         theRect.Bottom = theCoord.Y - 2;
         SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &theRect);
      }
   }

/*--------------------------------------------------------------------------*/

#if defined( PM_USE_TRACES ) || defined( PMLITE_TRACE ) 
   as_trace_init(GetModuleHandle(0));
#endif 

   NT_TestMain();

#if defined( PM_USE_TRACES ) || defined( PMLITE_TRACE ) 
   as_trace_close();
#endif 

   return 0;
}
