/*	===========================================================================
	cFileSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cFileSt_h
#define cFileSt_h

/*	---------------------------------------------------------------------------
	A stream that takes its data from a file specified by its name. This stream 
	should mainly be used to develop test applications.
	---------------------------------------------------------------------------	*/
	
#include "cStream.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dFileStream	*pFileStream;

/*	---------------------------------------------------------------------------	*/
	
	/*	Creates a stream that will get its data from the file content specified by
		its name. If the file is not found or does not exists, the object is not 
		created	(i.e: the function returns 0).	
		The file is open in read only binary mode.	*/
pFileStream		FileStream_New(char *aFileName);

	/*	Creates a stream that will write its data to the file specified by
		its name. If the file canot be created, the object is not 
		created	(i.e: the function returns 0).	
		The file is open in write only binary mode.	
		If the file already exists, it is replaced. */
pFileStream		FileStream_NewWrite(char *aFileName);

	/*	Delete a FileStream object, the file is closed.	*/
void			FileStream_Delete(pFileStream *self);

	/*	Read bytes from the stream. If an error code is returned it is get from 
		the standard C function 'ferror'.	*/
pmbool			FileStream_Read(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

	/*	Write bytes to the stream. If an error code is returned it is get from 
		the standard C function 'ferror'.	*/
pmbool			FileStream_Write(void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
