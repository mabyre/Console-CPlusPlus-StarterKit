// ===========================================================================
//	PMXTrace.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMXTrace.h"

#include "PMData.h"
#include "PMBuffer.h"

//-----------------------------------------------------------------------
#define PM_TRACEMODULE STRUCT
PM_DEFINE_TRACE_CODE(Trace, 1);

// kChunkSize is the maximum size of the trace data sent in one chunk.
// If the trace data is greater that it, it will be fragmented and sent
// fragmented. kChunkSize MUST be less than 65535.
// It is fixed to 3900 so with parameters overhead it should be less than
// 4096 but one should NOT rely on this fact but should use the chunk size 
// at the beginning of the block.
static const pmuint32 kChunkSize = 3904; // Divisible by 16

// ---------------------------------------------------------------------------

PMXTracer::PMXTracer()
{
}

// ---------------------------------------------------------------------------

PMXTracer::~PMXTracer()
{
}

// ---------------------------------------------------------------------------

pmbool PMXTracer::CanSend()
{
	PM_ASSERT_NOW(TL("PMXTracer::CanSend(): You can not use PMXTracer to send traces. You must subclass it and write your own CanSend() method"));
	return pmfalse;
}

// ---------------------------------------------------------------------------

void PMXTracer::SendChunk(PMDataRef PM_NOTUSED(aData))
{
	PM_ASSERT_NOW(TL("PMXTracer::SendChunk(): You can not use PMXTracer to send traces. You must subclass it and write your own SendChunk() method"));
}

// ---------------------------------------------------------------------------

void PMXTracer::Send(PMTraceSendParams* aParams)
{
	PMTraceActiver theActiver(pmfalse);

	if (aParams != 0 && CanSend())
	{
		size_t			theSize = aParams->itsDataLen, theOrgDataLen = aParams->itsDataLen;
		void*			theOrgData = aParams->itsData;
		PMTrace_eKind	theKind = aParams->itsKind;
		PMDataRef		theData;
		
		while (theSize != 0)
		{
			if (theSize > kChunkSize)
			{
				aParams->itsDataLen = kChunkSize;
				// If the trace is a EOT, only the last chunk must be sent this way
				if (theKind == PMTrace_kKindEOT)
					aParams->itsKind = PMTrace_kKindNormal;
				theSize -= kChunkSize;
			}
			else
			{
				aParams->itsDataLen = theSize;
				// Reset the original kind in case it has been changed.
				aParams->itsKind = theKind;
				theSize = 0;
			}
			theData = CreateData(aParams);
			// Terminate the message with the null trailing parameter.
			// See comments before the CreateData function for more information
			pmuint32 theU32 = 0;
			theData->Append(&theU32, 3);

			SendChunk(theData);

			aParams->itsData = ((char*)aParams->itsData) + aParams->itsDataLen;
		}
		aParams->itsData = theOrgData;
		aParams->itsDataLen = theOrgDataLen;
	}
}

// ---------------------------------------------------------------------------

void PMXTracer::DisplayAssert(const char* aTest, PMTraceSendParams* aParams)
{
	if (aParams != 0)
	{
		if (aParams->itsFile != 0 && aTest != 0 && aParams->itsData != 0)
			if (::pm_strlen(aTest) != 0)
				PM_TRACE(Trace, TL("ASSERTION: \"%s\" ([%s] failed) in file: %s, line: %lu", aParams->itsData, aTest, aParams->itsFile, (unsigned long)aParams->itsLine));
			else
				PM_TRACE(Trace, TL("IMMEDIATE ASSERTION: \"%s\" in file: %s, line: %lu", aParams->itsData, aParams->itsFile, (unsigned long)aParams->itsLine));
	}

	PMTracer::DisplayAssert(aTest, aParams);
}

//-----------------------------------------------------------------------
// Message format:
//
// vvXXXXXXXXXXXXXX
// vv Version of trace protocol (2 bytes: Minor, Major)
//
// ================================================
//
// For version 1.0 (0001XX...XX)
// XX...XX = NNNNp0p1...pn
// NNNN : total size of the message (including vvNNNN) (4 bytes)
// pi :	Message parameters
//		Format:	cnndd..dd
//				c	: Parameter opcode (1 byte)
//				n	: nn parameter data len (excluding cnn)
//				d..d: parameter data
//
// Parameters valid opcodes:
//		1 : Attributes: xxxxt
//			xxxx = module<<16 + code - 32 bits
//			t = kind (0 = normal, 1 = EOT, 2 = binary) - 8 bits
//		2 : Indentation level: i (8 bits)
//		3 :	Process ID
//		4 : Thread ID
//		5 : Trace Data
//		6 : Time Stamp.
//
//		0 : End of message. Every message must end with this parameter
//			As parameter data len is 0, the message MUST end with 3 0 bytes
//
// User parameters must be >=128 (bit 7 set). This will prevent collision with
// future ObjectSet parameters.
//
// Take care that 16 bits and 32 bits values are transmitted in network byte order
// (big-endian) over the network.
//-----------------------------------------------------------------------
// Create data will not append the trailing null parameter.
// That will allow to subclass it, call the base class implementation,
// and then append more parameters. The final null parameter is added by 
// Send just before calling SendChunk.
// Tough, the total size of the message is already set to take into account
// the trailing 3 bytes that will be added so you will have to only add your
// parameter size to the message size.

