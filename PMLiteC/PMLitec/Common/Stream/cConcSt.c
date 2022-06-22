/*	===========================================================================
	cConcSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cConcSt.h"
#include "cMemory.h"

/*	---------------------------------------------------------------------------	*/

typedef struct dConcatStream
{
	ReadCallBack	readCallBack1;				/*	First source stream read function */
	DeleteCallBack	deleteCallBack1;
	void			*stream1;					/*	First source stream */
	pmbool			fFinished1;
	
	ReadCallBack	readCallBack2;				/*	Second source stream read function */
	DeleteCallBack	deleteCallBack2;
	void			*stream2;					/*	Second source stream */
	pmbool			fFinished2;
	
	pmerror			err;
} dConcatStream;

/*	---------------------------------------------------------------------------	*/

pConcatStream ConcatStream_New2(ReadCallBack aCallBack1, void *aStream1, DeleteCallBack aDeleteCallBack1, ReadCallBack aCallBack2, void *aStream2, DeleteCallBack aDeleteCallBack2)
{	
	pConcatStream	self = Memory_CAlloc(sizeof(dConcatStream));
	
	if (self != 0)
	{
		self->readCallBack1 = aCallBack1;
		self->deleteCallBack1 = aDeleteCallBack1;
		self->stream1 = aStream1;
		self->readCallBack2 = aCallBack2;
		self->deleteCallBack2 = aDeleteCallBack2;
		self->stream2 = aStream2;
	}	
	else
	{
		if (aDeleteCallBack1 != 0)
			(*aDeleteCallBack1)(&aStream1);

		if (aDeleteCallBack2 != 0)
			(*aDeleteCallBack2)(&aStream2);
	}
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

pConcatStream ConcatStream_New(ReadCallBack aCallBack1, void *aStream1, ReadCallBack aCallBack2, void *aStream2)
{	
/*	pConcatStream	self = Memory_CAlloc(sizeof(dConcatStream));
	
	if (self != 0)
	{
		self->readCallBack1 = aCallBack1;
		self->deleteCallBack1 = 0;
		self->stream1 = aStream1;
		self->readCallBack2 = aCallBack2;
		self->deleteCallBack2 = 0;
		self->stream2 = aStream2;
	}	
	
	return self;
*/
	return ConcatStream_New2(aCallBack1, aStream1, NULL, aCallBack2, aStream2, NULL);
}

/*	---------------------------------------------------------------------------	*/

void ConcatStream_Delete(pConcatStream *self)
{
//	Memory_Free(self);

	if (*self != 0)
	{
		if ((*self)->deleteCallBack1 != 0)
			(*(*self)->deleteCallBack1)(&(*self)->stream1);

		if ((*self)->deleteCallBack2 != 0)
			(*(*self)->deleteCallBack2)(&(*self)->stream2);
			
	Memory_Free(self);
}
}

/*	---------------------------------------------------------------------------	*/

pmbool ConcatStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pConcatStream	self = (pConcatStream) voidSelf;
	
	(*anErr) = self->err;
	(*aReadSize) = 0;
	
	if (self->fFinished2 || (self->err != 0))
		return pmfalse;
		
	if (self->fFinished1)
	{
		if (self->stream2 == 0)
			self->fFinished2 = pmtrue;
		else
		{
			self->fFinished2 = !(*self->readCallBack2)(self->stream2, aBuffer, aBufferSize, aReadSize, anErr);
			if (self->fFinished2)
				self->err = (*anErr);
		}
		
		return (self->err == 0);
	}
	
	self->fFinished1 = !(*self->readCallBack1)(self->stream1, aBuffer, aBufferSize, aReadSize, anErr);
	if (self->fFinished1)
		self->err = (*anErr);
		
	return (self->err == 0);
}

/*	---------------------------------------------------------------------------	*/

void ConcatStream_GetStreams(pConcatStream self, void **aStream1, void **aStream2)
{	
	(*aStream1) = self->stream1;
	(*aStream2) = self->stream2;
}
