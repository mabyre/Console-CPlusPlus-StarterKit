/*	===========================================================================
	cDotSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cDotSt_h
#define cDotSt_h

/*	---------------------------------------------------------------------------	*/
	
#include "cBaseSt.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dDotterStream	*pDotterStream;

/*	---------------------------------------------------------------------------	*/
	
pDotterStream	DotterStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack);
void			DotterStream_Delete(pDotterStream *self);

pmbool			DotterStream_Read(void *self, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
