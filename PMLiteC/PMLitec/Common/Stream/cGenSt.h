/*	===========================================================================
	cGenSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cGenSt_h
#define cGenSt_h

/*	---------------------------------------------------------------------------
	A stream that takes its data from another stream. The purpose of this stream 
	is to encapsulate any kind of stream into a generic one.
	---------------------------------------------------------------------------	*/
	
#include "cStream.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dGenericStream	*pGenericStream;

/*	---------------------------------------------------------------------------	*/
	
	/*	Creates a stream that will take its data from the specified stream.	*/
pGenericStream	GenericStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack);

	/*	Delete a GenericStream object. The encapsulated stream is deleted (if the 
		DeleteCallBack is not null).	*/
void			GenericStream_Delete(pGenericStream *self);

	/*	Read bytes from the stream.	*/
pmbool			GenericStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
