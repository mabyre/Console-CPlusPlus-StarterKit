/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2022 SoDevLog. Written by Mabyre.
 *--------------------------------------------------------------------------*
 * TVirtual.c - Use Virtual function for polymorphism
\*--------------------------------------------------------------------------*/
using namespace std;

#include "cMenu.h"
#include "pmTrace.h"
#include <iostream>
#include <Windows.h>

/*--------------------------------------------------------------------------*/

class MyRectangle
{
public:
    MyRectangle( int width, int height );
    MyRectangle() {};

    virtual float perimeter();
};

MyRectangle::MyRectangle( int width, int height )
{
    pm_trace0( "rectangle's width: %d height: %d", width, height );
}

float MyRectangle::perimeter()
{
    pm_trace0( "rectangle's perimeter" );
    return 0.0;
}

/*--------------------------------------------------------------------------*/

class MySquare : public MyRectangle
{
public:
    MySquare() {};
    MySquare( int side );

    // virtual non mandatory, but recommanded.
    virtual float perimeter();
};

MySquare::MySquare( int side )
{
    pm_trace0( "square's side %d", side );
}

float MySquare::perimeter()
{
    pm_trace0( "square's perimeter" );
    return 0.0;
}

/*--------------------------------------------------------------------------*/

void DoTestVirtual( void )
{
    MyRectangle r( 100, 50 );
    MySquare s( 30 );
    MyRectangle *ptr1;

    // call to 'perimeter' from 'MySquare'
    // or 'MyRectangle' depending

    // ptr1 can take a rectangle
    ptr1 = &r;
    pm_trace0( "call to %f", ptr1->perimeter() );

    // but also can take square this is polymorphism
    ptr1 = &s;
    pm_trace0( "call to %f", ptr1->perimeter() );
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Virtual, "Virtual function" )
    PMMENU_ITEM_EX( 1, "Virtualization", DoTestVirtual )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Virtual )
