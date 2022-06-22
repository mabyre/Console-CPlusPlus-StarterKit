/* =======================================================================
   cSerial.c
   Copyright (C) 1998 AbyreSoft. All rights reserved.
   ======================================================================= */

#include "cSerial.h"
#include "cTrace.h"
#include "cMemory.h"
#include "cCore.h"

/* ----------------------------------------------------------------------- */

/* PMLITE_SERIAL_PRIVATE_IMPLEMENTATION must be defined if the cross platform implementation
   that can be found in this file must *not* be used. In that case, you need to write your own
   local implementation */
   
#ifndef PMLITE_SERIAL_PRIVATE_IMPLEMENTATION

/* ----------------------------------------------------------------------- */

pmerror Serial_Flush(pSerial self)
{
	pmerror		theError;
	pmbyte		theByte;
	size_t		theCount;

	while (pmtrue)
	{
		theError = Serial_ReadBytes(self, &theByte, sizeof(pmbyte), &theCount, 0);
		if ((theError != pmErr_NoErr) || (theCount == 0))
			break;
	}
	
	return theError;
}

/* ----------------------------------------------------------------------- */

pmerror Serial_ReadBytesExactCount(pSerial self, void *aBuffer, size_t *aCount, pmuint32 aTimeout)
{
	pmuint32	theEndTicks = c_xget_tick_count() + aTimeout;	
	pmbyte		*theBufferPtr = aBuffer;
	size_t		theBufferSize = *aCount;
	size_t		theReadCount = 0;
	pmerror		theError;
	
	(*aCount) = 0;
	
	while (pmtrue)
	{
			/*	Read bytes with a 0 timeout. This will immediately give us what is available.	*/
		theError = Serial_ReadBytes(self, theBufferPtr, theBufferSize, &theReadCount, 0);
		if (theError != 0)
			return theError;
			
		theBufferPtr += theReadCount;
		theBufferSize -= theReadCount;
		(*aCount) += theReadCount;
		
			/*	When theBufferSize gets down to 0, it means that we have read all what we wanted.	*/		
		if (theBufferSize == 0)
			return pmErr_NoErr;
			
		if (c_xget_tick_count() > theEndTicks)
			return pmErr_Serial_Timeout;
	}
}

/* ----------------------------------------------------------------------- */

pmerror Serial_ReadDelimitedChunk(pSerial self, char *aBuffer, size_t aMaxSize, size_t *aCount, char *aDelimiter, pmuint32 aTimeout)
{
		/*	Compute the ticks (milliseconds) at which the timeout will have elapsed.	*/
	pmuint32	theEndTicks = c_xget_tick_count() + aTimeout;
	pmuint16	theNextTestIndex = 0;
	pmbyte		theChar;
	pmbyte		theOriginalChar;
	char		theTempBuffer[kDelimiterStringMaxSize];
	pmerror		theError;

	c_assert(self != 0, "Serial_ReadDelimitedChunk(): self must not be 0\n");
	c_assert(aBuffer != 0, "Serial_ReadDelimitedChunk(): aBuffer cannot be 0\n");

		/*	Loop while timeout not elapsed, or strings not found.	*/
	(*aCount) = 0;
	while (pmtrue)
	{
			/*	Compute the remaining time before timeout elapses. We need that number at each loop of the read.	*/		
		size_t	theReadCount;
		pmint32	theCurrentTimeout = (pmint32) (theEndTicks - c_xget_tick_count());	// use long instead of unsigned because result can be < 0
		
		if (theCurrentTimeout <= 0)
			return pmErr_Serial_Timeout;
		
			/*	Read one byte and convert to uppercase (it is assumed that the delimiter IS uppercase.	*/	
		theReadCount = 1;
		theError = Serial_ReadBytesExactCount (self, &theChar, &theReadCount, (pmuint32) theCurrentTimeout);
		if (theError != pmErr_NoErr)
			return theError;

		theOriginalChar = theChar;
		if ((theChar >= 'a') && (theChar <= 'z'))
			theChar -= 32;
		
			/*	See if the read char is the awaited char. If so, don't return it to the caller.	*/
		if (aDelimiter[theNextTestIndex] == theChar)
		{
			theTempBuffer[theNextTestIndex] = (char) theOriginalChar;
			theNextTestIndex++;
			
			if (aDelimiter[theNextTestIndex] == 0)
				return pmErr_NoErr;
		}
		else
		{
				/*	The read char is not the awaited char. We must then copy the chars that have not been returned
					as part of the delimiter, but which now turn out not to be the delimiter.	*/			
			pmuint16 theCopyIndex = 0;
			
			while (theCopyIndex < theNextTestIndex)
			{
				if (*aCount == aMaxSize)
					return pmErr_Serial_BufferTooSmall;
				
				aBuffer[(*aCount)++] = theTempBuffer[theCopyIndex++];
			}
		
				/*	Check that the read char isn't the first char in the searched string. If it is, do
					as if it was found regularly through the above loop, else, return the char to the caller.	*/
			
			if ((*aDelimiter) == theChar)
			{
				theTempBuffer[0] = (char) theOriginalChar;
				theNextTestIndex = 1;
			}
			else
			{
				theNextTestIndex = 0;
				
				if (*aCount == aMaxSize)
					return pmErr_Serial_BufferTooSmall;
				
				aBuffer[(*aCount)++] = (char) theOriginalChar;
			}
		}		
	}
}

