/*	===========================================================================
	cSocket.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cSocket_h
#define cSocket_h

/*	---------------------------------------------------------------------------	*/
	
#include "cPPP.h"
#include "cSockDef.h"

/*	--------------------------------------------------------------------------- */

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------	*/

typedef pmbool (*SocketEventCallBack)(void *);

/*	---------------------------------------------------------------------------
	Module, traces and errors.
	---------------------------------------------------------------------------	*/

	/*	Module	*/
#define pmM_Socket				14

	/*	Trace	*/
#define pmT_Socket				PMTraceDefine(pmM_Socket, 0)

#define pmErr_Socket_UserCancel PMErrorDefine(pmM_Socket, pmErr_UserCancel)	/*	The user has cancelled the connection.	*/

/*	---------------------------------------------------------------------------	*/

pmbool		Socket_S_Resolve(pPPP aPPP, char *aHostName, pmuint32 *aHostIP, char* anIPName, pmuint32 aTimeOut);
pmbool		Socket_S_GetHostName(pPPP aPPP, char *aName, size_t aMaxLength);

pSocket		Socket_New(pPPP aPPP);
void		Socket_Delete(pSocket *self);

pmerror		Socket_GetError(pSocket self);

void		Socket_SetLinger(pSocket self);
void		Socket_SetTimeOut(pSocket self, pmuint32 aConnectTimeOut, pmuint32 aReadTimeOut);

pmbool		Socket_Connect(pSocket self, pmuint32 aHostIP, pmuint16 aPort);

pmbool		Socket_Close(pSocket self, pmbool afRead, pmbool afWrite);
pmbool		Socket_Terminate(pSocket self);

pmbool		Socket_Write(pSocket self, char *aBuffer, size_t aSize);
pmbool		Socket_SendCommand(pSocket self, char *aCommand, char *aData, char *aBuffer, size_t aBufferSize);

pmbool		Socket_Read(pSocket self, char *aBuffer, size_t aMaxSize, size_t *aReadSize);
pmbool		Socket_ReadLine(pSocket self, char *aBuffer, size_t aMaxSize, pmbool afAllowLFAlone);
pmbool		Socket_ReadToEndOfLine(pSocket self, pmbool afAllowLFAlone);

pmbool		Socket_IsOpen(pSocket self);
pmbool		Socket_CanRead(pSocket self);
pmbool		Socket_CanWrite(pSocket self);

void		Socket_SetEventCallBack(pSocket self, SocketEventCallBack aCallBack, void *aUserData);

pmbool		Socket_UserCancelled(pSocket self);

/*	---------------------------------------------------------------------------	*/
	
#include "cEnvSock.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/
	
#endif
