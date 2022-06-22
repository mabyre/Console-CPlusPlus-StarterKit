// ===========================================================================
//	PMOSHelp.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMOSHelp_h
#define PMOSHelp_h

// ---------------------------------------------------------------------------

#include "PMRC.h"
#include "PMStr.h"
#include "PMMedia.h"
#include "PMError.h"

// ---------------------------------------------------------------------------

	/**	Platform helper: error class code.	*/
const pmuint16 kPMOSHelperErrorClass = 0x80;

	/**	Platform helper: general error.	*/
const pmuint32 kPMOSHelperGeneralError = 0x800001;

	/**	Platform helper: ObjectSet initialization error.	*/
const pmuint32 kPMOSHelperInitPMError = 0x800002;

	/**	Platform helper: TCP initialization error.	*/
const pmuint32 kPMOSHelperInitTCPError = 0x800003;

// ---------------------------------------------------------------------------

	/**	Type for temporary media types.	*/
typedef pmuint16 PMMediaTemporary_eType;

	/** Memory media type.	*/
const PMMediaTemporary_eType PMMediaTemporary_kMemory = 0;

	/** File media type.	*/
const PMMediaTemporary_eType PMMediaTemporary_kFile = 1;

// ---------------------------------------------------------------------------

	/**	Type for end-of-line codes. */
typedef pmuint16 PMEndOfLine_eCode;

	/**	LF (Line Feed) end-of-line. Mainly for Unix platforms. */
const PMEndOfLine_eCode PMEndOfLine_kLF = 0;

	/**	CR (Carriage Return) end-of-line. Mainly for MacOS. */
const PMEndOfLine_eCode PMEndOfLine_kCR = 1;

	/**	CRLF end-of-line. Mainly for Windows platforms. */
const PMEndOfLine_eCode PMEndOfLine_kCRLF = 2;

// ---------------------------------------------------------------------------

	/**	Type for character sets.	*/
typedef	pmuint16 PMCharacterSet_eCode;

	/** ISO-8859-1 character set.	*/
const PMCharacterSet_eCode PMCharacterSet_kISO_8859_1 = 1;

	/** Unix character set.	*/
const PMCharacterSet_eCode PMCharacterSet_kUnix = 0;		//	(multinational)

	/** Windows character set.	*/
const PMCharacterSet_eCode PMCharacterSet_kWindows = PMCharacterSet_kISO_8859_1;

	/** X11 character set.	*/
const PMCharacterSet_eCode PMCharacterSet_kX11 = 2;		

	/** Amiga character set.	*/
const PMCharacterSet_eCode PMCharacterSet_kAmiga = 3;

	/** DOS (International) character set.	*/
const PMCharacterSet_eCode PMCharacterSet_kDos = 4;			//	(multinational)
	
	/** DOS (US) character set.	*/
const PMCharacterSet_eCode PMCharacterSet_kDosUS = 5;		//	(USA)

	/** MacOS character set.	*/
const PMCharacterSet_eCode PMCharacterSet_kMacintosh = 6;	

// ===========================================================================
//	PMOSHelper
// ===========================================================================

PMDEFINE(PMOSHelper);

// ---------------------------------------------------------------------------
/**
Base class for all Operating System helpers.
Each 'PMOSHelper' method has default behavior.
There are 'PMOSHelper' subclasses for each supported platform. This 
lets you implement behaviors in a more operating system-friendly manner. 
The 'PMOSHelper' default implementation is designed mainly for 
a standard POSIX system.
*/

class PMOSHelper : public PMRC
{
public:

		/**	Gets the current operating system helper. 	*/
	static PMOSHelperRef GetCurrent();

		/**	Sets the current operating system helper. Returns the previous one.	*/
	static PMOSHelperRef SetCurrent(const PMOSHelperRef& anHelper);

		/**
		Returns a valid file name for 'aFileName' according to the 
		operating system's file naming rules. Validation is performed by checking 
		characters, length and form.
		Default behavior is to return the same file name.
		*/
	virtual PMStrRef ValidateFileName(const PMStrRef &aFileName) 
		{ return aFileName; }

		/**	Returns a random file name. Returns a different file name at each call.	*/
	virtual PMStrRef CreateRandomFileName();

		/**
		Sets the type of temporary media to create. The type can be:
		PMMediaTemporary_kMemory (to use memory for temporary storage) or
		PMMediaTemporary_kFile (to use files for temporary storage).
		*/
	virtual void SetTemporaryMediaType(PMMediaTemporary_eType aType);

		/**	Returns the operating system's character set code.	*/
	virtual PMCharacterSet_eCode GetPlatformCharset();

		/**	Returns the operating system's end-of-line code.	*/
	virtual PMEndOfLine_eCode GetPlatformEndOfLine();

		/**
		Returns the computer's Internet host name. The implementation must avoid 
		establishing a TCP/IP connection to get this information. 
		Callers must remember that the returned name may not be the real host name.
		*/
	virtual PMStrRef GetHostName();

		/**
		Returns suggested size to use for temporary buffers, according to operating system
		capabilities and memory resource availability.
		*/
	virtual size_t GetTemporaryBufferSize();

		/**	Returns true if the operating system provides "unlimited" 
		memory (such as virtual memory).	*/
	virtual pmbool HasInfiniteMemory();

		/**	Returns a string describing 'anError'.	*/
	virtual PMStrRef GetErrorString(const PMError& anError);
};

// ---------------------------------------------------------------------------

#endif // PMOSHelp_h

