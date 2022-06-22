/*	===========================================================================
	cUndotSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cUndotSt_h
#define cUndotSt_h

/*	---------------------------------------------------------------------------	*/
	
#include "cBaseSt.h"

/*	--------------------------------------------------------------------------- */

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dUndotterStream	*pUndotterStream;

/*	---------------------------------------------------------------------------	*/
	
pUndotterStream	UndotterStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack);
void			UndotterStream_Delete(pUndotterStream *self);

pmbool			UndotterStream_Read(void *self, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
