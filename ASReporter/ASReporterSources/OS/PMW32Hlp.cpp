// ===========================================================================
//	PMW32Hlp.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMW32Hlp.h"

#include "PMWinInc.h"
#include <winreg.h>
#include <stdio.h> // sprintf
#include "PMTrace.h"
#include "PMMedia.h"
#include "PMTCPSoc.h"
#include "PMHTbl.h"
#include "PMPrintf.h"
#include "PMCriSec.h"

#include "PMWErr.h"

// ---------------------------------------------------------------------------
#define PM_TRACEMODULE STRUCT
PM_DEFINE_TRACE_CODE(Info, 1);

// ---------------------------------------------------------------------------
static pmbool cfPMInited, cfTCPInited;
HINSTANCE PMOSWin32Helper::chInstance, PMOSWin32Helper::chRCInstance;

PMMediaRef NewWin32TemporaryFileMedia();

// ---------------------------------------------------------------------------

PMOSWin32Helper::PMOSWin32Helper()
{
	itsfUseLongFileNames = pmfalse;
}

// ---------------------------------------------------------------------------

PMOSWin32HelperRef PMOSWin32Helper::GetCurrent()
{
	// This is not used any more because it was called for each trace in debug mode.
/*
#ifndef PMFinalRelease
	#if defined(_MSC_VER) && _MSC_VER >= 1000 && defined(_CPPRTTI)
		PMOSHelper* theHelper = PMOSHelper::GetCurrent().Get();
		pmbool		thefOK = pmfalse;
		try
		{
			thefOK = (::pm_strcmp(typeid(*theHelper).name(), "class PMOSWin32Helper") == 0);
		}
		catch(...)
		{
		}
		if (!thefOK)
			PM_ASSERT_NOW(TL("PMOSWin32Helper::GetCurrent(): The current OSHelper is not of class PMOSWin32Helper"));
	#endif
#endif
*/
	return (PMOSWin32Helper*)PMOSHelper::GetCurrent().Get();
}

// ---------------------------------------------------------------------------

HINSTANCE PMOSWin32Helper::GetInstanceHandle()
{
	return chInstance;
}

// ---------------------------------------------------------------------------

pmbool PMOSWin32Helper::UseLongFileNames(pmbool afUseLongFileNames)
{
	pmbool thefOld = itsfUseLongFileNames;
	itsfUseLongFileNames = afUseLongFileNames;

	return thefOld;
}

// ---------------------------------------------------------------------------

PMStrRef PMOSWin32Helper::ValidateFileName(const PMStrRef& aFileName)
{
	PMStrRef theFileName;

	if (itsfUseLongFileNames)
		theFileName = PMFileNameHelper::ToWin32FileName(aFileName);
	else
		theFileName = PMFileNameHelper::ToDOSFileName(aFileName);

	return theFileName;
}

// ---------------------------------------------------------------------------

PMStrRef PMOSWin32Helper::CreateRandomFileName()
{
	static DWORD sUnknownFileNameId = 0;

	// The name is created using the process id (%100) and an index incremented
	// each time the function is called.
	// If using long file names 100,000, 10,000 else.
	char theName[_MAX_PATH + 1];

	if (itsfUseLongFileNames)
		::sprintf(theName, "Unknown%03d%05d", (long)(::GetCurrentProcessId() % 1000), (long)(sUnknownFileNameId % 100000));
	else
		::sprintf(theName, "Un%02d%04d", (long)(::GetCurrentProcessId() % 100), (long)(sUnknownFileNameId % 10000));

	sUnknownFileNameId++;

	return ValidateFileName(theName);
}

// ---------------------------------------------------------------------------

void PMOSWin32Helper::SetTemporaryMediaType(PMMediaTemporary_eType aType)
{
	PM_ASSERT(aType == PMMediaTemporary_kMemory || aType == PMMediaTemporary_kFile, TL("PMOSHelper::SetTemporaryMediaType(): Invalid argument"));
	
	if (aType == PMMediaTemporary_kMemory)
		PMMedia::gTemporaryMediaFactory = PMMedia::NewTemporaryMemoryMedia;
	else
		// PMMediaTemporary_kFile
		PMMedia::gTemporaryMediaFactory = ::NewWin32TemporaryFileMedia;
}

// ---------------------------------------------------------------------------

PMCharacterSet_eCode PMOSWin32Helper::GetPlatformCharset()
{
	return PMCharacterSet_kISO_8859_1;
}

// ---------------------------------------------------------------------------

PMEndOfLine_eCode PMOSWin32Helper::GetPlatformEndOfLine()
{
	return PMEndOfLine_kCRLF;
}

// ---------------------------------------------------------------------------

PMStrRef PMOSWin32Helper::GetHostName()
{
	return PMTCPSocketFactory::GetDefaultFactory()->GetHostName();
}

// ---------------------------------------------------------------------------
// See PMWErr.h for explanations about error mapping
// Array is Class, First Code of class, Base index for WinError, Number for this class
// Error codes must be successives. If the error is not in range, the first (UNKNOWN) is returned
static int gErrorsMappingTable[] =
{
	0x10,	0x0001,	0x100L,		5,	// Sockets
	0x80,	0x0001,	0x800L,		8,	// PM
	0x100,	0x0001,	0x1000L,	10,	// Inet protocol
	0x101,	0x0001,	0x1100L,	7,	// POP3
	0x102,	0x0001,	0x1200L,	6,	// SMTP
	0x103,	0x0001,	0x1300L,	13,	// FTP
	0x104,	0x0001, 0x1600L,	10,	// UCP
	0x104,	0x0100, 0x1700L,	35,	// UCP NACK codes
	0x105,	0x0001,	0x1500L,	12,	// HTTP
	0x200,	0x0001,	0x2000L,	3,	// Serial
	0x200,	0x0101,	0x2080L,	7,	// Serial Modem connectors
	0x200,	0x0201,	0x2090L,	3,	// Serial Unix Login connectors
	0x200,	0x0301,	0x20A0L,	4,	// Serial Kiosque Micro connectors
};

