/*	===========================================================================
	cDotSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cDotSt.h"
#include "cMemory.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dDotterStream
{
	dBaseStream		base;
	pmbyte			curChar;
	pmint32			column;
	pmint32			state;			/*	0 Reading char (and generating char or '=')
										1 generating firstHexDigit
										2 generating secondHexDigit
										3 generating '\r'
										4 generating '\n' */
} dDotterStream;

/*	---------------------------------------------------------------------------	*/

pDotterStream DotterStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack)
{	
	pDotterStream	self = Memory_CAlloc(sizeof(dDotterStream));
	
	if (self != 0)
		BaseStream_Initialize(&self->base, aReadCallBack, aStream, aDeleteCallBack);
	else
		if (aDeleteCallBack != 0)
			(*aDeleteCallBack)(&aStream);
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void DotterStream_Delete(pDotterStream *self)
{
	if (*self != 0)
	{
		BaseStream_Finalize(&(*self)->base);
		Memory_Free(self);
	}
}

/*	---------------------------------------------------------------------------	*/

static pmbyte DotterStream_DecodeNext(pDotterStream self)
{	
	pmbyte	theChar;
	
	self->base.fOneDecoded = pmtrue;
	if (self->state == 3)
	{	
		self->state = 0;
		return '.';
	}

	if (!BaseStream_NextAvalaible(&self->base, &theChar))
	{
		self->base.fOneDecoded = pmfalse;
		return 0;
	}

	switch (self->state)
	{
		case 0:
			if (theChar == '\r')
				self->state = 1;
			break;
			
		case 1:
			if (theChar == '\n')
				self->state = 2;
			else
				if (theChar != '\r')
					self->state = 0;
			break;
			
		case 2:
			if (theChar == '.')
				self->state = 3;
			else
				if (theChar == '\r')
					self->state = 1;
				else
					self->state = 0;
			break;
	}
	
	return theChar;
}

/*	---------------------------------------------------------------------------	*/

pmbool DotterStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pDotterStream	self = (pDotterStream) voidSelf;

	return BaseStream_Read(&self->base, self, aBuffer, aBufferSize, aReadSize, anErr, (DecodeNextCallBack) DotterStream_DecodeNext, 0);
}
