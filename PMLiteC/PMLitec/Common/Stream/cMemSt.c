/*	===========================================================================
	cMemSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cMemSt.h"
#include "cMemory.h"
#include "cTrace.h"
#include "cCore.h"

/*	---------------------------------------------------------------------------	*/

	/*	If MEMORYSTREAM_TEST is defined the number of bytes returned by the read 
		function is	computed randomly (with respect to the maximum specified read 
		size).	*/
#define noMEMORYSTREAM_TEST

/*	---------------------------------------------------------------------------	*/

typedef struct dMemoryStream
{
	pmbyte	*buffer;
	size_t	offset;
	size_t	bufferLen;
} dMemoryStream;

/*	---------------------------------------------------------------------------	*/

pMemoryStream MemoryStream_New(pmbyte *aBuffer, size_t aBufferSize)
{	
	pMemoryStream	self = Memory_CAlloc(sizeof(dMemoryStream));
	
	if (self != 0)
	{
		self->buffer = aBuffer;
		self->bufferLen = aBufferSize;
	}	
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

pMemoryStream MemoryStream_NewStr(char *aStr)
{	
	return MemoryStream_New((pmbyte*) aStr, c_strlen(aStr));
}

/*	---------------------------------------------------------------------------	*/

void MemoryStream_Delete(pMemoryStream *self)
{
	Memory_Free(self);
}

/*	---------------------------------------------------------------------------	*/

pmuint32 MemoryStream_GetReadCount(pMemoryStream self)
{
	return self->offset;
}

/*	---------------------------------------------------------------------------	*/

pmbool MemoryStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pMemoryStream	self = (pMemoryStream) voidSelf;
	size_t			theSize = 0;
	
	(*anErr) = 0;
	(*aReadSize) = 0;
		
		/*	If all data has been read returns false.	*/
	if (self->offset >= self->bufferLen)
		return pmfalse;
		
#ifdef MEMORYSTREAM_TEST
	theSize = c_rand(5);
	if (theSize > (self->bufferLen - self->offset))
		theSize = (self->bufferLen - self->offset);
#else
	theSize = (self->bufferLen - self->offset);
#endif

		/*	Verify we are not going to read data past the buffer end.	*/
	if (theSize > aBufferSize)
		theSize = aBufferSize;
		
		/*	Copy data into the returned buffer, and adjust read offset and read size.	*/
	c_memcpy(aBuffer, &self->buffer[self->offset], theSize);
	self->offset += theSize;
	(*aReadSize) = theSize;
	
	return pmtrue;
}


