/*	===========================================================================
	cGenSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#include "cGenSt.h"
#include "cMemory.h"
#include "cTrace.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dGenericStream
{
	DeleteCallBack	deleteCallBack;				/*	Source stream delete function */
	ReadCallBack	readCallBack;				/*	Source stream read function */
	void			*stream;					/*	Source stream */
} dGenericStream;

/*	---------------------------------------------------------------------------	*/

pGenericStream GenericStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack)
{	
	pGenericStream	self = Memory_CAlloc(sizeof(dGenericStream));
	
	if (self != 0)
	{
		self->deleteCallBack = aDeleteCallBack;
		self->readCallBack = aReadCallBack;
		self->stream = aStream;
	}
	else
		if (aDeleteCallBack != 0)
			(*aDeleteCallBack)(&aStream);
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void GenericStream_Delete(pGenericStream *self)
{
	if (*self != 0)
	{
		if ((*self)->deleteCallBack != 0)
			(*(*self)->deleteCallBack)(&(*self)->stream);
			
		Memory_Free(self);
	}
}

/*	---------------------------------------------------------------------------	*/

pmbool GenericStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pGenericStream	self = (pGenericStream) voidSelf;
	pmbool			thefOK;
	
		/*	Just call the source stream to get data.	*/
	thefOK = (*self->readCallBack)(self->stream, aBuffer, aBufferSize, aReadSize, anErr);
	
	return thefOK;
}


