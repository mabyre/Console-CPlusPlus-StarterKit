/*	===========================================================================
	cCountSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#include "cXTrace.h"
#include "cCountSt.h"
#include "cMemory.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dCounterStream
{
	ReadCallBack	readCallBack;				/*	Source stream read function */
	DeleteCallBack	deleteCallBack;				/*	Source stream delete function */
	void			*stream;					/*	Source stream */
	pmuint32		readCount;					/*	Number of bytes read	*/
} dCounterStream;

/*	---------------------------------------------------------------------------	*/

pCounterStream CounterStream_New(ReadCallBack aCallBack, void *aStream, DeleteCallBack aDeleteCallBack)
{	
	pCounterStream	self = Memory_CAlloc(sizeof(dCounterStream));
	
	if (self != 0)
	{
		self->readCallBack = aCallBack;
		self->deleteCallBack = aDeleteCallBack;
		self->stream = aStream;
	}
	else
		if (aDeleteCallBack != 0)
			(*aDeleteCallBack)(&aStream);
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void CounterStream_Delete(pCounterStream *self)
{
	if ((*self) != 0)
	{
		if ((*self)->deleteCallBack != 0)
			(*(*self)->deleteCallBack)(&(*self)->stream);
	
		Memory_Free(self);
	}
}

/*	---------------------------------------------------------------------------	*/

pmuint32 CounterStream_GetReadCount(pCounterStream self)
{
	return self->readCount;
}

/*	---------------------------------------------------------------------------	*/

pmbool CounterStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pCounterStream	self = (pCounterStream) voidSelf;
	pmbool			thefOK;
	
		/*	Just call the source stream to get data.	*/
	thefOK = (*self->readCallBack)(self->stream, aBuffer, aBufferSize, aReadSize, anErr);
	
		/* Update the read count.	*/
	self->readCount += (*aReadSize);
	
	return thefOK;
}


