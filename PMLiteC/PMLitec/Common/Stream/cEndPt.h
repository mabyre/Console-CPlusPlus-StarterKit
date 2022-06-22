/*	===========================================================================
	cEndPt.h
	Copyright (C) 1998 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cEndPt_h
#define cEndPt_h

/*	---------------------------------------------------------------------------	*/
	
#include "cStream.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/
	
typedef struct dEndPoint	*pEndPoint;

/*	---------------------------------------------------------------------------	*/
	
pEndPoint	EndPoint_New(ReadCallBack aReadCallBack, WriteCallBack aWriteCallBack, void *aStream, pmbool afReliable);

void		EndPoint_Delete(pEndPoint *self);

	/*	Returns true if data reliability and delivery is guaranteed by the protocol. This is used by the client
   		to adapt its protocol, and in particular to know if he must implement retries or if he needs ack or not. */
   
pmbool		EndPoint_IsReliable(pEndPoint self);

	/*	Returns false when an error occurs or there is no more data to handle.  */

pmbool		EndPoint_Read(pEndPoint self, pmbyte *aBuffer, size_t aBufferSize, size_t *aReadSize, pmerror *anErr);

	/*	Returns false when an error occurs or there is no more data to handle. This is a total nonsense for the
 		write functions because they always return true, unless an error occured during the effective write. */

pmbool		EndPoint_Write(pEndPoint self, pmbyte *aBuffer, size_t aBufferSize, pmerror *anErr);

pmbool		EndPoint_WriteFromStream(pEndPoint self, ReadCallBack aCallBack, void *aStream, pmerror *anErr);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
