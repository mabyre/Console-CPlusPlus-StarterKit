/*	===========================================================================
	cStopSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cStopSt_h
#define cStopSt_h

/*	---------------------------------------------------------------------------	*/
	
#include "cBaseSt.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dStopStream	*pStopStream;

/*	---------------------------------------------------------------------------	*/
	
pStopStream		StopStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack, char *aStopText);
pStopStream		StopStream_NewStr(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack, char *aStopText, size_t aStopTextLen, pmbool afCopy);

void			StopStream_Delete(pStopStream *self);

pmbool			StopStream_Finished(pStopStream self);

void			StopStream_Reset(pStopStream self, ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack);

pmbool			StopStream_Read(void *self, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
