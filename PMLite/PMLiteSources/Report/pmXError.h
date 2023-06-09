/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------*
 * pmXError.h - Written by Bruno Raby.
 * Error trace functions.
\*--------------------------------------------------------------------------*/

#ifndef _PM_XERROR_H_
#define _PM_XERROR_H_

/*--------------------------------------------------------------------------*/

#include "pmEnv.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   extern "C" {
#endif

/*--------------------------------------------------------------------------*\
   
   Supported flags are:
      
      -     Display left justified (default is right justified)
      +     Always displays the sign symbol (default is to diplay only '-' symbol)
      space Displays a space (when the value is positive) instead of a '+' symbol
      #     Alternate form specifier
      
   Width should be a positive number

   Here is the list of supported format:
      
      %     Display the % character
      s     Display a 0 terminated string

      c     Display a character

      ld    Display a pmint32
      lu    Display a pmuint32      
      lX lx Display a pmint32 (or pmuint32) in hexadecimal

      hd    Display a pmint16
      hu    Display a pmuint16      
      hX hx Display a pmint16 (or pmuint16) in hexadecimal

      d     Display an int
      i     Display an int
      u     Display an unsigned int    
      X  x  Display an int (or unsigned int) in hexadecimal
   
   Not supported formats are:
   
      o     Display an unsigned int in octal
      
      f     Display a double
      e     Display a double
      E     Display a double
      F     Display a double
      g     Display a double
      G     Display a double
      
      p     Display a pointer to void
      
      L
      n

\*--------------------------------------------------------------------------*/

#ifdef PM_USE_TRACES

#pragma message("PM_USE_TRACES - Defined in ERROR")

#  include "cStdArg.h"

   void ERROR0_T( char* aFormat, ... );
   void ERROR0_TL( char* aFormat, ... );
   void ERROR0_B( char* aFormat, ... );

   void ERROR1_T( char* aFormat, ... );
   void ERROR1_TL( char* aFormat, ... );
   void ERROR1_B( char* aFormat, ... );

   void ERROR2_T( char* aFormat, ... );
   void ERROR2_TL( char* aFormat, ... );
   void ERROR2_B( char* aFormat, ... );

   void ERROR3_T( char* aFormat, ... );
   void ERROR3_TL( char* aFormat, ... );
   void ERROR3_B( char* aFormat, ... );

#  define PM_ERROR0(X) ERROR0_ ## X
#  define PM_ERROR1(X) ERROR1_ ## X
#  define PM_ERROR2(X) ERROR2_ ## X
#  define PM_ERROR3(X) ERROR3_ ## X

#else

#pragma message("PM_USE_TRACES - NOT Defined in ERROR")

/*--------------------------------------------------------------------------*\
 * Avoid traces functions if PM_USE_TRACES is NOT Defined
\*--------------------------------------------------------------------------*/

#  define PM_ERROR0(X)    ((void) 0)
#  define PM_ERROR1(X)    ((void) 0)
#  define PM_ERROR2(X)    ((void) 0)
#  define PM_ERROR3(X)    ((void) 0)

#endif /* PM_USE_TRACES */

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif

/*--------------------------------------------------------------------------*/

#endif
