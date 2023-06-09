/*--------------------------------------------------------------------------*\
 * TChar.c
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
\*--------------------------------------------------------------------------*/

#include <conio.h>
#include <ctype.h>
#include "cMenu.h"
#include "cCore.h"
#include "cConsole.h"
#include "cInput.h"
#include "pmXDebug.h"

/*--------------------------------------------------------------------------*/

int TestExtendToAscii( int c )
{
static int tab_code[127] = 
{ 
   0xC7, 0xFC, 0xE9, 0xE2, 0xE4, 0xE0, 0xE5, 0xE7, 0xEA, 0xEB,
   0xE8, 0xEF, 0xEE, 0xEC, 0xC4, 0xC5, 0xC9, 0xE6, 0xC6, 0xF4,
   0xF6, 0xF2, 0xFB, 0xF9, 0xFF, 0xD6, 0xDC, 0xF8, 0xA3, 0xD8,
   0xD7, 0x83, 0xE1, 0xED, 0xF3, 0xFA, 0xF1, 0xD1, 0xAA, 0xBA,
   0xBF, 0xAE, 0xAC, 0xBD, 0xBC, 0xA1, 0xAB, 0xBB, 0x5F, 0x5F,
   0x5F, 0xA6, 0xA6, 0xC1, 0xC2, 0xC0, 0xA9, 0xA6, 0xA6, 0x2B,
   0x2B, 0xA2, 0xA5, 0x2B, 0x2B, 0x2D, 0x2D, 0x2B, 0x2D, 0x2B,
   0xE3, 0xC3, 0x2B, 0x2B, 0x2D, 0x2D, 0xA6, 0x2D, 0x2B, 0xA4,
   0xF0, 0xD0, 0xCA, 0xCB, 0xC8, 0x69, 0xCD, 0xCE, 0xCF, 0x2B,
   0x2B, 0x5F, 0x5F, 0xA6, 0xCC, 0x5F, 0xD3, 0xDF, 0xD4, 0xD2,
   0xF5, 0xD5, 0xB5, 0xFE, 0xDE, 0xDA, 0xDB, 0xD9, 0xFD, 0xDD,
   0xAF, 0xB4, 0xAD, 0xB1, 0x5F, 0xBE, 0xB6, 0xA7, 0xF7, 0xB8,
   0xB0, 0xA8, 0xB7, 0xB9, 0xB3, 0xB2, 0x5F
};

int i;

   i = 0;
   while ( ( c != tab_code[i] )  &&  ( i < 127 ) )
   {
      i++ ;
   }
   
   return 0x80 + i;
}

/*--------------------------------------------------------------------------*/

void DoPrintAscii( void )
{
	int c = 0, c1 ;

	for ( c = 0 ; c < 255 ; c++  )
	{
      c1 = toascii( c );
      c_printf("%4d [%c]", c1, c1 );
		if ((c % 9) == 0) c_printf("\n");
	}
}

/*--------------------------------------------------------------------------*/

void DoPrintAllChar( void )
{
	int c = 0;

	for ( c = 0 ; c < 255 ; c++  )
	{
      c_printf("%4d [%c]", c, c );
		if ((c % 9) == 0) c_printf("\n");
	}
}

/*--------------------------------------------------------------------------*/

void DoPrintAllCharInHexa( void )
{
	int c = 0 ;

	for ( c = 0 ; c < 255 ; c++  )
	{
      c_printf("%3X [%c]", c, c );
		if ((c % 10) == 0) c_printf("\n");
	}
}

/*--------------------------------------------------------------------------*/

void DoPrintRangeChar( void )
{
/* 128 - 255
ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜø£Ø×ƒáíóúñÑªº¿®¬½¼¡«»___¦¦ÁÂÀ©¦¦++¢¥++--+-+ãÃ++--¦-+¤
ðÐÊËÈiÍÎÏ++__¦Ì_ÓßÔÒõÕµþÞÚÛÙýÝ¯´­±_¾¶§÷¸°¨·¹³²_
*/
   int c = 0 ;

	for ( c = 128 ; c < 255 ; c++  )
	{
		c_printf("%c", c );
	}
	c_printf("\n\n");
	for ( c = 128 ; c < 255 ; c++  )
	{
		c_printf("%0xX ", c );
	}

}

