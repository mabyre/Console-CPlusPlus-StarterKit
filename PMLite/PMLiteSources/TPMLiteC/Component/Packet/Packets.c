/* ===================================================================
   Packets.c
   Copyright (C) 1990-2000 AbyreSoft. All rights reserved.
   ===================================================================  */

#include "cXEnv.h"
#include "cXMemDbg.h"
#include "cXCore.h"
#include "cXTrace.h"
#include "cTrace.h"
#include "Packets.h"


TPacket* PacketListRemHead(TPacketList* aList)
{
   TPacket* thePacket = (TPacket*)DLL_RemHead((TDLList*)(aList));
   if(thePacket) aList->itsNumber--;
   return thePacket;
}
 
TPacket* PacketListRemTail(TPacketList* aList)
{
   TPacket* thePacket = (TPacket*)DLL_RemTail((TDLList*)(aList));
   if(thePacket) aList->itsNumber--;
   return thePacket;
}
 

/*  Allocate a packet: a packet header followed by *aDataLen* bytes.
 *  Initialize the reference count in the packet header to 1.
 */
TPacket* PacketAllocate(pmuint32 aPartitionId, size_t aDataLen)
{
   TPacket *thePacket;

   OS_BEGIN_PROC(PacketAllocate);
   c_trace(TL(M_OS,"Packet : Packet Allocate, size requested[%u], total allocated[%d]",aDataLen,sizeof(TPacket) + aDataLen - 1));

   /* Allocate the packet */
   thePacket = (TPacket*)pm_malloc( sizeof(TPacket) + aDataLen - 1 );
   
   if ( !thePacket )
      return NULL;
   
   c_memset( thePacket, 0, sizeof(TPacket) + aDataLen - 1 );
   thePacket->itsDataLen   = aDataLen;
   thePacket->itsData      = thePacket->itsInlineBuffer;
   thePacket->itsBuffer    = thePacket->itsInlineBuffer;
   thePacket->itsBufferLen = aDataLen;
   thePacket->itsRefCount  = 1;
   return thePacket;
}


/*  Decrement the reference count in the packet header.
 *  If the refernce count becomes zero, de-allocate the packet.
 */
void PacketRelease(TPacket* aBuffer)
{
   OS_BEGIN_PROC(PacketRelease);
   c_trace(TL(M_OS,"Packet : Packet release"));

   if ( !aBuffer ) {
      c_trace(TL(M_OS,"Packet: ### ERROR Releasing a null TPacket"));
      return;
   }

   if ( --(aBuffer->itsRefCount) == 0 ) {
         c_free(aBuffer);  /* Memory deallocation in system buffer pool */
   }
}
