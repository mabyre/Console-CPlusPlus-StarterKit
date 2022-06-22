/*	===========================================================================
	cMemSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cMemSt_h
#define cMemSt_h

/*	---------------------------------------------------------------------------
	A stream that takes its data from memory. Data can be specified either as a
	memory block (with its size) or as a C string.
	---------------------------------------------------------------------------	*/

#include "cStream.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dMemoryStream	*pMemoryStream;

/*	---------------------------------------------------------------------------	*/
	
	/*	Creates a stream from a memory buffer (giving its size), data are not copied
		and should remain valid while the MemoryStream is not deleted.	*/
pMemoryStream	MemoryStream_New(pmbyte *aBuffer, size_t aBufferSize);

	/*	Creates a stream from a C string, the string is not copied
		and should remain valid while the MemoryStream is not deleted.	*/
pMemoryStream	MemoryStream_NewStr(char *aStr);

	/*	Deletes a MemoryStream object.	*/
void			MemoryStream_Delete(pMemoryStream *self);

	/*	Returns the number of bytes already read from the stream.	*/
pmuint32		MemoryStream_GetReadCount(pMemoryStream self);

	/*	Read bytes from the stream.	No error can occur when reading from a MemoryStream. */
pmbool			MemoryStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
