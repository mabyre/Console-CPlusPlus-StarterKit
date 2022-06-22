/*	===========================================================================
	cSockSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
/*	---------------------------------------------------------------------------	*/

#define noDEBUG_SOCKET

/*	---------------------------------------------------------------------------	*/

#include "cSockSt.h"
#include "cMemory.h"
#include "cTrace.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dSocketStream
{
	pSocket		sock;
	pmerror		err;
	pmbool		fFinished;
	pmuint32	readCount;
	pmuint32	writeCount;
} dSocketStream;

/*	---------------------------------------------------------------------------	*/

pSocketStream SocketStream_New(pSocket aSocket)
{	
	pSocketStream	self = Memory_CAlloc(sizeof(dSocketStream));
	
	if (self != 0)
		self->sock = aSocket;
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void SocketStream_Delete(pSocketStream *self)
{
	Memory_Free(self);
}

/*	---------------------------------------------------------------------------	*/

pmuint32 SocketStream_GetReadCount(pSocketStream self)
{
	return self->readCount;
}

/*	---------------------------------------------------------------------------	*/

pmuint32 SocketStream_GetWriteCount(pSocketStream self)
{
	return self->writeCount;
}

/*	---------------------------------------------------------------------------	*/

pmbool SocketStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pSocketStream	self = (pSocketStream) voidSelf;
	
	(*anErr) = self->err;
	(*aReadSize) = 0;
	if ((self->err != 0) || self->fFinished)
		return pmfalse;
		
	if (!Socket_Read(self->sock, (char*) aBuffer, aBufferSize, aReadSize))
	{
		(*anErr) = self->err = Socket_GetError(self->sock);
		self->fFinished = pmtrue;
		return pmfalse;
	}

#ifdef DEBUG_SOCKET
	{
	size_t	i;
	
	for (i = 0 ; i < (*aReadSize) ; i++)
		c_trace(T("%c", (char) aBuffer[i]));
	
	}
#endif
	
	self->readCount += (*aReadSize);
	
	return pmtrue;
}

/*	---------------------------------------------------------------------------	*/

pmbool SocketStream_Write(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, pmerror *anErr)
{	
	pSocketStream	self = (pSocketStream) voidSelf;
	
	if (!Socket_Write(self->sock, (char*) aBuffer, aBufferSize))
	{
		(*anErr) = Socket_GetError(self->sock);
		return pmfalse;
	}
	
	(*anErr) = 0;
	self->writeCount += aBufferSize;
	
	return pmtrue;
}


