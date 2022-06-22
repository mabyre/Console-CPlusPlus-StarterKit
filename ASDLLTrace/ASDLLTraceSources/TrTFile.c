/*--------------------------------------------------------------------------*\
 * TrTFile.c
 * Copyright (C) 1990 - 1996 AbyreSoft - All rights reserved.
\*--------------------------------------------------------------------------*/

#include "TrHelp.h"
#include "TrTFile.h"

/*--------------------------------------------------------------------------*/

#define TRACE_FILE	"PMLite.log"

/*--------------------------------------------------------------------------*/

TTracer* TrFile_Create( void* aParam )
{
	TFileTracer*	theTracer;

	if (aParam == 0)
		aParam = TRACE_FILE;

	theTracer = malloc(sizeof(TFileTracer));
	theTracer->itsfOpened = FALSE;
	theTracer->itsKind = TraceKind_Normal;
	theTracer->itsOpenProc = TrFile_OpenProc;
	theTracer->itsCloseProc = TrFile_CloseProc;
	theTracer->itsSendProc = TrFile_SendProc;
	theTracer->itsDestroyProc = TrFile_DestroyProc;
	theTracer->itsFile = 0;
	strcpy(theTracer->itsFileName, (char*)aParam);

	return (TTracer*)theTracer;
}

/*--------------------------------------------------------------------------*/

BOOL TrFile_OpenProc( TTracer* aTracer )
{
	if (aTracer->itsfOpened)
		return TRUE;
	
	((TFileTracer*)aTracer)->itsFile = fopen(((TFileTracer*)aTracer)->itsFileName, "w");
	if (((TFileTracer*)aTracer)->itsFile == 0)
		return FALSE;
	
	aTracer->itsfOpened = TRUE;
	return TRUE;
}

/*--------------------------------------------------------------------------*/

void TrFile_CloseProc( TTracer* aTracer )
{
	if (!aTracer->itsfOpened)
		return;
	
	fclose(((TFileTracer*)aTracer)->itsFile);
	((TFileTracer*)aTracer)->itsFile = 0;
	aTracer->itsfOpened = FALSE;
}

/*--------------------------------------------------------------------------*/

#define DUMP_LINE_LEN	16
void TrFile_SendProc( TTracer* aTracer, TTraceSendParams* aParams )
{
	static char	sHexDigits[] = "0123456789ABCDEF";

	if (!aTracer->itsfOpened)
		return;
	
	if (aParams->itsKind == TraceKind_Binary)
	{
		size_t	theSize, i;
		char	theBuffer[DUMP_LINE_LEN * 3 + 2 + DUMP_LINE_LEN + 1];
		BYTE*	theDataPtr, *theBufferPtr;

		memset(theBuffer, ' ', sizeof(theBuffer));
		theDataPtr = aParams->itsData;
		while (aParams->itsDataLen > 0)
		{
			if (aParams->itsDataLen > DUMP_LINE_LEN)
				theSize = DUMP_LINE_LEN;
			else
				theSize = aParams->itsDataLen;

			theBufferPtr = theBuffer;
			for (i = 0; i < theSize; i++, theBufferPtr += 3)
			{
				theBufferPtr[0] = sHexDigits[theDataPtr[i] >> 4];
				theBufferPtr[1] = sHexDigits[theDataPtr[i] & 0x0F];
			}

			if (theSize < DUMP_LINE_LEN)
			{
				memset(theBufferPtr, ' ', 3 * (DUMP_LINE_LEN - theSize));
				theBufferPtr += 3 * (DUMP_LINE_LEN - theSize);
			}
			theBufferPtr += 2;

			for (i = 0; i < theSize; i++)
			{
				if (theDataPtr[i] < 0x20)
					*theBufferPtr++ = '.';
				else
					*theBufferPtr++ = theDataPtr[i];
			}

			*theBufferPtr = 0;
			fprintf(((TFileTracer*)aTracer)->itsFile, "%s\n", theBuffer);

			aParams->itsDataLen -= theSize;
			theDataPtr += theSize;
		}
	}
	else
	{
		fprintf(((TFileTracer*)aTracer)->itsFile, "%s", aParams->itsData);
		if (aParams->itsKind == TraceKind_EOL)
			fprintf(((TFileTracer*)aTracer)->itsFile, "\n");
	}
}

/*--------------------------------------------------------------------------*/

void TrFile_DestroyProc( TTracer* aTracer )
{
	if (aTracer->itsfOpened)
		TrFile_CloseProc(aTracer);
	free(aTracer);
}
