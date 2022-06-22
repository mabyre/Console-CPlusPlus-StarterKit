/*	===========================================================================
	cEndPt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
/*	---------------------------------------------------------------------------	*/

/* It is possible to trace input and output data by defining 
	PMLITE_DEBUG_ENDPOINT_READ and/or PMLITE_DEBUG_ENDPOINT_WRITE */

/*	---------------------------------------------------------------------------	*/

#include "cCore.h"
#include "cEndPt.h"
#include "cBaseSt.h"
#include "cMemory.h"
#include "cTrace.h"
#include "cSerSt.h"
#include "cSockSt.h"

/*	---------------------------------------------------------------------------	*/

#define kEndPoint_BufferSize	32

/*	---------------------------------------------------------------------------	*/

typedef struct dEndPoint
{
	ReadCallBack	readCallBack;
	WriteCallBack	writeCallBack;
	void			*stream;
	pmerror			err;
	pmbool			fReliable;	/* pmtrue if data delivery and reliability is guaranteed by the protocol */
} dEndPoint;

/*	---------------------------------------------------------------------------	*/

pEndPoint EndPoint_New(ReadCallBack aReadCallBack, WriteCallBack aWriteCallBack, void *aStream, pmbool afReliable)
{	
	pEndPoint	self = Memory_CAlloc(sizeof(dEndPoint));
	
	if (self != 0)
	{
		self->readCallBack = aReadCallBack;
		self->writeCallBack = aWriteCallBack;
		self->stream = aStream;
		self->fReliable = afReliable;
	}
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void EndPoint_Delete(pEndPoint *self)
{
	Memory_Free(self);
}

/*	---------------------------------------------------------------------------	*/

pmbool EndPoint_IsReliable(pEndPoint self)
{
	return self->fReliable;
}

/*	---------------------------------------------------------------------------	*/

pmbool EndPoint_Read(pEndPoint self, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{
	pmbool thefHasData = (*self->readCallBack)(self->stream, aBuffer, aBufferSize, aReadSize, anErr);
	
#ifdef PMLITE_DEBUG_ENDPOINT_READ
#ifdef PMLITE_TRACE
	if (thefHasData && (*aReadSize))
	{
		c_trace(TL(pmM_Stream, "EndPoint_Read :"));
		c_trace(B(pmM_Stream, aBuffer, *aReadSize));
	}
#endif
#endif
		
	return thefHasData;
}

/*	---------------------------------------------------------------------------	*/

pmbool EndPoint_Write(pEndPoint self, pmbyte *aBuffer, size_t aBufferSize, pmerror *anErr)
{
#ifdef PMLITE_DEBUG_ENDPOINT_WRITE
#ifdef PMLITE_TRACE
	if (aBufferSize)
	{
		c_trace(TL(pmM_Stream, "EndPoint_Write :"));
		c_trace(B(pmM_Stream, aBuffer, aBufferSize));
	}
#endif
#endif

	if ((*self->writeCallBack)(self->stream, aBuffer, aBufferSize, anErr))
		return pmtrue;
		
	return pmfalse;
}

/*	---------------------------------------------------------------------------	*/

pmbool EndPoint_WriteFromStream(pEndPoint self, ReadCallBack aCallBack, void *aStream, pmerror *anErr)
{
	char		theBuffer[kEndPoint_BufferSize];
	size_t		theReadSize;
	
	while (((*anErr) = Stream_Read(aCallBack, aStream, theBuffer, kEndPoint_BufferSize, &theReadSize)) == 0)
	{
		if (theReadSize != 0)
			if (!(*self->writeCallBack)(self->stream, (pmbyte*) theBuffer, theReadSize, anErr))
				return pmfalse;
	}
			
	return (*anErr == 0);		
}


