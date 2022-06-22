/*	===================================================================
	TrTStdEr.h
	Copyright (C) 1990 - 2000 AbyreSoft - All rights reserved.
	===================================================================	*/

#ifndef TrTStdEr_h
#define TrTStdEr_h

/*	------------------------------------------------------------------- */
#include "OSWinInc.h"
#include <stdio.h>

/*	------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*	------------------------------------------------------------------- */
/*	StdErr tracer uses the same structure as FileTracer */
/*	------------------------------------------------------------------- */
TTracer*	TrStdErr_Create(void* aParam);

/*	------------------------------------------------------------------- */
int		TrStdErr_OpenProc(TTracer* aTracer);
void	TrStdErr_CloseProc(TTracer* aTracer);
void	TrStdErr_DestroyProc(TTracer* aTracer);

/*	------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/*	------------------------------------------------------------------- */
#endif /* TrTStdEr_h */
