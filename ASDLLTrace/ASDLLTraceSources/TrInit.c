/*--------------------------------------------------------------------------*\
 * TrInit.c
 * Copyright (C) 1990 - 1996 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#include "TrHelp.h"
#include "TrFac.h"
#include <winreg.h>
#include <stdio.h>

/*--------------------------------------------------------------------------*/

static const char	sCompanyName[128] = "SoDevLog";
char				sAppName[128];
static const char	sSettingsSection[] = "ReportSettings";
static const char	sTraceKey[] = "DisplayTrace";
static const char	sTracerTypeKey[] = "TracerType";
static const char	sTracerDLLKey[] = "TracerDLL";
static const char	sMaxOpenRetryCountKey[] = "TracerRetryMax";
static const char	sOpenRetryDelayKey[] = "TracerRetryDelay";
static HINSTANCE	sTraceDLLInst;

static TTracer* 	(__stdcall *sCreateProc)(char* aName, char* aParam);
static void			(__stdcall *sDestroyProc)(TTracer* aTracer);

#define MAX_OPEN_RETRY_COUNT	1
#define OPEN_RETRY_DELAY		30000

/*--------------------------------------------------------------------------*\
 * Get the name of the executable, release the path name
\*--------------------------------------------------------------------------*/
static DWORD GetAppName( char* aBuffer, HINSTANCE anInstance )
{
	LPSTR	theExt;
	LPCSTR	theTemp;
	LPCSTR	theString;
	char	theBuffer[_MAX_PATH];
	
	if ( GetModuleFileName( anInstance, theBuffer, _MAX_PATH ) == 0 )
	{
		strcpy( aBuffer, "Unknown" );
		return 0;
	}

	theExt = strrchr( theBuffer, '.' );
	if ( theExt != NULL ) *theExt = 0;

	theTemp = ( LPSTR)theBuffer;
	for ( theString = theBuffer; *theString != '\0'; theString++ )
	{
		/* Remember last directory/drive separator */
		if ( *theString == '\\' || *theString == '/' || *theString == ':' )
			theTemp = theString + 1;
	}

	strcpy( sAppName, theTemp );
    return 1;
}

/*--------------------------------------------------------------------------*/

static HKEY OpenSectionKey( HINSTANCE anInstance, const char* aSection )
{
	HKEY theSectionKey = 0;
	HKEY theSoftwareKey = 0;

    // Le commentaire ci-dessous n'a que peu de sens
    // il ne semble pas opportun d'aller chercher des ressources dans le reporter

    // Must be synchronised with the Reporter :
	//	    AFX_IDS_COMPANY_NAME = 0xE004
	//	    AFX_IDS_APP_TITLE    = 0xE000

    // Try to get the compagny name by anInstance
    // If not set the default compagny name
	// if ( !LoadString( anInstance, 0xE004, sCompanyName, sizeof( sCompanyName ) ) )
	// if ( !LoadString( anInstance, 0xE000, sAppName, sizeof( sAppName )) )

    // GetAppName() : essaye un GetModuleFileName( sur anInstance )

	if ( !GetAppName( sAppName, anInstance ) )
    {
        /* Sinon on recupere directement sAppName - dangereux non ? */
        strcpy( sAppName, (char *)anInstance );
    }

	if ( RegOpenKeyEx( HKEY_CURRENT_USER, "Software", 0, KEY_WRITE|KEY_READ, &theSoftwareKey ) == ERROR_SUCCESS )
	{
		DWORD theDisposition;
		HKEY  theCompanyKey;

		if ( RegCreateKeyEx( theSoftwareKey, sCompanyName, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL, &theCompanyKey, &theDisposition) == ERROR_SUCCESS)
		{
			HKEY theAppKey;
			if ( RegCreateKeyEx( theCompanyKey, sAppName, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL, &theAppKey, &theDisposition) == ERROR_SUCCESS)
			{
				if ( aSection == 0 )
					theSectionKey = theAppKey;
				else
				{
					if ( RegCreateKeyEx( theAppKey, aSection, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL, &theSectionKey, &theDisposition) != ERROR_SUCCESS)
						theSectionKey = 0;
					RegCloseKey( theAppKey );
				}
			}
			RegCloseKey( theCompanyKey );
		}
		RegCloseKey( theSoftwareKey );
	}

	return theSectionKey;
}

/*	------------------------------------------------------------------- */
static DWORD GetDWORD( HKEY aKey, const char* aValueName, DWORD aDefault )
{
	DWORD	theSize, theValue, theType;
	LONG	theResult;

	theSize = sizeof( DWORD );
	theResult = RegQueryValueEx( aKey, aValueName, NULL, &theType, (LPBYTE)&theValue, &theSize );
	if ( theResult == ERROR_MORE_DATA || theType != REG_DWORD)
	{
		RegDeleteValue( aKey, aValueName );
		theResult  = ERROR_MORE_DATA;
	}
	if ( theResult != ERROR_SUCCESS )
	{
		theValue = aDefault;
		RegSetValueEx( aKey, aValueName, 0, REG_DWORD, (LPBYTE)&theValue, sizeof(DWORD) );
	}

	return theValue;
}

