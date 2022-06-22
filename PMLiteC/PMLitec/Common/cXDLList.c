/* ===================================================================
   cXDLLList.h
   Copyright (C) 1996-98 AbyreSoft - All rights reserved.
   ===================================================================  */

#include "cXDLList.h"

/* -------------------------------------------------------------------
   Double linked lists management.
   ------------------------------------------------------------------- */
void DLL_Init(TDLList* aList)
{
   PMLITE_BEGIN_PROC(DLL_Init);

   aList->itsHead = aList->itsTail = 0;
}

/* ------------------------------------------------------------------- */
void DLL_InsTail(TDLList* aList, TDLListElem* anElem)
{
   PMLITE_BEGIN_PROC(DLL_InsTail);

   /* if it's the first element of the list */
   if (aList->itsTail == 0)
   {
      aList->itsHead = aList->itsTail = anElem;
      anElem->itsPrev = anElem->itsNext = 0;
   }
   else
   {
      aList->itsTail->itsNext = anElem;
      anElem->itsNext = 0;
      anElem->itsPrev = aList->itsTail;
      aList->itsTail = anElem;
   }
}

/* ------------------------------------------------------------------- */
void DLL_InsHead(TDLList* aList, TDLListElem* anElem)
{
   PMLITE_BEGIN_PROC(DLL_InsHead);

   if (aList->itsHead == 0)
   {
      aList->itsHead = aList->itsTail = anElem;
      anElem->itsPrev = anElem->itsNext = 0;
   }
   else
   {
      aList->itsHead->itsPrev = anElem;
      anElem->itsNext = aList->itsHead;
      anElem->itsPrev = 0;
      aList->itsHead = anElem;
   }
}

/* ------------------------------------------------------------------- */
void DLL_InsAfter(TDLList* aList, TDLListElem* aListElem, TDLListElem* aNewElem)
{
   PMLITE_BEGIN_PROC(DLL_InsAfter);

   aNewElem->itsNext = aListElem->itsNext;
   aListElem->itsNext = aNewElem;
   aNewElem->itsPrev = aListElem;
   if (aNewElem->itsNext != 0)
      aNewElem->itsNext->itsPrev = aNewElem;

   if (aList->itsTail == aListElem)
      aList->itsTail = aNewElem;
}

/* ------------------------------------------------------------------- */
TDLListElem* DLL_RemTail(TDLList* aList)
{
   TDLListElem* theElem;

   PMLITE_BEGIN_PROC(DLL_RemTail);

   /* Empty list */
   if (aList->itsTail == 0)
      return 0;
   
   theElem = aList->itsTail;

   if (theElem->itsPrev == 0)
      /* One element */
      aList->itsTail = aList->itsHead = 0;
   else
   {
      theElem->itsPrev->itsNext = 0;
      aList->itsTail = theElem->itsPrev;
   }

   theElem->itsNext = theElem->itsPrev = 0;
   return theElem;
}

/* ------------------------------------------------------------------- */
TDLListElem* DLL_RemHead(TDLList* aList)
{
   TDLListElem* theElem;

   PMLITE_BEGIN_PROC(DLL_RemHead);

   if (aList->itsHead == 0)
      return 0;

   theElem = aList->itsHead;
   if (theElem->itsNext == 0)
      aList->itsTail = aList->itsHead = 0;
   else
   {
      theElem->itsNext->itsPrev = 0;
      aList->itsHead = theElem->itsNext;
   }

   theElem->itsNext = theElem->itsPrev = 0;
   return theElem;
}

/* ------------------------------------------------------------------- */
void DLL_RemElem(TDLList* aList, TDLListElem* anElem)
{
   PMLITE_BEGIN_PROC(DLL_RemElem);

   if (aList->itsHead == 0)
      return;

   if (anElem->itsPrev == 0)
      aList->itsHead = anElem->itsNext;
   else
      anElem->itsPrev->itsNext = anElem->itsNext;

   if (anElem->itsNext == 0)
      aList->itsTail = anElem->itsPrev;
   else
      anElem->itsNext->itsPrev = anElem->itsPrev;
}