// ---------------------------------------------------------------------------

DWORD PMOSWin32Helper::PMErrorToWinError(const PMError& anError)
{
	if (!anError.Error())
		return ERROR_SUCCESS;

	pmuint16 theClass = anError.GetClass(), theClassEntry = (pmuint16)-1;
	for (size_t i = 0; i < sizeof(gErrorsMappingTable) / sizeof(gErrorsMappingTable[0]) / 4; i++)
	{
		// Get the right class entry
		if (gErrorsMappingTable[4*i] == theClass)
		{
			// Entry for this class exists
			if (theClassEntry == (pmuint16)-1)
				theClassEntry = i;
			// Get the index of the error for this class
			pmuint16 theIndex = anError.GetCode() - gErrorsMappingTable[4*i+1];

			// If > last known error -> Unknown
			if (theIndex >= gErrorsMappingTable[4*i+3])
				continue;

			return (((DWORD)gErrorsMappingTable[4*i+2]) + theIndex) | 0x2000000;
		}
	}
	// The error code has not been found but this class has been found so
	// return unknown for this class (should be the first one)
	if (theClassEntry != (pmuint16)-1)
		return (((DWORD)gErrorsMappingTable[4*theClassEntry+2])) | 0x2000000;

	// If not class entry found return default error
	return ERROR_PM_UNKNOWN;
}
	
// ---------------------------------------------------------------------------

PMStrRef PMOSWin32Helper::GetErrorString(const PMError& anError)
{
	return GetErrorString(PMErrorToWinError(anError));
}

// ---------------------------------------------------------------------------

PMStrRef PMOSWin32Helper::GetErrorString(DWORD anError)
{
#define MAX_ERROR_LEN	256
	if (anError == ERROR_SUCCESS)
		return PMStr::Empty();

	PMStrRef theString;
	char* theBuffer = theString.NewBuffer(MAX_ERROR_LEN);
	theBuffer[0] = 0;
	LoadString(chRCInstance, anError & ~0x20000000, theBuffer, MAX_ERROR_LEN);
	theString.Adjust();
	if (theString.IsNull())
		theString = PMSTRSTATIC("PM: Unknown error");

	return theString;
#undef MAX_ERROR_LEN
}

// ===========================================================================
//	Win32 Initialization stuff.
// ===========================================================================
// Should always set the default file temporary media
// Should always set the semaphore factory
// Should pass m_hInstance of application
// Should never fail. 

#include "PMWSoc11.h"
#include "PMTLS.h"
#include "PMMedia.h"
#include "PMW32Hlp.h"

static void InitTraces(HINSTANCE anInstance);

#ifdef OBJECTSET_DEMO
	void NotifyIfDateElapsed();
	#define CHECK_EXPIRED ::NotifyIfDateElapsed()
#else
	#define CHECK_EXPIRED
#endif // OBJECTSET_DEMO

#define TRACE_FILE	"pm.log"

static char			sCompanyName[128];
static PMStrRef		sAppName;
static const char	sSettingsSection[] = "ReportSettings";
static pmbool		cfTraceActivated;

// Can be declared in external modules
HKEY PMWin32_OpenSectionKey(HINSTANCE anInstance, const char* aSection);
PMStrRef PMWin32_GetModuleFileName(HINSTANCE anInstance);

// ---------------------------------------------------------------------------

PMError	PMOSWin32Helper::Init(HINSTANCE anInstance, pmbool afInitTCP)
{
	if (!cfPMInited)
	{
		chInstance = (HINSTANCE)anInstance;
		if (chInstance == 0)
			chInstance = ::GetModuleHandle(0);
		if (!::pm_init(anInstance))
			return PMError(kPMOSHelperInitPMError);

		chRCInstance = ::LoadLibrary("PMRC.DLL");
		// If the DLL is not found may be resources are linked with the appli
		// to which the lib is linked
		if (chRCInstance == 0)
			chRCInstance = chInstance;

		if (!PMTLS::Init())
			return PMError(kPMOSHelperInitPMError);

		PMOSHelper::SetCurrent(new PMOSWin32Helper);

		PMMedia::gTemporaryMediaFactory = ::NewWin32TemporaryFileMedia;
//		PMSemaphore::gFactory = PMWinSemaphore::NewPMWinSemaphore;
//		PMTracer::SetEOL("\r\n");

#ifndef PMFinalRelease
		BOOL thefEnabled = EnableMemoryTracking(FALSE);
#endif
		// Don't track the tracer in memory leaks as it will be destroyed
		// after leaks report (that generates traces)
		::InitTraces(chInstance);
#ifndef PMFinalRelease
		EnableMemoryTracking(thefEnabled);
#endif

		PMTCPSocketFactory::SetDefaultFactory(new PMWinSock11Factory);

//		pm_busy_hook = ::OSBusyProc;
		CHECK_EXPIRED;

		PM_TRACE( Info, TL( "Win32 stub initialized." ) );

		cfPMInited = pmtrue;
	}

	if (afInitTCP)
		return InitTCP();

	return PMError();
}

// ---------------------------------------------------------------------------

void PMOSWin32Helper::Close(void* aData)
{
	if (cfTCPInited)
	{
		PMTCPSocketFactory::GetDefaultFactory()->Close();
		PMTCPSocketFactory::SetDefaultFactory(0);
		cfTCPInited = pmfalse;
	}

	if (cfPMInited)
	{
		PM_TRACE( Info, TL( "Win32 stub closed." ) );

		if (chRCInstance != 0 && chRCInstance != chInstance)
			::FreeLibrary(chRCInstance);

		PMTLS::Close();
		::pm_close();

		PMTracer::SetActive(pmfalse);
		PMTracer::SetTracer(0);
//		PMTracer::SetActive(!cfTraceActivated);

		cfPMInited = pmfalse;
	}
}