/*--------------------------------------------------------------------------*/

void DoTestSpecialChar( void )
{
	char c = 0 ;
   char *cmdbuf = "abcdefghij";

	c = '\n' ;
	c_printf("c : [%c] [%02X] [%d] \n", c , c , c) ;

	c = '\r' ;
	c_printf("c : [%c] [%02X] [%d] \n", c , c , c) ;

   c_printf("\n---------\n");
   c_printf("[\010 \010]");
   c_printf("[\010 \010]");
   c_printf("[\010 \010]");
   c_printf("[\010 \010]");

   c_printf("\n---------\n");
   c_printf("\x1b");

   c_printf("\n---------\n");
   c_printf("\x1b[M\x0d# %s\x1b[K",cmdbuf);


}

/*--------------------------------------------------------------------------*/

void DoEnterChar( void )
{
	int c = 'a';

   c_printf("Touch a key ? - <x> to exit\n");
   while ( c != 'x' )
   {
      c = _getch();
	   c_printf("c:[%c], x:[%02X], d:[%d]\n", c , c , c) ;
   }
}

/*--------------------------------------------------------------------------*/

void DoEnterInt( void )
{
	int c = 'a';

   c_printf("Touch a key ? - <0> to exit\n");
   while ( c != 0 )
   {
      c = c_get_int32();
	   c_printf("c:[%c], x:[%02X], d:[%d]\n", c , c , c) ;
   }
}

/*--------------------------------------------------------------------------*/

void DoTranslatetAccent1( void  )
{
/*
"ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜø£Ø×ƒáíóúñÑªº¿®¬½¼¡«»___¦¦ÁÂÀ©¦¦++"
"¢¥++--+-+ãÃ++--¦-+¤ðÐÊËÈiÍÎÏ++__¦Ì_ÓßÔÒõÕµþÞÚÛÙýÝ¯´­±_¾¶§÷¸°¨·¹³²_";
*/
   static const char table_conversion[] =
   "CueaaaaceeeiiiAAEæÆooouuyOUø£Ø×ƒaiounNªº¿®¬½¼¡«»___¦¦AAA©¦¦++"
   "¢¥++--+-+aA++--¦-+¤ðÐEEEiIII++__¦Ì_ÓßÔÒõÕµþÞUUUyY¯´­±_¾¶§÷¸°¨·¹³²_";

   int c = 0, c1;
   int   nb_accent = 0;
   FILE *file;
   int   i;

   char buffer[50];

   /* Ouvrir le fichier a traiter */
   file = fopen( "aa.txt", "rb" );
   if ( file == NULL )
   {
      c_printf("Impossible d'ouvrir le fichier !\n");
      exit(EXIT_FAILURE);
   }

   i = 0;
   while ( ( c = getc( file ) ) != EOF )
   {
      c1 = toascii( c );
      printf("%3d %c\n", (char)c , (char)c );

      buffer[i++] = (char)c;
      printf("%3d %c\n", (char)c , (char)c );
      PM_DEBUG3(TL("%3d %c", c , c ));
 /*     
      if ( c < 128 )
      {
         DEBUG3("%d %c", c , c );
      }
      else
      {
         DEBUG3("tc : %d %c", c - 128, table_conversion[c - 128] );
      }
 */
   }
   buffer[i] = '\0';

   printf("%s", buffer );
   PM_DEBUG3(TL("%s", buffer ));

   fclose( file );
}

/*--------------------------------------------------------------------------*/

