/*--------------------------------------------------------------------------*\
 * Copyright (c) 2022 Mabyre - All rights reserved.
 *--------------------------------------------------------------------------*
 * MyMacros.h
 * I discover usefull things to make macro with
\*--------------------------------------------------------------------------*/

#ifndef MyMacros_h
#define MyMacros_h

/*--------------------------------------------------------------------------*/

#include "OSWinInc.h"

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------*/

#define TRACE0_ENDL() PM_TRACE0(TL("%s", " "))

/* 
 * Make a string with an auto variable or a variable you
 * don't know the type
 */
#define STR(x) std::to_string(x).c_str()

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

/*--------------------------------------------------------------------------*/
#endif /* MyMacros_h */