/*--------------------------------------------------------------------------*\
 * cXDLList.h
 * Copyright (C) 1996-98 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#ifndef cXDLList_h
#define cXDLList_h

/*--------------------------------------------------------------------------*/

#include "pmEnv.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------
   Generic double linked list management.
   Elements stored in this list must be structs beginning by:
   X* itsNext, X* itsPrev.
   ------------------------------------------------------------------- */

/* Fake structure for implementation. */
typedef struct _TDLListElem
{
   struct _TDLListElem  *itsNext, *itsPrev;
}
TDLListElem;

typedef struct _TDLList
{
   TDLListElem *itsHead, *itsTail;
}
TDLList;

void        DLL_Init(TDLList* aList);

void        DLL_InsTail(TDLList* aList, TDLListElem* anElem);
void        DLL_InsHead(TDLList* aList, TDLListElem* anElem);
void        DLL_InsAfter(TDLList* aList, TDLListElem* aListElem, TDLListElem* aNewElem);

TDLListElem*   DLL_RemTail(TDLList* aList);
TDLListElem*   DLL_RemHead(TDLList* aList);
void           DLL_RemElem(TDLList* aList, TDLListElem* anElem);

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

/*--------------------------------------------------------------------------*/

#endif