void DoTranslatetAccent( void  )
{
   static const char table_conversion[] =
   "CueaaaaceeeiiiAAEæÆooouuyOUø£Ø×ƒaiounNªº¿®¬½¼¡«»___¦¦AAA©¦¦++"
   "¢¥++--+-+aA++--¦-+¤ðÐEEEiIII++__¦Ì_ÓßÔÒõÕµþÞUUUyY¯´­±_¾¶§÷¸°¨·¹³²_";

   int   c = 0;
   int   nb_accent = 0;
   FILE *file, *file_out;
   int   i;

   char buffer[50];

   /* Ouvrir le fichier a traiter */
   file = fopen( "aa.txt", "rb" );
   if ( file == NULL )
   {
      c_printf("Impossible d'ouvrir le fichier !\n");
      exit(EXIT_FAILURE);
   }
   /* Ouvrir le fichier resultat */
   file_out = fopen( "bb.txt", "wb" );
   if ( file_out == NULL )
   {
      c_printf("Impossible d'ouvrir le fichier !\n");
      exit(EXIT_FAILURE);
   }

   i = 0;
   while ( ( c = getc( file ) ) != EOF )
   {
      c = TestExtendToAscii( c );
      printf("%3d %c\n", c , c );
      PM_DEBUG3(TL("%3d %c", c , c ));

      printf("Sans accent :\n");
      if ( c < 128 )
      {
      }
      else
      {
         c = table_conversion[ c - 128 ];
         putc( c , file_out );
         printf("tc : %d %c\n", c );
      }
      buffer[i++] = (char)c;
   }
   buffer[i] = '\0';

   printf("%s\n", buffer );
   PM_DEBUG3(TL("%s", buffer ));

   fclose( file_out );
   fclose( file );
}

/*--------------------------------------------------------------------------*/

void DoWriteInFile( void )
{
   char tab[] = "ùàäâéèëêïîüûÿ";
   char *c = tab;
   FILE *file;
   int i;

   file = fopen( "aa.txt", "wb" );
   if ( file == NULL )
   {
      c_printf("Impossible d'ouvrir le fichier !\n");
      exit(EXIT_FAILURE);
   }

   for ( i = 0 ; i < 4 ; i++ )
   {
      putc( (int)(*c), file );
      
      c_printf("%4d [%c]\n", *c, *c );
      c++;
   }
   fclose( file );

}

/*--------------------------------------------------------------------------*/

void DoPrintSpecialChar( void )
{
   char c;

   c = '\a';
   printf("\\a Bell (alert) : %X\n", c );

   c= '\b';
   printf("\\b Backspace :%X\n", c);

   c= '\f';
   printf("\\f Formfeed :%X\n", c);

   c= '\n';
   printf("\\n New line :%X\n", c);

   c= '\r';
   printf("\\r Carriage return :%X\n", c);
/*
\a Bell (alert) : 7
\b Backspace :8
\f Formfeed :C
\n New line :A
\r Carriage return :D
*/
/*
\b Backspace 
\f Formfeed 
\n New line 
\r Carriage return 
\t Horizontal tab 
\v Vertical tab 
\' Single quotation mark 
\"  Double quotation mark 
\\ Backslash 
\? Literal question mark 
\ooo ASCII character in octal notation 
\xhhh ASCII character in hexadecimal notation 
*/

}

/*--------------------------------------------------------------------------*/

PMMENU_BEGIN(Char, "Test Chars")
	PMMENU_ITEM_EX(1, "Print all chars"         , DoPrintAllChar)
	PMMENU_ITEM_EX(2, "Print all chars in hexa" , DoPrintAllCharInHexa)
	PMMENU_ITEM_EX(3, "Print range of chars"    , DoPrintRangeChar)
	PMMENU_ITEM_EX(4, "Test special chars"      , DoTestSpecialChar)
	PMMENU_ITEM_EX(5, "Enter a char"            , DoEnterChar)
	PMMENU_ITEM_EX(6, "Enter an int"            , DoEnterInt)
	PMMENU_ITEM_EX(7, "Translate"               , DoTranslatetAccent)
	PMMENU_ITEM_EX(8, "Write in file"           , DoWriteInFile)
	PMMENU_ITEM_EX(9, "Print ascci"             , DoPrintAscii)
	PMMENU_ITEM_EX(10,"Print special char"      , DoPrintSpecialChar)
PMMENU_END()

/*--------------------------------------------------------------------------*/

PMMENU_DEFAULT_HANDLER(Char)
