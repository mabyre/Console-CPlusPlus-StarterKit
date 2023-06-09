/*--------------------------------------------------------------------------*\
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * TPMTrace.c - Utiliser les PM_TRACE0 et les pm funtions
\*--------------------------------------------------------------------------*/

#include "cMenu.h"
#include "cInput.h"

#include "pmXTrace.h"
#include "pmTrace.h"

/*--------------------------------------------------------------------------*/

static void DoTestTrace(void)
{
    pmerror		theError = 0xFE12;
    pmuint32	theUInt32 = 0xFEDC1234;
    pmuint16	theUInt16 = 0xFE12;
    int			theInt = (int) 0xFE12FE12;
    pmbyte		theBuffer[256];
    size_t		i;

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

static void DoTestTraceFloat(void)
{
    pm_trace0( "This is a float [3.123] = [%g]", 3.123, 1 );
}

/*--------------------------------------------------------------------------*/

static void DoTestVTL( void )
{
    char *string = "[-----]";

    PM_TRACE0(T("%s", string ));
//    PM_TRACE0(VT("%s", string ));
//    PM_TRACE0(VTL("%s", string ));
    PM_TRACE0(TL("%s", string ));
    PM_TRACE0(B("abcdef", 6 ));
}

/*--------------------------------------------------------------------------*/

static void DoSampleTest( void )
{
    PM_TRACE0(TL("Les fonctions des traces fonctionnent encore !"));
    PM_TRACE0(TL("Les fonctions de pm trace ne s'affiche plus"));
}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN( PMTrace, "PMTrace - Tests")
	PMMENU_ITEM_EX(1, "Traces test", DoTestTrace)
	PMMENU_ITEM_EX(2, "Trace float test", DoTestTraceFloat)
	PMMENU_ITEM_EX(3, "Differentes fonctions de trace", DoTestVTL)
	PMMENU_ITEM_EX(4, "Sample test", DoSampleTest)
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER( PMTrace )
