/*--------------------------------------------------------------------------*\
 * TBit.c
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
\*--------------------------------------------------------------------------*/

#include <conio.h>

#include "pmEnv.h"
#include "cMenu.h"
#include "cCore.h"
#include "cConsole.h"
#include "cInput.h"

/*----------------------------------------------------------------------------*/

#define MASK_TCP4 0x20
#define MASK_TCP7 0x80

typedef struct 
{
    unsigned b0 : 1;
    unsigned b1 : 1;
    unsigned b2 : 1;
    unsigned b3 : 1;
    unsigned b4 : 1;
    unsigned b5 : 1;
    unsigned b6 : 1;
    unsigned b7 : 1;

    unsigned filler : 24;
}
t_champ;

typedef union
{
   t_champ      b;
   unsigned int	w;
} 
t_champs;

t_champ c = {0,0,0,0,0,0,0,0, /* filler */ 0 };
unsigned int c1,c2,c3;

t_champs OldTCP = {0,0,0,0,0,0,0,0, /* filler */ 0 };

/*----------------------------------------------------------------------------*/

static void DoPrintTable( void )
{
    unsigned int a,b;

    printf("table ^\n");
    a=0;
    b=0;
    printf("0 0 | %d\n", a^b);
    a=0;
    b=1;
    printf("0 1 | %d\n", a^b);
    a=1;
    b=0;
    printf("1 0 | %d\n", a^b);
    a=1;
    b=1;
    printf("1 1 | %d\n", a^b);

    printf("table !^\n");
    a=0;
    b=0;
    printf("0 0 | %d\n", !(a^b));
    a=0;
    b=1;
    printf("0 1 | %d\n", !(a^b));
    a=1;
    b=0;
    printf("1 0 | %d\n", !(a^b));
    a=1;
    b=1;
    printf("1 1 | %d\n", !(a^b));

}

/*----------------------------------------------------------------------------*/

#define MASK_Bit4 0x20

static void DoTestBit( void )
{

    /* Recuperer le bit 3 pour le mettre dans le bit 4 */

    c.b2 = 1;
    c.b3 = 0;
    c.b4 = 1;

    printf("c  = %4X\n",c);

    memcpy( &c1 , &c, sizeof(c));

    printf("c1 = %4X\n",c1);

    c2 = ( c1 << 1 ) & MASK_Bit4;

    printf("c2 = %4X\n",c2);

    c1 = !( c1 ^ ( c2 | ~MASK_Bit4 ));

    printf("c1 = %4X\n",c1);

    memcpy( &c3 , &c, sizeof(c));

    c3 = c3 & ( (( c3 << 1 ) & MASK_Bit4) | ~MASK_Bit4 );

    printf("c3 = %4X\n",c3);
}

/*----------------------------------------------------------------------------*/

static void TraiterTCP( void )
{
    unsigned int NewTCP;


    NewTCP = OldTCP.w;
    printf("new    = %4X\t", NewTCP );

    /* Recopie du bit 4 (TCP3) dans le bit 5 (TCP4) */
    if ( ! (( NewTCP << 1 ) & MASK_TCP4) )
    {
      NewTCP = NewTCP & ~MASK_TCP4;
    }
    else
    {
      NewTCP = NewTCP | MASK_TCP4;
    }
    
    /* Recopie du bit 6 (TCP6) dans le bit 7 (TCP7) */
    if ( ! (( NewTCP << 1 ) & MASK_TCP7) )
    {
      NewTCP = NewTCP & ~MASK_TCP7;
    }
    else
    {
      NewTCP = NewTCP | MASK_TCP7;
    }

    printf("new dec = %4X\n", NewTCP );

}

/*----------------------------------------------------------------------------*/

static void Traiter( void )
{
    printf("c  = %4X\t",c);

    memcpy( &c1 , &c, sizeof(c));

    printf("c1 = %4X\n",c1);

    if ( ! (( c1 << 1 ) & MASK_Bit4) )
    {
        c1 = c1 & ~MASK_Bit4 ;
    }
    else
    {
        c1 = c1 | MASK_Bit4;
    }

    printf("c1 = %4X\n",c1);

    printf("-----------------------------\n");
}

/*----------------------------------------------------------------------------*/

static void Traiter2( void )
{
    printf("c  = %4X\t",c);

    memcpy( &c1 , &c, sizeof(c));

    printf("c1 = %4X\n",c1);

    c2 = ( c1 << 1 ) & MASK_Bit4;

    printf("c2 = %4X\n",c2);

    if ( ! c2 )
    {
        c1 = c1 & ~MASK_Bit4 ;
    }
    else
    {
        c1 = c1 | MASK_Bit4;
    }

    printf("c1 = %4X\n",c1);

    printf("-----------------------------\n");
}

/*----------------------------------------------------------------------------*/

static void DoValidTestBit( void )
{
    /* Recuperer le bit 3 pour le mettre dans le bit 4 */

    c.b2 = 1;
    c.b3 = 0;
    c.b4 = 0;
    c.b5 = 1;

    Traiter();

    c.b2 = 1;
    c.b3 = 1;
    c.b4 = 0;
    c.b5 = 1;

    Traiter();

    c.b2 = 1;
    c.b3 = 0;
    c.b4 = 1;
    c.b5 = 1;

    Traiter();

    c.b2 = 1;
    c.b3 = 1;
    c.b4 = 1;
    c.b5 = 1;

    Traiter();

/* On doit trouver :
c  =   24       c1 =   24
c1 =   24
-----------------------------
c  =   2C       c1 =   2C
c1 =   3C
-----------------------------
c  =   34       c1 =   34
c1 =   24
-----------------------------
c  =   3C       c1 =   3C
c1 =   3C
-----------------------------
*/
}

/*----------------------------------------------------------------------------*/

