/*	===========================================================================
	cUnfSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cUnfSt_h
#define cUnfSt_h

/*	---------------------------------------------------------------------------	*/
	
#include "cBaseSt.h"

/*	--------------------------------------------------------------------------- */

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dUnfoldStream	*pUnfoldStream;

/*	---------------------------------------------------------------------------	*/
	
pUnfoldStream	UnfoldStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack);
void			UnfoldStream_Delete(pUnfoldStream *self);

pmbool			UnfoldStream_Read(void *self, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
