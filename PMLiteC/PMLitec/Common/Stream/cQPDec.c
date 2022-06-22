/*	===========================================================================
	cQPDec.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cQPDec.h"
#include "cCore.h"
#include "cMemory.h"
#include "cTrace.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dQPDecoder
{
	dBaseStream	base;
	pmbyte		lastHex;
	pmint16		spaceCount;				/*	The number of spaces found		*/
	pmbyte		nextSignificant;
	pmint32		state;					/*	0	normal
											1	found =
											2	found =hex digit
											3	found '=\r'
											4	found spaces
											5	spaces are signifiant		*/
} dQPDecoder;

/*	---------------------------------------------------------------------------	*/

pQPDecoder QPDecoder_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack)
{	
	pQPDecoder	self = Memory_CAlloc(sizeof(dQPDecoder));
	
	if (self != 0)
		BaseStream_Initialize(&self->base, aReadCallBack, aStream, aDeleteCallBack);
	else
		if (aDeleteCallBack != 0)
			(*aDeleteCallBack)(&aStream);

	return self;
}

/*	---------------------------------------------------------------------------	*/

void QPDecoder_Delete(pQPDecoder *self)
{
	if (*self != 0)
	{
		BaseStream_Finalize(&(*self)->base);
		Memory_Free(self);
	}
}

/*	---------------------------------------------------------------------------	*/

#define PMQuotedGOSTATE(x) { self->state = (x); return 0; }

static pmbyte QPDecoder_DecodeNext(pQPDecoder self)
{	
	pmbyte	theChar;

	self->base.fOneDecoded = pmfalse;
	switch (self->state)
	{	
		case 0:
			if (!BaseStream_NextAvalaible(&self->base, &theChar))
				return 0;
				
			if (theChar == '=')
				PMQuotedGOSTATE(1);
				
			if (theChar == ' ')
			{	
				self->spaceCount = 1;
				PMQuotedGOSTATE(4);
			}
			
			self->base.fOneDecoded = pmtrue;
			return theChar;
			
		case 1:
			if (!BaseStream_NextAvalaible(&self->base, &theChar))
				return 0;
				
			if (theChar == '\r')
				PMQuotedGOSTATE(3);
				
			if (c_strchr("0123456789ABCDEF", theChar))
			{	
				self->lastHex = (pmbyte) (theChar - '0');
				if (self->lastHex > 9)
					self->lastHex -= ('A' - '0') - 10;
					
				PMQuotedGOSTATE(2);
			}
			
			c_trace(TL(pmT_Stream, "QPDecoder bad '=X': found %lx ", (pmuint32) theChar));

			PMQuotedGOSTATE(0);
			
		case 2:
			if (!BaseStream_NextAvalaible(&self->base, &theChar))
				return 0;
				
			if (c_strchr("0123456789ABCDEF",theChar))
			{	
				theChar -= '0';
				if (theChar > 9)
					theChar -= ('A' - '0') - 10;
				self->state = 0;
				
				self->base.fOneDecoded = pmtrue;
				return (pmbyte) ((self->lastHex * 16) + theChar);
			}
			else
			{	
				c_trace(TL(pmT_Stream,"QPDecoder bad '=XX': found %lx %lx", (pmuint32) self->lastHex, (pmuint32) theChar));

				PMQuotedGOSTATE(0);
			}
			return 0;
			
		case 3:
			if (!BaseStream_NextAvalaible(&self->base, &theChar))
				return 0;
				
			if (theChar == '\n')
				PMQuotedGOSTATE(0);
				
			self->state = 0;
			
			c_trace(TL(pmT_Stream,"QPDecoder bad 2 '=XX': found %lx %lx", (pmuint32) self->lastHex, (pmuint32) theChar));
			break;
			
		case 4:
			if (!BaseStream_NextAvalaible(&self->base, &theChar))
				return 0;
				
			if (theChar == ' ')
			{	
				self->spaceCount++;
				
				return 0;
			}
			
			if (theChar == '\r')
			{	
				self->state = 0;
				
				self->base.fOneDecoded = pmtrue;
				return theChar;
			}
			
			self->nextSignificant = theChar;
			PMQuotedGOSTATE(5);
			
		case 5:
			if (self->spaceCount)
			{	
				self->spaceCount--;
				
				self->base.fOneDecoded = pmtrue;
				return ' ';
			}
			
			if (self->nextSignificant == '=')
				PMQuotedGOSTATE(1);
				
			self->state = 0;
			
			self->base.fOneDecoded = pmtrue;
			return self->nextSignificant;
	}
	
	return 0;
}

/*	---------------------------------------------------------------------------	*/

pmbool QPDecoder_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pQPDecoder	self = (pQPDecoder) voidSelf;

	return BaseStream_Read(&self->base, self, aBuffer, aBufferSize, aReadSize, anErr, (DecodeNextCallBack) QPDecoder_DecodeNext, 0);
}