/* ----------------------------------------------------------------------- */

pmerror	Serial_WaitFor(pSerial self, char *aString[], pmuint16 aStringCount, pmuint16 *aRcvStringIndex, pmuint32 aTimeout)
{
	pmuint32	theEndTicks = c_xget_tick_count() + aTimeout;
	pmuint16	theNextTestIndex[kWaitForMaxStrings];
	pmuint16	theIndex;
	pmbyte		theChar;
	pmerror		theError;

		/*	There is a limit of kWaitForMaxStrings awaited strings.	*/
	if (aStringCount > kWaitForMaxStrings)
		return pmErr_Serial_OSError;

		/*	Init current position index in the searched string. This index points on the next char to be tested.	*/
	for (theIndex = 0; theIndex < aStringCount; theIndex++)
		theNextTestIndex[theIndex] = 0;

		/*	Loop while timeout not elapsed, or strings not found.	*/	
	while (pmtrue)
	{
			/*	Compute the remaining time before timeout elapses. We need that number at each loop of the read.	*/	
		size_t	theReadCount;
		pmint32 theCurrentTimeout = (pmint32) (theEndTicks - c_xget_tick_count());
		
		if (theCurrentTimeout <= 0)
			return pmErr_Serial_Timeout;
		
			/*	Read one byte and convert to uppercase (it is assumed that the user string IS uppercase.	*/	
		theReadCount = 1;
		theError = Serial_ReadBytesExactCount(self, &theChar, &theReadCount, (pmuint32) theCurrentTimeout);
		
		if (theError != pmErr_NoErr)
			return theError;

		if ((theChar >= 'a') && (theChar <= 'z'))
			theChar -= 32;
		
			/*	See if the read char is the awaited char. As soon as one string is found, return.	*/		
		for (theIndex = 0; theIndex < aStringCount; theIndex++)
		{
			if (*(aString[theIndex] + theNextTestIndex[theIndex]) == theChar)
			{
				theNextTestIndex[theIndex]++;
				if (*(aString[theIndex] + theNextTestIndex[theIndex]) == 0)
				{
					if (aRcvStringIndex)
						(*aRcvStringIndex) = theIndex;
						
					return pmErr_NoErr;
				}
			}
			else
			{
					/*	Check that the read char isn't the first char in the searched string. If it is, do
						as if it was found regularly through the above loop
			   			Don't need to test the case where the searched string would only be 1 char. If the searched string
						is only 1 char len, it will always be found in the above loop.	*/
				theNextTestIndex[theIndex] = (pmuint16) ((*(aString[theIndex]) == theChar) ? 1 : 0);
			}
		}
	}
}

/* ----------------------------------------------------------------------- */

#endif

