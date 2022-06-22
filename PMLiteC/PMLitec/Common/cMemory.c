/* ===================================================================
   cMemory.c
   Copyright (C) 1996-98 AbyreSoft - All rights reserved.
   =================================================================== */
   
#include "cMemory.h"
#include "cCore.h"

/* ------------------------------------------------------------------- */

void *Memory_CAlloc(size_t aSize)
{
   char *thePtr;
   PMLITE_BEGIN_PROC(Memory_CAlloc);
   
   thePtr = c_malloc(aSize);
   if (thePtr != 0)
      c_memset(thePtr, 0, aSize);
      
   return thePtr;
}

/* ------------------------------------------------------------------- */

void Memory_Free(void **aPtr)
{
   PMLITE_BEGIN_PROC(Memory_Free);

   if (*aPtr != 0)
   {
      c_free(*aPtr);
      (*aPtr) = 0;
   }
}
