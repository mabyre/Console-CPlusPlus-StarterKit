/*	===========================================================================
	cUnfSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cUnfSt.h"
#include "cMemory.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dUnfoldStream
{
	dBaseStream		base;
	pmbyte			out[2];
	pmint32			state;
} dUnfoldStream;

/*	---------------------------------------------------------------------------	*/

pUnfoldStream UnfoldStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack)
{	
	pUnfoldStream	self = Memory_CAlloc(sizeof(dUnfoldStream));
	
	if (self != 0)
		BaseStream_Initialize(&self->base, aReadCallBack, aStream, aDeleteCallBack);
	else
		if (aDeleteCallBack != 0)
			(*aDeleteCallBack)(&aStream);
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void UnfoldStream_Delete(pUnfoldStream *self)
{
	if (*self != 0)
	{
		BaseStream_Finalize(&(*self)->base);
		Memory_Free(self);
	}
}

/*	---------------------------------------------------------------------------	*/

static pmbool UnfoldStream_HasTerminated(pUnfoldStream self)
{
	if ((self->state == 0) || (self->state == 3))
		return pmtrue;
	
	if (self->state == 1)
	{
		self->out[0] = '\r';
		self->state = 4;
	}
	
	if (self->state == 2)
	{
		self->out[1] = '\r';
		self->out[0] = '\n';
		self->state = 5;
	}
	
	return pmfalse;				
}

/*	---------------------------------------------------------------------------	*/

static pmbyte UnfoldStream_DecodeNext(pUnfoldStream self)
{
	pmbyte	theChar;
	
	self->base.fOneDecoded = pmtrue;
	while (pmtrue)
	{	
		if (self->state < 4)
			if (!BaseStream_NextAvalaible(&self->base, &theChar))
			{
				self->base.fOneDecoded = pmfalse;
				return 0;
			}
			
		switch (self->state)
		{	
			case 6:
				self->state = 0;
				
			case 0:				
				if (theChar == '\r')
					self->state = 1;
				else	
					return theChar;
				break;
				
				/*	We saw a '\r'	*/
			case 1:				
				if (theChar == '\n')
					self->state = 2;
				else
				{
					self->out[0] = theChar;
					self->state = 4;
					return '\r';
				}						
				break;
				
				/*	\r\n	*/
			case 2:				
				if ((theChar == ' ') || (theChar == '\t'))
				{
					self->state = 3;
					return ' ';
				}
				else
				{
					self->out[1] = '\n';
					self->out[0] = theChar;
					self->state = 5;
					return '\r';
				}					
				break;
				
			case 3:
				if ((theChar != ' ') && (theChar != '\t'))
					self->state = 6;
				break;
				
			case 4:
				self->state = 0;
				return self->out[0];
				break;
				
			case 5:
				self->state = 4;
				return self->out[1];
				break;
		}
	}
	
		/* NOT REACHED */
	return 0;
}

/*	---------------------------------------------------------------------------	*/

pmbool UnfoldStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pUnfoldStream	self = (pUnfoldStream) voidSelf;

	return BaseStream_Read(&self->base, self, aBuffer, aBufferSize, aReadSize, anErr, (DecodeNextCallBack) UnfoldStream_DecodeNext, (TerminatedCallBack) UnfoldStream_HasTerminated);
}
