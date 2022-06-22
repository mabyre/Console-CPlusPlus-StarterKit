/*	===========================================================================
	cSockSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cSockSt_h
#define cSockSt_h

/*	---------------------------------------------------------------------------	*/
	
#include "cStream.h"
#include "cSocket.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dSocketStream	*pSocketStream;

/*	---------------------------------------------------------------------------	*/
	
pSocketStream	SocketStream_New(pSocket aSocket);
void			SocketStream_Delete(pSocketStream *self);

pmbool			SocketStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

pmuint32		SocketStream_GetReadCount(pSocketStream self);

pmbool			SocketStream_Write(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, pmerror *anErr);

pmuint32		SocketStream_GetWriteCount(pSocketStream self);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
