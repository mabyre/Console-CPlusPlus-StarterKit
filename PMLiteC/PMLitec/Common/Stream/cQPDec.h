/*	===========================================================================
	cQPDec.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cQPDec_h
#define cQPDec_h

/*	---------------------------------------------------------------------------	*/
	
#include "cBaseSt.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dQPDecoder	*pQPDecoder;

/*	---------------------------------------------------------------------------	*/
	
pQPDecoder	QPDecoder_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack);
void		QPDecoder_Delete(pQPDecoder *self);

pmbool		QPDecoder_Read(void *self, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