// ---------------------------------------------------------------------------

PMError PMOSWin32Helper::InitTCP()
{
	if (!cfTCPInited)
	{
		if (!PMTCPSocketFactory::GetDefaultFactory()->Init(chInstance))
			return PMError(kPMOSHelperInitTCPError);
		cfTCPInited = pmtrue;
	}

	return PMError();
}

//-----------------------------------------------------------------------
#include "PMANSIFM.h"
#include "PMWFilTr.h"
#include "PMWTCPTr.h"

//-----------------------------------------------------------------------
PMMediaRef NewWin32TemporaryFileMedia()
{
	char thePath[_MAX_PATH], theName[_MAX_PATH];

	thePath[0] = 0;
	::GetTempPath(_MAX_PATH, thePath);
	if (!::GetTempFileName(thePath, "~OS", 0, theName))
		return 0;

	return new PMANSIFileMedia(theName, pmtrue);
}

//-----------------------------------------------------------------------
#ifdef OBJECTSET_DEMO
	#include <time.h>

	/*
	static int cExpiredYear = 1997;
	static int cExpiredMonth = 01;
	static int cExpiredDay = 31;

	static const char cExpiredMessage[] = 
	"This evaluation version of ObjectSet Mail SDK has expired. Thank you for "
	"your interest in our products. Now it's time to contact Smartcode Software "
	"to purchase your very own licensed copy.";
	*/

	// New strategy is to be always expired and have the message displayed
	static int cExpiredYear = 1996;
	static int cExpiredMonth = 01;
	static int cExpiredDay = 31;

	static const char cExpiredMessage[] = 
	"You are currently using an evaluation version of ObjectSet Mail SDK. "
	"Please write to sales@smartcodesoft.com to obtain a licensed copy. Thank you.";

	//-----------------------------------------------------------------------
	void NotifyIfDateElapsed()
	{
		time_t theTime = ::time(0);
		struct tm* theTM = ::localtime(&theTime);
		pmbool thefExpired = (theTM == 0);
		if (!thefExpired)
		{
			if ((theTM->tm_year+1900) > cExpiredYear)
				thefExpired = pmtrue;
			else
				if ((theTM->tm_year+1900) == cExpiredYear)
				{
					if ((theTM->tm_mon+1) > cExpiredMonth)
						thefExpired = pmtrue;
					else
						if ((theTM->tm_mon+1) == cExpiredMonth)
							thefExpired = theTM->tm_mday > cExpiredDay;
				}
		}
		if (thefExpired)
		{
			::MessageBeep(MB_ICONEXCLAMATION);
			::MessageBox(0, cExpiredMessage, "ObjectSet SDK", MB_OK | MB_ICONEXCLAMATION);
		}
	}
#endif // OBJECTSET_DEMO

//-----------------------------------------------------------------------
//	Initializations
//-----------------------------------------------------------------------
static void InitTraces(HINSTANCE anInstance)
{
#ifndef PMFinalRelease
	static const char	sTraceKey[] = "DisplayTraceD";
	static const char	sTracerKey[] = "TracerTypeD";
#else
	static const char	sTraceKey[] = "DisplayTrace";
	static const char	sTracerKey[] = "TracerType";
#endif
	char*				theRegEntry;
	PMTracerRef			theTracer;

	cfTraceActivated = pmfalse;

#define COMPARE_TRACER(x) (::strnicmp(thePtr, x, ::strlen(x)) == 0)

	HKEY theSettingsKey = ::PMWin32_OpenSectionKey(anInstance, sSettingsSection);
	if (theSettingsKey != 0)
	{
		DWORD theValue = 0, theType, theSize = 0;
		theSize = sizeof(DWORD);
		if (::RegQueryValueEx(theSettingsKey, sTraceKey, NULL, &theType, (LPBYTE)&theValue, &theSize) != ERROR_SUCCESS
			|| theType != REG_DWORD)
		{
#ifdef PMFinalRelease
			theValue = 0;
#else
			theValue = 1;
			::RegSetValueEx(theSettingsKey, sTraceKey, 0, REG_DWORD, (LPBYTE)&theValue, sizeof(DWORD));
#endif
		}
		cfTraceActivated = theValue;

#ifdef PMFinalRelease
		if (cfTraceActivated)
		{
#endif
			pmbool thefKeyExists = pmfalse;
			// This code is always executed in debug mode but only is trace activated is defined in release mode
			if (::RegQueryValueEx(theSettingsKey, sTracerKey, NULL, &theType, 0, &theSize) == ERROR_SUCCESS && theType == REG_SZ)
			{
				thefKeyExists = pmtrue;
				theRegEntry = new char[theSize];

				if (::RegQueryValueEx(theSettingsKey, sTracerKey, NULL, &theType, (LPBYTE)theRegEntry, &theSize) == ERROR_SUCCESS)
				{
					char* thePtr = theRegEntry;
					while (*thePtr == ' ')
						thePtr++;
					char* thePtr2 = ::strchr(thePtr, ':');
					if (thePtr2 != 0)
					{
						*thePtr2 = 0;
						thePtr2++;
					}

					if (COMPARE_TRACER("file"))
					{
						if (thePtr2 == 0)
							thePtr2 = TRACE_FILE;

						FILE* theTraceFile = ::fopen(thePtr2, "w");
						if (theTraceFile != 0)
							theTracer = new PMWin32FileTracer(theTraceFile, pmtrue);
					}
					else
					if (COMPARE_TRACER("stderr"))
					{
						theTracer = new PMWin32FileTracer(stderr, pmfalse);
					}
					else
					if (COMPARE_TRACER("tcp"))
					{
						PMStrRef	theHost;
						pmuint16	thePort = PMWin32TCPTracer_kPort;
						if (thePtr2 == 0)
							theHost = PMSTRSTATIC("localhost");
						else
						{
							char* thePtr3 = ::strchr(thePtr2, ':');
							if (thePtr3 != 0)
							{
								*thePtr3++ = 0;
								thePort = ::atoi(thePtr3);
								if (thePort == 0)
									thePort = PMWin32TCPTracer_kPort;
							}
							theHost = thePtr2;
						}
						PMWin32TCPTracerRef theTCPTracer = new PMWin32TCPTracer(theHost, thePort);
						// theTCPTracer->GetConnectRetryCount() and not GetMaxConnectRetryCount because we tried once and if the max was one we do not want to try more.
						if (theTCPTracer->CanSend() || theTCPTracer->GetConnectRetryCount() != 0)
							theTracer = theTCPTracer.Get();
					}
				}
				delete[] theRegEntry;
			}

	#ifndef PMFinalRelease
			#define PM_WRITE_TRACER(x) ::RegSetValueEx(theSettingsKey, sTracerKey, 0, REG_SZ, (LPBYTE)x, ::strlen(x))
			if (theTracer.IsNull())
			{
				// If theTracerType entry was not found, set the default to file
				FILE* theTraceFile = ::fopen(TRACE_FILE, "w");
				if (theTraceFile != 0)
				{
					theTracer = new PMWin32FileTracer(theTraceFile, pmtrue);
					if (!thefKeyExists)
						PM_WRITE_TRACER("file");
				}
				else
				{
					theTracer = new PMWin32FileTracer(stderr, pmfalse);
					if (!thefKeyExists)
						PM_WRITE_TRACER("stderr");
				}
			}
	#endif
#ifdef PMFinalRelease
		}
#endif
		::RegCloseKey(theSettingsKey);
	}

	if (!theTracer.IsNull())
		PMTracer::SetTracer(theTracer);

	PMTracer::SetActive(cfTraceActivated);
}

