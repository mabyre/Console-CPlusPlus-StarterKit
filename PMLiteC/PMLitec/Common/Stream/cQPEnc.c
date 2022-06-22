/*	===========================================================================
	cQPEnc.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cQPEnc.h"
#include "cMemory.h"

/*	---------------------------------------------------------------------------	*/

#define kQPEncodingValue		"0123456789ABCDEF"

/*	---------------------------------------------------------------------------	*/

typedef struct dQPEncoder
{
	dBaseStream		base;
	pmbyte			curChar;
	pmint32			column;
	pmint32			state;			/*	0 Reading char (and generating char or '=')
										1 generating firstHexDigit
										2 generating secondHexDigit
										3 generating '\r'
										4 generating '\n' */
} dQPEncoder;

/*	---------------------------------------------------------------------------	*/

pQPEncoder QPEncoder_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack)
{	
	pQPEncoder	self = Memory_CAlloc(sizeof(dQPEncoder));
	
	if (self != 0)
		BaseStream_Initialize(&self->base, aReadCallBack, aStream, aDeleteCallBack);
	else
		if (aDeleteCallBack != 0)
			(*aDeleteCallBack)(&aStream);
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void QPEncoder_Delete(pQPEncoder *self)
{
	if (*self != 0)
	{
		BaseStream_Finalize(&(*self)->base);
		Memory_Free(self);
	}
}

/*	---------------------------------------------------------------------------	*/

static pmbyte QPEncoder_DecodeNext(pQPEncoder self)
{	
	pmbyte	theChar;

	self->base.fOneDecoded = pmtrue;
	switch (self->state)
	{	
		case 0:
			if (self->column++ > 74)
			{	
				self->state = 3;
				return (self->curChar = '=');
			}
			
			if (!BaseStream_NextAvalaible(&self->base, &theChar))
			{
				self->base.fOneDecoded = pmfalse;
				return 0;
			}

				/*	Do not encode CRLF.
					In case of CRLF reset self->column to 0  no white space at EOL */
			if ((theChar == '\r') || (theChar == '\n'))
			{
				
				self->column = 0;
				if ((self->curChar == ' ') || (self->curChar == '\t'))
				{
						/* = appended at end of line to avoid terminating with white spaces */
					self->curChar = theChar;
					self->state = 5;
					
					return '=';
				}
								
				return (self->curChar = theChar);
			}
			
			if (((theChar >= 33) && (theChar <= 60)) || ((theChar >= 62) && (theChar <= 126)) || (theChar == ' ') || (theChar == '\t'))
				return (self->curChar = theChar);
			
			self->state = 1;
			self->curChar = theChar;
			
			return '=';
			
		case 1:
			self->state = 2;
			return (pmbyte) kQPEncodingValue[self->curChar >> 4];
			
		case 2:
			self->state = 0;
			return (pmbyte) kQPEncodingValue[self->curChar & 0xf];
			
		case 3:
				/*	From state 0: Soft Line Break (Sure that no white space are terminating the line)	*/
			self->column = 0;
			self->state++;
			return (self->curChar = '\r');
			
		case 4:
			self->state = 0;
			return (self->curChar = '\n');

		case 5:
			self->state = 0;
			return self->curChar;
	}

		/*	NOT REACHED */
	return 0;
}

/*	---------------------------------------------------------------------------	*/

pmbool QPEncoder_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pQPEncoder	self = (pQPEncoder) voidSelf;

	return BaseStream_Read(&self->base, self, aBuffer, aBufferSize, aReadSize, anErr, (DecodeNextCallBack) QPEncoder_DecodeNext, 0);
}
