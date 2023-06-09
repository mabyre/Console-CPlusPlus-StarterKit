/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * TReport.c - Differentes facons de faire des traces avec le Reporter
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "pmEnv.h"
#include "pmXError.h"
#include "pmXTrace.h"
#include "pmXDebug.h"
#include "pmTrace.h"

/*--------------------------------------------------------------------------*/

static void DoTraceError(void)
{
    char buffer[34] = "Hello a coucou form the PM little";

    PM_ERROR0(TL("ERROR 0 - Error tres grave"));
    PM_ERROR1(TL("ERROR 1 - Error grave"));
    PM_ERROR2(TL("ERROR 2 - Error faible"));
    PM_ERROR3(TL("ERROR 3 - Error tres faible"));

    PM_ERROR0(T( "decimal %d,", 1 ));
    PM_ERROR0(TL( "chaine %s", "abc" ));
    PM_ERROR0(TL( "PM trace many parameters : %s %i %x", "abc", 123, 0x123 ));

    PM_ERROR0(TL("-----------Buffer"));
    PM_ERROR0(B( buffer, 33 ));
    PM_ERROR0(TL("-----------fin buffer"));
}

/*--------------------------------------------------------------------------*/

static void DoTraceTrace(void)
{
    PM_TRACE0(TL("TRACE 0 - Trace tres importante"));
    PM_TRACE1(TL("TRACE 1 - Trace importante"));
    PM_TRACE2(TL("TRACE 2 - Trace peu importante"));
    PM_TRACE3(TL("TRACE 3 - Trace pas importante"));
}

/*--------------------------------------------------------------------------*/

static void DoTraceFloat(void)
{
    double f1 = 1.234567E+3;
    double f2 = 1.234567E+4;

    pm_trace0( "float1 : %f float2 : %f", f1, f2 );
}

/*--------------------------------------------------------------------------*/

static void DoTraceDebug(void)
{
    PM_DEBUG0(TL("DEBUG 0 - Debug tres grave"));
    PM_DEBUG1(TL("DEBUG 1 - Debug grave"));
    PM_DEBUG2(TL("DEBUG 2 - Debug faible"));
    PM_DEBUG3(TL("DEBUG 3 - Debug tres faible"));
}

/*--------------------------------------------------------------------------*/

static void DoTestTrace(void)
{
   pmerror     theError = 0xFE12;
   pmuint32 theUInt32 = 0xFEDC1234;
   pmuint16 theUInt16 = 0xFE12;
   int         theInt = (int) 0xFE12FE12;
   pmbyte      theBuffer[256];
   size_t      i;
   
   // !!! some not supported
   PM_TRACE0(TL("This is an int32:"));
   PM_TRACE0(TL("  unsigned [4275835444]=[%lu]", theUInt32));
   PM_TRACE0(TL("  signed   [-19131852]=[%ld]", theUInt32));
   PM_TRACE0(TL("  hexa     [fedc1234]=[%lx]", theUInt32));
   
   PM_TRACE0(TL("This is an int16:"));
   PM_TRACE0(TL("  unsigned [65042]=[%hu]", theUInt16));
   PM_TRACE0(TL("  signed   [-494]=[%hd]", theUInt16));
   PM_TRACE0(TL("  hexa     [FE12]=[%hX]", theUInt16));
   
   PM_TRACE0(TL("This is an int:"));
   if (sizeof(int) == sizeof(pmint16))
   {
      PM_TRACE0(TL("  unsigned [65042]=[%u]", theInt));
      PM_TRACE0(TL("  signed   [-494]=[%d]", theInt));
      PM_TRACE0(TL("  hexa     [fe12]=[%x]", theInt));
   }
   else
   {
      PM_TRACE0(TL("  unsigned [4262657554]=[%u]", theInt));
      PM_TRACE0(TL("  signed   [-32309742]=[%d]", theInt));
      PM_TRACE0(TL("  hexa     [fe12fe12]=[%x]", theInt));
   }
   
   PM_TRACE0(TL("This is a string [Hello world]=[%s]", "Hello world"));
   PM_TRACE0(TL("This is a char [A]=[%c]", 'A'));

   PM_TRACE0(TL("This is a string [Hello]=[%5s]", "Hello world"));
   
   PM_TRACE0(TL("This is a int16  [    0x41]=[%#8hx]", (pmuint16) 0x41));
   PM_TRACE0(TL("This is an int32 [  0x4141]=[%#8lx]", (pmuint32) 0x4141));
   
   PM_TRACE0(TL("This is a int16  [0x41    ]=[%#-8hx]", (pmuint16) 0x41));
   PM_TRACE0(TL("This is an int32 [0x4141  ]=[%#-8lx]", (pmuint32) 0x4141));

   PM_TRACE0(TL("This is an int32 [+20000]=[%#+2ld]", (pmuint32) 20000));
   
   PM_TRACE0(TL("These are strange characters [\\t\\b\\n\\r]=[\t\b\n\r]"));
   
   PM_TRACE0(TL("This is a line"));
   PM_TRACE0(TL("break"));
   
   PM_TRACE0(TL("This is a percent sign [%%]=[%%]"));
   
   for (i = 0 ; i < 256 ; i++)
      theBuffer[i] = (pmbyte) i;
   PM_TRACE0(TL("This is a buffer:"));
   PM_TRACE0(B(theBuffer, (size_t) 256));
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( Report, "Rerport - Tests" )
    PMMENU_ITEM_EX( 1, "Test du module Error", DoTraceError )
    PMMENU_ITEM_EX( 2, "Test du module Trace", DoTraceTrace )
    PMMENU_ITEM_EX( 3, "Test du module Debug", DoTraceDebug )
    PMMENU_ITEM_EX( 4, "Test du moduel TraceFloat", DoTraceFloat )
    PMMENU_ITEM_EX( 5, "Test du format des traces", DoTestTrace )
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( Report )