//-----------------------------------------------------------------------
#if (_MSC_VER >= 1000) && !defined(NDEBUG)
	#include <crtdbg.h>
#endif
void PMOSWin32Helper::DisplayAssert(const char* aTest, PMTraceSendParams* aParams)
{
	if (aParams != 0)
	{
		if (aParams->itsFile != 0 && aTest != 0 && aParams->itsData != 0)
		{
			PMBufferRef theMessage;
			if (::pm_strlen(aTest) != 0)
				theMessage = ::pm_xsprintf("ASSERTION: \"%s\" ([%s] failed) in file: %s, line: %lu", aParams->itsData, aTest, aParams->itsFile, (unsigned long)aParams->itsLine);
			else
				theMessage = ::pm_xsprintf("IMMEDIATE ASSERTION: \"%s\" in file: %s, line: %lu", aParams->itsData, aParams->itsFile, (unsigned long)aParams->itsLine);
			PM_TRACE(Info, TL("%s", theMessage->buffer()));
#if (_MSC_VER >= 1000) && !defined(NDEBUG)
			if (_CrtDbgReport(_CRT_ASSERT, 0, 0, 0, (char*)theMessage->buffer()) == 1)
				_CrtDbgBreak();
#else
			::MessageBeep(MB_ICONSTOP);
			::MessageBox(0, (char*)theMessage->buffer(), "ObjectSet ASSERTION", MB_ICONSTOP | MB_OK);
			_asm int 3; // Breakpoint
#endif
		}
	}
}

//-----------------------------------------------------------------------
// Windows helper functions
//-----------------------------------------------------------------------
PMStrRef PMWin32_GetModuleFileName(HINSTANCE anInstance)
{
	char theBuffer[_MAX_PATH];
	if (::GetModuleFileName(anInstance, theBuffer, _MAX_PATH) == 0)
		return "Unknown";

	LPSTR theExt = ::strchr(theBuffer, '.');
	if (theExt != NULL)
		*theExt = 0;

	LPCSTR theTemp = (LPSTR)theBuffer;
	for (LPCSTR theString = theBuffer; *theString != '\0'; theString++)
	{
		// remember last directory/drive separator
		if (*theString == '\\' || *theString == '/' || *theString == ':')
			theTemp = theString+1;
	}

	return theTemp;
}

//-----------------------------------------------------------------------
HKEY PMWin32_OpenSectionKey(HINSTANCE anInstance, const char* aSection)
{
	if (*sCompanyName == 0)
	{
		//	AFX_IDS_COMPANY_NAME = 0xE004
		//	AFX_IDS_APP_TITLE = 0xE000
		if (!::LoadString(anInstance, 0xE004, sCompanyName, sizeof(sCompanyName)) )
			::strcpy(sCompanyName, "Smartcode Software");

		char* theBuffer = sAppName.NewBuffer(128);
		if (!::LoadString(anInstance, 0xE000, theBuffer, 128))
		{
			sAppName.Adjust(0);
			sAppName = ::PMWin32_GetModuleFileName(anInstance);
		}
		else
			sAppName.Adjust();
	}

	HKEY theSectionKey = 0;

	HKEY theSoftwareKey = 0;
	if (::RegOpenKeyEx(HKEY_CURRENT_USER, "Software", 0, KEY_WRITE|KEY_READ, &theSoftwareKey) == ERROR_SUCCESS)
	{
		DWORD	theDisposition;
		HKEY	theCompanyKey;

		if (::RegCreateKeyEx(theSoftwareKey, sCompanyName, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL, &theCompanyKey, &theDisposition) == ERROR_SUCCESS)
		{
			HKEY theAppKey;
			if (::RegCreateKeyEx(theCompanyKey, sAppName->c_str(), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL, &theAppKey, &theDisposition) == ERROR_SUCCESS)
			{
				if (aSection == 0)
					theSectionKey = theAppKey;
				else
				{
					if (::RegCreateKeyEx(theAppKey, aSection, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL, &theSectionKey, &theDisposition) != ERROR_SUCCESS)
						theSectionKey = 0;

					::RegCloseKey(theAppKey);
				}
			}

			::RegCloseKey(theCompanyKey);
		}

		::RegCloseKey(theSoftwareKey);
	}

	return theSectionKey;
}

