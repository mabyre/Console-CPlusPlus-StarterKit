// ===========================================================================
//	PMPrintf.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMPrintf.h"
#include "PMTrace.h"
#include "PMBuffer.h"
#include "PMStr.h"
#include <stdio.h> // for sprintf

// ---------------------------------------------------------------------------

PMBufferRef pm_xsprintf(const char* aFormat, ...)
{
	va_list	theArgs;
	va_start(theArgs, aFormat);

	PMBufferRef theData = ::pm_vxsprintf(aFormat, theArgs);

	va_end(theArgs);
	return theData;
}

// ---------------------------------------------------------------------------

const char kCharConverter[] = 
"\\?\\?\\?\\?\\?\\?\\?\\?\\b\\t\\n"
"\\?\\f\\r\\?\\?\\?\\?\\?\\?\\?\\?"
"\\?\\f\\r\\?\\?\\?\\?\\?\\?\\?\\?\\?";

const size_t PMPrintf_kBlockSize = 1024;

// ---------------------------------------------------------------------------

static void AppendString(PMBufferRef& aData, const char* aString, size_t* aCurrentSize, size_t* aUsedSize)
{
	size_t	theLen = ::pm_strlen(aString);
	
	if (*aUsedSize + theLen + 1 >= *aCurrentSize)
	{
		*aCurrentSize += PMPrintf_kBlockSize;
		aData->Realloc(*aCurrentSize);
	}
		
		// Faster than strcat
	::pm_strcpy((char*)aData->buffer() + *aUsedSize, aString); 
	*aUsedSize += theLen;
}

// ---------------------------------------------------------------------------

static void AppendBuffer(PMBufferRef& aData, void* aBuffer, size_t aLen, size_t* aCurrentSize, size_t* aUsedSize)
{
	
	if (*aUsedSize + aLen + 1 >= *aCurrentSize)
	{
		*aCurrentSize += PMPrintf_kBlockSize;
		aData->Realloc(*aCurrentSize);
	}
	
	char* theData = (char*)aData->buffer();
	::pm_memcpy(theData + *aUsedSize, aBuffer, aLen);
	*aUsedSize += aLen;

	theData[*aUsedSize] = 0;
}

// ---------------------------------------------------------------------------

