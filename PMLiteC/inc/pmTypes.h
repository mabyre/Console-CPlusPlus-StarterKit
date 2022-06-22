/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2000 AbyreSoft - All rights reserved.
 *--------------------------------------------------------------------------*
 * type.h - Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * Definition de type des objets de base
\*--------------------------------------------------------------------------*/

#ifndef _TYPES_H_
#define _TYPES_H_

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   extern "C" {
#endif

/*--------------------------------------------------------------------------*\
 *                   Type definitions                                   
\*--------------------------------------------------------------------------*/

typedef unsigned char   pmbyte;
typedef unsigned short  pmuint16;
typedef unsigned long   pmuint32;
typedef short           pmint16;
typedef long            pmint32;
typedef int             pmbool;

#define pmfalse    0
#define pmtrue     1

#ifndef _SIZE_T_DEFINED
      typedef unsigned int size_t;
#     define _SIZE_T_DEFINED
#endif

/* ------------------------------------------------------------------- */

#ifdef __cplusplus
   }
#endif

/* ------------------------------------------------------------------- */

#endif /* types */
