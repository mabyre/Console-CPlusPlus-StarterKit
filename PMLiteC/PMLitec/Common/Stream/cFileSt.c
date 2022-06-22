/*	===========================================================================
	cFileSt.c
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/
	
#include "cFileSt.h"
#include "cMemory.h"
#include "cTrace.h"
#include "cCore.h"
#include <stdio.h>

/*	---------------------------------------------------------------------------	*/

	/*	If FILESTREAM_TEST is defined the number of bytes returned by the read 
		function is	computed randomly (with respect to the maximum specified read 
		size).	*/
#define noFILESTREAM_TEST

/*	---------------------------------------------------------------------------	*/

typedef struct dFileStream
{
	FILE	*file;
	pmbool	fFinished;
	pmerror	err;
} dFileStream;

/*	---------------------------------------------------------------------------	*/

pFileStream FileStream_NewWrite(char *aFileName)
{	
	pFileStream	self = Memory_CAlloc(sizeof(dFileStream));
	
	if (self != 0)
	{
			/*	Try to create the file. If it cannot be open the object is not created.	*/
		self->file = fopen(aFileName, "wb");
		if (self->file == 0)
			FileStream_Delete(&self);
	}	
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

pFileStream FileStream_New(char *aFileName)
{	
	pFileStream	self = Memory_CAlloc(sizeof(dFileStream));
	
	if (self != 0)
	{
			/*	Try to open the file. If it cannot be open the object is not created.	*/
		self->file = fopen(aFileName, "rb");
		if (self->file == 0)
			FileStream_Delete(&self);
	}	
	
	return self;
}

/*	---------------------------------------------------------------------------	*/

void FileStream_Delete(pFileStream *self)
{
		/*	Close the file.	*/
	if ((*self) != 0)
		if ((*self)->file != 0)
			fclose((*self)->file);
			
	Memory_Free(self);
}

/*	---------------------------------------------------------------------------	*/

pmbool FileStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr)
{	
	pFileStream		self = (pFileStream) voidSelf;
	size_t			theReadSize, theSize;
	
	(*anErr) = self->err;
	(*aReadSize) = 0;

		/*	If all data has been read returns false.	*/
	if (self->fFinished)
		return pmfalse;
		
#ifdef FILESTREAM_TEST
	theSize = c_rand(5);
	if (theSize > aBufferSize)
		theSize = aBufferSize;
#else
	theSize = aBufferSize;
#endif
		
		/*	Read data from the file.	*/
	theReadSize = fread(aBuffer, 1, theSize, self->file);
	if (theReadSize == 0)
	{
		self->err = ferror(self->file);
		self->fFinished = (self->err != 0) || feof(self->file);
	}
	else
		(*aReadSize) = theReadSize;
	
	return !self->fFinished;
}

/*	---------------------------------------------------------------------------	*/

pmbool FileStream_Write(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, pmerror *anErr)
{	
	pFileStream	self = (pFileStream) voidSelf;
	size_t		theWriteSize;
	
	theWriteSize = fwrite(aBuffer, 1, aBufferSize, self->file);
	(*anErr) = ferror(self->file);
	
	return (theWriteSize == aBufferSize);
}

