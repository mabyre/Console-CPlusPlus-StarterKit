/*---------------------------------------------------------------------------*\
 * Copyright (c) 1998-2002 AbyreSoft. Written by Bruno Raby.
 *---------------------------------------------------------------------------*
 * TTheading.c - Test de la creation de taches
\*---------------------------------------------------------------------------*/

#include <windows.h>
#include "cMenu.h"
#include "pmEnv.h"
#include "pmXDebug.h"

/*---------------------------------------------------------------------------*/

HANDLE hStopEvent;
HANDLE hThread = NULL;

HANDLE hStopEventTQP;
HANDLE hThreadTQP = NULL;

/*--------------------------------------------------------------------------*/

DWORD WINAPI ThreadProc( LPVOID lpParameter );
DWORD WINAPI ThreadProcTempsQuiPasse( LPVOID lpParameter );

/*--------------------------------------------------------------------------*/

int GlobalCount = 0;

/*--------------------------------------------------------------------------*/

void DoCreateWaitingThread( void )
{
    DWORD threadId;
    DWORD t = 0;

    // Create the event to signal the service to stop.
    hStopEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
    if ( hStopEvent == NULL )
        PM_DEBUG0(TL("ERROR - CreateEvent"));

    hThread = CreateThread
    (
        NULL,
        0,
        ThreadProc,
        (LPVOID)t,
        0,
        &threadId
    );

    if ( hThread == INVALID_HANDLE_VALUE )
        PM_DEBUG0(TL("ERROR - CreateThread, ThreadProc()"));

}

/*--------------------------------------------------------------------------*/

void DoCreateTempsQuiPasseThread( void )
{
    DWORD threadId;
    DWORD t = 0;

    /* Create the event to signal the service to stop */
    hStopEventTQP = CreateEvent( NULL, TRUE, FALSE, NULL );
    if ( hStopEventTQP == NULL )
        PM_DEBUG0(TL("ERROR - CreateEvent"));

    hThreadTQP = CreateThread
    (
        NULL,
        0,
        ThreadProcTempsQuiPasse,
        (LPVOID)t,
        0,
        &threadId
    );

    if ( hThreadTQP == INVALID_HANDLE_VALUE )
        PM_DEBUG0(TL("ERROR - CreateThread"));
}

/*--------------------------------------------------------------------------*/

DWORD WINAPI ThreadProc( LPVOID lpParameter )
{
    INT nThreadNum = (INT)lpParameter;

    while( WaitForSingleObject( hStopEvent, 1000 ) != WAIT_OBJECT_0 )
    {
        // Just to have something to do, it will beep every second.
        PM_DEBUG0(T("Wakeup thread : %x, ", nThreadNum ));
        GlobalCount++;
        PM_DEBUG0(TL("%d times ...", GlobalCount ));
        Sleep(1000);
    }

    PM_DEBUG0(TL("C'est la mort de la thread : %x...", nThreadNum ));
    return 0;
}

/*--------------------------------------------------------------------------*/

DWORD WINAPI ThreadProcTempsQuiPasse( LPVOID lpParameter )
{
    INT nThreadNum = (INT)lpParameter;

    while( WaitForSingleObject( hStopEventTQP, 700 ) != WAIT_OBJECT_0 )
    {
        // Just to have something to do, it will beep every second.
        PM_DEBUG0(T("."));
        //Sleep(10);
    }

    PM_DEBUG0(TL("."));
    PM_DEBUG0(TL("C'est la mort de la thread du TQP : %x...", nThreadNum ));
    return 0;
}

/*--------------------------------------------------------------------------*/

void DoStopEvent( void )
{
    SetEvent( hStopEvent );
}

/*--------------------------------------------------------------------------*/

void DoStopTQP( void )
{
    SetEvent( hStopEventTQP );
}

/*--------------------------------------------------------------------------*/

static void DoNothing( void ) {}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Threading, "Threading - Creer et stopper des tache" )
    PMMENU_ITEM_EX( 1, "Creer une waiting thread", DoCreateWaitingThread )
    PMMENU_ITEM_EX( 2, "Creer la thread du temps qui passe", DoCreateTempsQuiPasseThread )
    PMMENU_ITEM_EX( 3, "Stoper la waiting thread", DoStopEvent )
    PMMENU_ITEM_EX( 4, "Stoper la temps qui passe thread", DoStopTQP )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Threading )

/*--------------------------------------------------------------------------*/
