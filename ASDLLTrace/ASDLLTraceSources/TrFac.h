/*	===================================================================
	TrFac.h
	Copyright (C) 1990 - 2000 AbyreSoft - All rights reserved.
	===================================================================	*/

#ifndef TrFac_h
#define TrFac_h

/*	------------------------------------------------------------------- */
#include "TrHelp.h"

/*	------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*	------------------------------------------------------------------- */
/* 
This should do nothing except filling the "virtual" table.
The Open Proc will then be called to make initializations.
These DLL Entry points will be called by the Main Trace DLL.
*/
TTracer* __stdcall	as_trace_create_instance(char* aName, char* aParam);
void __stdcall		as_trace_destroy_instance(TTracer* aTracer);

/*	------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/*	------------------------------------------------------------------- */
#endif /* TrFac_h */
