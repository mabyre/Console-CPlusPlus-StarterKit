/*	===========================================================================
	cCountSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cCountSt_h
#define cCountSt_h

/*	---------------------------------------------------------------------------
	A stream that takes its data from another stream. The purpose of this stream 
	is to count bytes that go through it. This valu can be retrieved using the
	'CounterStream_GetReadCount' function.
	---------------------------------------------------------------------------	*/
	
#include "cStream.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dCounterStream	*pCounterStream;

/*	---------------------------------------------------------------------------	*/
	
	/*	Creates a stream that will take its data from the specified stream.	*/
pCounterStream	CounterStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack);

	/*	Delete a CounterStream object.	*/
void			CounterStream_Delete(pCounterStream *self);

	/*	Returns the number of bytes already read from the stream.	*/
pmuint32		CounterStream_GetReadCount(pCounterStream self);

	/*	Read bytes from the stream.	*/
pmbool			CounterStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
