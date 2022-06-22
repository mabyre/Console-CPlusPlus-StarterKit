/*	===========================================================================
	cB64Enc.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cB64Enc.h"
#include "cMemory.h"

/*	---------------------------------------------------------------------------	*/

#define kB64Encoder_LineWidth	16		/* 16*4 Character wide output */

typedef struct dB64Encoder
{
	dBaseStream		base;
	pmbool			fAutoWrap;
	pmint32			number;					
	pmbyte			inputBuffer[3];
	pmbyte			outputBuffer[4];
	size_t			endPos;
	pmint32			state;	/*	0,1,2		read the first characters
								3,4,5,6		emit encoded characters
								7,8			emit a line break
								9,10		emit final "="	*/

} dB64Encoder;

/*	---------------------------------------------------------------------------	*/

pB64Encoder B64Encoder_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack)
{	
	pB64Encoder	self = Memory_CAlloc(sizeof(dB64Encoder));
	
	if (self != 0)
	{
		BaseStream_Initialize(&self->base, aReadCallBack, aStream, aDeleteCallBack);
		self->fAutoWrap = pmtrue;
	}
	else
		if (aDeleteCallBack != 0)
			(*aDeleteCallBack)(&aStream);
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void B64Encoder_Delete(pB64Encoder *self)
{
	if (*self != 0)
	{
		BaseStream_Finalize(&(*self)->base);
		Memory_Free(self);
	}
}

/*	---------------------------------------------------------------------------	*/

static void B64Encoder_PrepareOutBuffer(pB64Encoder self)
{	
	self->outputBuffer[0] = (pmbyte) kB64Bits2Char[self->inputBuffer[0] >> 2];
	self->outputBuffer[1] = (pmbyte) kB64Bits2Char[((self->inputBuffer[0] << 4) + (self->inputBuffer[1] >> 4)) & 0x3f];
	self->outputBuffer[2] = (pmbyte) kB64Bits2Char[((self->inputBuffer[1] << 2) + (self->inputBuffer[2] >> 6)) & 0x3f];
	self->outputBuffer[3] = (pmbyte) kB64Bits2Char[self->inputBuffer[2] & 0x3f];
}

/*	---------------------------------------------------------------------------	*/

static pmbool B64Encoder_HasTerminated(pB64Encoder self)
{	
	if ((self->state == 0) || (self->state == 10))
		return pmtrue;
		
	B64Encoder_PrepareOutBuffer(self);
	
	if (self->state == 1)
		self->outputBuffer[2] = self->outputBuffer[3] = '=';
	
	if (self->state == 2)
		self->outputBuffer[3] = '=';
		
	self->state = 9;	
	return pmfalse;
}

/*	---------------------------------------------------------------------------	*/

static pmbyte B64Encoder_DecodeNext(pB64Encoder self)
{	
	pmbyte	theChar;
	
	self->base.fOneDecoded = pmtrue;
	while (pmtrue)
	{	
		switch (self->state)
		{	
			case 0:
				self->inputBuffer[0] = self->inputBuffer[1] = self->inputBuffer[2] = 0;

			case 1:
			case 2:
				if (!BaseStream_NextAvalaible(&self->base, &theChar))
				{
					self->base.fOneDecoded = pmfalse;
					return 0;
				}
				
				self->inputBuffer[self->state] = theChar;
				self->state++;

				if (self->state == 3)
					B64Encoder_PrepareOutBuffer(self);
				continue;

				/*	Char output	*/
			case 3:
			case 4:
			case 5:
			case 6:
				theChar = self->outputBuffer[self->state - 3];
				self->state++;
				if (self->state == 7)
				{
					if (self->fAutoWrap)
					{
						self->number++;
						if (self->number < kB64Encoder_LineWidth)
							self->state = 0;
						else
							self->number = 0;
					}
					else
						self->state = 0;
				}
				return theChar;

				/*	\r\n	*/
			case 7:
				self->state++;
				return '\r';
				
			case 8:
				self->state = 0;
				return '\n';
				
			case 9:
				if (self->endPos == 3)
					self->state = 10;
				return self->outputBuffer[self->endPos++];

			case 10:
				self->base.fOneDecoded = pmfalse;
				return 0;
		}
	}

		/* NOT REACHED */
	return 0;
}

/*	---------------------------------------------------------------------------	*/

pmbool B64Encoder_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pB64Encoder	self = (pB64Encoder) voidSelf;

	return BaseStream_Read(&self->base, self, aBuffer, aBufferSize, aReadSize, anErr, (DecodeNextCallBack) B64Encoder_DecodeNext, (TerminatedCallBack) B64Encoder_HasTerminated);
}

