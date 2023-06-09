/* ===================================================================
   Packets.h
   Copyright (C) 1990-2000 AbyreSoft. All rights reserved.
   ===================================================================  */

#ifndef Packets_h
#define Packets_h

#include "cXTypes.h"
#include "cXDLList.h"

#ifdef __cplusplus
   extern "C" {
#endif

/*---------+
 |  Types  |
 +---------*/

/* A packet */
typedef struct _TPacket
{
   struct _TPacket *itsNext, *itsPrev; /* Forward, backward links in list */
   pmbyte          *itsData;           /* Current valid data in itsBuffer */
   size_t           itsDataLen;        /* Current valid data len in itsBuffer */
   pmbyte          *itsMarker;         /* Marker used for processing */
   pmbyte          *itsBuffer;         /* Total buffer; usually &itsInlineBuffer[0] */
   size_t           itsBufferLen;      /* Total buffer len */
   pmuint32         itsRefCount;       /* Reference count */
   pmbyte           itsInlineBuffer[1];
} TPacket;

/* A list of packets */
typedef struct _TPacketList
{
   TPacket *itsHead, *itsTail;
   pmuint16 itsNumber;     /* Total number of elements in PacketList */
} 
TPacketList;

/*-----------------------+
 |  Packet List methods  |
 +-----------------------*/

#define PacketListInit(aList)      \
   do {                            \
      DLL_Init((TDLList*)(aList)); \
      (aList)->itsNumber = 0;      \
      } while(0)                   

TPacket* PacketListRemHead(TPacketList* aList);

#define     PacketListInsTail(aList, anElem)                   \
   do {                                                        \
      DLL_InsTail((TDLList*)(aList), (TDLListElem*)(anElem));  \
      (aList)->itsNumber++;                                    \
      } while(0)                                               

TPacket* PacketListRemTail(TPacketList* aList);

#define     PacketListRemElem(aList,anElem)                       \
      do {                                                        \
         DLL_RemElem((TDLList*)(aList), (TDLListElem*)(anElem));  \
         (aList)->itsNumber--;                                    \
         } while(0)

#define     PacketListFlush(aList)                 \
   do {                                            \
      TPacket* theP;                               \
      while( (theP = PacketListRemHead(aList)) )   \
         PacketRelease(theP);                      \
      } while(0)

/*------------------+
 |  Packet Methods  |
 +------------------*/

/*  Allocate a packet: a packet header followed by *aDataLen* bytes.
 *  Initialize the reference count in the packet header to 1.
 */
TPacket* PacketAllocate(pmuint32 aPartitionId, size_t aDataLen);

/*  Decrement the reference count in the packet header.
 *  If the refernce count becomes zero, de-allocate the packet.
 */
void PacketRelease(TPacket* aBuffer);

/*  Increment the reference count in the packet header.
 */
#define PacketIncRef(aPacket) ((aPacket)->itsRefCount++)


#ifdef __cplusplus
   }
#endif

#endif /* Packets_h */
