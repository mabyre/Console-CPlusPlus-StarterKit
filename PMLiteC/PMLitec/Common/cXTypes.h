/* ===================================================================
   cXTypes.h
   Copyright (C) 1996-98 AbyreSoft - All rights reserved.
   ===================================================================  */

#ifndef cXTypes_h
#define cXTypes_h

/* -------------------------------------------------------------------
   Additional common declarations.  
   ------------------------------------------------------------------- */

#include "cXEnv.h"

/* ------------------------------------------------------------------- */

#ifdef __cplusplus
   extern "C" {
#endif

/* -------------------------------------------------------------------
   Maximal and minimal value definitions for pmint16, pmuint16, pmint32 and 
   pmuint32 types.
   ------------------------------------------------------------------- */
   
#define kPMUINT32_MIN      ((pmuint32) 0)
#define kPMUINT32_MAX      ((pmuint32) 4294967295)

#define kPMUINT16_MIN      ((pmuint16) 0)
#define kPMUINT16_MAX      ((pmuint16) 65535)

#define kPMINT32_MIN    (((pmint32) -2147483647) - 1)
#define kPMINT32_MAX    ((pmint32) 2147483647)

#define kPMINT16_MIN    (((pmint16) -32767) - 1)
#define kPMINT16_MAX    ((pmint16) 32767)

/* -------------------------------------------------------------------
   Error type definition. An error is stored using a pmint16, (this ensure a 
   smaller code generation for 16 bit processors).
   
   An error is made of two parts:
      - the 8 high bits are the module identifier
      - the 8 low bits are the code
      
   Mostly error can be tested without accessing their internal module identifier 
   and code. Two functions are provided to do this if necessary:
      - pmerror_getmodule
      - pmerror_getcode
   
   To declare an error, use the PMErrorDefine macro. The first parameter is the 
   module identifier, the second one is the code.
   
   Error 0 (defined as pmErr_NoErr) must be used by all modules to specify no error.

   Code 1 to 63 are reserved for PMLite internal use. These codes are generic: they
   must be used as error codes for each module that need to have an equivalent error
   code. 
   ------------------------------------------------------------------- */

typedef pmuint16  pmerror;

   /* Returns the error module identifier */
#define pmerror_getmodule(x)  (((pmerror)(x)) >> 8)

   /* Returns the error code  */
#define pmerror_getcode(x)    (((pmerror)(x)) & 0xff)

   /* Macro used to define errors   */
#define PMErrorDefine(x, y)      ((pmerror)(((x) << 8) + (y)))

/* ------------------------------------------------------------------- */

   /* No error.   */
#define pmErr_NoErr        ((pmerror) 0)     /* No error */

   /* first error code available to application */
#define pmErr_FirstAppErr  (64)

   /* Generic error codes. */
#define pmErr_Unknown      ((pmerror) 1)     /* Unknow error   */
#define pmErr_NoMemory     ((pmerror) 2)     /* Out of memory error  */
#define pmErr_Invalid      ((pmerror) 3)     /* Invalid parameter error */
#define pmErr_UserCancel   ((pmerror) 4)     /* User has cancelled   */
#define pmErr_Timeout      ((pmerror) 5)     /* Time out error */


/* ------------------------------------------------------------------- */

#ifdef __cplusplus
   }
#endif

/* ------------------------------------------------------------------- */

#endif