//-----------------------------------------------------------------------
// Version matching is done on the major only so increment the major
// when a change made this incompatible with the reporter.
#define TRACE_VERSION	0x0100

//-----------------------------------------------------------------------
PMDataRef PMXTracer::CreateData(PMTraceSendParams* aParams)
{
	PM_ASSERT(aParams != 0, TL("PMXTracer::CreateData(): aParams MUST NOT be 0"));

	pmbyte		theByte;
	pmuint16	theU16;
	pmuint32	theU32;
	pmbyte*		thePtr;

	#define ADD_BYTE(b)	theByte = b; theData->Append(&theByte,1);
	#define ADD_U16(s)	theU16 = ::pm_host_to_networku16(s); theData->Append(&theU16, 2);
	#define ADD_U32(l)	theU32 = ::pm_host_to_networku32(l); theData->Append(&theU32, 4);

	#define SET_U32(i, l)	thePtr = ((pmbyte*)theData->buffer()) + i; ::pm_set_networku32(&thePtr, (pmuint32)l);

	// 20% Overhead, min Overhead 0, All overhead after
	PMDataRef theData = new PMData(2048, 0, 20, 100);

	// Version
	ADD_U16(TRACE_VERSION);

	// Reserve place for length
	ADD_U32(0);

	// Attributes
	ADD_BYTE(PMXTracerOpcode_kAttributes); ADD_U16(5);
	ADD_U32(aParams->itsIdent);
	ADD_BYTE(aParams->itsKind);

	// Indentation level
	ADD_BYTE(PMXTracerOpcode_kIndentLevel); ADD_U16(1);
	ADD_BYTE(aParams->itsIndentLevel);

	// Time stamp
	ADD_BYTE(PMXTracerOpcode_kTimeStamp); ADD_U16(4);
	// So in the tracer we can assume that a null value means value not present.
	// The bias is 1 millisec only if the system timer is 0 (one millec every 49.7 days)
	if (aParams->itsTimeStamp == 0)
		aParams->itsTimeStamp++;
	ADD_U32(aParams->itsTimeStamp);

	// Thread ID and Process ID may be managed by subclasses

	// Trace Data. The trailing 0 will be transmitted over the network...
	ADD_BYTE(PMXTracerOpcode_kTrace); ADD_U16(aParams->itsDataLen);
	theData->Append(aParams->itsData, aParams->itsDataLen);

	size_t theSize = theData->size() + 3;
	SET_U32(2, theSize);

	return theData;
}

//-----------------------------------------------------------------------
#define GET_BYTE(ptr)	*ptr++
#define GET_U16(ptr)	::pm_get_networku16(&ptr)
#define GET_U32(ptr)	::pm_get_networku32(&ptr)

//-----------------------------------------------------------------------
pmbool PMXTracer::DecodeBlock(PMBufferRef aBlock, PMTraceSendParams* aParam, void* aUserData)
{
	PM_ASSERT(!aBlock.IsNull(), TL("PMXTracer::DecodeBlock(): Parameter 1: 0-Ref not allowed"));
	PM_ASSERT(aParam != 0, TL("PMXTracer::DecodeBlock(): Parameter 2: 0 not allowed"));

	const pmbyte* thePtr = (const pmbyte*)aBlock->buffer();

	pmuint16 theBlockVersion = GET_U16(thePtr);
	// We only check major version number.
	if ((theBlockVersion >> 8) != (TRACE_VERSION >> 8))
	{
		PM_TRACE(Trace, TL("PMXTracer::DecodeBlock(): Bad block version (%x)", (unsigned long)theBlockVersion));
		return pmfalse;
	}

	// Skip length
	pmuint32 theLen = GET_U32(thePtr);
	if (theLen != aBlock->size())
	{
		PM_TRACE(Trace, TL("PMXTracer::DecodeBlock(): Bad block size"));
		return pmfalse;
	}

	::pm_memset(aParam, 0, sizeof(PMTraceSendParams));

	pmbyte		theOpCode;
	pmuint16	theParamLen;
	while (1)
	{
		theOpCode = GET_BYTE(thePtr);
		if (theOpCode == PMXTracerOpcode_kEnd)
			break;

		theParamLen = GET_U16(thePtr);
		DecodeParameter(theOpCode, thePtr, theParamLen, aParam, aUserData);
		thePtr += theParamLen;
	}

	return pmtrue;
}

//-----------------------------------------------------------------------
pmbool PMXTracer::DecodeParameter(pmbyte anOpcode, const void* aPtr, size_t aLen, PMTraceSendParams* aParam, void* PM_NOTUSED(aUserData))
{
	const pmbyte* thePtr = (const pmbyte*)aPtr;
	switch (anOpcode)
	{
	case PMXTracerOpcode_kAttributes:
		aParam->itsIdent = GET_U32(thePtr);
		aParam->itsKind = GET_BYTE(thePtr);
		return pmtrue;

	case PMXTracerOpcode_kIndentLevel:
		aParam->itsIndentLevel = GET_BYTE(thePtr);
		return pmtrue;

	case PMXTracerOpcode_kTimeStamp:
		aParam->itsTimeStamp = GET_U32(thePtr);
		return pmtrue;

	case PMXTracerOpcode_kTrace:
		aParam->itsData = (void*)thePtr;
		aParam->itsDataLen = aLen;
		return pmtrue;
	}

	return pmfalse;
}
