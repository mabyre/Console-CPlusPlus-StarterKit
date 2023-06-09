/* ===================================================================
   TimerH.h
   Copyright (C) 1996-99 Smartcode Technologie - All rights reserved.
   ===================================================================  */
/*
 *  Internal header file for timer objects, WinNT version.
 */

#ifndef TimerH_h
#define TimerH_h

#include "cXEnv.h"
#include "cXDLList.h"

#include "OSWinInc.h"

#ifdef __cplusplus
   extern "C" {
#endif


/*  The private Timer structure.
 */
struct _Timer
{
   TDLListElem    itsLinks;
   void           (*itsFunc)(void*);
   void*          itsArg;
   pmuint32       itsExpTime; /* tick at which expiration occurs */
};


#ifdef __cplusplus
   }
#endif

#endif /* TimerH_h */

