/*--------------------------------------------------------------------------*\
 * TTime.c 
 * Copyright (c) 1998-2000 AbyreSoft. Written by Bruno Raby.
 *--------------------------------------------------------------------------*
 * Le temps sur la plateforme Windows NT
 *--------------------------------------------------------------------------*
 * Utilise :
 *    GetTickCount
 *    fonctions associees au timer de haute performance :
 *    QueryPerformanceCounter 
 *    QueryPerformanceFrequency
 * Implemente :
 *    StartTickCounter
 *    GetTimeUntilStart
\*--------------------------------------------------------------------------*/

#include <windows.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <time.h>

#include "pmXTrace.h"
#include "cMenu.h"
#include "cCore.h"
#include "cConsole.h"
#include "cInput.h"

/*---------------------------------------------------------------------------*/

static long double PeriodeCounter = 0;
LARGE_INTEGER PerformanceCounterDebut;

/*---------------------------------------------------------------------------*/

void sleep( clock_t wait );

void StartTickCounter( void );
double GetTimeUntilStart( void );

/*---------------------------------------------------------------------------*/

void DoTestTimePerformance( void )
{
   long    i = 600000L;
   volatile double a = 1.1111111;

   LARGE_INTEGER Frequency;
   
   unsigned long TimeAdjustment;         // size of time adjustment
   unsigned long TimeIncrement;          // time between adjustments
   unsigned long TimeAdjustmentDisabled; // disable option

   unsigned long debut, fin;
   long double duration;
   long double Periode = 0;

   LONGLONG duration_2;

   int result;

   LARGE_INTEGER PerformanceCounter;
   LARGE_INTEGER PerformanceCounterDebut;
   LARGE_INTEGER PerformanceCounterFin;

   Frequency.QuadPart = 0;
   PerformanceCounter.QuadPart      = 0;
   PerformanceCounterDebut.QuadPart = 0;
   PerformanceCounterFin.QuadPart   = 0;

   /*-----------*\
    * Frequency *
   \*-----------*/
   printf("\n**** Frequency test\n");
   if ( QueryPerformanceFrequency( &Frequency ) != 0 )
   {
      printf("YES hardware supports a high-resolution performance counter.\n");
   }
   else
   {
      printf("NO hardware does not supports a high-resolution performance counter.\n");
   }
   printf("Actual frequency : %d\n", Frequency.QuadPart );
   Periode = 1/(double)Frequency.QuadPart;
   printf("Actual periode   : %g\n\n", Periode );

   /*--------------------*\
    * PerformanceCounter *
   \*--------------------*/
   printf("**** PerformanceCounter test\n");
   if ( QueryPerformanceCounter( &PerformanceCounter ) != 0 )
   {
      printf("YES hardware supports a high-resolution performance counter.\n");
   }
   else
   {
      printf("NO hardware does not supports a high-resolution performance counter.\n");
   }

   printf("Actual counter value is : %ul\n\n", PerformanceCounter );

   /*----------------------*\
    * SystemTimeAdjustment *
   \*----------------------*/
   printf("**** SystemTimeAdjustment test\n");
   result = GetSystemTimeAdjustment( &TimeAdjustment, &TimeIncrement, &TimeAdjustmentDisabled );
   if ( result != 0 )
   {
      printf("YES the system is applying periodic time adjustments \n");
      printf("Number of 100-nanosecond units,\nadded to the time-of-day clock at each periodic time adjustment is : %u\n", TimeAdjustment );
      printf("Number of 100-nanosecond units,\nbetween periodic time adjustments is : %u\n", TimeIncrement );
      printf("Periodic time adjustment is in effect !\n\n");
   }
   else
   {
      printf("NO the system is applying periodic time adjustments\n");
      printf("The periodic time adjustment is NOT in effect !\n\n");
   }

   /*------------------*\
    * Mesures de temps *
   \*------------------*/
   printf("**** Time mesurement test\n");

   /* Delay for a specified time. */
   printf("Delay for 1 seconds\n");
   sleep( (clock_t)1 * CLOCKS_PER_SEC ); /* Attente d'une seconde */
   printf("Done!\n" );

   /* Measure the duration of an event. */
   printf("Time to do %ld loops is ", 10000000 );
   debut = GetTickCount();
   QueryPerformanceCounter( &PerformanceCounterDebut );
   for ( i = 0 ; i < 10000000 ; i++ )
   {
      a = a * 0.1111111;
   }
   fin = GetTickCount();
   QueryPerformanceCounter( &PerformanceCounterFin );

   duration = (long double)(fin - debut);
   printf("%f milli seconds\n", duration );

   duration_2 = (PerformanceCounterFin.QuadPart - PerformanceCounterDebut.QuadPart);
   duration = duration_2 * Periode;
   printf("%10.10g seconds\n", duration );

/* Affichage
**** Frequency test
YES hardware supports a high-resolution performance counter.
Actual frequency : 1193182
Actual periode   : 8.38095e-007

**** PerformanceCounter test
YES hardware supports a high-resolution performance counter.
Actual counter value is : 1956033142l

**** SystemTimeAdjustment test
YES the system is applying periodic time adjustments
Number of 100-nanosecond units,
added to the time-of-day clock at each periodic time adjustment is : 100144
Number of 100-nanosecond units,
between periodic time adjustments is : 100144
Periodic time adjustment is in effect !

**** Time mesurement test
Delay for three seconds
Done!
Time to do 10000000 loops is 471.000000 milli seconds
0.4696332999 seconds
*/
}

/*---------------------------------------------------------------------------*\
 * Calcul la periode une fois puis memorise le compteur de debut
\*---------------------------------------------------------------------------*/

