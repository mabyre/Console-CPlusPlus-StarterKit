/*	===========================================================================
	cSerSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

/*	---------------------------------------------------------------------------	*/

#define noDEBUG_SERIAL

/*	---------------------------------------------------------------------------	*/

#include "cSerSt.h"
#include "cMemory.h"
#include "cTrace.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dSerialStream
{
	pSerial		serial;
	pmerror		err;
	pmuint32	readCount;
	pmuint32	writeCount;
} dSerialStream;

/*	---------------------------------------------------------------------------	*/

pSerialStream SerialStream_New(pSerial aSerial)
{	
	pSerialStream	self = Memory_CAlloc(sizeof(dSerialStream));
	
	if (self != 0)
		self->serial = aSerial;
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void SerialStream_Delete(pSerialStream *self)
{
	Memory_Free(self);
}

/*	---------------------------------------------------------------------------	*/

pmuint32 SerialStream_GetReadCount(pSerialStream self)
{
	return self->readCount;
}

/*	---------------------------------------------------------------------------	*/

pmuint32 SerialStream_GetWriteCount(pSerialStream self)
{
	return self->writeCount;
}

/*	---------------------------------------------------------------------------	*/

pmbool SerialStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pSerialStream	self = (pSerialStream) voidSelf;
	
	(*anErr) = self->err;
	(*aReadSize) = 0;
	if (self->err != 0)
		return pmfalse;
		
	if (((*anErr) = Serial_ReadBytes(self->serial, aBuffer, aBufferSize, aReadSize, 0)) != 0)
	{
		(*anErr) = self->err;
		return pmfalse;
	}

#ifdef DEBUG_SERIAL
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

pmbool SerialStream_Write(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, pmerror *anErr)
{	
	pSerialStream	self = (pSerialStream) voidSelf;
	
	if (((*anErr) = Serial_WriteBytes(self->serial, aBuffer, aBufferSize)) != 0)
		return pmfalse;
	
	self->writeCount += aBufferSize;
	
	return pmtrue;
}


