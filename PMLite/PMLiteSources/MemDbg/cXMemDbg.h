/*--------------------------------------------------------------------------*\
 * cXMemDbg.h
 * Copyright (C) 1996-98 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#ifndef _cXMemDbg_h_
#define _cXMemDbg_h_

/*--------------------------------------------------------------------------*/

#include "pmTypes.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------*\
   This is the implementation of the debug memory component.
   It links all allocated blocks altogether in a list.
   c_xmemdbg_dump_state traces the current allocated blocks as well as some 
   statistics.
   c_xmemdbg_set_max allows to specify the maximum amount of memory to use.
   When trying to allocate more than this c_malloc_dbg returns 0.
   It helps simulating low memory conditions.
   The new allocated blocks are preceded and followed by 4 bytes with the 
   value 0xFD. This allows to find memory misuse.
   The data part of a new allocated block if filled with 0xCD.
   When the block is freed the data part of the block is filled with 0xDD.
\*--------------------------------------------------------------------------*/

void  *c_malloc_dbg(size_t aSize, const char* aFileName, int aLine);
void  *c_realloc_dbg(void* aBlock, size_t aSize, const char* aFileName, int aLine);
void   c_free_dbg(void* aBlock);

void  c_xmemdbg_dump_state( void );
void  c_xmemdbg_set_max( size_t aSize );
void  c_xmemdbg_check( void );

/* ------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

/* ------------------------------------------------------------------- */

#endif /* _cXMemDbg_h_ */
