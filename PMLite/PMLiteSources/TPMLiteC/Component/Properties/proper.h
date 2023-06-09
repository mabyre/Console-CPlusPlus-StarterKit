/*--------------------------------------------------------------------------*\
 * proper.h - For components that needed to deal with properties.
 * User definitions - External header.
 *----------------------------------------------------------------------------
 * Written by Bruno Raby (braby) - 2000.
\*--------------------------------------------------------------------------*/

#ifndef _PROPER_H_
#define _PROPER_H_

/*--------------------------------------------------------------------------*/

#include <errno.h>
#include "properH.h"

/*--------------------------------------------------------------------------*/

/* Properties error */
#define P_ERROR   EPERM  /* User error for properties */

/*--------------------------------------------------------------------------*/

/* Properties type 1 */
#define  P_PROPERTIE1_TYPE1   0x00000000    /* Type 1 */
#define  P_PROPERTIE1_TYPE2   0x00000001    /* Type 2 */
#define  P_PROPERTIE1_TYPE3   0x00000002    /* Type 3 */

/* Properties type 2 */
#define  P_PROPERTIE2_TYPE1   0x00000004    /* P2 T1 */
#define  P_PROPERTIE2_TYPE2   0x00000008    /* P2 T2 */

/* Combined properties */
#define  P_P1_T1_AND_P2_T2    ( P_PROPERTIE1_TYPE1 | P_PROPERTIE2_TYPE2 )

/*--------------------------------------------------------------------------*/

#endif /* _PROPER_H_ */
