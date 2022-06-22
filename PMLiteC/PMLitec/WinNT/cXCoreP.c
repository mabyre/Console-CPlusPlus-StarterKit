/* ===================================================================
   cXCoreP.c
   Copyright (C) 1996-98 AbyreSoft - All rights reserved.
   ===================================================================  */

#include "cXCore.h"
#include "cXTrace.h"

/* ------------------------------------------------------------------- */

pmuint32 c_xrand_imp(pmuint32 aMax)
{
   PMLITE_BEGIN_PROC(c_xrand_imp);

#ifdef PMLITE_TRACE
   if (aMax == 0)
      c_trace(TL(pmT_Core, "c_xrand_imp(): aMax == 0"));
#endif

   return (pmuint32) ( ((float)rand()) / ((float)RAND_MAX) * ((float)aMax) );
}
