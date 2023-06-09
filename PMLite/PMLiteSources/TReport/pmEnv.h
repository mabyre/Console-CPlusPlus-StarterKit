/*--------------------------------------------------------------------------*\
 * Copyright (c) 1999-2000 AbyreSoft - All rights reserved.
 *--------------------------------------------------------------------------*
 * pmEnv.h - Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * Conditional compilation for Protocol Machinerie (PM) environment
\*--------------------------------------------------------------------------*/

#ifndef _PMXENV_H_
#define _PMXENV_H_

/*--------------------------------------------------------------------------*/

#pragma message("PMXENV - du repertoire <TReport> utilise")

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
#  pragma message("PM_USE_TRACES - Defined")
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
#  define no_PM_USE_STACK_DEBUG
#endif

/*--------------------------------------------------------------------------*/

#ifdef PM_USE_STACK_DEBUG

#include "cXStack.h"
#include "pmXTrace.h"

#  define PM_BEGIN_PROC(x) c_xstack_update_usage(#x);

#else /* PM_USE_STACK_DEBUG */

#  define PM_BEGIN_PROC(x) ((void)0)

#endif /* PM_USE_STACK_DEBUG */

/*--------------------------------------------------------------------------*\
 * Thread Management           
\*--------------------------------------------------------------------------*/
//#include "cXThread.h"

//#define c_xthread_init  c_xthread_init_imp
//#define c_xthread_term  c_xthread_term_imp

/*--------------------------------------------------------------------------*\
 * Memory debug management                  
\*--------------------------------------------------------------------------*/
#ifndef NDEBUG
#  define no_PM_USE_MEMORY_DEBUG
#endif

/*--------------------------------------------------------------------------*\
 * Console management                  
\*--------------------------------------------------------------------------*/

#define not_PM_USE_CONSOLE

/*--------------------------------------------------------------------------*\
 * Core functions use
\*--------------------------------------------------------------------------*/

#define no_PM_USE_XCORE_FUNCTIONS
#define no_PM_USE_CORE_FUNCTIONS

/*--------------------------------------------------------------------------*\
 * Interface mapping function definitions                  
\*--------------------------------------------------------------------------*/

#include "pmInterface.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif

/*--------------------------------------------------------------------------*/

#endif /* _PM_ENV_H_ */
