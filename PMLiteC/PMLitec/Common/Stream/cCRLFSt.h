/*	===========================================================================
	cCRLFSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cCRLFSt_h
#define cCRLFSt_h

/*	---------------------------------------------------------------------------
	A stream that takes its data from another stream. This streams convert any 
	sequence of EOL characters (CR, LF, CRLF) to the specified 'CRLFStream_Mode'.
	---------------------------------------------------------------------------	*/
	
#include "cBaseSt.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dCRLFStream	*pCRLFStream;

/*	---------------------------------------------------------------------------	*/
	
typedef pmuint16	CRLFStream_EOLMode;

#define	kEndOfLine_LF 		0	/*	Converts EOL characters to LF (10, '\n')	*/
#define	kEndOfLine_CR		1	/*	Converts EOL characters to CR (13, '\r')	*/
#define	kEndOfLine_CRLF		2	/*	Converts EOL characters to CRLF (13-10, '\r\n')	*/

/*	---------------------------------------------------------------------------	*/
	
	/*	Creates a stream that will take its data from the specified stream. The EOL
		mode is used to specify how to convert end of line characters.	*/
pCRLFStream		CRLFStream_New(ReadCallBack aReadCallBack, void *aStream, DeleteCallBack aDeleteCallBack, CRLFStream_EOLMode anEOLMode);

	/*	Deletes a CRLFStream object.	*/
void			CRLFStream_Delete(pCRLFStream *self);

	/*	Read bytes from the stream.	*/
pmbool			CRLFStream_Read(void *self, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
