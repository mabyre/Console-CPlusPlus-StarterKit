/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2022 SoDevLog. Written by Mabyre.
 *--------------------------------------------------------------------------*
 * TCplusplus.c
\*--------------------------------------------------------------------------*/
using namespace std;

#include "cMenu.h"
#include "pmTrace.h"
#include <iostream>
#include <Windows.h>
#include <OleAuto.h>
#include <atlstr.h>

struct database {
    int id_number;
    int age;
    float salary;
};

/*--------------------------------------------------------------------------*/

void DoTest( void )
{
    cout << "Hello there !\n";
}

/*--------------------------------------------------------------------------*/

void DoStruct( void )
{
    database employee;  //There is now an employee variable that has modifiable 
                        // variables inside it.
    database *pEmployee = &employee;

    employee.age = 22;
    employee.id_number = 1;
    employee.salary = 12000.21;

    pm_trace0( "employee salary: %f", employee.salary );
    pm_trace0( "employee id: %d", pEmployee->id_number );
}

/*--------------------------------------------------------------------------*/

SYSTEMTIME convertStringToSystemTime( char *dateTimeString )
{
    SYSTEMTIME systime;

    memset( &systime, 0, sizeof( systime ) );
    
    // Date string should be "yyyy-MM-dd hh:mm"
    sscanf_s( dateTimeString, "%d-%d-%d%d:%d:%d",
        &systime.wYear,
        &systime.wMonth,
        &systime.wDay,
        &systime.wHour,
        &systime.wMinute );
    return systime;
}

void DoSystemTime( void )
{
    SYSTEMTIME st;
    string sMessage = "encore une grosse daube";
    
    GetSystemTime( &st );

    // 1. Using CString
    CString cstrMessage;

    cstrMessage.Format( _T("%d-%02d-%02d %02d:%02d:%02d.%03d"),
        st.wYear,
        st.wMonth,
        st.wDay,
        st.wHour,
        st.wMinute,
        st.wSecond,
        st.wMilliseconds );

    //std::cout << "System time = " << strMessage << std::endl;
    pm_trace0( "system time: %s", "ce CString est une énorme daube" );
    pm_trace0( "system time: %s", sMessage.c_str() );
    
    CStringA charstr( cstrMessage );
    pm_trace0( "system time: %s", charstr );

    // Well Noted 78 dans StackOverFlow
    CT2A ascii( cstrMessage, CP_UTF8 );
    pm_trace0( "system time: %s", ascii.m_psz );
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Cplusplus, "Test Cplus Plus" )
PMMENU_ITEM_EX( 1, "Test", DoTest )
PMMENU_ITEM_EX( 2, "Struct", DoStruct )
PMMENU_ITEM_EX( 3, "System Time", DoSystemTime )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Cplusplus )
