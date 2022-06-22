/*	===========================================================================
	cQPEnc.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cQPEnc_h
#define cQPEnc_h

/*	---------------------------------------------------------------------------	*/
	
#include "cBaseSt.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dQPEncoder	*pQPEncoder;

/*	---------------------------------------------------------------------------	*/
	
pQPEncoder	QPEncoder_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack);
void		QPEncoder_Delete(pQPEncoder *self);

pmbool		QPEncoder_Read(void *self, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif


/*	---------------------------------------------------------------------------	*/

#endif