// ---------------------------------------------------------------------------
// MIME mappings
// ---------------------------------------------------------------------------
#include <tchar.h>
#include "PMBuffer.h"
/*
typedef INTSHCUTAPI HRESULT (WINAPI *MIMEPROC)
		(HWND hwndParent, DWORD dwInFlags, PCSTR pcszFile, 
		 PCSTR pcszMIMEContentType, PSTR pszAppBuf, UINT ucAppBufLen);
*/
static const TCHAR sContentType[] = _T("Content Type");
static const TCHAR sSoftware[] = _T("Software");
static const TCHAR sDefaultIcon[] = _T("\\DefaultIcon");
static const TCHAR sCommand[] = _T("\\shell\\open\\command");
static const TCHAR sMIMEDatabase[] = _T("MIME\\Database\\Content Type\\");
static const TCHAR sExtension[] = _T("Extension");

static const TCHAR *sDefaultTypes[] = 
{
	_T("application/postscript"),		_T(".ps"),
	_T("application/x-zip-compressed"),	_T(".zip"),
	_T("application/x-stuffit"),		_T(".sit"),
	_T("application/mac-binhex40"),		_T(".hqx"),
	_T("application/rtf"),				_T(".rtf"),

	_T("video/x-msvideo"),				_T(".avi"),
	_T("video/quicktime"),				_T(".mov"),
	_T("video/mpeg"),					_T(".mpg"),

	_T("audio/x-wav"),					_T(".wav"),
	_T("audio/wav"),					_T(".wav"),
	_T("audio/basic"),					_T(".au"),

	_T("image/x-MS-bmp"),				_T(".bmp"), 
	_T("image/gif"),					_T(".gif"), 
	_T("image/jpeg"),					_T(".jpg"), 
	_T("image/tiff"),					_T(".tif"), 
	_T("image/x-portable-bitmap"),		_T(".pbm"), 
	_T("image/x-xbitmap"),				_T(".xbm"), 

	_T("text/html"),					_T(".htm"),
	_T("text/plain"),					_T(".txt"),
	_T("text/richtext"),				_T(".rtf"),
	_T("text/enriched"),				_T(".txt"),

	_T("message/rfc822"),				_T(".mim"),
	_T("message/news"),					_T(".mim")
};

//---------------------------------------------------------------------------
typedef PMHashTable<PMStrArrayRef> PMStrArrayHashTable;
typedef PMRef<PMStrArrayHashTable> PMStrArrayHashTableRef;
typedef PMHashTable<HICON> PMIconHashTable;
typedef PMRef<PMIconHashTable> PMIconHashTableRef;

static PMStrArrayHashTableRef	gMIME2Ext, gExt2MIME;
static PMIconHashTableRef		gExt2LargeIcon, gExt2SmallIcon;
static int						gUnknownShellLargeIconIndex = -1;
static int						gUnknownShellSmallIconIndex = -1;

//---------------------------------------------------------------------------
static PMStrRef ReadValueFromKey(HKEY aKey, LPCTSTR lpszValue, LPCTSTR lpszKeyName)
{
	HKEY theKey = NULL;

	PMStrRef theBuffer;

	if (::RegOpenKeyEx(aKey, lpszKeyName, 0, KEY_READ, &theKey) == ERROR_SUCCESS)
	{
		DWORD theType, theSize = 0;
		if (::RegQueryValueEx(theKey, lpszValue, 0, &theType, 0, &theSize) == ERROR_SUCCESS && (theType == REG_SZ || theType == REG_EXPAND_SZ))
		{
			LPBYTE thePtr = (LPBYTE)theBuffer.NewBuffer(theSize);
			thePtr[0] = 0;
			if (::RegQueryValueEx(theKey, lpszValue, 0, 0, thePtr, &theSize) == ERROR_SUCCESS)
				thePtr[theSize] = 0;
			theBuffer.Adjust();
			if (theType == REG_EXPAND_SZ)
			{
				DWORD theSize2;
				PMStrRef theBuffer2;
				LPSTR thePtr2 = theBuffer2.NewBuffer(theSize);
				thePtr2[0] = 0;
				if ((theSize2 = ::ExpandEnvironmentStrings(theBuffer->c_str(), thePtr2, theSize)) >= theSize)
				{
					theBuffer2.Adjust(0);
					thePtr2 = theBuffer2.NewBuffer(theSize2+1);
					theSize2 = ::ExpandEnvironmentStrings(theBuffer->c_str(), thePtr2, theSize2);
				}

				thePtr2[theSize2] = 0;
				theBuffer2.Adjust();
				theBuffer = theBuffer2;
			}
		}
		::RegCloseKey(theKey);
	}

	return theBuffer;
}

//---------------------------------------------------------------------------
//	MIME Types cache managment
//---------------------------------------------------------------------------
static PMStrArrayHashTableRef GetMIME2ExtTable();
static PMStrArrayHashTableRef GetExt2MIMETable();

//---------------------------------------------------------------------------
static void RegisterMIMEExtAssoc(PMStrRef aMIME, PMStrRef anExt)
{
	if (aMIME.IsNull() || anExt.IsNull())
		return;

	PMStrArrayRef theArray;
	PMStrRef theMIME(aMIME->LowerCase()), theExt(anExt->LowerCase());

	// Call GetMIME2ExtTable rather than using gMIME2Ext to eventually create it
	theArray = GetMIME2ExtTable()->ObjectForKey(theMIME);
	if (theArray.IsNull())
	{
		theArray = new PMArray<PMStrRef>;
		// Now created
		gMIME2Ext->SetObjectForKey(theArray, theMIME);
	}
	if (!theArray->IsObjectPresent(theExt))
		theArray->AppendObject(theExt);

	theArray = GetExt2MIMETable()->ObjectForKey(theExt);
	if (theArray.IsNull())
	{
		theArray = new PMArray<PMStrRef>;
		gExt2MIME->SetObjectForKey(theArray, theExt);
	}
	if (!theArray->IsObjectPresent(theMIME))
		theArray->AppendObject(theMIME);
}

