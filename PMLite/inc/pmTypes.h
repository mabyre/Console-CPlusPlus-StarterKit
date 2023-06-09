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

typedef pmuint16  pmerror;

#ifndef _SIZE_T_DEFINED
      typedef unsigned int size_t;
#     define _SIZE_T_DEFINED
#endif

/*---------------------------------------------------------------------------*\
 * Maximal and minimal value definitions for pmint16, pmuint16, pmint32 and 
 * pmuint32 types.
\*---------------------------------------------------------------------------*/
   
#define PM_UINT32_MIN   ((pmuint32) 0)
#define PM_UINT32_MAX   ((pmuint32) 4294967295)

#define PM_UINT16_MIN   ((pmuint16) 0)
#define PM_UINT16_MAX   ((pmuint16) 65535)

#define PM_INT32_MIN    (((pmint32) -2147483647) - 1)
#define PM_INT32_MAX    ((pmint32) 2147483647)

#define PM_INT16_MIN    (((pmint16) -32767) - 1)
#define PM_INT16_MAX    ((pmint16) 32767)

/*---------------------------------------------------------------------------*/

/* French boolean definition */
#define VRAI (0==0)
#define FAUX (!VRAI)

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif

/*---------------------------------------------------------------------------*/

#endif /* types */
