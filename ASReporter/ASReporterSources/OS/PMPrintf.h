// ===========================================================================
//	PMPrintf.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMPrintf_h
#define PMPrintf_h

// ---------------------------------------------------------------------------

#include "PMBuffer.h"
#include <stdarg.h>

// ---------------------------------------------------------------------------
/*
Valid flag list:
%s	 : C string
%c  : char
%p  : pointer (0xdddddddd)
%ld : long
%lu : unsigned long
%lx : long (output in hexadecimal form)
%t  : size_t
%b  : binary		// Not implemented
%n  : EOL			
%%  : %
%T  : long (output as a 4 character string)
Be careful: memory is allocated using '::pm_malloc' and must be freed by the 
caller using '::pm_free'.
*/

/**
	ANSI sprintf-like method. 
	The difference is that the memory is allocated by the function itself and so
	you do not need to take care of the size of the buffer you provide.
	See PMPrintf.h for a list of valid % parameters.
*/
PMBufferRef pm_xsprintf(const char* aFormat, ...);
/**
	The va_list version of \Ref{pm_xsprintf}.
*/
PMBufferRef pm_vxsprintf(const char* aFormat, va_list anArgs);

// ---------------------------------------------------------------------------

#endif // PMPrintf_h