//---------------------------------------------------------------------------
static void InitMIMETypes()
{
	static BOOL sfCleanUpRegistered = FALSE;
	if (!sfCleanUpRegistered)
	{
		sfCleanUpRegistered = TRUE;
		PMRC::RegisterRefCleanUp(&::gMIME2Ext);
		PMRC::RegisterRefCleanUp(&::gExt2MIME);
	}
	gMIME2Ext = new PMStrArrayHashTable;
	gExt2MIME = new PMStrArrayHashTable;

	DWORD		theIndex;
	for (theIndex = 0; theIndex < sizeof(sDefaultTypes) / sizeof(sDefaultTypes[0]); theIndex += 2)
		::RegisterMIMEExtAssoc(sDefaultTypes[theIndex], sDefaultTypes[theIndex+1]);

	HKEY		theHKey = NULL;
	char		theExt[_MAX_EXT+1];
	char*		theMIMEType = 0;
	DWORD		theTypeMaxLen = 0;
	DWORD		theLen = 0;
	PMStrRef	theString;
	FILETIME	theFT;

	if (::RegOpenKeyEx(HKEY_CLASSES_ROOT, sMIMEDatabase, 0, KEY_READ, &theHKey) == ERROR_SUCCESS)
	{
		if (::RegQueryInfoKey(theHKey, 0, 0, 0, 0, &theTypeMaxLen, 0, 0, 0, 0, 0, 0) == ERROR_SUCCESS)
		{
			PMBuffer theBuffer(theTypeMaxLen+1);
			theMIMEType = (char*)theBuffer.buffer();

			theIndex = 0;
			theLen = theTypeMaxLen + 1;
			while (::RegEnumKeyEx(theHKey, theIndex++, theMIMEType, &theLen, 0, 0, 0, &theFT) == ERROR_SUCCESS)
			{
				theLen = theTypeMaxLen + 1;
				theString = ::ReadValueFromKey(theHKey, sExtension, theMIMEType);
				if (!theString.IsNull())
					::RegisterMIMEExtAssoc(theMIMEType, theString);
			}
		}

		::RegCloseKey(theHKey);
	}

	theLen = (_MAX_EXT+1)*sizeof(TCHAR);
	theIndex = 0;
	while (::RegEnumKeyEx(HKEY_CLASSES_ROOT, theIndex++, theExt, &theLen, 0, 0, 0, &theFT) == ERROR_SUCCESS)
	{
		theLen = (_MAX_EXT+1)*sizeof(TCHAR);
		if (*theExt == '.')
		{
			theString = ::ReadValueFromKey(HKEY_CLASSES_ROOT, sContentType, theExt);
			if (!theString.IsNull() && theString->size() != 0)
				::RegisterMIMEExtAssoc(theString, theExt);
		}
	}

}

//---------------------------------------------------------------------------
static PMStrArrayHashTableRef GetMIME2ExtTable()
{
	if (gMIME2Ext.IsNull())
	{
		PMCriticalSectionLocker theLocker(PMRC::GetGlobalCriticalSection());
		if (gMIME2Ext.IsNull())
			::InitMIMETypes();
	}

	return gMIME2Ext;
}

//---------------------------------------------------------------------------
static PMStrArrayHashTableRef GetExt2MIMETable()
{
	if (gExt2MIME.IsNull())
	{
		PMCriticalSectionLocker theLocker(PMRC::GetGlobalCriticalSection());
		if (gExt2MIME.IsNull())
			::InitMIMETypes();
	}

	return gExt2MIME;
}

//---------------------------------------------------------------------------
// Icons cache managment
//---------------------------------------------------------------------------
static PMIconHashTableRef GetExt2IconTable(pmbool afLarge);

//---------------------------------------------------------------------------
static void RegisterExtIconAssoc(PMStrRef anExt, HICON aLargeIcon, HICON aSmallIcon)
{
	if (!anExt.IsNull())
	{
		PMStrRef theExt(anExt->LowerCase());
		HICON theIcon;
		if (aLargeIcon != 0)
		{
			theIcon = GetExt2IconTable(pmtrue)->ObjectForKey(theExt);
			if (theIcon == 0)
				gExt2LargeIcon->SetObjectForKey(aLargeIcon, theExt);
		}
		if (aSmallIcon != 0)
		{
			theIcon = GetExt2IconTable(pmfalse)->ObjectForKey(theExt);
			if (theIcon == 0)
				gExt2SmallIcon->SetObjectForKey(aSmallIcon, theExt);
		}
	}
}

//---------------------------------------------------------------------------
static void InitIcons()
{
	static BOOL sfCleanUpRegistered = FALSE;
	if (!sfCleanUpRegistered)
	{
		sfCleanUpRegistered = TRUE;
		PMRC::RegisterRefCleanUp(&::gExt2LargeIcon);
		PMRC::RegisterRefCleanUp(&::gExt2SmallIcon);
	}

	gExt2LargeIcon = new PMIconHashTable;
	gExt2LargeIcon->SetNullValue(0);
	gExt2SmallIcon = new PMIconHashTable;
	gExt2SmallIcon->SetNullValue(0);
}

