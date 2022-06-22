/*	===========================================================================
	cB64Dec.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cB64Dec_h
#define cB64Dec_h

/*	---------------------------------------------------------------------------	*/
	
#include "cBaseSt.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dB64Decoder	*pB64Decoder;

/*	---------------------------------------------------------------------------	*/
	
pB64Decoder	B64Decoder_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack);
void		B64Decoder_Delete(pB64Decoder *self);

pmbool		B64Decoder_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
