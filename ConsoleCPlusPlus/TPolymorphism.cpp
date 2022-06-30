/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2022 SoDevLog. Written by Mabyre.
 *--------------------------------------------------------------------------*
 * TPolymorphism.c
 * 
 * From :
 * https://cplusplus.com/doc/tutorial/polymorphism/
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
    void set_values( int a, int b )
    {
        width = a; height = b;
    }
    
    virtual int area() = 0; // pure virtual
    //{
    //    return 0;
    //}

    void printarea()
    {
        pm_trace0( "polygon area: %d", this->area() );
    }
};

class Rectangle : public Polygon {
public:
    int area()
    {
        return width * height;
    }
};

class Triangle : public Polygon {
public:
    int area()
    {
        return (width * height / 2);
    }
};

/*--------------------------------------------------------------------------*/

void DoTestPolymorphism( void )
{
    Rectangle r;
    Triangle t;
    //Polygon poly; impossible to intanciate an abstract class

    Polygon *ppoly1 = &r;
    Polygon *ppoly2 = &t;
    //Polygon *ppoly3 = &poly;

    ppoly1->set_values( 4, 5 );
    ppoly2->set_values( 4, 5 );
    //ppoly3->set_values( 4, 5 );

    pm_trace0( "ppoly1: %d", ppoly1->area() );
    pm_trace0( "ppoly2: %d", ppoly2->area() );
    //pm_trace0( "ppoly3: %d", ppoly3->area() );
}

/*--------------------------------------------------------------------------*/

void DoPolymorphismCallBasedFunction( void )
{
    Rectangle r;
    Triangle t;

    Polygon *ppoly1 = &r;
    Polygon *ppoly2 = &t;

    ppoly1->set_values( 4, 5 );
    ppoly2->set_values( 4, 5 );

    ppoly1->printarea();
    ppoly2->printarea();
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Polymorphism, "Polymorphism" )
    PMMENU_ITEM_EX( 1, "Polymorphism", DoTestPolymorphism )
    PMMENU_ITEM_EX( 2, "Based Function Calls", DoPolymorphismCallBasedFunction )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Polymorphism )

/* TRACES for non pure virtual
00002925:3294:TRACE0:ppoly1: 20
00002925:3294:TRACE0:ppoly2: 10
00002925:3294:TRACE0:ppoly3: 0

With calling based functions
00005673:339c:TRACE0:polygon area: 20
00005673:339c:TRACE0:polygon area: 10

*/
