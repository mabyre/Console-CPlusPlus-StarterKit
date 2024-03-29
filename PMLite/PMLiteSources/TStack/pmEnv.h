/*--------------------------------------------------------------------------*\
 * Copyright (c) 1999-2000 AbyreSoft - All rights reserved.
 *--------------------------------------------------------------------------*
 * pmXEnv.h - Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * Conditional compilation for Protocol Machinerie (PM) environment
\*--------------------------------------------------------------------------*/

#ifndef _PMXENV_H_
#define _PMXENV_H_

/*--------------------------------------------------------------------------*/

#pragma message("PMXENV - du repertoire <TPMLiteC> utilise")

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   extern "C" {
#endif

/*--------------------------------------------------------------------------*\
 * Byte Ordering                  
\*--------------------------------------------------------------------------*/
/* If nothing is defined big endian (network order) is assumed.
** PM_IN_LITTLE_ENDIAN must be explicitly defined for each kind of 
** architecture.
*/
#define PM_IN_LITTLE_ENDIAN 1

/*--------------------------------------------------------------------------*\
 * Processor capabilities                  
\*--------------------------------------------------------------------------*/
/* Define this macro only if the architecture allows accesses to non aligned
** data in memory.
*/
#define PM_MEMORY_NON_ALIGNED 1

/*--------------------------------------------------------------------------*\
 * Conditional use of traces
\*--------------------------------------------------------------------------*/
/* Default is to have traces in DEBUG builds only and no asserts.
*/
#ifndef NDEBUG
#  define PM_USE_TRACES
#endif 

#ifdef NDEBUG
#  define PM_USE_ASSERTS
#endif 

/*--------------------------------------------------------------------------*\
 * Stack Management                  
\*--------------------------------------------------------------------------*/
/* La macro PM_BEGIN_PROC affiche le nom de la fonction et les stats 
** de la stack apres l'appel de la fonction
*/

#ifndef NDEBUG
#  define PM_USE_STACK_DEBUG
#endif

#ifndef PM_USE_STACK_DEBUG

#  define PM_BEGIN_PROC(x) ((void)0)

#else /* PM_USE_STACK_DEBUG */

#include "cXStack.h"
#include "pmXTrace.h"

#  define PM_BEGIN_PROC(x) c_xstack_update_usage(#x);

#endif /* PM_USE_STACK_DEBUG */

/*--------------------------------------------------------------------------*\
 * Memory debug management                  
\*--------------------------------------------------------------------------*/

#define no_PM_USE_MEMORY_DEBUG
       
/*--------------------------------------------------------------------------*\
 * Console management                  
\*--------------------------------------------------------------------------*/

#define no_PM_USE_CONSOLE

/*--------------------------------------------------------------------------*\
 * Counter part function definitions                  
\*--------------------------------------------------------------------------*/

#include "pmInterface.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif

/*--------------------------------------------------------------------------*/

#endif /* _PMXENV_H_ */
