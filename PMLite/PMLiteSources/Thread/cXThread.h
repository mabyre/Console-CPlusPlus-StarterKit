/*--------------------------------------------------------------------------*\
 * cXThread.h
 * Copyright (C) 1996-98 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#ifndef cXThread_h
#define cXThread_h

/*--------------------------------------------------------------------------*/

#include "pmTypes.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   extern "C" {
#endif

/*--------------------------------------------------------------------------*/

pmbool c_xthread_init( void );
void   c_xthread_term( void );

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif

/*--------------------------------------------------------------------------*/

#endif /* cXThread_h */