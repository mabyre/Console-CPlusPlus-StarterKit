/*--------------------------------------------------------------------------*\
 * proper.c - A component that use the properties.
\*--------------------------------------------------------------------------*/

#include "ASDebug.h"
#include "cConsole.h"
#include "proper.h"

/*--------------------------------------------------------------------------*/

typedef struct 
{
   int type;             /* Type of component created */
   /* other caracteristics of the component */

} 
component_t;


/*--------------------------------------------------------------------------*\
 * Create a component. 
 * Example of how can be used the propertie's macros to create a component.
 *
 * componentType : user choice
\*--------------------------------------------------------------------------*/
int Create( int componentType ) 
{

   /* The component needed to have a type of propertie 1 set */
   if ( ! P_COMPONENT_HAS_PROPERTIE1( componentType ) ) 
   {
     DEBUG0("Create : Error : Bad type of component!");
     return -P_ERROR;
   }

   if ( P_COMPONENT_HAS_PROPERTIE1( componentType ) ) 
   {
      c_printf("the component has the properties 1 set");
   }
   
   if ( P_COMPONENT_HAS_PROPERTIE2( componentType ) ) 
   {
      c_printf("the component has the properties 2 set");
   }

   return 0;
}


