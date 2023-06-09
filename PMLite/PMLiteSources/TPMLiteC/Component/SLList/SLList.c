/*--------------------------------------------------------------------------*\
 * SLList.c - Implementation of circular singly-linked lists.
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
\*--------------------------------------------------------------------------*/

#include "SLList.h"

#define LINKPTR( aNode, aLinkOffset ) ((void**)((char*)(aNode)+(aLinkOffset)))

/*--------------------------------------------------------------------------*\
 * Public functions
\*--------------------------------------------------------------------------*/
void SLL_InsTail( SLL_List* aList, pmint32 aLinkOffset, void* aNode )
{
   SLL_Node* theTailNode = *aList;

   if ( theTailNode ) {
      *LINKPTR( aNode, aLinkOffset ) = *LINKPTR( theTailNode, aLinkOffset );
      *LINKPTR( theTailNode, aLinkOffset ) = aNode;
   } else {
      *LINKPTR( aNode, aLinkOffset ) = aNode;
   }
   *aList = aNode;
}

/*--------------------------------------------------------------------------*/

SLL_Node* SLL_RemHead( SLL_List* aList, pmint32 aLinkOffset )
{
   SLL_Node* theTailNode = *aList;
   SLL_Node* theHeadNode;

   if ( theTailNode == 0 )
      return 0;

   theHeadNode = *LINKPTR( theTailNode, aLinkOffset );
   if ( theTailNode == theHeadNode ) {
      *aList = 0;
   } else {
      *LINKPTR( theTailNode, aLinkOffset) = *LINKPTR( theHeadNode, aLinkOffset );
   }
   *LINKPTR( theHeadNode, aLinkOffset) = 0;
   return theHeadNode;
}

/*--------------------------------------------------------------------------*/

SLL_Node* SLL_RemElem( SLL_List* aList, pmint32 aLinkOffset, void* aNode )
{
   SLL_Node* theTailNode = *aList;
   SLL_Node* theCurrNode = theTailNode;
   SLL_Node* thePrevNode;

   if ( theTailNode == 0 )
      return 0;

   do {
      thePrevNode = theCurrNode;
      theCurrNode = *LINKPTR( thePrevNode, aLinkOffset );
      if ( theCurrNode == aNode ) {
         *LINKPTR( thePrevNode, aLinkOffset ) = *LINKPTR( theCurrNode, aLinkOffset );
         *LINKPTR( theCurrNode, aLinkOffset ) = 0;
         if ( theCurrNode == theTailNode ) {
            *aList = thePrevNode == theTailNode ? 0 : thePrevNode;
         }
         return theCurrNode;
      }
   } while ( theCurrNode != theTailNode );

   return 0;
}


/*--------------------------------------------------------------------------*\
 * Test driver 
\*--------------------------------------------------------------------------*/

#if 0

typedef struct _MyType { char *s; struct _MyType* next; } MyType;

#define LINK_OFFSET SLL_OFFSET_TO_FIELD( MyType, next )

static void printList( MyType* list )
{
   printf("[ ");
   if ( list ) {
      MyType* node = list->next;
      for (;;) {
         printf( "%s ", node->s );
         if ( node == list )
            break;
         node = node->next;
      }
   }
   printf("]\n");
}

static void printNode( MyType* node ) { printf( "%s ", node ? node->s : "NULL" ); }

main()
{
   MyType nodeA = { "NodeA", 0 };
   MyType nodeB = { "NodeB", 0 };
   MyType nodeC = { "NodeC", 0 };
   MyType *list = 0;
   MyType *node;

   SLL_InsTail( &list, LINK_OFFSET, &nodeA );
   SLL_InsTail( &list, LINK_OFFSET, &nodeB );
   SLL_InsTail( &list, LINK_OFFSET, &nodeC );
   printList( list );

   node = (MyType*) SLL_RemHead( &list, LINK_OFFSET ); printNode( node ); printList( list );
   node = (MyType*) SLL_RemHead( &list, LINK_OFFSET ); printNode( node ); printList( list );
   node = (MyType*) SLL_RemHead( &list, LINK_OFFSET ); printNode( node ); printList( list );
   node = (MyType*) SLL_RemHead( &list, LINK_OFFSET ); printNode( node ); printList( list );

   SLL_InsTail( &list, LINK_OFFSET, &nodeA );
   SLL_InsTail( &list, LINK_OFFSET, &nodeB );
   SLL_InsTail( &list, LINK_OFFSET, &nodeC );
   printList( list );

   node = (MyType*) SLL_RemElem( &list, LINK_OFFSET, &nodeC ); printNode( node ); printList( list );
   node = (MyType*) SLL_RemElem( &list, LINK_OFFSET, &nodeC ); printNode( node ); printList( list );
   node = (MyType*) SLL_RemElem( &list, LINK_OFFSET, &nodeA ); printNode( node ); printList( list );
   node = (MyType*) SLL_RemElem( &list, LINK_OFFSET, &nodeA ); printNode( node ); printList( list );
   node = (MyType*) SLL_RemElem( &list, LINK_OFFSET, &nodeB ); printNode( node ); printList( list );
   node = (MyType*) SLL_RemElem( &list, LINK_OFFSET, &nodeB ); printNode( node ); printList( list );
}

#endif
