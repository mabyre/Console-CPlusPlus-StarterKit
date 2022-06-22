/*	===========================================================================
	cCRLFSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cCRLFSt.h"
#include "cMemory.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dCRLFStream
{
	dBaseStream			base;
	CRLFStream_EOLMode	eolMode;	/*	Target EOL mode.	*/
	pmuint16			LFCount;	/*	Number of LF characters found.	*/
	pmuint16			CRCount;	/*	Number of CR characters found.	*/
	pmint32				state;		/*	State machine state.	*/
} dCRLFStream;

/*	---------------------------------------------------------------------------	*/

pCRLFStream CRLFStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack, CRLFStream_EOLMode anEOLMode)
{	
	pCRLFStream	self = Memory_CAlloc(sizeof(dCRLFStream));
	
	if (self != 0)
	{
		BaseStream_Initialize(&self->base, aReadCallBack, aStream, aDeleteCallBack);
		self->eolMode = anEOLMode;
	}
	else
		if (aDeleteCallBack != 0)
			(*aDeleteCallBack)(&aStream);
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void CRLFStream_Delete(pCRLFStream *self)
{
	if (*self != 0)
	{
		BaseStream_Finalize(&(*self)->base);
		Memory_Free(self);
	}
}

/*	---------------------------------------------------------------------------	*/

static pmbool CRLFStream_HasTerminated(pCRLFStream self)
{
		/* Stream processing is terminated only if we are in state 0.	*/
	return (self->state == 0);
}

/*	---------------------------------------------------------------------------	*/

static pmbyte CRLFStream_Out(pCRLFStream self)
{	
		/*	Retruns the character (or sequence of character) to use as EOL.	*/
	if (self->eolMode == kEndOfLine_LF)
		return '\n';
		
	if (self->eolMode == kEndOfLine_CR)
		return '\r';
		
	self->state = 1;
	
	return '\r';
}

/*	---------------------------------------------------------------------------	*/

static pmbyte CRLFStream_DecodeNext(pCRLFStream self)
{
	pmbyte	theChar;
	
	self->base.fOneDecoded = pmtrue;
	while (pmtrue)
	{	
		if (self->state != 0)
		{
			self->state = 0;
			return '\n';
		}
		
		if (!BaseStream_NextAvalaible(&self->base, &theChar))
		{
			self->base.fOneDecoded = pmfalse;
			return 0;
		}
			
		if (theChar == '\r')
		{	
			self->CRCount++;
			if (self->CRCount > self->LFCount)
				return CRLFStream_Out(self);
		}
		else 
		{
			if (theChar == '\n')
			{	
				self->LFCount++;
				if (self->LFCount > self->CRCount)
					return CRLFStream_Out(self);
			}
			else
			{	
				self->LFCount = self->CRCount = 0;
				return theChar;
			}
		}
	}
	
		/* NOT REACHED */
	return 0;
}

/*	---------------------------------------------------------------------------	*/

pmbool CRLFStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pCRLFStream	self = (pCRLFStream) voidSelf;

	return BaseStream_Read(&self->base, self, aBuffer, aBufferSize, aReadSize, anErr, (DecodeNextCallBack) CRLFStream_DecodeNext, (TerminatedCallBack) CRLFStream_HasTerminated);
}
