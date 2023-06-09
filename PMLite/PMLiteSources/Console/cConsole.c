/*--------------------------------------------------------------------------*\
 * cConsole.c
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
\*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>

#include "pmEnv.h"
#include "pmTypes.h"

/*--------------------------------------------------------------------------*/

#ifdef PM_USE_CONSOLE

/*--------------------------------------------------------------------------*/

#define MAX_INPUT_STRING   1024

static char gInputString[ MAX_INPUT_STRING + 1 ];

/*--------------------------------------------------------------------------*
** flusing stdout, use this functions if it is needed
*/
void c_printf( const char* aFormat, ... )
{
   va_list  theArgs;

   va_start(theArgs, aFormat);
   vprintf(aFormat, theArgs);
   va_end(theArgs);

   fflush(stdout);
}

/*--------------------------------------------------------------------------*/

char* c_ngets( char* aString, size_t aSize )
{
   if ( aSize == 0 || aString == 0 )
      return aString;

   gets( gInputString );
   if ( pm_strlen(gInputString) + 1 > aSize )
   {
      pm_memcpy( aString, gInputString, aSize - 1 );
      aString[aSize - 1] = 0;
   }
   else
      pm_strcpy( aString, gInputString );

   return aString;
}

/*--------------------------------------------------------------------------*/

pmuint32 c_getu32( void )
{
   char     theString[32], *thePtr, *thePtr2;
   double   theValue;

   for (;;) {
      c_ngets( theString, sizeof(theString) );
      for ( thePtr = theString; *thePtr == ' '; thePtr++ ) {
      }
      for ( thePtr2 = thePtr; isdigit(*thePtr2); thePtr2++ ) {
      }
      if ( thePtr < thePtr2 && *thePtr2 == 0 ) {
         theValue = atof( thePtr );
         if ( 0 <= theValue && theValue <= PM_UINT32_MAX ) {
            return (pmuint32) theValue;
         }
      }
      printf("\n");
   }
   return 0;
}

/*--------------------------------------------------------------------------*/

pmuint16 c_getu16( void )
{
   pmuint32 theValue;

   do
   {
      theValue = c_getu32();
   } 
   while ( theValue > PM_UINT16_MAX );

   return (pmuint16)theValue;
}

/*--------------------------------------------------------------------------*/

char c_getc(void)
{
   return (char)getch();
}

/*--------------------------------------------------------------------------*/

char c_keypressed(void)
{
   return (char)(_kbhit() ? getch() : 0);
}

/*--------------------------------------------------------------------------*/

#endif /* PM_USE_CONSOLE */
