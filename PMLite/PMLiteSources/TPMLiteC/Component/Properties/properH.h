/*--------------------------------------------------------------------------*\
 * properH.h - For components that needed to deal with properties.
 * Macro defintions - Internal header.
 *----------------------------------------------------------------------------
 * Written by Bruno Raby (braby) - 2000.
\*--------------------------------------------------------------------------*/

#ifndef _PROPER_H_H_
#define _PROPER_H_H_

/*--------------------------------------------------------------------------*\
 * Macros for analysing the properties mode
\*--------------------------------------------------------------------------*/

/* Propertie 1, 2 */
#define P_PROPERTIE1   \
   ( P_PROPERTIE1_TYPE1 | P_PROPERTIE1_TYPE2 | P_PROPERTIE1_TYPE3 )

#define P_PROPERTIE2   \
   ( P_PROPERTIE2_TYPE1 | P_PROPERTIE2_TYPE2 )

/*--------------------------------------------------------------------------*/

/* Macros for analysing the component's type for the propertie 1 */
#define P_IS_PROPERTIE1_TYPE1(x) ( ((x) & P_PROPERTIE1) == P_PROPERTIE1_TYPE1 )
#define P_IS_PROPERTIE1_TYPE2(x) ( ((x) & P_PROPERTIE1) == P_PROPERTIE1_TYPE2 )
#define P_IS_PROPERTIE1_TYPE3(x) ( ((x) & P_PROPERTIE1) == P_PROPERTIE1_TYPE3 )

/* Macros for analysing the component's type for the propertie 2 */
#define P_IS_PROPERTIE2_TYPE1(x) ( ((x) & P_PROPERTIE2) == P_PROPERTIE2_TYPE1 )
#define P_IS_PROPERTIE2_TYPE2(x) ( ((x) & P_PROPERTIE2) == P_PROPERTIE2_TYPE2 )

/*--------------------------------------------------------------------------*/

/* Has the component a propertie 1 ? */
#define P_COMPONENT_HAS_PROPERTIE1(x) \
   ( ((x) & (~P_PROPERTIE1)) == 0 ) && \
   ( P_IS_PROPERTIE1_TYPE1(x) || P_IS_PROPERTIE1_TYPE2(x) || P_IS_PROPERTIE1_TYPE3(x) )

/* Has the component a propertie 1 ? */
#define P_COMPONENT_HAS_PROPERTIE2(x) \
   ( ((x) & (~P_PROPERTIE2)) == 0 ) && \
   ( P_IS_PROPERTIE2_TYPE1(x) || P_IS_PROPERTIE2_TYPE2(x) )

/*--------------------------------------------------------------------------*/

#endif /* _PROPER_H_H_ */


