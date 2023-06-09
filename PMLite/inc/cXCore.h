/*--------------------------------------------------------------------------*\
 * cXCore.h
 * Copyright (c) 1996-98 AbyreSoft. All rights reserved.
\*--------------------------------------------------------------------------*/

#ifndef cXCore_h
#define cXCore_h

/*--------------------------------------------------------------------------*/

#include "pmTypes.h"
#include "pmEnv.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   extern "C" {
#endif

/*--------------------------------------------------------------------------*\
 * Standard Posix compatible implementations
\*--------------------------------------------------------------------------*/

void  *c_memcpy( void *aDest, const void *aSrc, size_t aCount );
void  *c_memset( void *aDest, int aChar, size_t aCount );
void  *c_memmove( void *aDest, const void *aSrc, size_t aCount );
int    c_memcmp( const void *aBuf1, const void *aBuf2, size_t aCount );
void  *c_memchr( const void *aBuf, int aChar, size_t aCount);

int    c_strcmp( const char *aString1, const char *aString2 );
char  *c_strcpy( char *aDest, const char *aSrc );
char  *c_strcat( char *aDest, const char *aSrc );
size_t c_strlen(const char *aString);
char  *c_strchr( const char *aString, int aChar );

void     c_srand( pmuint32 aSeed );
pmuint32 c_xrand( pmuint32 aMax );

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif

/*--------------------------------------------------------------------------*/

#endif

