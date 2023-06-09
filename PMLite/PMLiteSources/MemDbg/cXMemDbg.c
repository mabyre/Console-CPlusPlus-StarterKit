/*--------------------------------------------------------------------------*\
 * cXMemDbg.c
 * Copyright (c) 1998-2000 AbyreSoft. All rights reserved.
\*--------------------------------------------------------------------------*/

#include "cXCore.h"
#include "cXStack.h"
#include "pmXError.h"

/*--------------------------------------------------------------------------*/

#ifdef PM_USE_MEMORY_DEBUG

/*--------------------------------------------------------------------------*/

#define MEM_SAFETY_GAP_SIZE      4
#define MEM_SAFETY_GAP_CONTENT   0xFD
#define MEM_FREED_CONTENT        0xDD
#define MEM_NEW_CONTENT          0xCD

/*--------------------------------------------------------------------------*/

struct _PM_MBH;
typedef struct _PM_MBH PM_MBH;

struct _PM_MBH
{
   PM_MBH*     itsNext;
   PM_MBH*     itsPrev;
   const char* itsFileName;
   int         itsLine;
   size_t      itsSize;
   pmuint32    itsID;
   pmbyte      itsGap1[ MEM_SAFETY_GAP_SIZE ];
};

/*--------------------------------------------------------------------------*/

#define PM_MBH_H2B(h) ( (pmbyte*) (((PM_MBH*) h) + 1) )
#define PM_MBH_B2H(b) ( ((PM_MBH*) b) - 1 )

/*--------------------------------------------------------------------------*/

static pmuint32   sCurID = 1;
static pmuint32   sTotalAlloc;
static pmuint32   sCurAlloc;
static pmuint32   sPeakAlloc;

/* If this one is not 0 then it is the maximum memory that can be allocated */
static pmuint32   sMaxAlloc;

static PM_MBH  *sMBHHead;
static PM_MBH  *sMBHTail;

/*--------------------------------------------------------------------------*/

static pmbool CheckBytes( pmbyte* aBlock, pmbyte aChar, size_t aSize )
{
   pmbool thefOK = pmtrue;
   
   while ( aSize-- && thefOK )
   {
      if ( *aBlock++ != aChar )
      {
         PM_ERROR0(TL("Memory block corrupted at address: [%lx]", aBlock - 1));
         thefOK = pmfalse;
      }
   }
   
   c_assert( thefOK, "Memory corruption");
   
   return thefOK;
}

/*--------------------------------------------------------------------------*/

void *c_malloc_dbg( size_t aSize, const char* aFileName, int aLine )
{
   pmuint32 theID = sCurID;
   size_t      theSize;
   PM_MBH      *theMBH;
   pmbyte      *theBlock;

   PM_BEGIN_PROC(c_malloc_dbg);

   if ( (sMaxAlloc != 0) && ((sCurAlloc + aSize) > sMaxAlloc) )
   {
      PM_ERROR0(TL("c_malloc_dbg(): Out of memory when allocating [%lu] bytes in file [%s], line [%d]", aSize, aFileName, aLine));
      return 0;
   }

   theSize = sizeof(PM_MBH) + aSize + MEM_SAFETY_GAP_SIZE;
   theMBH = (PM_MBH*) c_malloc_proc(theSize);
   if (theMBH == 0)
      return 0;

      /* Allocation is done.  */
   sCurID++;
   sTotalAlloc += aSize;
   sCurAlloc += aSize;
   if (sCurAlloc > sPeakAlloc)
      sPeakAlloc = sCurAlloc;

   /* Link the new block into the list */
   if (sMBHHead == 0)
   {
         /* Empty list. */
      sMBHHead = sMBHTail = theMBH;
      theMBH->itsPrev = 0;
   }
   else
   {
      theMBH->itsPrev = sMBHTail;
      sMBHTail->itsNext = theMBH;
      sMBHTail = theMBH;
   }

   /* Initialize all other fields */
   theMBH->itsNext = 0;
   theMBH->itsFileName = aFileName;
   theMBH->itsLine = aLine;
   theMBH->itsSize = aSize;
   theMBH->itsID = theID;

   /* Fill in all gaps...  */
   pm_memset( theMBH->itsGap1, MEM_SAFETY_GAP_CONTENT, MEM_SAFETY_GAP_SIZE );
   theBlock = PM_MBH_H2B( theMBH );
   pm_memset( theBlock, MEM_NEW_CONTENT, aSize );
   pm_memset( theBlock + theMBH->itsSize, MEM_SAFETY_GAP_CONTENT, MEM_SAFETY_GAP_SIZE );

   return theBlock;
}

/*--------------------------------------------------------------------------*/

void c_free_dbg( void* aBlock )
{
   PM_MBH   *theMBH;
 
   PM_BEGIN_PROC( c_free_dbg );

   if (aBlock == 0)
      return;

   theMBH = PM_MBH_B2H(aBlock);

   CheckBytes(theMBH->itsGap1, MEM_SAFETY_GAP_CONTENT, MEM_SAFETY_GAP_SIZE);
   CheckBytes(((unsigned char*)aBlock) + theMBH->itsSize, MEM_SAFETY_GAP_CONTENT, MEM_SAFETY_GAP_SIZE);

   sCurAlloc -= theMBH->itsSize;

   /* Unlink it from the list */
   if (theMBH->itsPrev == 0)
      sMBHHead = theMBH->itsNext;
   else
      theMBH->itsPrev->itsNext = theMBH->itsNext;

   if (theMBH->itsNext == 0)
      sMBHTail = theMBH->itsPrev;
   else
      theMBH->itsNext->itsPrev = theMBH->itsPrev;

   pm_memset(aBlock, MEM_FREED_CONTENT, theMBH->itsSize);
   c_free_proc( theMBH );
}

