/*--------------------------------------------------------------------------*\
 * TList.c - test for double linked list
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

#include "cXMemDbg.h"

#include "stdlib.h"		/* itoa() */
#include "cMenu.h"
#include "cXDLList.h"
#include "cInput.h"

#include "ASDebug.h"
//#include "clemenv.h"
#include <nexcore/types.h>
#include <nexcore/lists.h>
#include "list.h"

/*--------------------------------------------------------------------------*\
  Elements managed by the double linked list
\*--------------------------------------------------------------------------*/

typedef int elem_t;
elem_t a = 1 ;
elem_t b = 2 ;
elem_t c = 3 ;
elem_t d = 4 ;
elem_t e = 5 ;
elem_t f = 6 ;
elem_t g = 7 ;
elem_t h = 8 ;

typedef struct 
{
	uint size ;
	void *data ;
}
Element_t ;

Element_t aa = { 2 , "aa" };
Element_t bb = { 3 , "bbb" };
Element_t cc = { 4 , "cccc" };


/*--------------------------------------------------------------------------*/

list_t *globalList ;

/*--------------------------------------------------------------------------*/

static void DoDumpList( list_t *iPtrList ) ;

/*--------------------------------------------------------------------------*/

static void DoConstruct( void )
{
	DEBUG3("DoConstruct the globalList");

	// Insert all elements in the tail of the list
	globalList = ListNew( &a );
	globalList = ListInsertTail( globalList , &b ) ;
	globalList = ListInsertTail( globalList , &c ) ;
	globalList = ListInsertTail( globalList , &d ) ;
	globalList = ListInsertTail( globalList , &e ) ;
	globalList = ListInsertTail( globalList , &f ) ;
	globalList = ListInsertTail( globalList , &g ) ;
	globalList = ListInsertTail( globalList , &h ) ;
	DoDumpList( globalList );
}

/*--------------------------------------------------------------------------*/

nodel_t * DoFindElement( list_t *pList, elem_t iElem )
{
	nodel_t *theElem ; 

	theElem = LIST_NEXT( pList ) ;

	while ( ! LIST_END( pList, theElem ) )
	{
		DEBUG3("DoFindElement : searching for : %d", *((elem_t *)(theElem->data)) ) ;
		if ( *((elem_t *)(theElem->data)) == iElem ) return theElem ;
		
		if ( LISTLAST( pList ,theElem ) ) return NULL ;

        theElem = LIST_NEXT( theElem ) ;
	}
	return theElem ;
}

/*--------------------------------------------------------------------------*/

static void DoTestFindElement( void )
{
	elem_t theValue ;
	nodel_t *theElem ; 

	DEBUG3("=============================> DoTestFindElement");
	DoConstruct();
	
	theValue = 3;
	theElem = DoFindElement( globalList, theValue );

	/* Retirer de la liste l'element trouve */
	if ( theElem != NULL )
		ListRemoveElement( globalList, theElem );

	DoDumpList( globalList ) ;

	/* Trouver un element qui n'existe pas */
	theValue = 495 ;
	theElem = DoFindElement( globalList, theValue );
	if ( theElem != NULL )
		DEBUG3("DoTestFindElement : Error");
	else
		DEBUG3("DoTestFindElement : Element %d not in List",theValue);


	DEBUG3("------------------------------ Destroy GlobalList");
	ListDestroy( globalList ) ;
}

/*--------------------------------------------------------------------------*/

static void DoDumpList( list_t * iPtrList )
{
	nodel_t *theElem ; 

	char   *c = pm_malloc( sizeof( char ) + 1 ) ;
	elem_t *p ;
	
	DEBUG3("DoDumpList");

	theElem = LIST_NEXT( iPtrList ) ;
	while ( ! LIST_END( iPtrList , theElem ) )
	{
		p = (int *)(theElem->data) ;
		itoa( *p,  c , 10);
		c_printf("%s", c );

		DEBUG3("%s", c );

        theElem = LIST_NEXT( theElem ) ;
		c_printf(" ");
	}
	c_printf("\n");

	pm_free( c ) ;
}

/*--------------------------------------------------------------------------*/

