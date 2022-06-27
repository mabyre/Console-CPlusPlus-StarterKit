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

/*--------------------------------------------------------------------------*/

struct database {
    int id_number;
    int age;
    float salary;
};

/*--------------------------------------------------------------------------*\
 * https://stackoverflow.com/questions/16703835/how-can-i-see-cout-output-in-a-non-console-application
 * I would like to redirect cout to TraceReporter ...
\*--------------------------------------------------------------------------*/
void InitConsole( void )
{
    AllocConsole();
    
    //Don't work !
    //freopen_s( "CONOUT$", "w", stdout );
    //freopen_s( "CONOUT$", "w", stderr );
}

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

/*--------------------------------------------------------------------------*\
 * Use of "cout" to print unformatted data otherwise it's not possible
\*--------------------------------------------------------------------------*/
void DoLambdaExpressions( void )
{
    // generic lambda, operator() is a template with two parameters
    auto glambda = []( auto a, auto &&b ) { return a < b; };
    bool b1 = glambda( 3, 3.14 ); // ok
    bool b2 = glambda( 4, 3.14 ); // false

    pm_trace0( "lambda expression result b1: %s", b1 == true ? "True" : "False" );
    pm_trace0( "lambda expression result b2: %s", b2 == true ? "True" : "False" );

    // Since  C++14
    // generic lambda, operator() is a template with one parameter
    auto vglambda = []( auto printer )
    {
        return [=]( auto&&... ts ) // generic lambda, ts is a parameter pack
        {
            printer( std::forward<decltype(ts)>( ts )... );
            return [=] { printer( ts... ); }; // nullary lambda (takes no parameters)
        };
    };
    auto p = vglambda( []( auto v1, auto v2, auto v3 ) { std::cout << v1 << v2 << v3 << endl; } );
    auto q = p( 1, 'a', 3.14 ); // outputs 1a3.14
    q();
    auto q2 = p( 1, 'a', "3.14" ); // outputs 1a3.14
}

/*--------------------------------------------------------------------------*/

// This is not possible with actual VS 2019 Community
// https://en.cppreference.com/w/cpp/language/lambda
//void DoLambdaExpressionSinceCplusplus20( void )
//{
//    // Since C++ 20
//    // generic lambda, operator() is a template with two parameters
//    auto glambda = []<class T>(T a, auto && b) { return a < b; };
//
//    // generic lambda, operator() is a template with one parameter pack
//    auto f = []<typename... Ts>(Ts&&... ts)
//    {
//        return foo( std::forward<Ts>( ts )... );
//    };
//}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Cplusplus, "Cplus Plus" )
    PMMENU_ITEM_EX( 1, "Init Console", InitConsole )
    PMMENU_ITEM_EX( 2, "Test", DoTest )
    PMMENU_ITEM_EX( 3, "Struct", DoStruct )
    PMMENU_ITEM_EX( 4, "System Time", DoSystemTime )
    PMMENU_ITEM_EX( 5, "Lambda expressions", DoLambdaExpressions )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Cplusplus )