//---------------------------------------------------------------------------
static PMIconHashTableRef GetExt2IconTable(pmbool afLarge)
{
	if (gExt2LargeIcon.IsNull() || gExt2SmallIcon.IsNull())
	{
		PMCriticalSectionLocker theLocker(PMRC::GetGlobalCriticalSection());
		if (gExt2LargeIcon.IsNull() || gExt2SmallIcon.IsNull())
			::InitIcons();
	}

	return afLarge?gExt2LargeIcon:gExt2SmallIcon;
}

// ---------------------------------------------------------------------------
//	MIME Types
// ---------------------------------------------------------------------------

PMStrRef PMOSWin32Helper::ExtensionToMimeType(const PMStrRef& anExtension)
{
	if (anExtension.IsNull() || anExtension->size() == 0)
		return DefaultMimeType();

	PMStrRef theExt(_T("."));
	if (anExtension->c_str()[0] == '.')
		theExt += (anExtension->c_str() + 1);
	else
		theExt += anExtension;

	PMStrRef theExt2(theExt->LowerCase());

	PMStrArrayRef theArray = ::GetExt2MIMETable()->ObjectForKey(theExt2);
	if (theArray.IsNull() || theArray->GetCount() == 0)
		return DefaultMimeType();

	return theArray->GetObjectAtIndex(0);
}

// ---------------------------------------------------------------------------

PMStrRef PMOSWin32Helper::FileNameToMimeType(const PMStrRef& aFileName)
{
	if (aFileName.IsNull() || aFileName->size() == 0)
		return DefaultMimeType();

	char theExt[_MAX_EXT];

	::_splitpath(aFileName->c_str(), 0, 0, 0, theExt);

	return ExtensionToMimeType(theExt);
}

// ---------------------------------------------------------------------------

PMStrArrayRef PMOSWin32Helper::MimeTypeToExtension(const PMStrRef& aMimeType, pmbool afReturnDefault)
{
	PMStrArrayRef theArray;
	if (!aMimeType.IsNull() && aMimeType->size() != 0)
		theArray = ::GetMIME2ExtTable()->ObjectForKey(aMimeType->LowerCase());

	if (theArray.IsNull())
		theArray = new PMArray<PMStrRef>;

	if (theArray->GetCount() == 0 && afReturnDefault)
		theArray->AppendObject(ExtensionForUnknownMimeType(aMimeType));

	return theArray;
}
	
// ---------------------------------------------------------------------------
static HICON PMExtractIcon(const PMStrRef& anExt, const PMStrRef& aFileName, int anIndex, pmbool afLarge)
{
	HICON theLargeIcon, theSmallIcon;

	UINT theResult = ::ExtractIconEx(aFileName->c_str(), anIndex, &theLargeIcon, &theSmallIcon, 1);
	if (theResult > 1)
	{
		if (afLarge)
		{
			if (!anExt.IsNull())
				::RegisterExtIconAssoc(anExt, theLargeIcon, 0);
			return theLargeIcon;
		}
		else
		{
			if (!anExt.IsNull())
				::RegisterExtIconAssoc(anExt, 0, theSmallIcon);
			return theSmallIcon;
		}
	}
	PM_TRACE(Info, TL("PMExtractIcon(): ExtractIconEx failed for file [%s], index: [%ld]", aFileName->c_str(), (long)anIndex));

	theLargeIcon = ::ExtractIcon(::GetModuleHandle(0), aFileName->c_str(), anIndex);
	if (((DWORD)theLargeIcon) <= 1)
	{
		PM_TRACE(Info, TL("PMExtractIcon(): ExtractIcon failed for file [%s], index: [%ld]", aFileName->c_str(), (long)anIndex));
		return 0;
	}
	if (afLarge)
	{
		if (!anExt.IsNull())
			::RegisterExtIconAssoc(anExt, theLargeIcon, 0);
		return theLargeIcon;
	}
	static int sCXSM = ::GetSystemMetrics(SM_CXSMICON);
	static int sCYSM = ::GetSystemMetrics(SM_CYSMICON);
	theSmallIcon = (HICON)::CopyImage(theLargeIcon, IMAGE_ICON, sCXSM, sCYSM, 0);
	if (theSmallIcon == 0)
	{
		PM_TRACE(Info, TL("PMExtractIcon(): CopyImage failed."));
		return 0;
	}
	if (!anExt.IsNull())
		::RegisterExtIconAssoc(anExt, 0, theSmallIcon);

	return theSmallIcon;
}

// ---------------------------------------------------------------------------

