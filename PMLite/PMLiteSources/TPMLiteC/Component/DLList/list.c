/*--------------------------------------------------------------------------*\
 * LIST.c - double linked lists
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 1998-2000 NexWave-Solutions. Written by Bruno Raby.
 *
 * This file is part of the sourcecode for the Clementine operating system.
 * You can redistribute and/or modify it under the terms and conditions of the
 * Clementine license, described in doc/COPYING-CLEMENTINE of this source tree
 *----------------------------------------------------------------------------
 * $Id: std.c,v 1.1 2000/11/02 18:11:50 fsalv Exp $
\*--------------------------------------------------------------------------*/

#include <nexcore/types.h>
#include <nexcore/lists.h>
#include "clemenv.h"
#include "list.h"

/*--------------------------------------------------------------------------*/

list_t *ListNew( void *pElem )
{
  list_t *plist;
  nodel_t *pnode;

  /* Create an empty list */

  plist = (list_t *)memory->malloc( sizeof(list_t) );
  if ( ! plist ) return NULL;

  NEWLIST( plist );

  if ( ! pElem ) return plist;

  pnode = (nodel_t *)memory->malloc( sizeof(nodel_t) );

  pnode->data = pElem;
  ADDTAIL( plist, pnode );

  return plist;

}

/*--------------------------------------------------------------------------*/

void ListDestroy( list_t *pList ) 
{
   nodel_t *pnode;

   pnode =  LIST_NEXT( pList );
 
   while ( ! ENDLIST(pList, pnode) ) 
   {
      nodel_t *pNext = LIST_NEXT(pnode);
      memory->free(pnode);
      pnode = pNext;
 
   } /* while */    

   memory->free( pList );
}

/*--------------------------------------------------------------------------*/

list_t *ListClone( list_t *iPtrList ) 
{
   list_t  *pnewlist;
   nodel_t *pnode;
   list_t  *pnewnode;

   pnewlist = ListNew( NULL ); 
   if ( ! pnewlist ) return NULL;

   pnode = LIST_NEXT( iPtrList );
 
   while ( ! ENDLIST( iPtrList, pnode ) ) 
   {
      pnewnode = ListInsertTail( pnewlist, pnode->data ); 
      if ( ! pnewnode ) 
	  {
         ListDestroy( pnewlist );
         return NULL;
      }
      pnode = LIST_NEXT( pnode );
 
   } /* while */    

   return pnewlist;

}

/*--------------------------------------------------------------------------*\
 * ListInsertTail - Add a cell to the tail of a doubly linked list.
 *
 * Parameters:
 *   pList: pointer to the list.
 *   pElem: value to be appended.
 *
 * Returns:
 *   a pointer to the head of the list if successfuly allocated, 
 *   NULL otherwise.
\*--------------------------------------------------------------------------*/

list_t *ListInsertTail( list_t *pList, void *pElem ) 
{
  nodel_t *pnode;
  
  if ( ! pList ) return NULL;
  pnode = (nodel_t *) memory->malloc( sizeof(nodel_t) );
  if ( ! pnode ) return NULL;

  pnode->data = pElem;
  ADDTAIL( pList, pnode );

  return pList;

}

/*--------------------------------------------------------------------------*\
 * ListInsertHead - Insert an element to the head of a doubly linked list.
 *
 * Parameters:
 *   pList: pointer to the list.
 *   pElem: value to be inserted.
 *
 * Returns:
 *   a pointer to the head of the list if successfuly allocated, 
 *   NULL otherwise.
\*--------------------------------------------------------------------------*/

list_t *ListInsertHead(list_t *pList, void *pElem) 
{
  nodel_t *pnode;
 
  if ( ! pList ) return NULL;

  pnode = (nodel_t *) memory->malloc(sizeof(nodel_t));
  if ( ! pnode ) return NULL;
 
  pnode->data = pElem;
  ADDHEAD( pList, pnode );
 
  return pList;
 
}

/*--------------------------------------------------------------------------*/

list_t *ListInsertAfter( list_t *pList, nodel_t *pElem, void *pNewElem )
{
  nodel_t *pnode;
  
  if ( ! pList ) return NULL;
  pnode = (nodel_t *) memory->malloc( sizeof(nodel_t) );
  if ( ! pnode ) return NULL;

  pnode->data = pNewElem;
  ADDAFTER( pElem, pnode );

  return pList;
}

/*--------------------------------------------------------------------------*/

list_t *ListInsertBefore( list_t *pList, nodel_t *pElem, void *pNewElem )
{
  nodel_t *pnode;
  
  if ( ! pList ) return NULL;
  pnode = (nodel_t *) memory->malloc( sizeof(nodel_t) );
  if ( ! pnode ) return NULL;

  pnode->data = pNewElem;
  ADDBEFORE( pElem, pnode );

  return pList;
}

/*--------------------------------------------------------------------------*/

void ListRemoveTail( list_t *pList )
{
   nodel_t *pnode;

   pnode =  LIST_PREV( pList );
 
   REMOVE( pnode );
   memory->free( pnode );
}

/*--------------------------------------------------------------------------*/

void ListRemoveHead( list_t *pList )
{
   nodel_t *pnode;

   pnode =  LIST_NEXT( pList );
 
   REMOVE( pnode );
   memory->free( pnode );
}

/*--------------------------------------------------------------------------*/

int ListRemoveElement( list_t *pList, nodel_t *pElem )
{
	if ( ! pList ) return 0;
	
	/* Paranoide can test if pElem is really in pList */
	REMOVE( pElem );
	memory->free( pElem );

	return 1;
}

/* EOF list.c */
