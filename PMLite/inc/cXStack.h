/*--------------------------------------------------------------------------*\
 * cXStack.h
 * Copyright (C) 1996-98 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#ifndef _cXStack_h_
#define _cXStack_h_

/*--------------------------------------------------------------------------*/

#include "pmTypes.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------*/

typedef struct _TStackInfo
{
   size_t   itsBase;
   size_t   itsMax;
   pmbool   itsfReset;
   char* itsProcName;
} 
TStackInfo;

/*--------------------------------------------------------------------------*\ 
   This must be implemented for each platform.
   This should take multiple threads into account and return a per thread 
   structure.
   Must set itsfReset to pmfalse the first time it is called. This is used 
   to avoid stack measurement before c_stack_reset_usage_imp is called.
\*--------------------------------------------------------------------------*/

TStackInfo* c_xstack_get_info( void );

void     c_xstack_reset_usage( void );
void     c_xstack_update_usage( char* aProcName );
void     c_xstack_dump_usage( void );

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

/*--------------------------------------------------------------------------*/

#endif /* _cXStack_h_ */
