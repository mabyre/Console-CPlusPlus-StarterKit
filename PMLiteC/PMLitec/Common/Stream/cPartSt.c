/*	===========================================================================
	cPartSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cPartSt.h"
#include "cMemory.h"
#include "cCore.h"
#include "cTrace.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dPartStream
{
	char			*stopText;
	size_t			stopTextSize;
	size_t			stopTextPos;
	pmbool			fInFinished;
	pmbool			fFinished;
	
	ReadCallBack	readCallBack;
	void			*stream;
	DeleteCallBack	deleteCallBack;
	pmerror			err;
	
	pmbyte			*buffer;
	size_t			bufferSize;
} dPartStream;

/*	---------------------------------------------------------------------------	*/

pPartStream PartStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack, char *aStopText)
{	
	pPartStream	self = Memory_CAlloc(sizeof(dPartStream));
	
	if (self != 0)
	{
		self->stopText = aStopText;
		self->stopTextSize = c_strlen(aStopText);
		
		self->readCallBack = aReadCallBack;
		self->deleteCallBack = aDeleteCallBack;
		self->stream = aStream;

		self->buffer = Memory_CAlloc(self->stopTextSize);
		if (self->buffer == 0)
			PartStream_Delete(&self);
	}
	else
		if (aDeleteCallBack != 0)
			(*aDeleteCallBack)(&aStream);
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void PartStream_Reset(pPartStream self, ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack)
{
	self->readCallBack = aReadCallBack;
	self->deleteCallBack = aDeleteCallBack;
	self->stream = aStream;
	self->bufferSize = self->stopTextPos = 0;
	self->fInFinished = self->fFinished = pmfalse;
	self->err = 0;
}

/*	---------------------------------------------------------------------------	*/

void PartStream_Delete(pPartStream *self)
{
	if ((*self) != 0)
	{
		Memory_Free(&(*self)->buffer);
	
		if ((*self)->deleteCallBack != 0)
			(*(*self)->deleteCallBack)(&(*self)->stream);
		
		Memory_Free(self);
	}
}

/*	---------------------------------------------------------------------------	*/

static void PartStream_FillBuffer(pPartStream self)
{
	size_t	theReadSize;
	size_t	theRemainingSize;;
	
	if (!self->fInFinished)
	{
		theRemainingSize = (self->stopTextSize - self->bufferSize);
		self->fInFinished = !(*self->readCallBack)(self->stream, &self->buffer[self->bufferSize], theRemainingSize, &theReadSize, &self->err);	
		self->bufferSize += theReadSize;
	}
}

/*	---------------------------------------------------------------------------	*/

pmbool PartStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pPartStream	self = (pPartStream) voidSelf;
	pmbool		thefGetCharacter;
	
	(*aReadSize) = 0;
	(*anErr) = 0;
	
	if ((!self->fFinished) && (self->err == 0))
	{
		while ((*aReadSize) < aBufferSize)
		{
			PartStream_FillBuffer(self);
			thefGetCharacter = pmfalse;
			
			if (self->bufferSize == self->stopTextSize)
			{
				if (c_strncmp((char*) self->buffer, self->stopText, self->stopTextSize) != 0)
					thefGetCharacter = pmtrue;
				else
				{
					self->fFinished = pmtrue;
					return pmtrue;
				}
			}

			if (self->fInFinished)
			{
				if (self->bufferSize != 0)
					thefGetCharacter = pmtrue;
				else
					self->fFinished = pmtrue;
			}
			
			if (thefGetCharacter)		
			{
				aBuffer[*aReadSize] = self->buffer[0];
				(*aReadSize)++;
			
				self->bufferSize--;
				c_memmove(self->buffer, &self->buffer[1], self->bufferSize);
			}
			
			if (self->fFinished)
				break;			
		}
	}

	if (self->fFinished && ((*aReadSize) != 0))
		return pmtrue;
	
	(*anErr) = self->err;
	if ((self->err != 0) || self->fFinished)
		return pmfalse;
		
	return pmtrue;
}

/*	---------------------------------------------------------------------------	*/

pmbool PartStream_Finished(pPartStream self)
{	
	return self->fFinished;
}

/*	---------------------------------------------------------------------------	*/

pmuint16 PartStream_ReadPart(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pPartStream	self = (pPartStream) voidSelf;
	pmbool		thefBufferFull = pmfalse;
	pmbool		thefGetCharacter;
	
	c_assert(aBufferSize != 0, "PartStream_ReadPart");
	
	(*anErr) = 0;
	(*aReadSize) = 0;

	while (pmtrue)
	{
		PartStream_FillBuffer(self);
		if (self->bufferSize != 0)
		{
			if (self->bufferSize == self->stopTextSize)
			{
				if (c_strncmp((char*) self->buffer, self->stopText, self->stopTextSize) == 0)
				{
					self->bufferSize = 0;
					return kPartStream_PartComplete;
				}
				else
					thefGetCharacter = pmtrue;
			}
			else
				thefGetCharacter = self->fInFinished;

			if (thefGetCharacter)
			{
				if (thefBufferFull)
					return kPartStream_PartIncomplete;
					
				aBuffer[*aReadSize] = self->buffer[0];
				(*aReadSize)++;
				
				self->bufferSize--;
				c_memmove(self->buffer, &self->buffer[1], self->bufferSize);
				
				thefBufferFull = ((*aReadSize) == aBufferSize);
			}
		}
		else
		{
			if (self->fInFinished)
			{
				self->fFinished = pmtrue;
				
				if ((*aReadSize) != 0)
					return kPartStream_PartIncomplete;
					
				(*anErr) = self->err;
				return (self->err == 0) ? kPartStream_End : kPartStream_Error;
			}
		}
	}
	
		/* NEVER REACHED */
	return kPartStream_End;
}


