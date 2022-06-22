/* ===================================================================
   TDLList.c
   Copyright (C) 1996-98 AbyreSoft - All rights reserved.
   ===================================================================  */

#include "cXMem.h"

#include "cMenu.h"
#include "cXDLList.h"
#include "cInput.h"

#include "pmXDebug.h"

/* ------------------------------------------------------------------- */

typedef struct _TElem
{
   struct _TElem* itsNext;
   struct _TElem* itsPrev;
   int            itsValue;
} 
TElem;

typedef struct _TList
{
   TElem *itsHead, *itsTail;
} 
TList;

#define  TDLLInit(aList)                            DLL_Init((TDLList*)aList);

#define  TDLLInsertTail(aList, anElem)          DLL_InsTail((TDLList*)(aList), (TDLListElem*)(anElem))
#define  TDLLInsertHead(aList, anElem)          DLL_InsHead((TDLList*)(aList), (TDLListElem*)(anElem));
#define  TDLLInsertAfter(aList, anElem, aNewElem) DLL_InsAfter((TDLList*)(aList), (TDLListElem*)(anElem), (TDLListElem*)(aNewElem));

#define  TDLLRemoveHead(aList)                  (TElem*)DLL_RemHead((TDLList*)(aList))
#define  TDLLRemoveTail(aList)                  (TElem*)DLL_RemTail((TDLList*)(aList))
#define  TDLLRemoveElement(aList, anElem)         DLL_RemElem((TDLList*)(aList), (TDLListElem*)(anElem))

/* ------------------------------------------------------------------- */

static TElem* DoAllocateElem(int aValue)
{
   TElem*   theElem = (TElem*)c_malloc(sizeof(TElem));
   theElem->itsNext = theElem->itsPrev = 0;
   theElem->itsValue = aValue;

   return theElem;
}

/* ------------------------------------------------------------------- */
static void DoDumpList(TList* aList)
{
   TElem*   theElem = aList->itsHead;
   pmbool   thefFirst = pmtrue;

   while (theElem)
   {
      if (thefFirst) thefFirst = pmfalse;
      else c_printf(", ");

      c_printf("%d", theElem->itsValue);
      theElem = theElem->itsNext;
   }
   c_printf("\n");
}

/* ------------------------------------------------------------------- */

static void DoTestList(void)
{
   TList theList;
   TElem*   theElem;
   TElem*  theElemCross;

   TDLLInit(&theList);

   c_printf("Filling the list with 1, 2, 3, 4, 5, 6\n");
   PM_DEBUG0(TL("Filling the list with 1, 2, 3, 4, 5, 6"));
   TDLLInsertTail(&theList, DoAllocateElem(1));
   TDLLInsertTail(&theList, DoAllocateElem(2));
   TDLLInsertTail(&theList, DoAllocateElem(3));
   TDLLInsertTail(&theList, DoAllocateElem(4));
   TDLLInsertTail(&theList, DoAllocateElem(5));
   TDLLInsertTail(&theList, DoAllocateElem(6));
   DoDumpList(&theList);

   theElem = TDLLRemoveHead(&theList);
   c_printf("Removing list's head: (%d).\n", theElem->itsValue);
   c_free(theElem);
   DoDumpList(&theList);

   theElem = TDLLRemoveTail(&theList);
   c_printf("Removing list's tail: (%d).\n", theElem->itsValue);
   c_free(theElem);
   DoDumpList(&theList);

   theElem = theList.itsHead->itsNext;
   c_printf("Removing element: (%d).\n", theElem->itsValue);
   TDLLRemoveElement(&theList, theElem);
   c_free(theElem);
   DoDumpList(&theList);

   theElem = DoAllocateElem(10);
   c_printf("Inserting element: (%d) in the head of the list.\n", theElem->itsValue);
   TDLLInsertHead(&theList, theElem) ;
   DoDumpList(&theList);

   theElem = DoAllocateElem(11);
    theElemCross = theList.itsHead;
   /* Crossing the list searching for 4 element */
    while ( (theElemCross->itsNext != 0) && (theElemCross->itsValue != 4) )
     theElemCross = theElemCross->itsNext;
   TDLLInsertAfter( &theList, theElemCross, theElem );
   c_printf("Inserting element: (%d) after the element which value is 4.\n", theElem->itsValue);
   DoDumpList(&theList);

   /* Removing the all list */
   while ( theList.itsHead != 0 )
      c_free(TDLLRemoveHead(&theList));
}

/* ------------------------------------------------------------------- */

PMMENU_BEGIN(DLList, "Double Linked List")
   PMMENU_ITEM_EX(1, "Double Linked List", DoTestList)
PMMENU_END()

PMMENU_DEFAULT_HANDLER(DLList)