HICON PMOSWin32Helper::ExtensionToIcon(const PMStrRef& anExtension, pmbool afLarge)
{
	if (anExtension.IsNull() || anExtension->size() == 0)
		return 0;

	PMStrRef theExt(_T("."));
	if (anExtension->c_str()[0] == '.')
		theExt += (anExtension->c_str() + 1);
	else
		theExt += anExtension;
	theExt = theExt->LowerCase();

	HICON theIcon = ::GetExt2IconTable(afLarge)->ObjectForKey(theExt);
	if (theIcon != 0)
	{
		PM_TRACE(Info, TL("WinMailHelper::ExtensionToIcon(): icon in cache"));
		return theIcon;
	}

	if (((BYTE)::GetVersion()) >= 4)
	{
		UINT theFlags = SHGFI_ICON | SHGFI_USEFILEATTRIBUTES;
		if (afLarge)
			theFlags |= SHGFI_LARGEICON;
		else
			theFlags |= SHGFI_SMALLICON;

		SHFILEINFO theFI;
		if (::SHGetFileInfo(theExt->c_str(), FILE_ATTRIBUTE_NORMAL, &theFI, sizeof(theFI), theFlags) != 0)
		{
			if (afLarge)
				::RegisterExtIconAssoc(theExt, theFI.hIcon, 0);
			else
				::RegisterExtIconAssoc(theExt, 0, theFI.hIcon);

			return theFI.hIcon;
		}
	}

	PMStrRef theType = ::ReadValueFromKey(HKEY_CLASSES_ROOT, _T(""), theExt);
	if (theType.IsNull()) 
	{
		PM_TRACE(Info, TL("WinMailHelper::ExtensionToIcon(): No type registered for extension: %s", theExt->c_str()));
		return 0;
	}

	PMStrRef theType2(theType->c_str());
	PMStrRef theString;
	int theComma;

	theType += sDefaultIcon;
	theString = ::ReadValueFromKey(HKEY_CLASSES_ROOT, _T(""), theType);
	if (theString.IsNull())
		PM_TRACE(Info, TL("WinMailHelper::ExtensionToIcon(): No default icon registered for extension: %s", theExt->c_str()));
	else
	{
		theComma = theString->FindChar(',');
		int theIndex = 0;
		if (theComma != (size_t)-1)
		{
			theIndex = ::atoi(((LPCSTR)theString) + theComma + 1);
			theString = theString->Left(theComma);
		}

		theIcon = PMExtractIcon(theExt, theString, theIndex, afLarge);
		if (theIcon != 0)
			return theIcon;
	}

	theType2 += sCommand;
	theString = ::ReadValueFromKey(HKEY_CLASSES_ROOT, _T(""), theType2);
	if (theString.IsNull())
		PM_TRACE(Info, TL("WinMailHelper::ExtensionToIcon(): No shell command registered for extension: %s", theExt->c_str()));
	else
	{
		theComma = theString->FindChar(_T(' '));
		if (theComma != (size_t)-1)
			theString = theString->Left(theComma);

		theIcon = PMExtractIcon(theExt, theString, 0, afLarge);
		if (theIcon != 0)
			return theIcon;
	}

//	return ::LoadIcon(0, IDI_WINLOGO);
	return 0;
}

// ---------------------------------------------------------------------------

HICON PMOSWin32Helper::FileNameToIcon(const PMStrRef& aFileName, pmbool afLarge, pmbool* afIsDefault)
{
	if (afIsDefault != 0)
		*afIsDefault = pmtrue;
	if (aFileName.IsNull() || aFileName->size() == 0)
		return 0;

	if (((BYTE)::GetVersion()) >= 4)
	{
		SHFILEINFO	theFI;
		UINT		theFlags = SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX;
		int			theUnknownIconIndex;
		if (afLarge)
		{
			theFlags |= SHGFI_LARGEICON;
			if (gUnknownShellLargeIconIndex == -1)
			{
				// Let's hope that zzy is not a registered type
				::SHGetFileInfo("Unknown Type.zzy", FILE_ATTRIBUTE_NORMAL, &theFI, sizeof(theFI), theFlags);
					gUnknownShellLargeIconIndex = theFI.iIcon;
			}
			theUnknownIconIndex = gUnknownShellLargeIconIndex;
		}
		else
		{
			theFlags |= SHGFI_SMALLICON;
			if (gUnknownShellSmallIconIndex == -1)
			{
				::SHGetFileInfo("Unknown Type.zzy", FILE_ATTRIBUTE_NORMAL, &theFI, sizeof(theFI), theFlags);
					gUnknownShellSmallIconIndex = theFI.iIcon;
			}
			theUnknownIconIndex = gUnknownShellSmallIconIndex;
		}

		theFI.hIcon = 0;
		::SHGetFileInfo(aFileName->c_str(), FILE_ATTRIBUTE_NORMAL, &theFI, sizeof(theFI), theFlags);
		if (theFI.hIcon != 0)
		{
			if (afIsDefault != 0)
				*afIsDefault = (theFI.iIcon == theUnknownIconIndex);
			return theFI.hIcon;
		}
	}

	char theExt[_MAX_EXT];

	if (::GetFileAttributes(aFileName->c_str()) != 0xFFFFFFFF)
	{
		HICON theIcon = PMExtractIcon(0, aFileName, 0, afLarge);
		if (theIcon != 0)
			return theIcon;
	}

	::_splitpath(aFileName->c_str(), 0, 0, 0, theExt);

	return ExtensionToIcon(theExt, afLarge);
}

// ---------------------------------------------------------------------------

PMStrRef PMOSWin32Helper::ExtensionForUnknownMimeType(const PMStrRef& aMimeType)
{
	return ".txt";
}

// ---------------------------------------------------------------------------

PMStrRef PMOSWin32Helper::DefaultMimeType()
{
	return "application/octet-stream";
}

// ---------------------------------------------------------------------------

void PMOSWin32Helper::RegistryChanged()
{
	gMIME2Ext = 0;
	gExt2MIME = 0;
	gExt2LargeIcon = 0;
	gExt2SmallIcon = 0;
	gUnknownShellLargeIconIndex = -1;
	gUnknownShellSmallIconIndex = -1;
}

// ---------------------------------------------------------------------------

//-----------------------------------------------------------------------
//	OSMIMEHelper implementation
//-----------------------------------------------------------------------
#if 0
UINT OSMIMEHelper::NeedAssociation(HWND hwnd, LPCTSTR lpszFileName, LPCTSTR lpszMIMEType, LPTSTR lpExecutable, UINT uSize)
{
	HINSTANCE	theInstLib = ::LoadLibrary(_T("url.dll"));
	HRESULT		theResult;

	if (theInstLib != 0)
	{
		MIMEPROC theProc = (MIMEPROC)::GetProcAddress(theInstLib, "MIMEAssociationDialog");
		char theBuffer[_MAX_PATH + 1];

		if (theProc != 0)
		{
			theResult = theProc(hwnd, MIMEASSOCDLG_FL_REGISTER_ASSOC, lpszFileName, lpszMIMEType, theBuffer, _MAX_PATH);
		};
		::FreeLibrary(theInstLib);

		if (theResult == S_OK || theResult == S_FALSE)
			return OSPMTools::FillBufferFromString(lpExecutable, uSize, theBuffer);
	}

	return 0;
}
#endif
