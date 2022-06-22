// ===========================================================================
//	PMError.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMError_h
#define PMError_h

// ---------------------------------------------------------------------------

#include "PMRC.h"
#include "PMStr.h"

// ===========================================================================
//	PMError
// ===========================================================================

	/**	The value for no error.	*/
const pmuint32 kPMNoError = 0;

// ---------------------------------------------------------------------------
/**
Handles error codes. Errors are stored in a 'pmuint32'. Errors are made of 
two parts: The two (2) high-order bytes of the code give the error class (accessible 
using 'GetClass'). The two (2) lower bytes give the error code (accessible
using 'GetCode'). The class for platform errors is 0. In addition, some components 
have their own error class defined.
To test if a PMError object is not 'no error', use 'Error' (which will return a 
non-zero value if the error is not 'kPMNoError').
*/

class PMError 
{
public:
	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**
		Default constructor.
		Error code is set to 'kPMNoError' (this means no error; its value is 0).
		*/
	PMError();

		/**	Error constructor. */
	PMError(pmuint32 anError) : 
			itsError(anError) 
		{}

		/** Copy constructor. Both error code and error class are copied. */
	PMError(const PMError& anError);

		/**	Destructor. */
	virtual ~PMError();

	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------

		/**	Returns the error class. */
	pmuint16	GetClass() const	
		{ return (pmuint16) (itsError >> 16); }

		/**	Returns the error code. */
	pmuint16	GetCode() const		
		{ return (pmuint16) itsError; }
	
		/**	Returns the entire error (code and class). */
	pmuint32	GetError() const	
		{ return itsError; }

		/**	Copy operator. Both error code and error class are copied. */
	PMError&	operator=(const PMError& anError);

		/**
		Equality comparison operator. Errors are only equal if their code and their
		class are the same.
		*/
	int			operator==(const PMError& anError);

		/**	The int casting operator allows boolean testing. */
	operator int()		
		{ return Error(); }

		/**
		Returns a non-0 value if the error code is not 'no error'.
		The return value can be used as a boolean value.
		*/
	int Error() const	
		{ return itsError != kPMNoError; }

		/** Returns a human-readable string that describes the error. */
	PMStrRef	GetAsString() const;

protected:

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

	pmuint32	itsError;
};

// ---------------------------------------------------------------------------

#endif // PMError_h

