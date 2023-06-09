/*--------------------------------------------------------------------------*\
 * cCore.h
 * Copyright (C) 1996-98 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#ifndef cCore_h
#define cCore_h

/*--------------------------------------------------------------------------*/

#include "pmTypes.h"
#include "cXCore.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   extern "C" {
#endif

/*--------------------------------------------------------------------------*\
 * Additional common implementations
\*--------------------------------------------------------------------------*/

char  *c_xhexdigits();

char  *c_strrev(char *aStr);

int   c_isspace(int aChar);

int   c_isalnum(int aChar);

int   c_toupper(int aChar);

int   c_stricmp(const char *aStr1, const char *aStr2);

int   c_strncmp(const char *aStr1, const char *aStr2, size_t aLength);

int   c_strnicmp(const char *aStr1, const char *aStr2, size_t aLength);

char  *c_strncpymax(char *aStr1, const char *aStr2, size_t aLength, size_t aMax);

char  *c_strcpymax(char *aStr1, const char *aStr2, size_t aMax);

char  *c_strncatmax(char *aStr1, const char *aStr2, size_t aLength, size_t aMax);

char  *c_strcatmax(char *aStr1, const char *aStr2, size_t aMax);

char  *c_strpbrk(const char *aSrc, const char *aChars);

char  *c_xstrpbrk(const char *aSrc, const char *aChars, char aDelimiter);

int    c_isdigit(int aChar);

char  *c_strnchr(const char *aStr, size_t aLen, int aChar);

char  *c_strnstr(const char *aStr, size_t aLen, const char *aPattern, size_t *aPos);

/*--------------------------------------------------------------------------*\
   About the next 4 functions:
   aSize must be the buffer size, not the string max size that is 1 less.
   If the string generated is bigger than the buffer 0 is returned and
   a trace is emited.
   If you use a buffer whose size is greater or equal to 12, then you 
   are sure that the string will fit inside the buffer and you can ignore
   the return value that will never be 0.
   12 is 10 digits, minus sign and leading 0.
   11 is enough for unsigned values.
\*--------------------------------------------------------------------------*/
   
char  *c_int16toa(pmint16 aValue, char *aStr, size_t aSize);

char  *c_uint16toa(pmuint16 aValue, char *aStr, size_t aSize);

char  *c_int32toa(pmint32 aValue, char *aStr, size_t aSize);

char  *c_uint32toa(pmuint32 aValue, char *aStr, size_t aSize);

/*--------------------------------------------------------------------------*/

pmbool   c_splitstr(char *aStr, char aSplit, size_t anIndex, size_t *aStart, size_t *anEnd);

pmbool   c_splitnstr(char* aStr, size_t aStrLength, char aSplit, size_t anIndex, size_t *aStart, size_t *anEnd);

/*--------------------------------------------------------------------------*/

pmbool   c_natouint32(const char *aStr, size_t aLength, pmuint32 *anUInt32);

pmint32  c_atoint32(const char *aStr);

pmuint32 c_atouint32(const char *aStr);

/*--------------------------------------------------------------------------*\
 * Min and Max
\*--------------------------------------------------------------------------*/

#define c_min(a,b)      ((a) < (b) ? (a) : (b))
#define c_max(a,b)      ((a) > (b) ? (a) : (b))

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif

/*--------------------------------------------------------------------------*/

#endif