PMBufferRef pm_vxsprintf(const char* aFormat, va_list anArgs)
{
	PMTraceActiver theActiver(pmfalse);

	size_t		theCurrentSize = PMPrintf_kBlockSize, theUsedSize = 0;
	PMBufferRef	theData = new PMBuffer(PMPrintf_kBlockSize);
	char*		thePtr = (char*)aFormat;
	char		theChar;

	while ((theChar = *thePtr++) != 0)
	{
		if ((theChar < ' ') && (theChar >= 0))
		{
			::AppendBuffer(theData, (void*)&kCharConverter[((size_t) theChar) * 2], 2, &theCurrentSize, &theUsedSize);
			continue;
		}

		if (theChar == '%' && *thePtr != 0)
		{
			switch (*thePtr)
			{
				case '%':
					::AppendBuffer(theData, &theChar, 1, &theCurrentSize, &theUsedSize);
					break;

				case 's':
				{
					unsigned char*	theString = (unsigned char*) va_arg(anArgs, unsigned char*);
					if (theString == 0)
						::AppendBuffer(theData, "(null)", 6, &theCurrentSize, &theUsedSize);
					else
					{
						size_t			theLength = ::pm_strlen((char*) theString);
						size_t			theLastPos = 0;
						
						for (size_t i = 0 ; i < theLength; i++)
						{
							if (theString[i] < ' ')
							{
								if (i > theLastPos)
									::AppendBuffer(theData, &theString[theLastPos], i - theLastPos, &theCurrentSize, &theUsedSize);
								::AppendBuffer(theData, (void*)&kCharConverter[((size_t) theString[i]) * 2], 2, &theCurrentSize, &theUsedSize);
								theLastPos = i + 1;
							}
						}
						::AppendBuffer(theData, &theString[theLastPos], theLength - theLastPos, &theCurrentSize, &theUsedSize);
					}
				}
					break;

				case 'c':
				{
					unsigned char theChar= (unsigned char) va_arg(anArgs, unsigned char);
					if (theChar < ' ')
						::AppendBuffer(theData, (void*)&kCharConverter[((size_t) theChar) * 2], 2, &theCurrentSize, &theUsedSize);
					else
						::AppendBuffer(theData, &theChar, 1, &theCurrentSize, &theUsedSize);
				}
					break;

				case 'p':
				{
					void* thePtr = va_arg(anArgs, void*);
					char theString[16];
					::sprintf(theString, "0x%p", thePtr);
					::AppendString(theData, theString, &theCurrentSize, &theUsedSize);
				}
					break;

				case 'l':
				{
					thePtr++;
					if (*thePtr == 'd')
					{
						long theLong = va_arg(anArgs, long);
						char theString[16];
						::sprintf(theString, "%ld", theLong);
						::AppendString(theData, theString, &theCurrentSize, &theUsedSize);
					}
					else
					if (*thePtr == 'u')
					{
						unsigned long theLong = va_arg(anArgs, unsigned long);
						char theString[16];
						::sprintf(theString, "%lu", theLong);
						::AppendString(theData, theString, &theCurrentSize, &theUsedSize);
					}
					else
					if (*thePtr == 'x')
					{
						unsigned long theLong = va_arg(anArgs, unsigned long);
						char theString[16];
						::sprintf(theString, "%08lx", theLong);
						::AppendString(theData, theString, &theCurrentSize, &theUsedSize);
					}
					else
					{
						char* theString = "Unknwon trace tag: %l";
						::AppendString(theData, theString, &theCurrentSize, &theUsedSize);
						::AppendBuffer(theData, thePtr, 1, &theCurrentSize, &theUsedSize);
						char theChar = ']';
						::AppendBuffer(theData, &theChar, 1, &theCurrentSize, &theUsedSize);
					}
				}
					break;

				case 'b':
				{
					char*	theString = "<binary>";
					void*	thePtr = va_arg(anArgs, void*);
					size_t	theLong = va_arg(anArgs, size_t);
					::AppendString(theData, theString, &theCurrentSize, &theUsedSize);
				}
					break;

				case 'n':
					::AppendString(theData, PMStr::PlatformEOL(), &theCurrentSize, &theUsedSize);
					break;


				case 'S':
				{
					unsigned char*	theString = (unsigned char*) va_arg(anArgs, unsigned char*);
					size_t			theLength = (*(char*) theString) + 1;
					size_t			theLastPos = 1;
					
					for (size_t i = 1 ; i < theLength; i++)
					{
						if (theString[i] < ' ')
						{
							if (i > theLastPos)
								::AppendBuffer(theData, &theString[theLastPos], i - theLastPos, &theCurrentSize, &theUsedSize);
							::AppendBuffer(theData, (void*)&kCharConverter[((size_t) theString[i]) * 2], 2, &theCurrentSize, &theUsedSize);
							theLastPos = i + 1;
						}
					}
					::AppendBuffer(theData, &theString[theLastPos], theLength - theLastPos, &theCurrentSize, &theUsedSize);
				}
					break;

				case 'd':
				case 'i':
				{
					int theInt = va_arg(anArgs, int);
					char theString[16];
					::sprintf(theString, "%d", theInt);
					::AppendString(theData, theString, &theCurrentSize, &theUsedSize);
				}
					break;
					
				case 't':
				{
					size_t theSizet = va_arg(anArgs, size_t);
					char theString[16];
					::sprintf(theString, "%lu", (unsigned long) theSizet);
					::AppendString(theData, theString, &theCurrentSize, &theUsedSize);
				}
					break;
					
				case 'u':
				{
					unsigned int theInt = va_arg(anArgs, unsigned int);
					char theString[16];
					::sprintf(theString, "%u", theInt);
					::AppendString(theData, theString, &theCurrentSize, &theUsedSize);
				}
					break;

				case 'x':
				case 'X':
				{
					unsigned int theInt = va_arg(anArgs, unsigned int);
					char theString[16];
					if (*thePtr == 'x')
						::sprintf(theString, "%x", theInt);
					else
						::sprintf(theString, "%X", theInt);
					::AppendString(theData, theString, &theCurrentSize, &theUsedSize);
				}
					break;
				
				case 'T':
				{
					unsigned long	theType = (unsigned long) va_arg(anArgs, unsigned long);
					char theTypeString[5];
					int len = ::sprintf(theTypeString, "%.4s", &theType);
					::AppendBuffer(theData, &theTypeString, len, &theCurrentSize, &theUsedSize);
				}
					break;


				default:
				{
					PMTraceActiver theReactiver(pmtrue);
					PM_ASSERT_NOW(TL("pm_vxsprintf(): Unknown trace tag: %c", *thePtr));
				}
					break; // NEVER REACHED because of the exception thrown by Assert
			}

			thePtr++;
		}
		else
			::AppendBuffer(theData, &theChar, 1, &theCurrentSize, &theUsedSize);
	}
	// Add one trailing 0.
	theChar = 0;
	::AppendBuffer(theData, &theChar, 1, &theCurrentSize, &theUsedSize);

	return theData;
}
