/*	===========================================================================
	cPartSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cPartSt_h
#define cPartSt_h

/*	---------------------------------------------------------------------------	*/
	
#include "cBaseSt.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dPartStream	*pPartStream;

/*	---------------------------------------------------------------------------	*/
	
#define kPartStream_PartComplete	((pmuint16) 0)
#define kPartStream_PartIncomplete	((pmuint16) 1)
#define kPartStream_Error			((pmuint16) 2)
#define kPartStream_End				((pmuint16) 3)

/*	---------------------------------------------------------------------------	*/
	
pPartStream		PartStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack, char *aStopText);
void			PartStream_Delete(pPartStream *self);

void			PartStream_Reset(pPartStream self, ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack);

pmuint16		PartStream_ReadPart(void *self, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

pmbool			PartStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

pmbool			PartStream_Finished(pPartStream self);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
