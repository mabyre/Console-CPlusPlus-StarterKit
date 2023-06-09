/*--------------------------------------------------------------------------*\
 * LIST.h - double linked lists
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 1998-2000 NexWave-solutions. Written by Bruno Raby.
 *
 * This file is part of the sourcecode for the Clementine operating system.
 * You can redistribute and/or modify it under the terms and conditions of the
 * Clementine license, described in doc/COPYING-CLEMENTINE of this source tree
 *----------------------------------------------------------------------------
 * $Id: std.c,v 1.1 2000/11/02 18:11:50 fsalv Exp $
\*--------------------------------------------------------------------------*/

#ifndef _LIST_H_
#define _LIST_H_

/*--------------------------------------------------------------------------*/

#include <nexcore/types.h>
#include <nexcore/lists.h>

/*--------------------------------------------------------------------------*/

typedef struct listNode  list_t ;

typedef struct  
{
  list_t node;                   
  void   *data;
}
nodel_t;


/*--------------------------------------------------------------------------*/

#define LIST_NEXT( pElem )			((nodel_t *)LISTNEXT( (pElem) ))
#define LIST_PREV( pElem )			((nodel_t *)LISTPREV( (pElem) ))
#define LIST_END( pList, pElem )     ENDLIST( ((list_t*)(pList)), ((nodel_t *)(pElem)) )

/*--------------------------------------------------------------------------*/

list_t *ListNew( void *pElem ) ;
void    ListDestroy( list_t *pList ) ;
list_t *ListClone( list_t *pList ) ;

list_t *ListInsertTail( list_t *pList, void *pElem ) ;
list_t *ListInsertHead( list_t *pList, void *pElem ) ;
list_t *ListInsertAfter( list_t *pList, nodel_t *pElem, void *pNewElem ) ;
list_t *ListInsertBefore( list_t *pList, nodel_t *pElem, void *pNewElem ) ;

void    ListRemoveTail( list_t *pList ) ;
void    ListRemoveHead( list_t *pList ) ;
int     ListRemoveElement( list_t *pList, nodel_t *pElem ) ;

/*--------------------------------------------------------------------------*/

#endif /* _LIST_H_ */

/* EOF list.h */
