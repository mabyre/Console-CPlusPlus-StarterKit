/*--------------------------------------------------------------------------*\
 * cXBorder.c
 * Copyright (C) 1996-98 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#include "cXBorder.h"
#include "pmEnv.h"

/*--------------------------------------------------------------------------*/

pmuint16 c_swap_u16( pmuint16 anU16 )
{
   PM_BEGIN_PROC(c_swap_u16);

   return (pmuint16)( (anU16 << 8) + (anU16 >> 8) );
}

/*--------------------------------------------------------------------------*/

pmuint32 c_swap_u32( pmuint32 anU32 )
{
   PM_BEGIN_PROC(c_swap_u32);

   return   ((anU32 & 0xFF) << 24) + 
         ((anU32 & 0xFF00) << 8) +
         ((anU32 & (pmuint32) 0xFF0000) >> 8) + 
         ((anU32 & (pmuint32) 0xFF000000) >> 24);
}

/*--------------------------------------------------------------------------*/

pmuint16 c_read_u16( pmbyte* aPtr )
{
#if !defined(PM_IN_LITTLE_ENDIAN) && defined(PM_MEMORY_NON_ALIGNED)

   return *(pmuint16*)aPtr;

#else /* PM_IN_LITTLE_ENDIAN || !PM_MEMORY_NON_ALIGNED */

   return (pmuint16)(( ((pmuint16)aPtr[0]) << 8 ) + aPtr[1]);

#endif /* PM_IN_LITTLE_ENDIAN || !PM_MEMORY_NON_ALIGNED */
}

/*--------------------------------------------------------------------------*/

pmuint32 c_read_u32( pmbyte* aPtr )
{
#if !defined(PM_IN_LITTLE_ENDIAN) && defined(PM_MEMORY_NON_ALIGNED)

   return *(pmuint32*)aPtr;

#else /* PM_IN_LITTLE_ENDIAN || !PM_MEMORY_NON_ALIGNED */

   return (pmuint32)(( ((pmuint32)aPtr[0]) << 24 ) + ( ((pmuint32)aPtr[1]) << 16 ) + ( ((pmuint32)aPtr[2]) << 8 ) + aPtr[3]);

#endif /* PM_IN_LITTLE_ENDIAN || !PM_MEMORY_NON_ALIGNED */
}

/*--------------------------------------------------------------------------*/

pmuint16 c_read_le_u16( pmbyte* aPtr )
{
#if defined(PM_IN_LITTLE_ENDIAN) && defined(PM_MEMORY_NON_ALIGNED)

   return *(pmuint16*)aPtr;

#else /* PM_IN_LITTLE_ENDIAN || !PM_MEMORY_NON_ALIGNED */

   return (pmuint16)(( ((pmuint16)aPtr[1]) << 8 ) + aPtr[0]);

#endif /* PM_IN_LITTLE_ENDIAN || !PM_MEMORY_NON_ALIGNED */
}

/*--------------------------------------------------------------------------*/

pmuint32 c_read_le_u32( pmbyte* aPtr )
{
#if defined(PM_IN_LITTLE_ENDIAN) && defined(PM_MEMORY_NON_ALIGNED)

   return *(pmuint32*)aPtr;

#else /* PM_IN_LITTLE_ENDIAN || !PM_MEMORY_NON_ALIGNED */

   return (pmuint32)(( ((pmuint32)aPtr[3]) << 24 ) + ( ((pmuint32)aPtr[2]) << 16 ) + ( ((pmuint32)aPtr[1]) << 8 ) + aPtr[0]);

#endif /* PM_IN_LITTLE_ENDIAN || !PM_MEMORY_NON_ALIGNED */
}
