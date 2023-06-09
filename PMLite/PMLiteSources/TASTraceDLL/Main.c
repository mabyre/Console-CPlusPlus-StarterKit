/*==========================================================================*\
 * Main.c
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
\*==========================================================================*/

#include "pmEnv.h"
#include <windows.h>
#include "ASTrace.h"

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

   as_trace_init( GetModuleHandle(0) );

   //NT_TestMain();

    /* Depending on settings of the Reporter Application
    **
        public const uint ERROR0 = 0x0100;
        public const uint ERROR1 = 0x0101;
        public const uint ERROR2 = 0x0102;
        public const uint ERROR3 = 0x0103;

        public const uint TRACE0 = 0x1000;
        public const uint TRACE1 = 0x1001;
        public const uint TRACE2 = 0x1002;
        public const uint TRACE3 = 0x1003;

        public const uint DEBUG0 = 0x2000;
        public const uint DEBUG1 = 0x2001;
        public const uint DEBUG2 = 0x2002;
        public const uint DEBUG3 = 0x2003;
    **
    */
    as_trace_STR( 0x1000, "Hello Trace Reporter module : TRACE0" ); 
    as_trace_STR( 0x1001, "Hello Trace Reporter module : TRACE1" ); 


#if defined( PM_USE_TRACES ) || defined( PMLITE_TRACE ) 
    as_trace_close();
#endif 

   return 0;
}
