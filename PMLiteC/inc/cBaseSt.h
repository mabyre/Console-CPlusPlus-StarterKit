/*	===========================================================================
	cBaseSt.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cBaseSt_h
#define cBaseSt_h

/*	---------------------------------------------------------------------------	*/
	
#include "cStream.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/

#define	Stream_r_NoErr				0
#define	Stream_r_Memory				500

/*	---------------------------------------------------------------------------	*/

#define kBaseStream_BufferSize		64

#define kB64Bits2Char "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

/*	---------------------------------------------------------------------------	*/

#define kStream_ReadAllBufferSize	10

/*	---------------------------------------------------------------------------	*/

typedef struct dBaseStream
{
	pmbyte			inBuffer[kBaseStream_BufferSize];	/*  Buffer */
	size_t			inBufferLen;						/*	How much bytes are in buffer	*/
	size_t			inBufferOffset;						/*	Where is next byte to use		*/
	pmbool			fInFinished;						/*	No more data to read from source stream */
	pmerror			err;
	
	pmbool			fFinished;
	pmbool			fOneDecoded;
	
	ReadCallBack	readCallBack;				/*	Source stream read function */
	DeleteCallBack	deleteCallBack;				/*	Source stream delete function */
	void			*stream;					/*	Source stream */
} dBaseStream, *pBaseStream;


typedef pmbyte (*DecodeNextCallBack)(void *self);
typedef size_t (*DecodeNextManyCallBack)(void *self, pmbyte *aBuffer, size_t aBufferSize);
typedef pmbool (*TerminatedCallBack)(void *self);

/*	---------------------------------------------------------------------------	*/
	
void	BaseStream_Initialize(pBaseStream self, ReadCallBack aCallBack, void *aStream, DeleteCallBack aDeleteCallBack);
void	BaseStream_Finalize(pBaseStream self);

void	BaseStream_FillInBuffer(pBaseStream self, size_t aReadSize);
pmbool	BaseStream_NextAvalaible(pBaseStream self, pmbyte *aByte);

pmbool	BaseStream_Read(pBaseStream self, void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr, DecodeNextCallBack aDecodeNext, TerminatedCallBack aTerminated);

pmbool	BaseStream_ReadMany(pBaseStream self, void *voidSelf, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr, DecodeNextManyCallBack aDecodeNextMany, TerminatedCallBack aTerminated);

/*	---------------------------------------------------------------------------	*/

pmerror Stream_ReadAll(ReadCallBack aCallBack, void *aStream);

pmerror Stream_Read(ReadCallBack aCallBack, void *aStream, char *aBuffer, size_t aBufferSize, size_t *aReadSize);

pmbool	Stream_Contains(ReadCallBack aCallBack, void *aStream, char *aStr, size_t aStrLen, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
