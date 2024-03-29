/*--------------------------------------------------------------------------*\
 * cXStack.c
 * Copyright (C) 1996-98 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#include "pmEnv.h"

#ifdef PM_USE_STACK_DEBUG

#pragma message("PM_USE_STACK_DEBUG defined in <cXStack.c>")

/*--------------------------------------------------------------------------*/

#include "OSWinInc.h"
#include "pmXTrace.h"

/*--------------------------------------------------------------------------*/

static pmbool  sfInited, sfStackUpDown;

/*--------------------------------------------------------------------------*/

static void Stack_P_GetDir(size_t *aPtr)
{
   size_t thePtr1, thePtr2;
   if (aPtr == 0)
   {
      thePtr1 = (size_t)&thePtr1;
      Stack_P_GetDir(&thePtr2);
      sfStackUpDown = (thePtr2 < thePtr1);
      sfInited = pmtrue;
      
      PM_TRACE0(TL("Stack_P_GetDir(): Stack is [%s]", sfStackUpDown ? "Up-Down" : "Down-Up"));
   }
   else
      *aPtr = (size_t) &thePtr1;
}

/*--------------------------------------------------------------------------*/

void c_xstack_reset_usage(void)
{
   TStackInfo* theSI = c_xstack_get_info();

   if (theSI == 0)
      return;
      
   theSI->itsBase = (size_t) &theSI;
   theSI->itsMax = 0;
   theSI->itsfReset = pmtrue;
   theSI->itsProcName = 0;
}

/*--------------------------------------------------------------------------*/

void c_xstack_update_usage( char* aProcName )
{
   TStackInfo* theSI;
   size_t      theSize;
   
   if (!sfInited)
      Stack_P_GetDir(0);

   theSI = c_xstack_get_info();
   if ((theSI == 0) || !theSI->itsfReset)
      return;
      
   if (sfStackUpDown)
      theSize = (size_t)(theSI->itsBase - (size_t)&theSI);
   else
      theSize = (size_t)((size_t)&theSI - theSI->itsBase);

   /* This case happens when the stack is going under base. */
   if (theSize > ((((size_t)-1) / 10) * 9))
      return;

   if (theSize > theSI->itsMax)
   {
      theSI->itsMax = theSize;
      theSI->itsProcName = aProcName;
      c_xstack_dump_usage();
   }
}

/*--------------------------------------------------------------------------*/

TStackInfo* c_xstack_get_info(void)
{
   static DWORD   theTLS = 0xFFFFFFFF;
   TStackInfo*    theSI;

   if (theTLS == 0xFFFFFFFF)
   {
      theTLS = TlsAlloc();
      if (theTLS == 0xFFFFFFFF)
         return 0;
   }

   theSI = (TStackInfo*)TlsGetValue(theTLS);
   if (theSI == 0)
   {
      theSI = (TStackInfo*)malloc(sizeof(TStackInfo));
      theSI->itsfReset = pmfalse;
      if (!TlsSetValue(theTLS, theSI))
         return 0;
   }

   return theSI;
}

/*--------------------------------------------------------------------------*/

void c_xstack_dump_usage(void)
{
   TStackInfo* theSI = c_xstack_get_info();
   
   if ((theSI == 0) || !theSI->itsfReset)
      return;
   
   PM_TRACE0(TL("c_stack_dump_usage(): Stack maximum depth reached: (%lu) in [%s]", (pmuint32)theSI->itsMax, theSI->itsProcName==0?"Unknown":theSI->itsProcName));
}

/*--------------------------------------------------------------------------*/

#endif /* PM_USE_STACK_DEBUG */
