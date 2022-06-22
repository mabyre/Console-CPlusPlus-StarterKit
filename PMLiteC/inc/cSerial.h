/*	===========================================================================
	cSerial.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	Author : ELp
	===========================================================================	*/

#ifndef cSerial_h
#define cSerial_h

/*	---------------------------------------------------------------------------	*/

#include "cTypes.h"

/*	--------------------------------------------------------------------------- */

#ifdef __cplusplus
	extern "C" {
#endif

/*	---------------------------------------------------------------------------
	Module, traces and errors.
	---------------------------------------------------------------------------	*/

	/*	Module	*/
#define pmM_Serial						13

	/*	Trace	*/
#define pmT_Serial						PMTraceDefine(pmM_Serial, 0)

	/*	Errors	*/
#define pmErr_Serial_NoMemory			PMErrorDefine(pmM_Serial, pmErr_NoMemory)	/*	Memory storage error. (ie: user supplied callback has returned false).	*/
#define pmErr_Serial_UserCancel 		PMErrorDefine(pmM_Serial, pmErr_UserCancel)	/*	The user has cancelled the connection.	*/
#define pmErr_Serial_Timeout 			PMErrorDefine(pmM_Serial, pmErr_Timeout)

#define pmErr_Serial_NoComm 			PMErrorDefine(pmM_Serial, 64)	/*	Serial_Open has not been called. */
#define pmErr_Serial_BufferTooSmall 	PMErrorDefine(pmM_Serial, 65)
#define pmErr_Serial_OSError 			PMErrorDefine(pmM_Serial, 66)	/*	An OSError that cannot be remapped to a Serial_Err. See Serial_GetOSError() */

/*	--------------------------------------------------------------------------
	Some other constants used in the implementation.
	--------------------------------------------------------------------------	*/

#define kWaitForMaxStrings 			20		/*	Maximum number of strings that can be awaited in WaitFor. */
#define kDelimiterStringMaxSize 	20		/*	Taille maxi de la chaine de delimiter. */

/*	---------------------------------------------------------------------------	*/

typedef struct dSerial *pSerial;

/*	---------------------------------------------------------------------------
	SerialParams constants.
	Values must be equal to their meaning because some platforms may make this assumption
	to ease writing their configuration code.
	---------------------------------------------------------------------------	*/

#define kSpeed300 		300
#define kSpeed1200 		1200
#define kSpeed2400 		2400
#define kSpeed4800 		4800
#define kSpeed9600 		9600
#define kSpeed19200 	19200
#define kSpeed38400 	38400
#define kSpeed57600		57600
#define kSpeed115200	115200

#define kCharsize5 		5
#define kCharsize6 		6
#define kCharsize7 		7
#define kCharSize8 		8

#define kParityEven 	1
#define kParityOdd 		2
#define kParityNone 	3

#define kStopBit1 		1
#define kStopBit15 		2
#define kStopBit2 		3

/*	---------------------------------------------------------------------------	*/

typedef pmbool (*SerialEventCallBack)(void *);

typedef void *spPortSpecifier;

typedef struct
{
	pmuint32	speed;
	pmuint16	parity;
	pmuint16	bitsPerByte;
	pmuint16	stopBit;
	pmbool		fUseHWFlowControlIn;
	pmbool		fUseHWFlowControlOut;
} sdSerialParams, *spSerialParams;

/*	---------------------------------------------------------------------------
	General functions
	---------------------------------------------------------------------------	*/

	/*	Allocates the object and can return either the object or NULL, but no other error.	
		It is mandatory to call Serial_Init after Serial_New.	*/
pSerial	Serial_New(void);

	/*	Serial_Init initializes the object (allocate memory or do other stuff) and can return any 
		error.	*/
pmerror	Serial_Init(pSerial self);

	/*	Serial_Delete frees all and can be called anytime even if the object has not been created 
		or initialized.	*/
void	Serial_Delete(pSerial* self);

	/*	Set a callback function vector that will be called during dial, read, etc 
   		the callback has the prototype pmbool myCallBack (void *aUserData); it must return true
   		if the current operation has to be canceled.	*/
void	Serial_SetEventCallBack(pSerial self, SerialEventCallBack aCallBack, void *aUserData);

	/*	Serial_Open opens the serial port and makes it suitable to use the transaction functions.
		Return values:
		- pmErr_Serial_OSError if open failed
		*/
pmerror	Serial_Open(pSerial self, spPortSpecifier aPortSpecifier, spSerialParams aSerialParams);

	/*	Serial_Close frees the port (NOT the object).
		- pmErr_Serial_NoComm, if com not open
		*/
pmerror	Serial_Close(pSerial self);

	/*	Returns the real low level last OS Error if you ever need it.	*/
pmint32 Serial_GetOSError(pSerial self);

/*	---------------------------------------------------------------------------
	Transaction functions.
	---------------------------------------------------------------------------	*/

	/* 	Returns the number of bytes available to read.	*/
pmerror Serial_GetInputAvailableCount (pSerial self, size_t *aCount);

	/*	Read bytes and stores them in aBuffer. Upon return, aReadCount always tells how many bytes 
		were read. The function will return at the occurence of one of the following conditions:
			- if aTimeout == 0, read what is available, up to aBufferSize, and returns 
			  immediately (pmErr_Serial_Timeout not possible here)
			- if aTimeout != 0,
	   		- if one up to aBufferSize bytes are available within the timeout, reads 
	   		  them and returns pmErr_NoErr
	   		- if no bytes are available within the timeout, returns pmErr_Serial_Timeout.	*/
pmerror Serial_ReadBytes(pSerial self, void *aBuffer, size_t aBufferSize, size_t *aReadCount, pmuint32 aTimeout);

	/*	Read bytes and stores them in aBuffer. Upon return, aReadCount always tells how many bytes were read.
		if aTimeout == 0, only one read attempt will be done.
		The function will return at the occurence of one of the following conditions :
			- All the wanted bytes were read before aTimeout : returns pmErr_NoErr
			- Not all the wanted bytes were read before aTimeout : returns pmErr_Serial_Timeout and the 
			  bytes that were available.	*/
pmerror Serial_ReadBytesExactCount(pSerial self, void *aBuffer, size_t *aCount, pmuint32 aTimeout);

	/*  Read several bytes until either aDelimiter is found, or aTimeout (milliseconds) has elapsed
		note : the delimiter bytes are read from the serial connection but are not included in the 
		returned bytes. aCount returns the number of bytes that were actually copied in aBuffer. */
pmerror Serial_ReadDelimitedChunk(pSerial self, char *aBuffer, size_t aMaxSize, size_t *aCount,
								  char *aDelimiter, pmuint32 aTimeout);

	/*  Writes aCount bytes from aBuffer to the serial connection */
pmerror Serial_WriteBytes(pSerial self, void *aBuffer, size_t aCount);

	/*  Waits aTimeout (milliseconds) for one or more specified strings on the line.
		It is possible to wait for up to 20 strings (aStringCount must tell how many)
		if one of the string is found, returns pmErr_noErr and aRcvStringIndex tells which one was found
		If it is not, returns pmErr_Serial_Timeout.
		While listening, any incoming byte not eligible to be kept as part of one of the awaited string 
		is flushed. This means you can for example send "ATZ\r" and then just wait for "OK" without 
		bothering with the echo or other intermediate characters 
		IMPORTANT NOTE : the function expects only ascii codes and uppercase chars in the search strings. It will
		                 however uppercase the serial input.
		*/
pmerror	Serial_WaitFor(pSerial self, char *aString[], pmuint16 aStringCount, pmuint16 *aRcvStringIndex, pmuint32 aTimeout);

	/*	Flushes the serial port by reading all available data.	*/
pmerror Serial_Flush(pSerial self);

	/*	Sets the value of the DTR.	Returns true only if the specified value has been set. 
		Always returns false when the DTR signal cannot be managed (ie: DTR signal is not 
		handled by software or hardware).	*/
pmbool	Serial_SetDTR(pSerial self, pmbool afON);

	/*	Check if the RLSD (receive-line-signal-detect) signal is on - afOn is only set if pmtrue is return */
pmbool	Serial_GetRLSD(pSerial self, pmbool *afOn);

/*	---------------------------------------------------------------------------
	Test functions
	---------------------------------------------------------------------------	*/

	/*	Returns a port specifier.
		Use this function for test purpose only.	*/
spPortSpecifier	GetDefaultSerialPortSpecifier(void);

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif

/*	---------------------------------------------------------------------------	*/

#endif
