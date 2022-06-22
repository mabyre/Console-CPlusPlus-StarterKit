/*	===========================================================================
	cB64Dec.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cB64Dec.h"
#include "cMemory.h"
#include "cTrace.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dB64Decoder
{
	dBaseStream		base;
	pmbyte			out[3];
	pmuint32		b24bits;
	pmbyte			char2Bits[256];
	pmint32			state;
} dB64Decoder;

/*	---------------------------------------------------------------------------	*/

static void B64Decoder_BuildInvertedMap(pmbyte *aBits2Char, pmbyte *aChar2Bits)
{	
	size_t i;
	
	for (i = 0 ; i != 256 ; i++)
		aChar2Bits[i] = 0xff;
	
	i = 0;
	while ((*aBits2Char) != 0)
		aChar2Bits[*aBits2Char++] = (pmbyte) i++;
}

/*	---------------------------------------------------------------------------	*/

pB64Decoder B64Decoder_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack)
{	
	pB64Decoder	self = Memory_CAlloc(sizeof(dB64Decoder));
	
	if (self != 0)
	{
		BaseStream_Initialize(&self->base, aReadCallBack, aStream, aDeleteCallBack);
		B64Decoder_BuildInvertedMap((pmbyte*) kB64Bits2Char, self->char2Bits);
	}
	else
		if (aDeleteCallBack != 0)
			(*aDeleteCallBack)(&aStream);
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void B64Decoder_Delete(pB64Decoder *self)
{
	if (*self != 0)
	{
		BaseStream_Finalize(&(*self)->base);
		Memory_Free(self);
	}
}

/*	---------------------------------------------------------------------------	*/

static pmbool B64Decoder_HasTerminated(pB64Decoder self)
{
	c_assert(self->state <= 3, "Base64-like decoder error");
	
		/*	Just on a multiple	*/
	if (self->state == 0)
		return pmtrue;
	
	if (self->state == 1)
	{	
		c_trace(TL(pmT_Stream, "%s", "B64Decoder error: Loosing 6 last bits"));
		return pmtrue;
	}
	
	if (self->state == 2)
	{	
		self->b24bits >>= 4;
		self->out[0] = (pmbyte) self->b24bits;	
		self->state = 9;
	}
	else
	{
		self->b24bits >>= 2;
		self->out[1] = (pmbyte) (self->b24bits >> 8);
		self->out[0] = (pmbyte) self->b24bits;
		self->state = 8;
	}
	
	return pmfalse;				
}

/*	---------------------------------------------------------------------------	*/
/*	Return next 6 bits from the input	*/

static pmbool B64Decoder_GetNext6Bits(pB64Decoder self, pmbyte *aByte)
{	
	pmbyte	theByte;
	pmbyte	theBits;
	
	do
	{	
		if (!BaseStream_NextAvalaible(&self->base, &theByte))
			return pmfalse;
			
		theBits = self->char2Bits[theByte];
	}	
	while (theBits == 0xff);

	(*aByte) = theBits;
	return pmtrue;
}

/*	---------------------------------------------------------------------------	*/
/*	Get one char for client	*/

static pmbyte B64Decoder_DecodeNext(pB64Decoder self)
{	
	pmbyte	theCode;
	pmbyte	theByte;
	
	self->base.fOneDecoded = pmtrue;
	while (pmtrue)
	{	
			/*	Are we filling buffer?	*/
		if (self->state < 4)
		{	
			if (!B64Decoder_GetNext6Bits(self, &theCode))
			{
				self->base.fOneDecoded = pmfalse;
				return 0;
			}

			self->b24bits <<= 6;
			self->b24bits += theCode;
			self->state++;
			if (self->state == 4)
			{	
				self->out[0] = (pmbyte) (self->b24bits >> 16);
				self->out[1] = (pmbyte) (self->b24bits >> 8);
				self->out[2] = (pmbyte) (self->b24bits);
			}
		}
			/*	Are there characters avalaible?	*/
		if ((self->state >= 4) && (self->state <= 7))
		{	
			theByte = self->out[self->state++ - 4];
			
			if (self->state == 7)
				self->state = 0;
				
			return theByte;
		}
		
		if (self->state == 8)
		{
			self->state++;
			return self->out[1];
		}
		
		if (self->state == 9)
		{
			self->state = 0;
			return self->out[0];
		}
	}

		/* NOT REACHED */
	return 0;
}

/*	---------------------------------------------------------------------------	*/

pmbool B64Decoder_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pB64Decoder	self = (pB64Decoder) voidSelf;

	return BaseStream_Read(&self->base, self, aBuffer, aBufferSize, aReadSize, anErr, (DecodeNextCallBack) B64Decoder_DecodeNext, (TerminatedCallBack) B64Decoder_HasTerminated);
}

/*	---------------------------------------------------------------------------	*/

