/*	===========================================================================
	cStopSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cStopSt.h"
#include "cMemory.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dStopStream
{
	char			*stoptextPtr;
	char			*stopText;
	size_t			stopTextSize;
	size_t			stopTextPos;
	pmbool			fFinished;
	pmbool			fInFinished;
	
	ReadCallBack	readCallBack;
	DeleteCallBack	deleteCallBack;
	void			*stream;
	pmerror			err;
	
	size_t			remainCount;
	size_t			remainPos;
	pmbyte			remainChar;
	pmbool			fRemainChar;
} dStopStream;

/*	---------------------------------------------------------------------------	*/

pStopStream StopStream_NewStr(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack, char *aStopText, size_t aStopTextLen, pmbool afCopy)
{	
	pStopStream	self = Memory_CAlloc(sizeof(dStopStream));
	
	if (self != 0)
	{			
		self->stopTextSize = aStopTextLen;
		self->readCallBack = aReadCallBack;
		self->deleteCallBack = aDeleteCallBack;
		self->stream = aStream;

		if (afCopy)
		{
			if ((self->stoptextPtr = Memory_CAlloc(aStopTextLen)) == 0)
			{
				StopStream_Delete(&self);
				return 0;
			}
			
			self->stopText = self->stoptextPtr;
			c_memmove(self->stopText, aStopText, aStopTextLen);
		}
		else
			self->stopText = aStopText;
	}
	else
		if (aDeleteCallBack != 0)
			(*aDeleteCallBack)(&aStream);

	return self;
}

/*	---------------------------------------------------------------------------	*/

pStopStream StopStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack, char *aStopText)
{	
	return StopStream_NewStr(aReadCallBack, aStream, aDeleteCallBack, aStopText, c_strlen(aStopText), pmfalse);
}

/*	---------------------------------------------------------------------------	*/

pmbool StopStream_Finished(pStopStream self)
{
	return self->fFinished;
}

/*	---------------------------------------------------------------------------	*/

void StopStream_Delete(pStopStream *self)
{
	if ((*self) != 0)
	{
		Memory_Free(&(*self)->stoptextPtr);
		
		if ((*self)->deleteCallBack != 0)
			(*(*self)->deleteCallBack)(&(*self)->stream);
		
		Memory_Free(self);
	}
}

/*	---------------------------------------------------------------------------	*/

void StopStream_Reset(pStopStream self, ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack)
{
	self->readCallBack = aReadCallBack;
	self->deleteCallBack = aDeleteCallBack;
	self->stream = aStream;
	self->stopTextPos = 0;
	self->fFinished = self->fInFinished = pmfalse;
	self->err = 0;

	self->remainCount = 0;
	self->fRemainChar = pmfalse;
}

/*	---------------------------------------------------------------------------	*/

pmbool StopStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pStopStream	self = (pStopStream) voidSelf;
	pmbyte		theChar;
	size_t		theReadSize;
	
	(*anErr) = 0;
	(*aReadSize) = 0;

	if (!self->fFinished)
	{		
		while ((*aReadSize) < aBufferSize)
		{
			if (self->remainCount != 0)
			{
				aBuffer[*aReadSize] = (pmbyte) self->stopText[self->remainPos];
				(*aReadSize)++;
				
				self->remainPos++;
				self->remainCount--;
				continue;	
			}
			
			if (self->fRemainChar)
			{
				self->fRemainChar = pmfalse;
				aBuffer[*aReadSize] = self->remainChar;
				(*aReadSize)++;
				continue;
			}
			
			if (self->fInFinished)
			{
				self->fFinished = pmtrue;
				break;
			}
			
			self->fInFinished = !(*self->readCallBack)(self->stream, &theChar, 1, &theReadSize, &self->err);
			if (self->fInFinished && (self->stopTextPos != 0))
			{
				self->remainCount = self->stopTextPos;
				self->stopTextPos = 0;
				continue;
			}				
			
			if (theReadSize != 0)
			{
				if (theChar != self->stopText[self->stopTextPos])
				{
					self->remainCount = self->stopTextPos;
					self->stopTextPos = 0;
				}
					
				if (theChar == self->stopText[self->stopTextPos])
				{
					self->stopTextPos++;
					if (self->stopTextPos == self->stopTextSize)
					{
						self->fFinished = pmtrue;
						break;
					}
				}

				if (self->remainCount != 0)
				{
					self->remainPos = 0;
					if (self->stopTextPos == 0)
					{
						self->remainChar = theChar;
						self->fRemainChar = pmtrue;
					}
				}
				else
				{
					if (self->stopTextPos == 0)
					{
						aBuffer[*aReadSize] = theChar;
						(*aReadSize)++;
					}
				}			
			}		
		}
	}
	
	if ((*aReadSize) != 0)
		return pmtrue;
	
	if ((self->err != 0) && self->fFinished)
	{
		(*anErr) = self->err;
		return pmfalse;
	}
		
	return !self->fFinished;
}
