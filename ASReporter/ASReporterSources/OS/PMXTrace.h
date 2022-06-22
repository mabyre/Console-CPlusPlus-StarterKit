// ===========================================================================
//	PMXTrace.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMXTrace_h
#define PMXTrace_h

// ---------------------------------------------------------------------------

#include "PMTrace.h"
#include "PMStr.h"

// ===========================================================================
//	PMXTracer
// ===========================================================================

	/**	Type for PMXTracer Opcodes.	*/
typedef pmuint16 PMXTracerOpcode_eType;

	/** PMXTracer Opcode: Special Opcode ending a trace.	*/
const PMXTracerOpcode_eType PMXTracerOpcode_kEnd = 0;
	/** PMXTracer Opcode: Attributes Opcode (Module, Code, Kind).	*/
const PMXTracerOpcode_eType PMXTracerOpcode_kAttributes = 1;
	/** PMXTracer Opcode: Indentation Level Opcode.	*/
const PMXTracerOpcode_eType PMXTracerOpcode_kIndentLevel = 2;
	/** PMXTracer Opcode: Process ID Opcode.	*/
const PMXTracerOpcode_eType PMXTracerOpcode_kProcessID = 3;
	/** PMXTracer Opcode: Thread ID Opcode.	*/
const PMXTracerOpcode_eType PMXTracerOpcode_kThreadID = 4;
	/** PMXTracer Opcode: Trace Data Opcode.	*/
const PMXTracerOpcode_eType PMXTracerOpcode_kTrace = 5;
	/** PMXTracer Opcode: TimeStamp Opcode.	*/
const PMXTracerOpcode_eType PMXTracerOpcode_kTimeStamp = 6;

// ---------------------------------------------------------------------------

PMDEFINE(PMData);
PMDEFINE(PMBuffer);
PMDEFINE(PMXTracer);

// ---------------------------------------------------------------------------
/** Base tracer class to be used when you want to send traces in binary form.	*/

class PMXTracer : public PMTracer
{
public:
		/** Constructor. */
	PMXTracer();

		/** Destructor. */
	virtual ~PMXTracer();

		/** 
		Returns pmtrue when tracer can send this block.
		This function is called for each trace. When it returns false, the
		trace is skipped without implying that subsequent traces will be skipped too.
		*/
	virtual pmbool	CanSend();
	
		/** Inherited. See: \Ref{PMTracer::Send}. */
	virtual void	Send(PMTraceSendParams* aParams);

		/** Inherited. See: \Ref{PMTracer::DisplayAssert}. */
	virtual void	DisplayAssert(const char* aTest, PMTraceSendParams* aMessage);

		/**
		Decodes a trace block.
		It fills in the aParam struct with fields found in 'aBlock'. 
		It calls DecodeParameter for each parameter so you can subclass it in order to manage
		your own parameters. 'aUserData' is for your own use and is passed unmodified to 
		DecodeParameter.
		Returns false if an error occurred while decoding the block.
		Be very careful that the 'itsData' member of 'aParam' points inside 'aBlock'. 
		When 'aBlock' goes out of range, aParam->itsData will be an invalid pointer.
		Moreover you must imperatively set aParam->itsData to 0 before it is destroyed otherwise
		pm_free will be called on it, which will cause a crash.
		*/
	virtual pmbool	DecodeBlock(PMBufferRef aBlock, PMTraceSendParams* aParam, void* aUserData);

		/**
		Decodes a trace parameter.
		This method is called for each trace parameter. It manages the standard one by
		filling in the PMTraceSendParams aParam structure. You can sub-class it to manage your
		own parameters. 'aUserData' is the same as that which you passed to DecodeBlock.
		It returns true if it decoded the parameter (known opcode).
		*/
	virtual pmbool	DecodeParameter(pmbyte anOpcode, const void* aPtr, size_t aLen, PMTraceSendParams* aParam, void* aUserData);

protected:
		/** Creates the binary block to send. */
	virtual	PMDataRef	CreateData(PMTraceSendParams* aParams);

		/** Does send the block. Designed to be sub-classed in your tracers. */
	virtual void		SendChunk(PMDataRef aData);

private:
	PMXTracer(const PMXTracer&);
	PMXTracer& operator=(const PMXTracer&);
};

// ---------------------------------------------------------------------------

#endif // PMXTrace_h
