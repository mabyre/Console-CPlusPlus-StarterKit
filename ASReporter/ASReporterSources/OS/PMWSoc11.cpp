// ===========================================================================
//	PMWSoc11.cpp
//	Copyright (C) 1996 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMWSoc11.h"

#include "PMWinInc.h"
#include "PMTLS.h"
#include "PMPrintf.h"
#include <winsock.h>

#define PM_TRACEMODULE SOCKET
PM_DEFINE_TRACE_CODE(Info, 1); // -> PMTrace_kSOCKETInfoCode
PM_DEFINE_TRACE_CODE(Impl, 2); // -> PMTrace_kSOCKETImplCode

//-----------------------------------------------------------------------
// PMWinsock11 Header (Private) Use Factory to create
//-----------------------------------------------------------------------
class PMWinSock11 : public PMTCPSocketImplementation
{
public:
	PMWinSock11();
	virtual ~PMWinSock11();

	//	Class (static) method accessed via instance
	//	[for the ability to change implementations easily]
	virtual pmbool Resolve(const PMStrRef &aPeerName, pmuint32 *aPeerAddress);

	//	Do the connection
	virtual pmbool Connect(pmuint32 aPeerAddress, pmuint16 aDestinationPort);

	//	Close the connection
	//	If flag is true, the close should be as fast as possible (even if dirty)
	virtual pmbool Close(pmbool afAbort);

	//	Returns true if data is avalaible
	virtual pmbool IsDataAvalaible();

	//	Read method. Should block until data is avalaible if afBlock is pmtrue
	//	Returns (size_t)-1 if an error occured
	virtual size_t Read(char *p, size_t aMaxSize, pmbool afBlock);

	//	Write method. Will block until data are sent to the lower layer
	//	Will not wait for ACK from peer
	virtual pmbool Write(const char *p, size_t aLen);

		//	Write method. Will not block and returns the count of bytes written
		//	Should return (size_t)-1 if error occured
	virtual size_t TryWrite( const char *p, size_t aLen );

		//	Information about buffer size. DEFAULT_BUF_SIZE (4Kb) by default
	virtual size_t GetBufferSize();

		//	Get real state of the underlying socket
		//	Values are pre-filled 
	virtual void GetRealState( PMSocket_eState *aState, pmbool *afCanRead, pmbool *afCanWrite );

	virtual pmbool CancelBlockingCall();

		// For server part
	virtual pmbool BindAndListen(pmuint16 aPort);
	virtual PMTCPSocketImplementationRef WaitForConnection(pmuint32 aMsTimeout);
		
		// Must return 0 on failure
	virtual pmuint16 GetPort();

		// Must return 0 on failure
	virtual PMStrRef GetDotAddress(); // "162.38.250.12"

		// Returns the address of the remote server or 0
	virtual PMStrRef GetRemoteDotAddress();

protected:
	SOCKET	itsSocket;
	HWND	itsHwnd;
	DWORD	itsThreadID;
	BOOL	itsfInProgress;
	int		itsError;

	BOOL	InternalCreateWindow();
	void	PumpMessages(BOOL afMustWait = TRUE);
	BOOL	InternalDispatchMessage(const MSG* aMsg);

private:
	PMWinSock11(const PMWinSock11&);
	PMWinSock11& operator=(const PMWinSock11&);
};

//-----------------------------------------------------------------------
// WSock32.dll management
//-----------------------------------------------------------------------
// All this staff is done to load winsock dynamically.
// How does it work
// Instead of using accept for example use PM_accept
// #define PM_accept	sWSManager.P_accept
// Initially sWSManager.P_accept points to PMX_accept
// The first time PM_accept is called, in fact PMX_accept is called
// PMX_accept loads and initializes the DLL and changes sWSManager.P_accept
// to the real pointer of the function in the DLL
// So the next time PM_accecpt is called the DLL function is called directly
// And so on for every function (except the DLL is loaded once)
//-----------------------------------------------------------------------
#define WS_VERSION	1
#define WS_REVISION	1

// WINSOCK.DLL for 16 bits
#define WINSOCK_DLL	"WSOCK32.DLL"
// should be _T("WSOCK32.DLL")
#define WNDCLASSNAME	"PMWinSock11ClassWnd"

//-----------------------------------------------------------------------
static void _LoadProc(FARPROC* aProc, HINSTANCE anInstance, LPCSTR aProcName)
{
	PM_ASSERT(anInstance != 0, TL("PMWinSock11: Socket DLL (%s) not loaded or unloaded", WINSOCK_DLL));
	*aProc = ::GetProcAddress(anInstance, aProcName);
	if (*aProc == 0)
	{
		PM_TRACE( Info, TL("PMWinSock11: Unable to load function \"%s\" in %s\n", aProcName, WINSOCK_DLL));
		PM_ASSERT(*aProc != 0, TL("PMWinSock11: Function %s not found in \"%s\"", aProcName, WINSOCK_DLL));
	}
}

