/*	===========================================================================
	cUndotSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cUndotSt.h"
#include "cMemory.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dUndotterStream
{
	dBaseStream		base;
	pmbyte			curChar;
	pmint32			column;
	pmint32			state;			/*	0 Reading char (and generating char or '=')
										1 generating firstHexDigit
										2 generating secondHexDigit
										3 generating '\r'
										4 generating '\n' */
} dUndotterStream;

/*	---------------------------------------------------------------------------	*/

pUndotterStream UndotterStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack)
{	
	pUndotterStream	self = Memory_CAlloc(sizeof(dUndotterStream));
	
	if (self != 0)
		BaseStream_Initialize(&self->base, aReadCallBack, aStream, aDeleteCallBack);
	else
		if (aDeleteCallBack != 0)
			(*aDeleteCallBack)(&aStream);
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void UndotterStream_Delete(pUndotterStream *self)
{
	if (*self != 0)
	{
		BaseStream_Finalize(&(*self)->base);
		Memory_Free(self);
	}
}

/*	---------------------------------------------------------------------------	*/

static size_t UndotterStream_DecodeNextMany(pUndotterStream self, pmbyte *aBuffer, size_t aBufferSize)
{
	pmbyte	theChar;
	pmbyte	*thePtr;
	pmbyte	*theBuffer;
	
	if (self->base.inBufferLen == 0)
		BaseStream_FillInBuffer(&self->base, aBufferSize);

	if (self->base.inBufferLen == 0)
		return 0;
		
		/*	Undot copy (work well because len("\r\n..") > len("\r\n."))	*/
	theBuffer = aBuffer;		
	thePtr = self->base.inBuffer;
	while (self->base.inBufferLen > 0)
	{	
		theChar = *thePtr++;
		self->base.inBufferLen--;
		
		switch (self->state)
		{	
				/*	Normal	*/
			case 0:				
				if (theChar == '\r')
					self->state = 1;
					
				*aBuffer++ = theChar;
				break;
				
				/*	We saw a '\r'	*/
			case 1:				
				if (theChar == '\n')
					self->state = 2;
				else
					if (theChar == '\r')
						self->state = 1;
					else
						self->state = 0;
						
				*aBuffer++ = theChar;
				break;
				
				/*	\r\n	*/
			case 2:				
				if (theChar == '.')
					self->state = 3;
				else
					if (theChar == '\r')
						self->state = 1;
					else
						self->state = 0;
						
				*aBuffer++ = theChar;
				break;
				
				/*	\r\n.	*/
			case 3:				
				if (theChar == '.')
				{	
					self->state = 0;
					break;
				}
				
				if (theChar == '\r')
					self->state = 1;
				else
					self->state = 0;
					
				*aBuffer++ = theChar;
				break;
		}
	}
	
	return (size_t) (aBuffer - theBuffer);
}

/*	---------------------------------------------------------------------------	*/

pmbool UndotterStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pUndotterStream	self = (pUndotterStream) voidSelf;

	return BaseStream_ReadMany(&self->base, self, aBuffer, aBufferSize, aReadSize, anErr, (DecodeNextManyCallBack) UndotterStream_DecodeNextMany, 0);
}