/*--------------------------------------------------------------------------*/

void InitTraces( HINSTANCE anInstance )
{
	char*		theRegEntry;
	HKEY		theSettingsKey;
	static char	sTracerDLLPath[MAX_PATH];

	gTracer = 0;
	gfTracesActivated = 0;
	sTraceDLLInst = 0;
	sCreateProc = as_trace_create_instance;
	sDestroyProc = as_trace_destroy_instance;
	theSettingsKey = OpenSectionKey( anInstance, sSettingsSection );
	InitializeCriticalSection( &gSendCS );

	if ( theSettingsKey != 0 )
	{
		DWORD theValue = 0, theType, theSize = 0;
		LONG  theResult;

		gfTracesActivated = GetDWORD( theSettingsKey, sTraceKey, 1 );
		if ( gfTracesActivated )
		{
			theSize = sizeof( sTracerDLLPath );
			
            theResult = RegQueryValueEx( theSettingsKey, sTracerDLLKey, 0, &theType, sTracerDLLPath, &theSize );
			if ( theResult == ERROR_SUCCESS && theType == REG_SZ )
			{
				sTraceDLLInst = LoadLibrary( sTracerDLLPath );
				if ( sTraceDLLInst != 0 )
				{
					sCreateProc = ( TTracer* (__stdcall*)(char*, char*))GetProcAddress(sTraceDLLInst, "as_trace_create_instance");
					sDestroyProc = (void (__stdcall*)(TTracer*))GetProcAddress(sTraceDLLInst, "as_trace_destroy_instance");
					if ( sCreateProc == 0 || sDestroyProc == 0 )
					{
						sCreateProc = as_trace_create_instance;
						sDestroyProc = as_trace_destroy_instance;
						FreeLibrary( sTraceDLLInst );
						sTraceDLLInst = 0;
					}
				}
			}

			theResult = RegQueryValueEx( theSettingsKey, sTracerTypeKey, NULL, &theType, 0, &theSize );
			if ( (theResult == ERROR_SUCCESS || theResult == ERROR_MORE_DATA) && theType != REG_SZ )
			{
				RegDeleteValue( theSettingsKey, sTracerTypeKey );
				theResult = ERROR_MORE_DATA;
			}
			if ( theResult != ERROR_SUCCESS )
			{
				RegSetValueEx( theSettingsKey, sTracerTypeKey, 0, REG_SZ, (LPBYTE)"tcp", 4 );
				theSize = 4;
			}
			theRegEntry = (char*)malloc( theSize );

			if ( RegQueryValueEx(theSettingsKey, sTracerTypeKey, NULL, &theType, (LPBYTE)theRegEntry, &theSize) == ERROR_SUCCESS )
			{
				TTracer*	theTracer;
				char		*thePtr = theRegEntry, *thePtr2;

				while ( *thePtr == ' ' )
					thePtr++;
				thePtr2 = strchr( thePtr, ':' );
				if ( thePtr2 != 0 )
				{
					*thePtr2 = 0;
					thePtr2++;
				}
				theTracer = sCreateProc( thePtr, thePtr2 );
				if ( theTracer == 0 )
				{
					sCreateProc = as_trace_create_instance;
					sDestroyProc = as_trace_destroy_instance;
					FreeLibrary( sTraceDLLInst );
					sTraceDLLInst = 0;
					theTracer = sCreateProc( thePtr, thePtr2 );
				}
				if ( theTracer != 0 )
				{
					theTracer->itsfFirstTrace = TRUE;

					theTracer->itsMaxOpenRetryCount = GetDWORD(theSettingsKey, sMaxOpenRetryCountKey, MAX_OPEN_RETRY_COUNT );
					theTracer->itsOpenRetryDelay = GetDWORD(theSettingsKey, sOpenRetryDelayKey, OPEN_RETRY_DELAY );

					theTracer->itsOpenRetryCount = theTracer->itsMaxOpenRetryCount;
					theTracer->itsLastOpenTryTime = GetTickCount() - theTracer->itsOpenRetryDelay;

					gTracer = theTracer;
				}
			}
			free( theRegEntry );
		}
		RegCloseKey( theSettingsKey );
	}
}

/*--------------------------------------------------------------------------*/

void CloseTraces( void )
{
	TTracer* theTracer;

	theTracer = gTracer;
	gTracer = 0;

	Trace_Close( theTracer );
	sDestroyProc( theTracer );
	if ( sTraceDLLInst != 0 )
	{
		FreeLibrary( sTraceDLLInst );
		sTraceDLLInst = 0;
	}
	DeleteCriticalSection( &gSendCS );
}
