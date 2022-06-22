/* ===================================================================
   cTypes.h
   Copyright (C) 1990-2001 AbyreSoft - All rights reserved.
   =================================================================== */

#ifndef cTypes_h
#define cTypes_h

/* ------------------------------------------------------------------- */

#include "cEnv.h"
#include "cXTypes.h"

/* ------------------------------------------------------------------- */

#ifdef __cplusplus
   extern "C" {
#endif

/* ------------------------------------------------------------------- */

/* French boolean definition */
#define BOOL unsigned char
#define VRAI (0==0)
#define FAUX (!VRAI)

/* ------------------------------------------------------------------- */

#ifdef __cplusplus
   }
#endif

/* ------------------------------------------------------------------- */

#endif
