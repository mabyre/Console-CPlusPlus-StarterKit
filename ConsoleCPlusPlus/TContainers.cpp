/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2022 SoDevLog. Written by Mabyre.
 *--------------------------------------------------------------------------*
 * TContainers.c
 *
 * Play with:
 * https://en.cppreference.com/w/cpp/container
 *--------------------------------------------------------------------------*
 * There is many types of containers
 * Sequence: array, vector
 * Associative
 * Underscored
 * Adaptors
\*--------------------------------------------------------------------------*/
using namespace std;

#include "cMenu.h"
#include "pmTrace.h"
#include "MyMacros.h"
#include <iostream>
#include <array>
#include <vector>
#include <string>

/*--------------------------------------------------------------------------*/

void InitContainers( void )
{
    cout << "Init Containers !" << endl;
}

/*--------------------------------------------------------------------------*\
 * https://en.cppreference.com/w/cpp/container/array
 * 
 * Microsoft C++ does not like array very much
 * 
 * https://docs.microsoft.com/fr-fr/cpp/standard-library/array-class-stl?view=msvc-170
 * 
 * Does not work !
 * sort is not a member of std:: 
 * 
    // construction uses aggregate initialization
    std::array<int, 3> a1{ {1, 2, 3} }; // double-braces required in C++11 prior to
                                        // the CWG 1270 revision (not needed in C++11
                                        // after the revision and in C++14 and beyond)

    std::array<int, 3> a2 = {1, 2, 3};  // double braces never required after =

    std::array<std::string, 2> a3 = { std::string("a"), "b" };

    // container operations are supported
    std::sort(a1.begin(), a1.end());
    std::reverse_copy(a2.begin(), a2.end(),
                      std::ostream_iterator<int>(std::cout, " "));

    std::cout << '\n';

    // ranged for loop is supported
    for(const auto& s: a3)
        std::cout << s << ' ';

    // deduction guide for array creation (since C++17)
    [[maybe_unused]] std::array a4{3.0, 1.0, 4.0};  // -> std::array<double, 3>

  * sorting an array will not be easy Microsoft specific array can be used
  * but this is Microsoft specific
 /*--------------------------------------------------------------------------*/

void DoContainerArrayVerifyCompatibility( void )
{
    // construction uses aggregate initialization
    std::array<int, 3> a1{ {1, 2, 3} }; // double-braces required in C++11 prior to
                                        // the CWG 1270 revision (not needed in C++11
                                        // after the revision and in C++14 and beyond)

    std::array<int, 3> a2 = { 1, 2, 3 };  // double braces never required after =

    std::array<std::string, 2> a3 = { std::string( "a" ), "b" };

    // display contents " 1 2 3"
    for ( const auto &it : a1 )
    {
        std::cout << " " << it;
        PM_TRACE0( T( " %s", std::to_string( it ).c_str() ) );
    }
    std::cout << std::endl;
    TRACE0_ENDL();

    // sorted array's elements does not work !
    //std::sort( a1.begin(), a1.end() );
    //std::reverse_copy( a2.begin(), a2.end(),
    //    std::ostream_iterator<int>( std::cout, " " ) );

    // ranged for loop is supported
    for ( const auto &s : a3 )
        std::cout << s << ' ';

    std::cout << std::endl;

    // deduction guide for array creation (since C++17)
    //[[maybe_unused]] std::array a4{ 3.0, 1.0, 4.0 };  // -> std::array<double, 3>
}

/*--------------------------------------------------------------------------*/

typedef std::array<int, 4> MyArrayType;

void DoContainerArraySwap( void )
{
    MyArrayType c0 = { 0, 1, 2, 3 };
    MyArrayType c1 = { 4, 5, 6, 7 };

    c0.swap( c1 );

    // display swapped contents " 4 5 6 7"
    for ( const auto &it : c0 )
    {
        std::cout << " " << it;
        PM_TRACE0( T( " %s", std::to_string( it ).c_str() ) );
    }
    std::cout << std::endl;
    TRACE0_ENDL();

    std::swap( c0, c1 );

    // display swapped contents " 0 1 2 3"
    for ( const auto &it : c0 )
    {
        std::cout << " " << it;
        PM_TRACE0( T( " %s", std::to_string( it ).c_str() ) );

    }
    std::cout << std::endl;
    TRACE0_ENDL();
}

/*--------------------------------------------------------------------------*\
 * does not work
\*--------------------------------------------------------------------------*/
int compare( const void *arg1, const void *arg2 )
{
    /* Compare all of both strings: */
    return *(int **)arg1 < *(int **)arg2;
}

void DoContainerArraySort( void )
{
    MyArrayType c2 = { 41, 15, 6, 74 };

    MyArrayType::const_pointer ptr = &*c2.begin();

    // does not work !
    std::qsort( (void *)ptr, c2.size(), 1, compare);

    for ( const auto &it : c2 )
    {
        std::cout << " " << it;
    }
    std::cout << std::endl;
}

/*--------------------------------------------------------------------------*/

void DoArrayIterator( void )
{
    MyArrayType c0 = { 0, 1, 2, 3 };

    // display contents " 0 1 2 3"
    for ( const auto &it : c0 )
    {
        PM_TRACE0( T( " %s", STR( it ) ) );
    }
    TRACE0_ENDL();

    // display first element " 0"
    MyArrayType::iterator it2 = c0.begin();
    std::cout << " " << *it2;
    std::cout << std::endl;

    
    std::cout << "it1:";
    for ( MyArrayType::iterator it1 = c0.begin(); it1 != c0.end(); ++it1 )
    {
        PM_TRACE0( T( " %s", STR( *it1 ) ) );
    }
    TRACE0_ENDL();
}

/*--------------------------------------------------------------------------*/

void DoUseVector( void )
{
    // Create a vector containing integers
    std::vector<int> v = { 7, 5, 16, 8 };

    // Add two more integers to vector
    v.push_back( 25 );
    v.push_back( 13 );

    // Print out the vector
    PM_TRACE0( T( "%s", "v = { " ) );
    for ( int n : v ) {
        PM_TRACE0( T( " %s,", STR(n) ) );
    }
    PM_TRACE0( TL( "%s", " }" ) );
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Containers, "Test Containers" )
    PMMENU_ITEM_EX( 1, "Declare Array", DoContainerArrayVerifyCompatibility )
    PMMENU_ITEM_EX( 2, "Swap Array", DoContainerArraySwap )
    PMMENU_ITEM_EX( 3, "Sort Array", DoContainerArraySort )
    PMMENU_ITEM_EX( 4, "Iterator", DoArrayIterator )
    PMMENU_ITEM_EX( 5, "Use Vector", DoUseVector )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Containers )