static void DoValidTCP( void )
{
    /* Recuperer le bit 4 dans le bit 5 */
    OldTCP.b.b4 = 1;
    OldTCP.b.b5 = 0;
    OldTCP.b.b6 = 1;
    OldTCP.b.b7 = 0;

    TraiterTCP();

    OldTCP.b.b4 = 0;
    OldTCP.b.b5 = 1;
    OldTCP.b.b6 = 0;
    OldTCP.b.b7 = 1;

    TraiterTCP();

    OldTCP.b.b4 = 0;
    OldTCP.b.b5 = 0;
    OldTCP.b.b6 = 0;
    OldTCP.b.b7 = 0;

    TraiterTCP();

    OldTCP.b.b4 = 1;
    OldTCP.b.b5 = 1;
    OldTCP.b.b6 = 1;
    OldTCP.b.b7 = 1;

    TraiterTCP();

/* 

On doit trouver : 0101 -> 1111, 1010->0000

new    =   50   new dec =   F0
new    =   A0   new dec =    0
new    =    0   new dec =    0
new    =   F0   new dec =   F0

*/
}

/*----------------------------------------------------------------------------*/

#define UINT32      unsigned int

int ReceiveReady( UINT32 iSpcr )
{
    return ( ( iSpcr >> 0x01 )  & 0x00000001 );
}

int TransmitReady( UINT32 iSpcr )
{
    return ( ( iSpcr >> 0x11 ) & 0x00000001 );
}

#define RReady(X) \
    ( ( X >> 0x01 )  & 0x00000001 )

#define XReady(X) \
    ( ( X >> 0x11 ) & 0x00000001 )

/*----------------------------------------------------------------------------*/

static void DoTestRXReady( void)
{
    UINT32 SPCR;
    int rf;

    SPCR = 0x02;

    rf = RReady( SPCR );
    c_printf("R is ready : %d\n", rf );

    rf = XReady( SPCR );
    c_printf("X is not ready : %d\n", rf );

    SPCR = 0x020000;

    rf = RReady( SPCR );
    c_printf("R is not ready : %d\n", rf );

    rf = XReady( SPCR );
    c_printf("X is ready : %d\n", rf );

/* Affichage
R is ready : 1
X is not ready : 0
R is not ready : 0
X is ready : 1
*/
    SPCR = 0x02;

    rf = ReceiveReady( SPCR );
    c_printf("R is ready : %d\n", rf );

    rf = TransmitReady( SPCR );
    c_printf("X is not ready : %d\n", rf );

    SPCR = 0x020000;

    rf = ReceiveReady( SPCR );
    c_printf("R is not ready : %d\n", rf );

    rf = TransmitReady( SPCR );
    c_printf("X is ready : %d\n", rf );

}

/*---------------------------------------------------------------------------*/

unsigned long _LEDs_reg = 0xFFFFFFFF;

//#define LED1_on           *(volatile int *)0x90080000 = 0x0E000000
//#define LED2_on           *(volatile int *)0x90080000 = 0x0D000000
//#define LED3_on           *(volatile int *)0x90080000 = 0x0B000000
#define LEDs_on           _LEDs_reg = 0x08000000
#define LEDs_off          _LEDs_reg = 0x07000000

#define LED1_on           _LEDs_reg &= ~0x01000000
#define LED1_off          _LEDs_reg |=  0x01000000

#define LED2_on           _LEDs_reg &= ~0x02000000
#define LED2_off          _LEDs_reg |=  0x02000000

#define LED3_on           _LEDs_reg &= ~0x04000000
#define LED3_off          _LEDs_reg |=  0x04000000

/*----------------------------------------------------------------------------*/

static void DoValidLed( void )
{
    LED1_on;
    printf( "LED 1 on  : 0x%08x\n", _LEDs_reg );

    LED1_off;
    printf( "LED 1 off : 0x%08x\n", _LEDs_reg );

    LED2_on;
    printf( "LED 2 on  : 0x%08x\n", _LEDs_reg );

    LED2_off;
    printf( "LED 2 off : 0x%08x\n", _LEDs_reg );

    LED3_on;
    printf( "LED 3 on  : 0x%08x\n", _LEDs_reg );

    LED3_off;
    printf( "LED 3 off : 0x%08x\n", _LEDs_reg );

    LED1_on;
    printf( "LED 1 on  : 0x%08x\n", _LEDs_reg );
    LED2_on;
    printf( "LED 2 on  : 0x%08x\n", _LEDs_reg );
    LED3_on;
    printf( "LED 3 on  : 0x%08x\n", _LEDs_reg );

    LED3_off;
    printf( "LED 3 off : 0x%08x\n", _LEDs_reg );

    LED2_off;
    printf( "LED 2 off : 0x%08x\n", _LEDs_reg );

    LED1_off;
    printf( "LED 1 off : 0x%08x\n", _LEDs_reg );

    LEDs_on;
    printf( "LEDs on   : 0x%08x\n", _LEDs_reg );

    LEDs_off;
    printf( "LEDs off  : 0x%08x\n", _LEDs_reg );
}

/*----------------------------------------------------------------------------*/

PMMENU_BEGIN(Bit, "Bit - Tests")
   PMMENU_ITEM_EX( 1, "Table de verite", DoPrintTable)
   PMMENU_ITEM_EX( 2, "Test bit", DoTestBit)
   PMMENU_ITEM_EX( 3, "Validation", DoValidTestBit)
   PMMENU_ITEM_EX( 4, "Test RX TX ready", DoTestRXReady)
   PMMENU_ITEM_EX( 5, "Validation TCP", DoValidTCP)
   PMMENU_ITEM_EX( 6, "Validation led", DoValidLed)
PMMENU_END()

/*----------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER(Bit)