static void DoTestRemove(void)
{

	DEBUG3("=============================> DoTestRemove");
	DoConstruct();
	DEBUG3("------------------------------ ListRemoveTail");
	ListRemoveTail( globalList ) ;
	DoDumpList( globalList );

	DEBUG3("------------------------------ ListRemoveHead");
	ListRemoveHead( globalList ) ;
	DoDumpList( globalList );

	DEBUG3("------------------------------ Destroy GlobalList");
	ListDestroy( globalList ) ;
}

/*--------------------------------------------------------------------------*/

static void DoTestInsert(void)
{	
	elem_t theValue ;
	nodel_t *theElem ; 

	DEBUG3("=============================> DoTestInsert");
	DoConstruct();

	theValue = 3;
	theElem = DoFindElement( globalList, theValue );

	if ( theElem != NULL )
		DEBUG3("Element value : %d",*((elem_t *)(theElem->data)));
	else
		DEBUG3("DoTestInsert : Error : Element %d not founded", theValue );

	DEBUG3("Inserting element <a> after the founded one");
	ListInsertAfter( globalList, theElem , &a ) ;
	DoDumpList( globalList ) ;

	DEBUG3("Inserting element <a> before the founded one");
	ListInsertBefore( globalList, theElem , &a ) ;
	DoDumpList( globalList ) ;

	/* Essayer a la fin de la liste */
	theValue = 8; /* max liste */
	theElem = DoFindElement( globalList, theValue );

	DEBUG3("Inserting element <a> after the founded one");
	ListInsertAfter( globalList, theElem , &a ) ;
	DoDumpList( globalList ) ;

	DEBUG3("Inserting element <a> before the founded one");
	ListInsertBefore( globalList, theElem , &a ) ;
	DoDumpList( globalList ) ;

	DEBUG3("------------------------------ Destroy GlobalList");
	ListDestroy( globalList ) ;
}

/*--------------------------------------------------------------------------*/

static void DoTestClone(void)
{
	list_t *theListClone ;

	DEBUG3("=============================> DoTestClone");
	DoConstruct() ;

	// Insert an element in the head
	globalList = ListInsertHead( globalList , &b ) ;
	DoDumpList( globalList );

	// Clonage de la liste
	DEBUG3("---------------------- Clonage");
	theListClone = ListClone( globalList ) ;
	DoDumpList( theListClone );
	
	DEBUG3("---------------------- Destroy All");
	ListDestroy( theListClone ) ;
	ListDestroy( globalList ) ;
}

/*--------------------------------------------------------------------------*/

void DoTestNewKindOfElement( void )
{
	list_t		*list ;
	nodel_t		*node ;
	Element_t	*element ;

	list = ListNew( &aa );
	list = ListInsertTail( list , &bb ) ;
	list = ListInsertTail( list , &cc ) ;

	DEBUG3("DoTestNewNode : DoDumpList");

	node = LIST_NEXT( list ) ;
	while ( ! LIST_END( list , node ) )
	{
		element = (Element_t *)(node->data) ;
		
		c_printf("size : %d ", element->size );
		c_printf("data : %s\n", element->data );

		DEBUG3("size : [%2d] data [%s]", element->size, element->data );

        node = LIST_NEXT( node ) ;
	}
	c_printf("\n");

	DEBUG3("DoTestNewNode : Destroying list");
	ListDestroy( list );
}

/*--------------------------------------------------------------------------*/

static void DoTestAll(void)
{
	DoTestFindElement() ;
	DoTestRemove() ;
	DoTestInsert() ;
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN(List, "DLL Clementine style")
	PMMENU_ITEM_EX(1, "Test new kind of node", DoTestNewKindOfElement)
	PMMENU_ITEM_EX(2, "Test find element functions", DoTestFindElement)
	PMMENU_ITEM_EX(3, "Test remove functions", DoTestRemove)
	PMMENU_ITEM_EX(4, "Test insert functions", DoTestInsert)
	PMMENU_ITEM_EX(5, "Test cloning function", DoTestClone)
	PMMENU_ITEM_EX(6, "Double Linked List Test All", DoTestAll)
PMMENU_END()

PMMENU_DEFAULT_HANDLER(List)
