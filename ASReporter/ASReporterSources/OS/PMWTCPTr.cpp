// ===========================================================================
//	PMWTCPTr.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMWTCPTr.h"

#include "PMW32Hlp.h"
#include "PMData.h"
#include <winsock.h>
#include "PMPrintf.h"

//-----------------------------------------------------------------------
#define PM_TRACEMODULE STRUCT
PM_DEFINE_TRACE_CODE(Trace, 1);

// ---------------------------------------------------------------------------
//	Winsock management...
// ---------------------------------------------------------------------------
#define WS_VERSION	1
#define WS_REVISION	1

// WINSOCK.DLL for 16 bits
#define WINSOCK_DLL	"WSOCK32.DLL"

// ---------------------------------------------------------------------------
static pmbool _LoadProc(FARPROC* aProc, HINSTANCE anInstance, LPCSTR aProcName)
{
	if (anInstance == 0)
		return pmfalse;
	*aProc = ::GetProcAddress(anInstance, aProcName);
	return (*aProc != 0);
}

#define SOCKINIT(x)		\
	::_LoadProc((FARPROC*)&sWSManager.P_##x, PMWS11TracerManager::cWSInstance, #x)

#define SOCKCLOSE(x)	\
	*((void (**)())&sWSManager.P_##x) = PMWS11TracerManager::NODLL

//-----------------------------------------------------------------------
#define PM_closesocket		sWSManager.P_closesocket
#define PM_connect			sWSManager.P_connect
#define PM_gethostbyname	sWSManager.P_gethostbyname
#define PM_htons			sWSManager.P_htons
#define PM_inet_addr		sWSManager.P_inet_addr
#define PM_send				sWSManager.P_send
#define PM_socket			sWSManager.P_socket

//#define PM_select			sWSManager.P_select

#define PM_WSAStartup				sWSManager.P_WSAStartup
#define PM_WSACleanup				sWSManager.P_WSACleanup

//-----------------------------------------------------------------------
struct PMWS11TracerManager
{
	int (PASCAL	*P_closesocket)(SOCKET s);
	int (PASCAL	*P_connect)(SOCKET s, const struct sockaddr FAR *name, int namelen);
	struct hostent FAR*	(PASCAL *P_gethostbyname)(const char FAR* name);
	u_short (PASCAL	*P_htons)(u_short hostshort);
	unsigned long (PASCAL *P_inet_addr)(const char FAR * cp);
	int (PASCAL	*P_send)(SOCKET s, const char FAR * buf, int len, int flags);
	SOCKET (PASCAL *P_socket)(int af, int type, int protocol);

//	int	(PASCAL	*P_select)(int ndfs, fd_set FAR* readfs, fd_set FAR* writefs, fd_set FAR* exceptfs, const struct timeval FAR* timeout);

	int (PASCAL	*P_WSAStartup)(WORD wVersionRequired, LPWSADATA lpWSAData);
	int (PASCAL	*P_WSACleanup)(void);

	static HINSTANCE	cWSInstance;
	static DWORD		cInitCount;

	static	BOOL	Init();
	static	void	Close();

	static	void	NODLL();
};

//-----------------------------------------------------------------------
DWORD		PMWS11TracerManager::cInitCount = 0;
HINSTANCE	PMWS11TracerManager::cWSInstance = 0;
static		PMWS11TracerManager sWSManager;

//-----------------------------------------------------------------------
BOOL PMWS11TracerManager::Init()
{
	if (cInitCount == 0)
	{
		if ( (cWSInstance = ::LoadLibrary(WINSOCK_DLL)) == 0)
			// Unable to load library
			return FALSE;

		BOOL thefOK = TRUE;
		// If we reloaded the DLL we must reset the func pointer
		thefOK = thefOK && SOCKINIT(closesocket);
		thefOK = thefOK && SOCKINIT(connect);
		thefOK = thefOK && SOCKINIT(gethostbyname);
		thefOK = thefOK && SOCKINIT(htons);
		thefOK = thefOK && SOCKINIT(inet_addr);
		thefOK = thefOK && SOCKINIT(send);
		thefOK = thefOK && SOCKINIT(socket);

	//	thefOK = thefOK && SOCKINIT(select);

		thefOK = thefOK && SOCKINIT(WSAStartup);
		thefOK = thefOK && SOCKINIT(WSACleanup);

		if (!thefOK)
		{
			// One function failed to load
			::FreeLibrary(cWSInstance);
			return FALSE;
		}

		WSADATA theWSAData;

		int theResult = PM_WSAStartup(MAKEWORD(WS_VERSION, WS_REVISION), &theWSAData);
		if (theResult != 0)
		{
			// Error while initializing WINSOCK
			::FreeLibrary(cWSInstance);
			return FALSE;
		}

		if ((LOBYTE(theWSAData.wVersion) < WS_VERSION) || 
			(LOBYTE(theWSAData.wVersion) == WS_REVISION && HIBYTE(theWSAData.wVersion) < WS_REVISION))
		{
			// Error while initializing WINSOCK: Not compatible versions
			PM_WSACleanup();
			::FreeLibrary(cWSInstance);
			return FALSE;
		}
	}
    
	cInitCount++;
	return TRUE;
}

//-----------------------------------------------------------------------
void PMWS11TracerManager::NODLL()
{
	PM_ASSERT_NOW(TL("PMWS11TracerManager: Socket DLL (%s) not loaded or unloaded", WINSOCK_DLL));
}

//-----------------------------------------------------------------------
void PMWS11TracerManager::Close()
{
	PM_ASSERT(cInitCount != 0, TL("PMWS11TracerManager::Close(): Close more called than Init ???"));
	if (--cInitCount != 0)
		return;
	PM_ASSERT(cWSInstance != 0, TL("PMWS11TracerManager::Close(): Internal error"));

	PM_WSACleanup();
	::FreeLibrary(cWSInstance);
	cWSInstance = 0;

	// Reset the pointer to 0 to avoid pointing to a bad location
	SOCKCLOSE(closesocket);
	SOCKCLOSE(connect);
	SOCKCLOSE(gethostbyname);
	SOCKCLOSE(htons);
	SOCKCLOSE(inet_addr);
	SOCKCLOSE(send);
	SOCKCLOSE(socket);

//	SOCKCLOSE(select);

	SOCKCLOSE(WSAStartup);
	SOCKCLOSE(WSACleanup);
}

// ---------------------------------------------------------------------------
//	PMWin32TCPTracer
// ---------------------------------------------------------------------------
// Must be the same as in PMW32Hlp.cpp
static const char sSettingsSection[] = "ReportSettings";
#ifndef PMFinalRelease
	static const char sTracerRetryDelayKey[] = "TCPTracerRetryDelayD";
	static const char sTracerRetryMaxKey[] = "TCPTracerRetryMaxD";
#else
	static const char sTracerRetryDelayKey[] = "TCPTracerRetryDelay";
	static const char sTracerRetryMaxKey[] = "TCPTracerRetryMax";
#endif
#define PMTCPTRACER_DEFAULT_RETRY_DELAY	60000
#define PMTCPTRACER_DEFAULT_RETRY_MAX	3
extern HKEY	PMWin32_OpenSectionKey(HINSTANCE anInstance, const char* aSection);

// ---------------------------------------------------------------------------
PMWin32TCPTracer::PMWin32TCPTracer(const PMStrRef& anAddress, pmuint16 aPort) :
	PMXTracer()
{
	if (anAddress.IsNull() || anAddress->size() == 0)
		itsHost = PMSTRSTATIC("127.0.0.1");
	else
		itsHost = anAddress;
	
	itsPort = aPort;

	itsSocket = INVALID_SOCKET;
	itsfWinsockLoaded = itsfTryConnect = PMWS11TracerManager::Init();
	itsfFirstTrace = pmfalse;

#ifdef PMFinalRelease
	itsConnectRetryDelay = 0;
	itsMaxConnectRetryCount = 0;
#else
	itsConnectRetryDelay = PMTCPTRACER_DEFAULT_RETRY_DELAY;
	itsMaxConnectRetryCount = PMTCPTRACER_DEFAULT_RETRY_MAX;
#endif
	
	HKEY theSettingsKey = ::PMWin32_OpenSectionKey(PMOSWin32Helper::GetCurrent()->GetInstanceHandle(), sSettingsSection);
	if (theSettingsKey != 0)
	{
		DWORD theValue = 0, theType, theSize = 0;
		theSize = sizeof(DWORD);
		if (::RegQueryValueEx(theSettingsKey, sTracerRetryDelayKey, NULL, &theType, (LPBYTE)&theValue, &theSize) == ERROR_SUCCESS
			&& theType == REG_DWORD)
			itsConnectRetryDelay = theValue;
#ifndef PMFinalRelease
		else
			::RegSetValueEx(theSettingsKey, sTracerRetryDelayKey, 0, REG_DWORD, (LPBYTE)&itsConnectRetryDelay, sizeof(DWORD));
#endif // PMFinalRelease

		if (::RegQueryValueEx(theSettingsKey, sTracerRetryMaxKey, NULL, &theType, (LPBYTE)&theValue, &theSize) == ERROR_SUCCESS
			&& theType == REG_DWORD)
			itsMaxConnectRetryCount = (pmuint16)theValue;
#ifndef PMFinalRelease
		else
		{
			theValue = itsMaxConnectRetryCount;
			::RegSetValueEx(theSettingsKey, sTracerRetryMaxKey, 0, REG_DWORD, (LPBYTE)&theValue, sizeof(DWORD));
		}
#endif // PMFinalRelease

		::RegCloseKey(theSettingsKey);
	}
	if (itsConnectRetryDelay == 0)
		itsMaxConnectRetryCount = 0;
	itsConnectRetryCount = itsMaxConnectRetryCount;
}

// ---------------------------------------------------------------------------
PMWin32TCPTracer::~PMWin32TCPTracer()
{
	if (itsfWinsockLoaded)
	{
		if (itsSocket != INVALID_SOCKET)
		{
			PM_closesocket(itsSocket);
			itsSocket = INVALID_SOCKET;
		}
		PMWS11TracerManager::Close();
	}
}

// ---------------------------------------------------------------------------
pmuint16 PMWin32TCPTracer::GetMaxConnectRetryCount()
{
	return itsMaxConnectRetryCount;
}

// ---------------------------------------------------------------------------
pmuint16 PMWin32TCPTracer::GetConnectRetryCount()
{
	return itsConnectRetryCount;
}

// ---------------------------------------------------------------------------
// Just try to connect once and if it fails never more
pmbool PMWin32TCPTracer::CanSend()
{
	if (!itsfWinsockLoaded)
		return pmfalse;

	if (itsSocket == INVALID_SOCKET)
	{
		if (!itsfTryConnect)
		{
			if (itsConnectRetryCount == 0 || ::GetTickCount() - itsLastConnectTry < itsConnectRetryDelay)
				return pmfalse;
		}
		itsConnectRetryCount--;
		itsLastConnectTry = ::GetTickCount();
		itsfTryConnect = pmfalse;
		pmuint32 theAddr = PM_inet_addr(itsHost->c_str());
		if (theAddr == INADDR_NONE)
		{
			HOSTENT* theHE = PM_gethostbyname(itsHost->c_str());
			if (theHE == 0)
				return pmfalse;
			theAddr = *((pmuint32*)theHE->h_addr_list[0]);
		}
		itsSocket = PM_socket(AF_INET,SOCK_STREAM,0);
		if (itsSocket == INVALID_SOCKET)
			return pmfalse;

		sockaddr_in	theSin;
		size_t		theSinLength = sizeof(sockaddr_in);
		::pm_memset(&theSin,0,theSinLength);
		theSin.sin_family = AF_INET;
//		theSin.sin_addr.s_addr = PM_htonl(aPeerAddress);
		// Addr is already in network order because returned by a winsock function
		theSin.sin_addr.s_addr = theAddr;
		theSin.sin_port = PM_htons(itsPort);

		if (PM_connect(itsSocket,(LPSOCKADDR)&theSin,theSinLength) == SOCKET_ERROR)
		{
			PM_closesocket(itsSocket);
			itsSocket = INVALID_SOCKET;
			return pmfalse;
		}
		itsfFirstTrace = pmtrue;
		itsConnectRetryCount = itsMaxConnectRetryCount;
	}

	return pmtrue;
}

// ---------------------------------------------------------------------------
void PMWin32TCPTracer::SendChunk(PMDataRef aData)
{
	if (aData->size() == 0)
		return;
//	PM_send(itsSocket, (const char*)aData->buffer(), aData->size(), 0);
	if (PM_send(itsSocket, (const char*)aData->buffer(), aData->size(), 0) == SOCKET_ERROR)
	{
		PM_closesocket(itsSocket);
		itsSocket = INVALID_SOCKET;
	}
}

//-----------------------------------------------------------------------
#include "PMW32Hlp.h"
void PMWin32TCPTracer::DisplayAssert(const char* aTest, PMTraceSendParams* aParams)
{
	PMOSWin32Helper::GetCurrent()->DisplayAssert(aTest, aParams);

	// Skip PMXTrace::DisplayAssert
	PMTracer::DisplayAssert(aTest, aParams);
}

//-----------------------------------------------------------------------
// Message format:
//
// vvXXXXXXXXXXXXXX
// vv Version of trace protocol (2 bytes: Minor, Major)
//
// ================================================
//
// For version 1.0 (0001XX...XX)
// XX...XX = NNNNp0p1...pn
// NNNN : total size of the message (including vvNNNN) (4 bytes)
// pi :	Message parameters
//		Format:	cnndd..dd
//				c	: Parameter opcode (1 byte)
//				n	: nn parameter data len (excluding cnn)
//				d..d: parameter data
//
// Parameters valid opcodes:
//		1 : Attributes: xxxxt
//			xxxx = module<<16 + code - 32 bits
//			t = kind (0 = normal, 1 = EOT, 2 = binary) - 8 bits
//		2 : Indentation level: i (8 bits)
//		3 :	Process ID
//		4 : Thread ID
//		5 : Trace Data
//
//		0 : End of message. Every message must end with this parameter
//			As parameter data len is 0, the message MUST end with 3 0 bytes
//
//	Win32 parameters:
//		128 : App Name (sent only once with the first trace)
//
// User parameters must be >=128 (bit 7 set). This will prevent collision with
// future ObjectSet parameters.
//
// Take care that 16 bits and 32 bits values are transmitted in network byte order
// (big-endian) over the network.
//-----------------------------------------------------------------------
// Create data will not append the trailing null parameter.
// That will allow to subclass it, call the base class implementation,
// and then append more parameters. The final null parameter is added by 
// Send just before calling SendChunk.
// Tough, the total size of the message is already set to take into account
// the trailing 3 bytes that will be added so you will have to only add your
// parameter size to the message size.
extern PMStrRef PMWin32_GetModuleFileName(HINSTANCE anInstance);

PMDataRef PMWin32TCPTracer::CreateData(PMTraceSendParams* aParams)
{
	pmbyte		theByte;
	pmuint16	theU16;
	pmuint32	theU32;
	pmbyte*		thePtr;

	#define ADD_BYTE(b)	theByte = b; theData->Append(&theByte,1);
	#define ADD_U16(s)	theU16 = ::pm_host_to_networku16(s); theData->Append(&theU16, 2);
	#define ADD_U32(l)	theU32 = ::pm_host_to_networku32(l); theData->Append(&theU32, 4);

	#define SET_U32(i, l)	thePtr = ((pmbyte*)theData->buffer()) + i; ::pm_set_networku32(&thePtr, (pmuint32)l);

	PMDataRef	theData = PMXTracer::CreateData(aParams);
	DWORD		theID;

	// Thread ID and Process ID
	theID = ::GetCurrentProcessId();
	ADD_BYTE(PMXTracerOpcode_kProcessID); ADD_U16(4);
	ADD_U32(theID);
	theID = ::GetCurrentThreadId();
	ADD_BYTE(PMXTracerOpcode_kThreadID); ADD_U16(4);
	ADD_U32(theID);

	if (itsfFirstTrace)
	{
		PMStrRef theName = PMWin32_GetModuleFileName(PMOSWin32Helper::GetInstanceHandle());
		ADD_BYTE(PMXTracerOpcode_kAppName); ADD_U16(theName->size()+1);
		theData->Append(theName->c_str(), theName->size()+1);
		itsfFirstTrace = pmfalse;
	}

	size_t theSize = theData->size() + 3;
	SET_U32(2, theSize);

	return theData;
}

//-----------------------------------------------------------------------
#define GET_BYTE(ptr)	*ptr++
#define GET_U16(ptr)	::pm_get_networku16(&ptr)
#define GET_U32(ptr)	::pm_get_networku32(&ptr)

pmbool PMWin32TCPTracer::DecodeParameter(pmbyte anOpcode, const void* aPtr, size_t aLen, PMTraceSendParams* aParam, void* aUserData)
{
	if (PMXTracer::DecodeParameter(anOpcode, aPtr, aLen, aParam, aUserData))
		return pmtrue;

	const pmbyte* thePtr = (const pmbyte*)aPtr;
	switch (anOpcode)
	{
	case PMXTracerOpcode_kProcessID:
		if (aUserData != 0)
			((PMWin32TCPTracerInfo*)aUserData)->itsProcessID = GET_U32(thePtr);
		return pmtrue;

	case PMXTracerOpcode_kThreadID:
		if (aUserData != 0)
			((PMWin32TCPTracerInfo*)aUserData)->itsThreadID = GET_U32(thePtr);
		return pmtrue;

	case PMXTracerOpcode_kAppName:
		if (aUserData != 0)
			((PMWin32TCPTracerInfo*)aUserData)->itsAppName = (const char*)thePtr;
		return pmtrue;
	}

	return pmfalse;
}
