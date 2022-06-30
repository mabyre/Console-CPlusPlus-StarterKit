/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2022 SoDevLog. Written by Mabyre.
 *--------------------------------------------------------------------------*
 * TPolymorphism2.c
 * 
 * End of :
 * https://cplusplus.com/doc/tutorial/polymorphism/
 * 
 * Constructors have been added to use dynamic allocation
\*--------------------------------------------------------------------------*/
using namespace std;

#include "cMenu.h"
#include "pmTrace.h"
#include <iostream>

/*--------------------------------------------------------------------------*/

class Polygon {
protected:
    int width, height;
public:
    Polygon( int a, int b ) : width( a ), height( b ) {}
    
    virtual int area() = 0; // pure virtual

    void printarea()
    {
        pm_trace0( "polygon area: %d", this->area() );
    }
};

class Rectangle : public Polygon {
public:
    Rectangle( int a, int b ) : Polygon( a, b ) {}
    int area()
    {
        return width * height;
    }
};

class Triangle : public Polygon {
public:
    Triangle( int a, int b ) : Polygon( a, b ) {}
    int area()
    {
        return (width * height / 2);
    }
};

/*--------------------------------------------------------------------------*/

void DoDynamicAllocation( void )
{
    Polygon *ppoly1 = new Rectangle( 4, 5 );
    Polygon *ppoly2 = new Triangle( 4, 5 );

    ppoly1->printarea();
    ppoly2->printarea();

    delete ppoly1;
    delete ppoly2;
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Polymorphism2, "Polymorphism2" )
    PMMENU_ITEM_EX( 1, "Polymorphism and dynamic allocation", DoDynamicAllocation )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Polymorphism2 )

/* TRACES for non pure virtual
00002925:3294:TRACE0:ppoly1: 20
00002925:3294:TRACE0:ppoly2: 10
00002925:3294:TRACE0:ppoly3: 0

With calling based functions
00005673:339c:TRACE0:polygon area: 20
00005673:339c:TRACE0:polygon area: 10

*/
