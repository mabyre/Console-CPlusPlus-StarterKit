/*	===========================================================================
	cStream.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cStream_h
#define cStream_h

#include "cTypes.h"

/*	--------------------------------------------------------------------------- */

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------
	Module, traces and errors.
	---------------------------------------------------------------------------	*/

#define pmM_Stream					10

#define pmT_Stream					PMTraceDefine(pmM_Stream, 0)

#define pmErr_Stream_NoMemory		PMErrorDefine(pmM_Stream, pmErr_NoMemory)	/*	Memory storage error. (ie: user supplied callback has returned false).	*/

/*	---------------------------------------------------------------------------	*/
	
	/* Returns false when an error occurs or there is no more data to handle. */
typedef pmbool (*ReadCallBack)(void *aStream, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);
	
	/* Returns false when an error occurs or there is no more data to handle. */
typedef pmbool (*WriteCallBack)(void *aStream, pmbyte *aBuffer, size_t aBufferSize, pmerror *anErr);
	
	/* Each stream should implement its own delete using a function of this kind. */
typedef void (*DeleteCallBack)(void **aStream);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
