/*	===========================================================================
	cBaseSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cBaseSt.h"
#include "cMemory.h"
#include "cTrace.h"
#include "cCore.h"

/*	---------------------------------------------------------------------------	*/

void BaseStream_Initialize(pBaseStream self, ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack)
{	
	self->inBufferLen = self->inBufferOffset = 0;
	self->err = 0;
	self->fInFinished = self->fFinished = self->fOneDecoded = pmfalse;
	self->readCallBack = aReadCallBack;
	self->stream = aStream;
	self->deleteCallBack = aDeleteCallBack;
}

/*	---------------------------------------------------------------------------	*/

void BaseStream_Finalize(pBaseStream self)
{	
	if (self->deleteCallBack != 0)
		(*self->deleteCallBack)(&self->stream);
}

/*	---------------------------------------------------------------------------	*/

void BaseStream_FillInBuffer(pBaseStream self, size_t aReadSize)
{	
	self->inBufferOffset = 0;
	self->inBufferLen = 0;

	if (!self->fInFinished)
		self->fInFinished = !(*self->readCallBack)(self->stream, self->inBuffer, aReadSize, &self->inBufferLen, &self->err);
}

/*	---------------------------------------------------------------------------	*/

pmbool BaseStream_NextAvalaible(pBaseStream self, pmbyte *aByte)
{	
	if (self->inBufferOffset == self->inBufferLen)
		BaseStream_FillInBuffer(self, kBaseStream_BufferSize);

	if (self->inBufferOffset != self->inBufferLen)
	{
		(*aByte) = self->inBuffer[self->inBufferOffset++];
		return pmtrue;
	}
	
	return pmfalse;
}

/*	---------------------------------------------------------------------------	*/

pmbool BaseStream_Read(pBaseStream self, void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr, DecodeNextCallBack aDecodeNext, TerminatedCallBack aTerminated)
{	
	size_t		theCount = 0;
	
	if ((!self->fFinished) && (self->err == 0))
	{		
		while (theCount < aBufferSize)
		{
			aBuffer[theCount] = (*aDecodeNext)(voidSelf);
		
			if (self->fOneDecoded)
				theCount++;
			else
			{
				if (!self->fInFinished)
				{
						/* ### The blocking factor is defined by this test */
					if (self->inBufferOffset != self->inBufferLen)
						continue;
					break;
				}
				
				if (aTerminated != 0)
					if (!(*aTerminated)(voidSelf))
						continue;
					
				self->fFinished = pmtrue;
				break;
			}
		}
	}

	if (theCount != 0)
	{
		(*aReadSize) = theCount;
		(*anErr) = 0;
		return pmtrue;
	}
	
	(*anErr) = self->err;
	(*aReadSize) = 0;
	
	if ((self->err != 0) || self->fFinished)
		return pmfalse;
		
	return pmtrue;
}

/*	---------------------------------------------------------------------------	*/

pmbool BaseStream_ReadMany(pBaseStream self, void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr, DecodeNextManyCallBack aDecodeNextMany, TerminatedCallBack aTerminated)
{	
	(*aReadSize) = 0;
	if ((!self->fFinished) && (self->err == 0))
	{		
		(*aReadSize) = (*aDecodeNextMany)(voidSelf, aBuffer, aBufferSize);
		
		if ((self->fInFinished) && (self->inBufferOffset == self->inBufferLen))
		{
			if (aTerminated != 0)
				self->fFinished = (*aTerminated)(voidSelf);
			else
				self->fFinished = pmtrue;
		}
	}

	(*anErr) = self->err;	
	if ((self->err != 0) || self->fFinished)
		return pmfalse;
		
	return pmtrue;
}

/*	---------------------------------------------------------------------------	*/

pmerror Stream_ReadAll(ReadCallBack aCallBack, void *aStream)
{
	pmerror	theErr;
	size_t	theReadSize;
	pmbyte	theBuffer[kStream_ReadAllBufferSize];
	
	while ((*aCallBack)(aStream, theBuffer, kStream_ReadAllBufferSize, &theReadSize, &theErr))
		;
		
	return theErr;
}

/*	---------------------------------------------------------------------------	*/

pmerror Stream_Read(ReadCallBack aCallBack, void *aStream, char *aBuffer, size_t aBufferSize, size_t *aReadSize)
{
	pmerror	theErr = 0;
	size_t	theBufferPos = 0;
	size_t	theReadSize;
	
	while (theBufferPos != aBufferSize)
	{
		if (!(*aCallBack)(aStream, (pmbyte*) &aBuffer[theBufferPos], aBufferSize - theBufferPos, &theReadSize, &theErr))
			break;
		
		theBufferPos += theReadSize;		
	}
		
	(*aReadSize) = theBufferPos;
	
	return theErr;
}

/*	---------------------------------------------------------------------------	*/

pmbool Stream_Contains(ReadCallBack aCallBack, void *aStream, char *aStr, size_t aStrLen, pmerror *anErr)
{
	size_t	theReadSize;
	pmbool	thefFound = pmfalse;
	size_t	thePos = 0;
	pmbyte	*theBuffer = Memory_CAlloc(aStrLen);
	
	if (theBuffer == 0)
	{
		(*anErr) = Stream_r_Memory;
		return pmfalse; 	
	}

	(*anErr) = 0;
	
	while (1)
	{	
		if (!(*aCallBack)(aStream, &theBuffer[thePos], aStrLen - thePos, &theReadSize, anErr))
			goto end;
			
		thePos += theReadSize;
		
		if (thePos == aStrLen)
		{
			thefFound = (c_xstrnicmp((char*) theBuffer, aStr, aStrLen) == 0);
			if (thefFound)
			{
				(*anErr) = Stream_ReadAll(aCallBack, aStream);
				goto end;
			}
			
			thePos--;
			c_memmove(theBuffer, &theBuffer[1], thePos);			
		}
	}

end:	
	Memory_Free(&theBuffer);
	
	return thefFound;
}
