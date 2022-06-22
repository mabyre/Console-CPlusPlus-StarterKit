/*	===========================================================================
	cSerSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cSerSt_h
#define cSerSt_h

/*	---------------------------------------------------------------------------	*/
	
#include "cStream.h"
#include "cSerial.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dSerialStream	*pSerialStream;

/*	---------------------------------------------------------------------------	*/
	
pSerialStream	SerialStream_New(pSerial aSerial);
void			SerialStream_Delete(pSerialStream *self);

pmbool			SerialStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

pmuint32		SerialStream_GetReadCount(pSerialStream self);

pmbool			SerialStream_Write(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, pmerror *anErr);

pmuint32		SerialStream_GetWriteCount(pSerialStream self);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
