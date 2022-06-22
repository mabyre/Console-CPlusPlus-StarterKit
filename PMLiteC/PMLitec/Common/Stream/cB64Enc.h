/*	===========================================================================
	cB64Enc.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cB64Enc_h
#define cB64Enc_h

/*	---------------------------------------------------------------------------	*/
	
#include "cBaseSt.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dB64Encoder	*pB64Encoder;

/*	---------------------------------------------------------------------------	*/
	
pB64Encoder	B64Encoder_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack);
void		B64Encoder_Delete(pB64Encoder *self);

pmbool		B64Encoder_Read(void *self, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
