/*--------------------------------------------------------------------------*\
 * SLList.h - Public header for circular singly-linked lists.
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
 *--------------------------------------------------------------------------*
 *
 *      The physically 1st node on a list is the logical TAIL node.
 *      The physically 2nd node on a list is the logical HEAD node.
 *      Thus inserting at the tail and removing from the head are fast.
 *
 *      Suppose you have a single struct type and you want to put objects
 *      of that type onto multiple lists by using multiple link fields within
 *      that struct. Example:
 *
 *          typedef struct _MyType {
 *              ...
 *              struct _MyType* brownLink;  // link to next node in brown list
 *              struct _MyType* greenLink;  // link to next node in green list
 *          } MyType;
 *          
 *          MyType* brownList;  // the brown list
 *          MyType* greenList;  // the green list
 *
 *          MyType  aNode;      // a node to put into the lists
 *          MyType* aNodePtr;   // a node pointer
 *
 *      Then this module is just what you need. Here's how you might use it.      
 *      First use the convenient SLL_OFFSET_TO_FIELD macro to define offsets
 *      to your link fields:
 *
 *          #define BROWN_LINK_OFFSET  SLL_OFFSET_TO_FIELD( MyType, brownLink )
 *          #define GREEN_LINK_OFFSET  SLL_OFFSET_TO_FIELD( MyType, greenLink )
 *
 *      Initialize your lists to be empty:
 *
 *          brownList = 0;
 *          greenList = 0;
 *
 *      Here's how to insert a node onto the tail of the two lists:
 *
 *          SLL_InsTail( &brownList, &aNode, BROWN_LINK_OFFSET );
 *          SLL_InsTail( &greenList, &aNode, GREEN_LINK_OFFSET );
 *
 *      Here's how to remove nodes from the head of the lists:
 *
 *          aNodePtr = (MyType*) SLL_RemHead( &brownList, BROWN_LINK_OFFSET );
 *          aNodePtr = (MyType*) SLL_RemHead( &greenList, GREEN_LINK_OFFSET );
 *
 *      And here's how visit the nodes of a list starting with the logical HEAD
 *      (note that this uses nothing at all from this api):
 *
 *          if ( brownList ) {
 *             aNodePtr = brownList;
 *             do {
 *                aNodePtr = aNodePtr->brownLink;
 *                Visit( aNodePtr );
 *             } while ( aNodePtr != brownList );
 *          }
 *
\*--------------------------------------------------------------------------*/

#ifndef SLList_h
#define SLList_h

#include "cXEnv.h"

#ifdef __cplusplus
   extern "C" {
#endif

/*--------------------------------------------------------------------------*\
 * Type definitions for lists and nodes.
\*--------------------------------------------------------------------------*/

typedef void* SLL_List;  /* a list is just a pointer to the logical tail node */
typedef void  SLL_Node;  /* a node is any structure with a link field */

/*--------------------------------------------------------------------------*\
 * A macro for obtaining the offset to a field in a struct.
\*--------------------------------------------------------------------------*/

#define SLL_OFFSET_TO_FIELD(StructTy,fld) ((int)&((StructTy*)0)->fld)


/*--------------------------------------------------------------------------*\
 * Public methods.
\*--------------------------------------------------------------------------*/

void      SLL_InsTail( SLL_List* aList, pmint32 aLinkOffset, SLL_Node* aNode );
SLL_Node* SLL_RemHead( SLL_List* aList, pmint32 aLinkOffset );
SLL_Node* SLL_RemElem( SLL_List* aList, pmint32 aLinkOffset, SLL_Node* aNode );

#ifdef __cplusplus
   }
#endif

#endif /* SLList_h */
