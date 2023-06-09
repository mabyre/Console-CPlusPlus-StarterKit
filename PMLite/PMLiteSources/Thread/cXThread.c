/*--------------------------------------------------------------------------*\
 * cXThread.c
 * Copyright (C) 1996-98 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#include "pmTypes.h"
#include "pmEnv.h"

/*--------------------------------------------------------------------------*/

#ifdef PM_USE_STACK_DEBUG

/*--------------------------------------------------------------------------*/

pmbool c_xthread_init( void )
{
   TStackInfo* theSI;

   /* Initialize stack info */
   theSI = c_xstack_get_info();
   if ( theSI == 0 )
      return pmfalse;

   pm_xstack_reset_usage();

   return pmtrue;
}

/*--------------------------------------------------------------------------*/

void c_xthread_term( void )
{
   TStackInfo* theSI;

   /* Initialize stack info */
   theSI = c_xstack_get_info();
   if ( theSI != 0 )
      free( theSI );
}

/*--------------------------------------------------------------------------*/

#endif /* PM_USE_STACK_DEBUG */  
