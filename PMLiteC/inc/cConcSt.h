/*	===========================================================================
	cConcSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cConcSt_h
#define cConcSt_h

/*	---------------------------------------------------------------------------	*/
	
#include "cStream.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dConcatStream	*pConcatStream;

/*	---------------------------------------------------------------------------	*/
	
pConcatStream	ConcatStream_New2(ReadCallBack aCallBack1, void *aStream1, DeleteCallBack aDeleteCallBack1, ReadCallBack aCallBack2, void *aStream2, DeleteCallBack aDeleteCallBack2);

pConcatStream	ConcatStream_New(ReadCallBack aCallBack1, void *aStream1, ReadCallBack aCallBack2, void *aStream2);
void			ConcatStream_Delete(pConcatStream *self);

pmbool			ConcatStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

void			ConcatStream_GetStreams(pConcatStream self, void **aStream1, void **aStream2);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