void StartTickCounter( void )
{
   static int    faire_une_fois = 1;
   LARGE_INTEGER frequency;

   /* Calculer la periode du compteur de haute performance */
   if ( faire_une_fois )
   {
      if ( QueryPerformanceFrequency( &frequency ) != 0 )
      {
         PeriodeCounter = 1/(double)frequency.QuadPart; /* T = 1/f */      
      }
      else
      {
         PeriodeCounter = 0;
      }
      faire_une_fois = 0;
   }

   /* Memoriser le compteur de debut */
   QueryPerformanceCounter( &PerformanceCounterDebut );
}

/*---------------------------------------------------------------------------*\
 * Retourne le temps en secondes depuis StartTickCounter()
\*---------------------------------------------------------------------------*/

double GetTimeUntilStart( void )
{
   LONGLONG duration;
   LARGE_INTEGER counter_fin;

   QueryPerformanceCounter( &counter_fin );
   duration = counter_fin.QuadPart - PerformanceCounterDebut.QuadPart;
   return duration * PeriodeCounter;
}

/*---------------------------------------------------------------------------*\
 * Pauses for a specified number of milliseconds.
\*---------------------------------------------------------------------------*/

void sleep( clock_t wait )
{
   clock_t goal;
   goal = wait + clock();
   while( goal > clock() )
      ;
}

/*---------------------------------------------------------------------------*\
 * Fonction d'expiration - Elle sert sur plateforme si une interruption
 * vient allonger l'attente du delai au dela d'un temps raisonnable
 * On specifie un temps min et un temps max
 *---------------------------------------------------------------------------*
 * iDelai : delai d'attente min
 * iExpire : delai d'attente max
 * retourne 1 si on sortie par le temps min retourne 0 si on est sortie
 * par expiration
\*---------------------------------------------------------------------------*/

int WaitForDelaiUntilExpire( clock_t iDelai , clock_t iExpire )
{
    clock_t delai;
    clock_t expire;
    clock_t reference;

    int bool_delai = 1, bool_expire = 1;

    reference = clock();
    delai = iDelai + reference;
    expire = iExpire + reference;

    PM_TRACE0(TL("delai : %d"));
    PM_TRACE0(TL("expire : %d"));

    while( bool_delai && bool_expire )
    {
        bool_delai = ( delai > clock() );
        bool_expire = ( expire > clock() );
        PM_TRACE0(TL("bool_delai : %d",bool_delai));
        PM_TRACE0(TL("bool_expire : %d",bool_expire));
    }

    if ( ! bool_delai ) return 1;
    if ( ! bool_expire ) return 0;

    /* Avoid compilation warning */
    return 0;
}

/*---------------------------------------------------------------------------*/

static void DoTestDelai( void )
{
   /* Delai est inferieur au temps d'expiration */
   if ( WaitForDelaiUntilExpire( 1 * CLOCKS_PER_SEC , 2 * CLOCKS_PER_SEC ) )
   {
      c_printf("Sortie par delai\n");
   }
   else
   {
      c_printf("Sortie par expiration\n");
   }

   /* Expiration plus courte que le delai */
   if ( WaitForDelaiUntilExpire( 2 * CLOCKS_PER_SEC , 1 * CLOCKS_PER_SEC ) )
   {
      c_printf("Sortie par delai\n");
   }
   else
   {
      c_printf("Sortie par expiration\n");
   }

/* Affichage
Sortie par delai
Sortie par expiration
*/
}

/*---------------------------------------------------------------------------*/

void DoTestTime1()
{
   int i;
   volatile double a = 1.1111111;

   struct _timeb timebuffer_begin;
   struct _timeb timebuffer_end;

   _ftime( &timebuffer_begin );
   

   for ( i = 0 ; i < 100000 ; i++ )
   {
      a = a * 0.1111111;
   }

   _ftime( &timebuffer_end );

   printf("\nTemps ecoule : %d\n", timebuffer_end.millitm - timebuffer_begin.millitm );
}

/*---------------------------------------------------------------------------*/

void DoTestPrecisionSleep( void )
{
   double duration;

   /* Delay for a specified time. */
   printf("Attendre 1 seconde\n");
   
   StartTickCounter();
   sleep( (clock_t)1 * CLOCKS_PER_SEC );
   duration = GetTimeUntilStart();

   printf("C'est fait !\n" );
   printf("ca a dure : %10.10g secondes\n", duration );
}

/*---------------------------------------------------------------------------*/

void DoTestPrecisionGetTickCount( void )
{
   long i;
   volatile double a = 1.1111111;

   long double duration_1, duration_2;
   unsigned long debut, fin;

   debut = GetTickCount();
   StartTickCounter();
   for ( i = 0 ; i < 10000000 ; i++ )
   {
      a = a * 0.1111111;
   }
   fin = GetTickCount();
   duration_1 = GetTimeUntilStart();

   duration_2 = (long double)(fin - debut);
   printf("En nombre de tick ca fait            : %f milli seconds\n", duration_2 );
   printf("Avec le performance compteur ca dure : %10.10g secondes\n", duration_1 );
}

/*---------------------------------------------------------------------------*/

PMMENU_BEGIN( Time, "Time - Les fonctions du temps" )
   PMMENU_ITEM_EX( 1, "Test time performance", DoTestTimePerformance )
   PMMENU_ITEM_EX( 2, "Test de la precision de sleep", DoTestPrecisionSleep )
   PMMENU_ITEM_EX( 3, "Test de la precision de GetTickCount", DoTestPrecisionGetTickCount )
   PMMENU_ITEM_EX( 4, "Test de l'expiration d'un delai", DoTestDelai )
PMMENU_END()

/* ---------------------------------------------------------------------------   */

PMMENU_DEFAULT_HANDLER( Time )