#define SOCKLOAD(x)		\
	::_LoadProc((FARPROC*)&sWSManager.P_##x, PMWinSock11Manager::cDLLInstance, #x)

#define SOCKINIT(x)		\
	sWSManager.P_##x = PMX_##x

#define SOCKCLOSE(x)	\
	*((void (**)())&sWSManager.P_##x) = PMWinSock11Manager::NODLL

//-----------------------------------------------------------------------
#define PM_accept			sWSManager.P_accept
#define PM_bind				sWSManager.P_bind
#define PM_closesocket		sWSManager.P_closesocket
#define PM_connect			sWSManager.P_connect
#define PM_ioctlsocket		sWSManager.P_ioctlsocket
#define PM_getpeername		sWSManager.P_getpeername
#define PM_getsockname		sWSManager.P_getsockname
#define PM_getsockopt		sWSManager.P_getsockopt
#define PM_htonl			sWSManager.P_htonl
#define PM_htons			sWSManager.P_htons
#define PM_inet_addr		sWSManager.P_inet_addr
#define PM_inet_ntoa		sWSManager.P_inet_ntoa
#define PM_listen			sWSManager.P_listen
#define PM_ntohl			sWSManager.P_ntohl
#define PM_ntohs			sWSManager.P_ntohs
#define PM_recv				sWSManager.P_recv
#define PM_send				sWSManager.P_send
#define PM_setsockopt		sWSManager.P_setsockopt
#define PM_shutdown			sWSManager.P_shutdown
#define PM_socket			sWSManager.P_socket

#define PM_gethostname		sWSManager.P_gethostname

#define PM_WSAStartup				sWSManager.P_WSAStartup
#define PM_WSACleanup				sWSManager.P_WSACleanup
#define PM_WSASetLastError			sWSManager.P_WSASetLastError
#define PM_WSAGetLastError			sWSManager.P_WSAGetLastError
#define PM_WSAAsyncGetHostByName	sWSManager.P_WSAAsyncGetHostByName
#define PM_WSAAsyncGetHostByAddr	sWSManager.P_WSAAsyncGetHostByAddr
#define PM_WSACancelAsyncRequest	sWSManager.P_WSACancelAsyncRequest
#define PM_WSAAsyncSelect			sWSManager.P_WSAAsyncSelect

//-----------------------------------------------------------------------
struct PMWinSock11Manager
{
	SOCKET (PASCAL	*P_accept)(SOCKET s, struct sockaddr FAR *addr, int FAR *addrlen);
	int (PASCAL		*P_bind)(SOCKET s, const struct sockaddr FAR *addr, int namelen);
	int (PASCAL		*P_closesocket)(SOCKET s);
	int (PASCAL		*P_connect)(SOCKET s, const struct sockaddr FAR *name, int namelen);
	int (PASCAL		*P_ioctlsocket)(SOCKET s, long cmd, u_long FAR *argp);
	int (PASCAL		*P_getpeername)(SOCKET s, struct sockaddr FAR *name, int FAR * namelen);
	int (PASCAL		*P_getsockname)(SOCKET s, struct sockaddr FAR *name, int FAR * namelen);
	int (PASCAL		*P_getsockopt)(SOCKET s, int level, int optname, char FAR * optval, int FAR *optlen);
	u_long (PASCAL	*P_htonl)(u_long hostlong);
	u_short (PASCAL	*P_htons)(u_short hostshort);
	unsigned long (PASCAL *P_inet_addr)(const char FAR * cp);
	char* (PASCAL	*P_inet_ntoa)(struct in_addr in);
	int (PASCAL		*P_listen)(SOCKET s, int backlog);
	u_long (PASCAL	*P_ntohl)(u_long netlong);
	u_short (PASCAL	*P_ntohs)(u_short netshort);
	int (PASCAL		*P_recv)(SOCKET s, char FAR * buf, int len, int flags);
	int (PASCAL		*P_send)(SOCKET s, const char FAR * buf, int len, int flags);
	int (PASCAL		*P_setsockopt)(SOCKET s, int level, int optname, const char FAR * optval, int optlen);
	int (PASCAL		*P_shutdown)(SOCKET s, int how);
	SOCKET (PASCAL	*P_socket)(int af, int type, int protocol);

	int (PASCAL		*P_gethostname)(char FAR *name, int namelen);

	int (PASCAL		*P_WSAStartup)(WORD wVersionRequired, LPWSADATA lpWSAData);
	int (PASCAL		*P_WSACleanup)(void);
	void (PASCAL	*P_WSASetLastError)(int iError);
	int (PASCAL		*P_WSAGetLastError)(void);
	HANDLE (PASCAL	*P_WSAAsyncGetHostByName)(HWND hWnd, u_int wMsg, const char FAR *name, char FAR *buf, int buflen);
	HANDLE (PASCAL	*P_WSAAsyncGetHostByAddr)(HWND hWnd, u_int wMsg, const char FAR *addr, int len, int type, char FAR *buf, int buflen);
	int (PASCAL		*P_WSACancelAsyncRequest)(HANDLE hAsyncTaskHandle);
	int (PASCAL		*P_WSAAsyncSelect)(SOCKET s, HWND hWnd, u_int wMsg, long lEvent);

	static HINSTANCE	cDLLInstance, cCurrentModule;
	static BOOL			cfInited;

	static	BOOL Init(HINSTANCE anInstance);
	static	void Close();

	static	void NODLL();
};

//-----------------------------------------------------------------------
SOCKET PASCAL	PMX_accept(SOCKET s, struct sockaddr FAR *addr, int FAR *addrlen);
int PASCAL		PMX_bind(SOCKET s, const struct sockaddr FAR *addr, int namelen);
int PASCAL		PMX_closesocket(SOCKET s);
int PASCAL		PMX_connect(SOCKET s, const struct sockaddr FAR *name, int namelen);
int PASCAL		PMX_ioctlsocket(SOCKET s, long cmd, u_long FAR *argp);
int PASCAL		PMX_getpeername(SOCKET s, struct sockaddr FAR *name, int FAR * namelen);
int PASCAL		PMX_getsockname(SOCKET s, struct sockaddr FAR *name, int FAR * namelen);
int PASCAL		PMX_getsockopt(SOCKET s, int level, int optname, char FAR * optval, int FAR *optlen);
u_long PASCAL	PMX_htonl(u_long hostlong);
u_short PASCAL	PMX_htons(u_short hostshort);
unsigned long PASCAL PMX_inet_addr(const char FAR * cp);
char* PASCAL	PMX_inet_ntoa(struct in_addr in);
int PASCAL		PMX_listen(SOCKET s, int backlog);
u_long PASCAL	PMX_ntohl(u_long netlong);
u_short PASCAL	PMX_ntohs(u_short netshort);
int PASCAL		PMX_recv(SOCKET s, char FAR * buf, int len, int flags);
int PASCAL		PMX_send(SOCKET s, const char FAR * buf, int len, int flags);
int PASCAL		PMX_setsockopt(SOCKET s, int level, int optname, const char FAR * optval, int optlen);
int PASCAL		PMX_shutdown(SOCKET s, int how);
SOCKET PASCAL	PMX_socket(int af, int type, int protocol);

int PASCAL		PMX_gethostname(char FAR *name, int namelen);

int PASCAL		PMX_WSAStartup(WORD wVersionRequired, LPWSADATA lpWSAData);
int PASCAL		PMX_WSACleanup(void);
void PASCAL		PMX_WSASetLastError(int iError);
int PASCAL		PMX_WSAGetLastError(void);
HANDLE PASCAL	PMX_WSAAsyncGetHostByName(HWND hWnd, u_int wMsg, const char FAR *name, char FAR *buf, int buflen);
HANDLE PASCAL	PMX_WSAAsyncGetHostByAddr(HWND hWnd, u_int wMsg, const char FAR *addr, int len, int type, char FAR *buf, int buflen);
int PASCAL		PMX_WSACancelAsyncRequest(HANDLE hAsyncTaskHandle);
int PASCAL		PMX_WSAAsyncSelect(SOCKET s, HWND hWnd, u_int wMsg, long lEvent);

//-----------------------------------------------------------------------
BOOL		PMWinSock11Manager::cfInited;
HINSTANCE	PMWinSock11Manager::cDLLInstance, PMWinSock11Manager::cCurrentModule;
static PMWinSock11Manager sWSManager;

//-----------------------------------------------------------------------
BOOL PMWinSock11Manager::Init(HINSTANCE anInstance)
{
//	SC_MUTEXLOCKER(theLocker, SCCore::GetMutex(SCCore::kSCTCPMutex));
	if (!cfInited)
	{
		cCurrentModule = anInstance;

		WNDCLASS theWC;
		if (!::GetClassInfo(cCurrentModule, WNDCLASSNAME, &theWC))
		{
			theWC.style = 0;
			theWC.lpfnWndProc = ::DefWindowProc;
			theWC.cbClsExtra = 0;
			theWC.cbWndExtra = 0;
			theWC.hInstance = cCurrentModule;
			theWC.hIcon = NULL;
			theWC.hCursor = NULL;
			theWC.hbrBackground = NULL;
			theWC.lpszMenuName = NULL;
			theWC.lpszClassName = WNDCLASSNAME;
        
			if (::RegisterClass(&theWC) == 0)
			{
				PM_TRACE(Impl, TL("PMWinSock11: Unable to register hidden window class"));
				return FALSE;
			}
		}

		if ( (cDLLInstance = ::LoadLibrary(WINSOCK_DLL)) == 0)
		{
			PM_TRACE(Impl, TL("PMWinSock11: Unable to load library: %s", WINSOCK_DLL));
			return FALSE;
		}
		// If we reloaded the DLL we must reset the func pointer
		SOCKINIT(accept);
		SOCKINIT(bind);
		SOCKINIT(closesocket);
		SOCKINIT(connect);
		SOCKINIT(ioctlsocket);
		SOCKINIT(getpeername);
		SOCKINIT(getsockname);
		SOCKINIT(getsockopt);
		SOCKINIT(htonl);
		SOCKINIT(htons);
		SOCKINIT(inet_addr);
		SOCKINIT(inet_ntoa);
		SOCKINIT(listen);
		SOCKINIT(ntohl);
		SOCKINIT(ntohs);
		SOCKINIT(recv);
		SOCKINIT(send);
		SOCKINIT(setsockopt);
		SOCKINIT(shutdown);
		SOCKINIT(socket);

		SOCKINIT(gethostname);

		SOCKINIT(WSAStartup);
		SOCKINIT(WSACleanup);
		SOCKINIT(WSASetLastError);
		SOCKINIT(WSAGetLastError);
		SOCKINIT(WSAAsyncGetHostByName);
		SOCKINIT(WSAAsyncGetHostByAddr);
		SOCKINIT(WSACancelAsyncRequest);
		SOCKINIT(WSAAsyncSelect);

		WSADATA theWSAData;

		int theResult = PM_WSAStartup(MAKEWORD(WS_VERSION, WS_REVISION), &theWSAData);
		if (theResult != 0)
		{
			PM_TRACE(Impl, TL("PMWinSock11: Error while initializing WINSOCK"));
			return FALSE;
		}

		if ((LOBYTE(theWSAData.wVersion) < WS_VERSION) || 
			(LOBYTE(theWSAData.wVersion) == WS_REVISION && HIBYTE(theWSAData.wVersion) < WS_REVISION))
		{
			PM_TRACE(Impl, TL("PMWinSock11: Error while initializing WINSOCK: Not compatible versions"));
			PM_WSACleanup();
			return FALSE;
		}
        
		cfInited = TRUE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------
void PMWinSock11Manager::NODLL()
{
	PM_ASSERT_NOW(TL("PMWinSock11Manager: Socket DLL (%s) not loaded or unloaded", WINSOCK_DLL));
}

//-----------------------------------------------------------------------
void PMWinSock11Manager::Close()
{
//	SC_MUTEXLOCKER(theLocker, SCCore::GetMutex(SCCore::kSCTCPMutex));

	if (cDLLInstance != 0)
	{
		PM_WSACleanup();
		::FreeLibrary(cDLLInstance);
		cDLLInstance = 0;
		// Win16 ports needs the next line
		::UnregisterClass(WNDCLASSNAME, cCurrentModule);

		// Reset the pointer to 0 to avoid pointing to a bad location
		SOCKCLOSE(accept);
		SOCKCLOSE(bind);
		SOCKCLOSE(closesocket);
		SOCKCLOSE(connect);
		SOCKCLOSE(ioctlsocket);
		SOCKCLOSE(getpeername);
		SOCKCLOSE(getsockname);
		SOCKCLOSE(getsockopt);
		SOCKCLOSE(htonl);
		SOCKCLOSE(htons);
		SOCKCLOSE(inet_addr);
		SOCKCLOSE(inet_ntoa);
		SOCKCLOSE(listen);
		SOCKCLOSE(ntohl);
		SOCKCLOSE(ntohs);
		SOCKCLOSE(recv);
		SOCKCLOSE(send);
		SOCKCLOSE(setsockopt);
		SOCKCLOSE(shutdown);
		SOCKCLOSE(socket);

		SOCKCLOSE(gethostname);

		SOCKCLOSE(WSAStartup);
		SOCKCLOSE(WSACleanup);
		SOCKCLOSE(WSASetLastError);
		SOCKCLOSE(WSAGetLastError);
		SOCKCLOSE(WSAAsyncGetHostByName);
		SOCKCLOSE(WSAAsyncGetHostByAddr);
		SOCKCLOSE(WSACancelAsyncRequest);
		SOCKCLOSE(WSAAsyncSelect);

	}

	cfInited = FALSE;
}

//-----------------------------------------------------------------------
SOCKET PASCAL PMX_accept(SOCKET s, struct sockaddr FAR *addr, int FAR *addrlen)
{
	SOCKLOAD(accept);
	return sWSManager.P_accept(s, addr, addrlen);
}

//-----------------------------------------------------------------------
int PASCAL PMX_bind(SOCKET s, const struct sockaddr FAR *addr, int namelen)
{
	SOCKLOAD(bind);
	return sWSManager.P_bind(s, addr, namelen);
}

//-----------------------------------------------------------------------
int PASCAL PMX_closesocket(SOCKET s)
{
	SOCKLOAD(closesocket);
	return sWSManager.P_closesocket(s);
}

//-----------------------------------------------------------------------
int PASCAL PMX_connect(SOCKET s, const struct sockaddr FAR *name, int namelen)
{
	SOCKLOAD(connect);
	return sWSManager.P_connect(s, name, namelen);
}

//-----------------------------------------------------------------------
int PASCAL PMX_ioctlsocket(SOCKET s, long cmd, u_long FAR *argp)
{
	SOCKLOAD(ioctlsocket);
	return sWSManager.P_ioctlsocket(s, cmd, argp);
}

//-----------------------------------------------------------------------
int PASCAL PMX_getpeername(SOCKET s, struct sockaddr FAR *name, int FAR * namelen)
{
	SOCKLOAD(getpeername);
	return sWSManager.P_getpeername(s, name, namelen);
}

//-----------------------------------------------------------------------
int PASCAL PMX_getsockname(SOCKET s, struct sockaddr FAR *name, int FAR * namelen)
{
	SOCKLOAD(getsockname);
	return sWSManager.P_getsockname(s, name, namelen);
}

//-----------------------------------------------------------------------
int PASCAL PMX_getsockopt(SOCKET s, int level, int optname, char FAR * optval, int FAR *optlen)
{
	SOCKLOAD(getsockopt);
	return sWSManager.P_getsockopt(s, level, optname, optval, optlen);
}

//-----------------------------------------------------------------------
u_long PASCAL PMX_htonl(u_long hostlong)
{
	SOCKLOAD(htonl);
	return sWSManager.P_htonl(hostlong);
}

//-----------------------------------------------------------------------
u_short PASCAL PMX_htons(u_short hostshort)
{
	SOCKLOAD(htons);
	return sWSManager.P_htons(hostshort);
}

//-----------------------------------------------------------------------
unsigned long PASCAL PMX_inet_addr(const char FAR * cp)
{
	SOCKLOAD(inet_addr);
	return sWSManager.P_inet_addr(cp);
}

//-----------------------------------------------------------------------
char* PASCAL PMX_inet_ntoa(struct in_addr in)
{
	SOCKLOAD(inet_ntoa);
	return sWSManager.P_inet_ntoa(in);
}

//-----------------------------------------------------------------------
int PASCAL PMX_listen(SOCKET s, int backlog)
{
	SOCKLOAD(listen);
	return sWSManager.P_listen(s, backlog);
}

//-----------------------------------------------------------------------
u_long PASCAL PMX_ntohl(u_long netlong)
{
	SOCKLOAD(ntohl);
	return sWSManager.P_ntohl(netlong);
}

//-----------------------------------------------------------------------
u_short PASCAL PMX_ntohs(u_short netshort)
{
	SOCKLOAD(ntohs);
	return sWSManager.P_ntohs(netshort);
}

//-----------------------------------------------------------------------
int PASCAL PMX_recv(SOCKET s, char FAR * buf, int len, int flags)
{
	SOCKLOAD(recv);
	return sWSManager.P_recv(s, buf, len, flags);
}

//-----------------------------------------------------------------------
int PASCAL PMX_send(SOCKET s, const char FAR * buf, int len, int flags)
{
	SOCKLOAD(send);
	return sWSManager.P_send(s, buf, len, flags);
}

//-----------------------------------------------------------------------
int PASCAL PMX_setsockopt(SOCKET s, int level, int optname, const char FAR * optval, int optlen)
{
	SOCKLOAD(setsockopt);
	return sWSManager.P_setsockopt(s, level, optname, optval, optlen);
}

//-----------------------------------------------------------------------
int PASCAL PMX_shutdown(SOCKET s, int how)
{
	SOCKLOAD(shutdown);
	return sWSManager.P_shutdown(s, how);
}

//-----------------------------------------------------------------------
SOCKET PASCAL PMX_socket(int af, int type, int protocol)
{
	SOCKLOAD(socket);
	return sWSManager.P_socket(af, type, protocol);
}


//-----------------------------------------------------------------------
int PASCAL PMX_gethostname(char FAR *name, int namelen)
{
	SOCKLOAD(gethostname);
	return sWSManager.P_gethostname(name, namelen);
}


//-----------------------------------------------------------------------
int PASCAL PMX_WSAStartup(WORD wVersionRequired, LPWSADATA lpWSAData)
{
	SOCKLOAD(WSAStartup);
	return sWSManager.P_WSAStartup(wVersionRequired, lpWSAData);
}

//-----------------------------------------------------------------------
int PASCAL PMX_WSACleanup(void)
{
	SOCKLOAD(WSACleanup);
	return sWSManager.P_WSACleanup();
}

//-----------------------------------------------------------------------
void PASCAL PMX_WSASetLastError(int iError)
{
	SOCKLOAD(WSASetLastError);
	sWSManager.P_WSASetLastError(iError);
}

//-----------------------------------------------------------------------
int PASCAL PMX_WSAGetLastError(void)
{
	SOCKLOAD(WSAGetLastError);
	return sWSManager.P_WSAGetLastError();
}

//-----------------------------------------------------------------------
HANDLE PASCAL PMX_WSAAsyncGetHostByName(HWND hWnd, u_int wMsg, const char FAR *name, char FAR *buf, int buflen)
{
	SOCKLOAD(WSAAsyncGetHostByName);
	return sWSManager.P_WSAAsyncGetHostByName(hWnd, wMsg, name, buf, buflen);
}

//-----------------------------------------------------------------------
HANDLE PASCAL PMX_WSAAsyncGetHostByAddr(HWND hWnd, u_int wMsg, const char FAR *addr, int len, int type, char FAR *buf, int buflen)
{
	SOCKLOAD(WSAAsyncGetHostByAddr);
	return sWSManager.P_WSAAsyncGetHostByAddr(hWnd, wMsg, addr, len, type, buf, buflen);
}

//-----------------------------------------------------------------------
int PASCAL PMX_WSACancelAsyncRequest(HANDLE hAsyncTaskHandle)
{
	SOCKLOAD(WSACancelAsyncRequest);
	return sWSManager.P_WSACancelAsyncRequest(hAsyncTaskHandle);
}

//-----------------------------------------------------------------------
int PASCAL PMX_WSAAsyncSelect(SOCKET s, HWND hWnd, u_int wMsg, long lEvent)
{
	SOCKLOAD(WSAAsyncSelect);
	return sWSManager.P_WSAAsyncSelect(s, hWnd, wMsg, lEvent);
}

//-----------------------------------------------------------------------
// PMWinsock11 Implementation
//-----------------------------------------------------------------------
#define WM_PMSOCKET_NOTIFY	(WM_USER+1)
#define WM_PMSOCKET_ABORT	(WM_USER+2)

#define INTERNAL_ABORT_ERROR	1000

//-----------------------------------------------------------------------
PMWinSock11::PMWinSock11() : PMTCPSocketImplementation()
{
	itsHwnd = 0;
	itsThreadID = 0;
	itsSocket = INVALID_SOCKET;
	itsfInProgress = FALSE;
	itsError = 0;
}

//-----------------------------------------------------------------------
PMWinSock11::~PMWinSock11()
{
	if (itsSocket != INVALID_SOCKET)
	{
		PM_TRACE(Info, TL("PMWinSock11::~PMWinSock11(): Socket not closed: Aborting it."));
		Close(pmtrue);
	}

	if (itsHwnd != 0)
		::DestroyWindow(itsHwnd);
}

//-----------------------------------------------------------------------
//	Class (static) method accessed via instance
//	[for the ability to change implementations easily]
pmbool PMWinSock11::Resolve(const PMStrRef &aPeerName, pmuint32 *aPeerAddress)
{
	PM_ASSERT(!aPeerName.IsNull(), TL("PMWinSock11::Resolve(): Null Name"));

	if (!InternalCreateWindow())
		return pmfalse;

	(*aPeerAddress) = PM_ntohl(PM_inet_addr(aPeerName->c_str()));
	if (*aPeerAddress != INADDR_NONE)
		return pmtrue;

	char	theBuffer[MAXGETHOSTSTRUCT];
	HANDLE	theHandle;

	itsError = 0;
	PumpMessages(FALSE); // Empty message queue
	theHandle = PM_WSAAsyncGetHostByName(itsHwnd,WM_PMSOCKET_NOTIFY,aPeerName->c_str(),(char*)theBuffer,MAXGETHOSTSTRUCT);
	if (theHandle == 0)
		return pmfalse;

	itsfInProgress = TRUE;
	PumpMessages(TRUE);
	if (itsError == INTERNAL_ABORT_ERROR)
		::PM_WSACancelAsyncRequest(theHandle);
	itsfInProgress = FALSE;
	if (itsError != 0)
		return pmfalse;

	HOSTENT* theHost = (HOSTENT*)theBuffer;
	(*aPeerAddress) = PM_ntohl(*((pmuint32*)theHost->h_addr_list[0]));

	return pmtrue;
}

//-----------------------------------------------------------------------
//	Do the connection
pmbool PMWinSock11::Connect(pmuint32 aPeerAddress, pmuint16 aDestinationPort)
{
	if (!InternalCreateWindow())
		return pmfalse;

	if (itsSocket == INVALID_SOCKET)
	{
		itsSocket = PM_socket(AF_INET,SOCK_STREAM,0);
		if (itsSocket == INVALID_SOCKET)
			return pmfalse;

		u_long theBlocking = 1;
		if (PM_ioctlsocket(itsSocket,FIONBIO,&theBlocking) == SOCKET_ERROR)
		{
			PM_closesocket(itsSocket);
			return pmfalse;
		}
	}

	sockaddr_in	theSin;
	size_t		theSinLength = sizeof(sockaddr_in);

	::pm_memset(&theSin,0,theSinLength);
	theSin.sin_family = AF_INET;
	theSin.sin_addr.s_addr = PM_htonl(aPeerAddress);
	theSin.sin_port = PM_htons(aDestinationPort);

	itsError = 0;
	PumpMessages(FALSE); // Empty message queue
	if (PM_WSAAsyncSelect(itsSocket, itsHwnd, WM_PMSOCKET_NOTIFY, FD_CONNECT | FD_CLOSE) != 0)
		return pmfalse;

	if (PM_connect(itsSocket,(LPSOCKADDR)&theSin,theSinLength) == SOCKET_ERROR)
	{
		int theWSAError = PM_WSAGetLastError();
		if (theWSAError != WSAEWOULDBLOCK)
		{
			PM_WSAAsyncSelect(itsSocket, itsHwnd, 0, 0); // Added
			itsfInProgress = FALSE; // Added
			PM_closesocket(itsSocket);
			return pmfalse;
		}
		else
		{
			itsfInProgress = TRUE;
			PumpMessages(TRUE);
		}
	}

	PM_WSAAsyncSelect(itsSocket, itsHwnd, 0, 0);
	itsfInProgress = FALSE;
	if (itsError == INTERNAL_ABORT_ERROR)
		Close(pmtrue);
	if (itsError != 0)
		return pmfalse;

	return pmtrue;
}

//-----------------------------------------------------------------------
//	Close the connection
//	If flag is true, the close should be as fast as possible (even if dirty)
pmbool PMWinSock11::Close(pmbool afAbort)
{
/*
	if (itsSocket == INVALID_SOCKET)
	{
		// A gethostbyname may be in progress without any socket
		if (itsfInProgress)
			::PostMessage(itsHwnd, WM_PMSOCKET_ABORT, 0, 0L);

		return pmtrue;
	}
*/
	// #### Late modif because of a blocking in ImmediateClose
	if (itsfInProgress && itsHwnd != 0)
		::PostMessage(itsHwnd, WM_PMSOCKET_ABORT, 0, 0L);
	// A gethostbyname may be in progress without any socket
	if (itsSocket == INVALID_SOCKET)
		return pmtrue;

	LINGER	theLinger;
	int		theError;

	if (!afAbort)
	{
		// Close gracefully
		theLinger.l_onoff = 0; // SO_DONTLINGER
		theLinger.l_linger = 0;
		PM_setsockopt(itsSocket, SOL_SOCKET, SO_LINGER, (char*)&theLinger, sizeof(LINGER));

		if ( (theError = PM_closesocket(itsSocket)) != 0)
		{
			PM_TRACE(Impl, TL("PMWinSock11::Close(): Error while closing socket. Trying to abort it"));
			Close(pmtrue);
		}
	}
	else
	{
		theLinger.l_onoff = 1;
		theLinger.l_linger = 0;
		PM_setsockopt(itsSocket, SOL_SOCKET, SO_LINGER, (char*)&theLinger, sizeof(LINGER));
				
		if ( (theError = PM_closesocket(itsSocket)) != 0)
		{
			PM_TRACE(Impl, TL("PMWinSock11::Close(): Error while hard-closing socket"));
		}
	}

	itsSocket = INVALID_SOCKET;

	return (theError == 0);
}

//-----------------------------------------------------------------------
//	Returns true if data is avalaible
pmbool PMWinSock11::IsDataAvalaible()
{
	if (itsSocket == INVALID_SOCKET)
		return pmfalse;

	u_long theReadable = 0;
	if (PM_ioctlsocket(itsSocket, FIONREAD, &theReadable) == SOCKET_ERROR)
		return pmfalse;

	return (theReadable != 0);
}

//-----------------------------------------------------------------------
//	Read method. Should block until data is avalaible if afBlock is pmtrue
//	Returns (size_t)-1 if an error occured
size_t PMWinSock11::Read(char *p, size_t aMaxSize, pmbool afBlock)
{
	if (!InternalCreateWindow())
		return (size_t)-1;

	if (itsSocket == INVALID_SOCKET)
		return (size_t)-1;

	int theReceived = 0;
	{	//	Patch for close event
		char theBuffer;

		if ( (theReceived = PM_recv(itsSocket,&theBuffer,1,MSG_PEEK)) == 0)
			return 0;
	}

	if (theReceived == SOCKET_ERROR)
	{
		if (PM_WSAGetLastError() == WSAEWOULDBLOCK)
		{
			itsError = 0;
			PumpMessages(FALSE); // Empty message queue
			if (PM_WSAAsyncSelect(itsSocket, itsHwnd, WM_PMSOCKET_NOTIFY, FD_READ | FD_CLOSE) != 0)
				return pmfalse;
			itsfInProgress = TRUE;
			PumpMessages(TRUE);
			PM_WSAAsyncSelect(itsSocket, itsHwnd, 0, 0);
			itsfInProgress = FALSE;
			if (itsError == INTERNAL_ABORT_ERROR)
				Close(pmtrue);

			if (itsError != 0)
				return (size_t)-1;
		}
		else
			return (size_t)-1;
	}

	theReceived = PM_recv(itsSocket, p, aMaxSize, 0);
	if (theReceived == SOCKET_ERROR)
		return (size_t)-1;

	return theReceived;
}

//-----------------------------------------------------------------------
//	Write method. Will block until data are sent to the lower layer
//	Will not wait for ACK from peer
pmbool PMWinSock11::Write(const char *p, size_t aLen)
{
	if (!InternalCreateWindow())
		return pmfalse;

	if (itsSocket == INVALID_SOCKET)
		return pmfalse;

	PM_TRACE(Impl, TL("PMWinSock11::Write(): Sending %lu bytes", (unsigned long)aLen));

	const char*	thePtr = p;
	int			theLen = aLen, theSent;

	PumpMessages(FALSE); // Empty message queue
	if (PM_WSAAsyncSelect(itsSocket, itsHwnd, WM_PMSOCKET_NOTIFY, FD_WRITE | FD_CLOSE) != 0)
		return pmfalse;
	itsfInProgress = TRUE;
	while (theLen > 0)
	{
		itsError = 0;
		PumpMessages(TRUE);
		if (itsError == INTERNAL_ABORT_ERROR)
			Close(pmtrue);
		if (itsError != 0)
			return pmfalse;

		while (theLen > 0)
		{
			theSent = PM_send(itsSocket, thePtr, theLen, 0);
			if (theSent == SOCKET_ERROR)
			{
				itsError = PM_WSAGetLastError();
				break;
			}

			thePtr += theSent;
			theLen -= theSent;
		}

		if (itsError == WSAEWOULDBLOCK)
		{
			PM_TRACE(Impl, TL("PMWinSock11::Write(): Internal buffers full. Waiting."));
			itsError = 0;
		}
		else
			break;
	}
	PM_WSAAsyncSelect(itsSocket, itsHwnd, 0, 0);
	itsfInProgress = FALSE;

	return pmtrue;
}

//-----------------------------------------------------------------------
//	Write method. Will not block and returns the count of bytes written
//	Should return (size_t)-1 if error occured
size_t PMWinSock11::TryWrite( const char *p, size_t aLen )
{
	return 0;
}

//-----------------------------------------------------------------------
//	Information about buffer size. DEFAULT_BUF_SIZE (4Kb) by default
size_t PMWinSock11::GetBufferSize()
{
	return 32768;
}

//-----------------------------------------------------------------------
//	Get real state of the underlying socket
//	Values are pre-filled 
void PMWinSock11::GetRealState( PMSocket_eState *aState, pmbool *afCanRead, pmbool *afCanWrite )
{
	if (itsSocket == INVALID_SOCKET)
	{
		*aState = PMSocket_kNotConnected;
		*afCanRead = pmfalse;
		*afCanWrite = pmfalse;
		return;
	}
	if (itsfInProgress)
		return;
	
	char	theBuffer;
	int		theReceived;

	if ( (theReceived = PM_recv(itsSocket,&theBuffer,1,MSG_PEEK)) == SOCKET_ERROR)
	{
		switch (PM_WSAGetLastError())
		{
			case WSAEWOULDBLOCK:
				*afCanRead = pmtrue;
//				*afCanWrite = pmtrue;
//				theResult = kSCPeerConnected;
				break;

//			case WSAECONNRESET:
//				theResult = kSCPeerClosed;
//				break;
				
			case WSAESHUTDOWN:
				*afCanRead = pmfalse;
				break;

			case WSAECONNRESET:
			case WSAECONNABORTED:
				*afCanRead = pmfalse;
				*afCanWrite = pmfalse;
//				theResult = kSCPeerAborted;
				break;
				
			default:
				*afCanRead = pmfalse;
				*afCanWrite = pmfalse;
				*aState = PMSocket_kError;
//				theResult = kSCPeerError;
				break;
		}
	}
	else
		if (theReceived == 0)
			*afCanRead = pmfalse;
//			theResult = kSCPeerClosed;
		else
			*afCanRead = pmtrue;
//			theResult = kSCPeerConnected;

//	return theResult;
}

//-----------------------------------------------------------------------
pmbool PMWinSock11::CancelBlockingCall()
{
/*
	if (!itsfInProgress || itsHwnd == 0)
		return pmtrue;

	::PostMessage(itsHwnd, WM_PMSOCKET_ABORT, 0, 0L);
	return pmtrue;
*/
	::PostMessage(itsHwnd, WM_PMSOCKET_ABORT, 0, 0L);

	return PMTCPSocketImplementation::CancelBlockingCall();
}

//-----------------------------------------------------------------------
pmbool PMWinSock11::BindAndListen(pmuint16 aPort)
{
//	Not needed because no async operation involved
//	if (!InternalCreateWindow())
//		return pmfalse;

	PM_ASSERT(PMWinSock11Manager::cfInited, TL("Trying to use socket while not initialized."));
//	if (!PMWinSock11Manager::Init())
//		return pmfalse;

	if (itsSocket == INVALID_SOCKET)
	{
		itsSocket = PM_socket(AF_INET,SOCK_STREAM,0);
		if (itsSocket == INVALID_SOCKET)
			return pmfalse;

		u_long theBlocking = 1;
		if (PM_ioctlsocket(itsSocket,FIONBIO,&theBlocking) == SOCKET_ERROR)
		{
			PM_closesocket(itsSocket);
			return pmfalse;
		}
	}

	sockaddr_in	theSin;
	size_t		theSinLen = sizeof(sockaddr_in);

	::pm_memset(&theSin, 0, theSinLen);
	theSin.sin_family = AF_INET;
	theSin.sin_addr.s_addr = 0;
	theSin.sin_port = PM_htons(aPort);

	if (PM_bind(itsSocket, (LPSOCKADDR)&theSin, theSinLen) == SOCKET_ERROR)
	{
		itsError = PM_WSAGetLastError();
		PM_TRACE(Impl, TL("PMWinSock11::BindAndListen(): Unable to bind on port %lu: Error: %ld", (unsigned long)aPort, (long)itsError));
		return pmfalse;
	}

	#define kMaxListen 8 // system limitation
        
	if (PM_listen(itsSocket, kMaxListen) == SOCKET_ERROR)
	{
		itsError = PM_WSAGetLastError();
		PM_TRACE(Impl, TL("PMWinSock11::BindAndListen(): Unable to listen: Error: %ld", (long)itsError));
		return pmfalse;
	}

	#undef kMaxListen

	return pmtrue;
}

//-----------------------------------------------------------------------
PMTCPSocketImplementationRef PMWinSock11::WaitForConnection(pmuint32 aMsTimeout)
{
	if (!InternalCreateWindow())
		return 0;

	if (itsSocket == INVALID_SOCKET)
		return 0;

	itsError = 0;
	PumpMessages(FALSE); // Empty message queue
	if (PM_WSAAsyncSelect(itsSocket, itsHwnd, WM_PMSOCKET_NOTIFY, FD_ACCEPT | FD_CLOSE) != 0)
		return 0;

	itsfInProgress = TRUE;
	PumpMessages(TRUE);
	PM_WSAAsyncSelect(itsSocket, itsHwnd, 0, 0);
	itsfInProgress = FALSE;
	if (itsError == INTERNAL_ABORT_ERROR)
		Close(pmtrue);

	if (itsError != 0)
		return 0;

#ifndef NDEBUG
	struct sockaddr_in 	theSin;
	size_t				theSinLen = sizeof(sockaddr_in);

	::pm_memset(&theSin, 0, theSinLen);
	SOCKET theSocket = PM_accept(itsSocket, (LPSOCKADDR)(&theSin),(int*)&theSinLen);
	if (theSocket != INVALID_SOCKET)
	{
		#define NetByte(x) (unsigned long)theSin.sin_addr.S_un.S_un_b.s_b##x
		PM_TRACE(Info, TL("PMWinSock11::WaitForConnection(): Accepting connection from: %lu.%lu.%lu.%lu port %lu", NetByte(1), NetByte(2), NetByte(3), NetByte(4), (unsigned long)PM_ntohs(theSin.sin_port)));
		#undef NetByte
	}
#else
	SOCKET theSocket = PM_accept(itsSocket, 0, 0);
#endif

	if (theSocket == INVALID_SOCKET)
		return 0;

	PMRef<PMWinSock11> theRef = new PMWinSock11;
	theRef.Get()->itsSocket = theSocket;

	return theRef.Get();
}

//-----------------------------------------------------------------------
pmuint16 PMWinSock11::GetPort()
{
	if (!PMWinSock11Manager::cfInited || itsSocket == INVALID_SOCKET)
		return 0;

	struct sockaddr_in 	theSin;
	int					theSinLen = sizeof(sockaddr_in);

	if (PM_getsockname(itsSocket, (LPSOCKADDR)&theSin, &theSinLen) == SOCKET_ERROR)
		return 0;

	return PM_ntohs(theSin.sin_port);
}

//-----------------------------------------------------------------------
PMStrRef PMWinSock11::GetDotAddress()
{
	if (!PMWinSock11Manager::cfInited || itsSocket == INVALID_SOCKET)
		return 0;

	struct sockaddr_in 	theSin;
	int					theSinLen = sizeof(sockaddr_in);

	if (PM_getsockname(itsSocket, (LPSOCKADDR)&theSin, &theSinLen) == SOCKET_ERROR)
		return 0;

	#define NetByte(x) (unsigned long)theSin.sin_addr.S_un.S_un_b.s_b##x
	char* theString = (char*)::pm_xsprintf("%lu.%lu.%lu.%lu", NetByte(1), NetByte(2), NetByte(3), NetByte(4))->TakeOwnership();
	#undef NetByte

	return PMStr::NewFromCString(theString, PMStr_kDelete, PMStr_kUnknown);
}

// ---------------------------------------------------------------------------

PMStrRef PMWinSock11::GetRemoteDotAddress()
{	
	if (!PMWinSock11Manager::cfInited || itsSocket == INVALID_SOCKET)
		return 0;

	struct sockaddr_in 	theSin;
	int					theSinLen = sizeof(sockaddr_in);

	if (PM_getpeername(itsSocket, (LPSOCKADDR)&theSin, &theSinLen) == SOCKET_ERROR)
		return 0;

	#define NetByte(x) (unsigned long)theSin.sin_addr.S_un.S_un_b.s_b##x
	char* theString = (char*)::pm_xsprintf("%lu.%lu.%lu.%lu", NetByte(1), NetByte(2), NetByte(3), NetByte(4))->TakeOwnership();
	#undef NetByte

	return PMStr::NewFromCString(theString, PMStr_kDelete, PMStr_kUnknown);
}

//-----------------------------------------------------------------------
// PMWinsock11 Internal implementation
//-----------------------------------------------------------------------
BOOL PMWinSock11::InternalCreateWindow()
{
	PM_ASSERT(PMWinSock11Manager::cfInited, TL("Trying to use socket while not initialized."));
//	if (!PMWinSock11Manager::Init())
//		return FALSE;

	// Each thread has a unique ID stored in its TLS. 
	// (We cannot use system id because they are recycled)
	// Each socket maintain the ID of the thread owning the control window in itsThreadID
	// If not equal you must change the window
	
//	pmuint32 theThreadID = PMTLS::Get()->itsUniqueID;
	pmuint32 theThreadID = PMTLS::GetUniqueID();
	// If the current ThreadID is not the same as the thread owner ID
	if (theThreadID != itsThreadID)
	{
		if (itsHwnd != 0)
		{
			PM_TRACE(Info, TL("PMWinSock11::InternalCreateWindow(): Socket used from a different thread than the previous call. Replacing internal structures"));
			if (itsfInProgress)
				PM_TRACE(Info, TL("PMWinSock11::InternalCreateWindow(): A call was in progress and it will be aborted"));

			::PostMessage(itsHwnd, WM_PMSOCKET_ABORT, 0, 0L); // Really useful ?
			::DestroyWindow(itsHwnd);
			itsHwnd = 0;
			itsThreadID = 0;
		}
	}

	if (itsHwnd == 0)
	{
		itsHwnd = ::CreateWindow(WNDCLASSNAME,"",0,0,0,0,0,NULL,NULL,PMWinSock11Manager::cCurrentModule,NULL);
		itsThreadID = theThreadID;
		PumpMessages(FALSE); // Empty message queue (WM_CREATE, ...)
	}

	return (itsHwnd != 0);
}

//-----------------------------------------------------------------------
BOOL PMWinSock11::InternalDispatchMessage(const MSG* aMsg)
{
	BOOL thefDispatched = FALSE;

	switch (aMsg->message)
	{
		case WM_PMSOCKET_NOTIFY:
//			PM_TRACE(Impl, TL("PMWinSock11::InternalDispatchMessage(): Notified"));
			PM_ASSERT(itsSocket == SOCKET_ERROR || aMsg->wParam == itsSocket, TL("PMWinSock11::InternalDispatchMessage(): Internal Error."));
			if (WSAGETSELECTEVENT(aMsg->lParam) == FD_CLOSE)
				itsError = WSAENOTCONN; // ++++
			else
				itsError = WSAGETSELECTERROR(aMsg->lParam);
			if (itsError != 0)
				PM_TRACE(Impl, TL("PMWinSock11::InternalDispatchMessage(): Notified with error: %ld", (long)itsError));
			thefDispatched = TRUE;
			break;

		case WM_PMSOCKET_ABORT:
			PM_TRACE(Impl, TL("PMWinSock11::InternalDispatchMessage(): Aborted"));
			itsError = INTERNAL_ABORT_ERROR;
			thefDispatched = TRUE;
			break;

		default:
			::DispatchMessage(aMsg);
			break;
	}

	return thefDispatched;
}

//-----------------------------------------------------------------------
void PMWinSock11::PumpMessages(BOOL afMustWait)
{
	PM_ASSERT(itsHwnd != 0, TL("PMWinSock11::PumpMessages(): InternalError"));

	MSG	theMsg;

	if (afMustWait)
	{
		do
		{
			PM_TRACE(Impl, TL("PMWinSock11::PumpMessages(): Waiting..."));
			if (!::PeekMessage(&theMsg, itsHwnd, WM_PMSOCKET_ABORT, WM_PMSOCKET_ABORT, PM_REMOVE))
				::GetMessage(&theMsg, itsHwnd, 0, 0);
		} while (!InternalDispatchMessage(&theMsg));
	}
	else
	{
//		PM_TRACE(Impl, TL("PMWinSock11::PumpMessages(): Removing messages..."));
		while (::PeekMessage(&theMsg, itsHwnd, 0, 0, PM_REMOVE) && !InternalDispatchMessage(&theMsg))
			;
	}
}

//-----------------------------------------------------------------------
// PMWinSock11 Factory Implementation
//-----------------------------------------------------------------------

PMTCPSocketRef	PMWinSock11Factory::NewSocketConnectedTo(const PMStrRef &aHost, pmuint16 aPort)
{
	return new PMTCPSocket(new PMWinSock11, aHost, aPort);
}

//-----------------------------------------------------------------------
PMTCPSocketRef	PMWinSock11Factory::NewSocketConnectedTo(pmuint32 aPeerAddress, pmuint16 aPort)
{
	return new PMTCPSocket(new PMWinSock11, aPeerAddress, aPort);
}

//-----------------------------------------------------------------------
pmbool PMWinSock11Factory::Init(void* aParam)
{	
	if (!PMWinSock11Manager::Init((HINSTANCE)aParam))
		return pmfalse;

	return pmtrue;
}

//-----------------------------------------------------------------------
void PMWinSock11Factory::Close(void*)
{
	PMWinSock11Manager::Close();
}

//-----------------------------------------------------------------------
PMStrRef PMWinSock11Factory::GetHostName()
{	
	char theName[128];

	if (PM_gethostname(theName, sizeof(theName)) == 0)
		return (PMStrRef)theName;

	return 0;
}

//-----------------------------------------------------------------------
PMTCPSocketAcceptorRef PMWinSock11Factory::NewAcceptorFor(pmuint16 aPort)
{	
	PMRef<PMWinSock11> theImplementation = new PMWinSock11;
	if (!theImplementation->BindAndListen(aPort))
		return 0;

	return new PMTCPSocketAcceptor(theImplementation.Get());
}