/*--------------------------------------------------------------------------*/

void *c_realloc_dbg( void* aBlock, size_t aSize, const char* aFileName, int aLine )
{
   PM_MBH   *theMBH, *theMBH2;
   size_t   theSize, theOldSize;
   pmbyte   *theBlock;

   PM_BEGIN_PROC( c_realloc_dbg );

   if (aBlock == 0)
      return c_malloc_dbg(aSize, aFileName, aLine);
      
   if (aSize == 0)
   {
      c_assert(aBlock != 0, "c_realloc_dbg(): aBlock and aSize == 0");
      c_free_dbg(aBlock);
      return 0;
   }

   theMBH = PM_MBH_B2H(aBlock);
   CheckBytes(theMBH->itsGap1, MEM_SAFETY_GAP_CONTENT, MEM_SAFETY_GAP_SIZE);
   CheckBytes(((unsigned char*)aBlock) + theMBH->itsSize, MEM_SAFETY_GAP_CONTENT, MEM_SAFETY_GAP_SIZE);

   if ( sMaxAlloc != 0 && sCurAlloc + aSize - theMBH->itsSize > sMaxAlloc )
   {
      PM_TRACE0(TL("c_malloc_dbg(): Out of memory when reallocating [%lu] bytes in file [%s], line [%d]", aSize, aFileName, aLine));
      return 0;
   }

   theOldSize = theMBH->itsSize;
   theSize = sizeof(PM_MBH) + aSize + MEM_SAFETY_GAP_SIZE;
   theMBH2 = (PM_MBH*)c_realloc_proc(theMBH, theSize);
   if (theMBH2 == 0)
   {
      void* theNewBlock = c_malloc_dbg( aSize, theMBH->itsFileName, theMBH->itsLine );
      if (theNewBlock == 0)
         return 0;
      pm_memcpy(theNewBlock, aBlock, theMBH->itsSize);
      c_free_dbg(aBlock);
      return theNewBlock;
   }

   if (aSize > theOldSize)
      sTotalAlloc += (aSize - theOldSize);
   sCurAlloc += (aSize - theOldSize);
   if (sCurAlloc > sPeakAlloc)
      sPeakAlloc = sCurAlloc;

   /* Change the links in the list */
   if (theMBH2->itsPrev == 0)
      sMBHHead = theMBH2;
   else
      theMBH2->itsPrev->itsNext = theMBH2;

   if (theMBH2->itsNext == 0)
      sMBHTail = theMBH2;
   else
      theMBH2->itsNext->itsPrev = theMBH2;

   /* Initialize all other fields */
   theMBH2->itsSize = aSize;

   /* Fill in all gaps...  */
   theBlock = PM_MBH_H2B(theMBH2);
   if (aSize > theOldSize)
      pm_memset(theBlock + theOldSize, MEM_NEW_CONTENT, aSize - theOldSize);
   pm_memset(theBlock + theMBH2->itsSize, MEM_SAFETY_GAP_CONTENT, MEM_SAFETY_GAP_SIZE);

   return theBlock;
}

/*--------------------------------------------------------------------------*/

void c_xmemdbg_dump_state()
{
   PM_MBH   *theMBH = sMBHHead;
   pmuint32 theCount = 0;
   
   PM_TRACE0(TL("--------------"));
   PM_TRACE0(TL("Memory report:"));
   PM_TRACE0(TL("Blocks allocated: %lu", (sCurID - 1)));
   PM_TRACE0(TL("Peak allocated: %lu", sPeakAlloc));
   PM_TRACE0(TL("Total allocated: %lu", sTotalAlloc));
   PM_TRACE0(TL("--------------"));
   
   if ( sMBHHead != 0 )
      PM_TRACE0(TL( "*** Memory leaks ***" ));
   else
      PM_TRACE0(TL( "No memory leak. Well done." ));
   
   while ( theMBH != 0 )
   {
      theCount++;
      PM_TRACE0(TL("ID %lu, Size %lu, Allocated in %s, Line %d", theMBH->itsID, theMBH->itsSize, theMBH->itsFileName, theMBH->itsLine));
      theMBH = theMBH->itsNext;
   }

   if ( theCount != 0 )
   {
      PM_TRACE0(TL("--------------"));
      PM_TRACE0(TL("Blocks not deallocated: %lu", theCount ));
      PM_TRACE0(TL("Total not deallocated: %lu", sCurAlloc ));
   }
   
   PM_TRACE0(TL("--------------"));
}

/*--------------------------------------------------------------------------*/

void c_xmemdbg_set_max( size_t aSize )
{
   sMaxAlloc = aSize;
}

/*--------------------------------------------------------------------------*/

void c_xmemdbg_check( void )
{
   PM_MBH   *theMBH = sMBHHead;
   pmbyte   *theBlock;

   while ( theMBH != 0 )
   {
      theBlock = PM_MBH_H2B( theMBH );
      
      CheckBytes( theMBH->itsGap1, MEM_SAFETY_GAP_CONTENT, MEM_SAFETY_GAP_SIZE );
      CheckBytes( theBlock + theMBH->itsSize, MEM_SAFETY_GAP_CONTENT, MEM_SAFETY_GAP_SIZE );
      
      theMBH = theMBH->itsNext;
   }
}

/*--------------------------------------------------------------------------*/

#endif /* PM_USE_MEMORY_DEBUG */
