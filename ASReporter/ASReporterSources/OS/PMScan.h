// ===========================================================================
//	PMScan.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMScan_h
#define PMScan_h

//-----------------------------------------------------------------------

#include "PMCharS.h"
#include "PMStr.h"

// ===========================================================================
//	PMScanner
// ===========================================================================

	/**	Type used to specify a scanner status.	*/
typedef pmuint16 PMScanner_eScannerStatus;

	/**	The scanner status is OK. (ie: there is more data to scan).	*/
const PMScanner_eScannerStatus PMScanner_kOk = 0;

	/**	The scanner has reach the data end. (ie: there is no more data to scan).	*/
const PMScanner_eScannerStatus PMScanner_kAtEnd = 1;

/**
Helper for "parsing" strings.
A scanner may be pictured as a "cursor" onto a string which can advance if it encouters 
certain characters.
This class has been designed to help developers avoid accessing PMStr using 'c_str()',
and dealing with character pointers.
This class is initialized for a scan on a particular string. There is no way to change the scanned string.
Internally, there is a private pointer which can move around the string while searching for
particular occurrences of specified characters.
Strings located before or after the scanner position can be retrieved (by copy) at any time.
A PMScanner can be seen as a high-level string content pointer.
Every operation on a scanner returns a boolean value that indicates success
or failure of the operation. The internal pointer is not moved in case of failure.
It is legitimate to copy scanners using the operator= or copy contructors.
A scanner is not a reference-counted object, as it is not designed to be shared.
*/

class PMScanner
{
public:

	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**
		Creates a PMStr from the string between the position of the two 
		specified scanners. The scanners must be issued from the same string.
		*/
	static PMStrRef StringFromScanners(PMScanner& aScan1, PMScanner& aScan2);
 
		/**
		Default constructor.
		The scanner string is empty, and the scanner position is set to 0.
		*/
	PMScanner();

		/**
		PMStr constructor.
		The scanner position is set to the beginning of the string.
		*/
	PMScanner(const PMStrRef& aString);

		/**	Copy constructor.	*/
	PMScanner(const PMScanner& aScanner);
		
		/**	Destructor.	*/
	~PMScanner();

		/**	Copy operator.	*/
	PMScanner& operator= (const PMScanner& aScanner);

	// -----------------------------------------------------------------------
	//	Status inquiry	
	// -----------------------------------------------------------------------

		/**
		Returns scanner status. This method returns PMScanner_kAtEnd if the position 
		has reached the end of the string, otherwise it returns PMScanner_kOk.
		*/
	PMScanner_eScannerStatus GetStatus() const;

		/**	Returns true if the position has reached the end of the string.	*/
	pmbool	IsAtEnd() const;

		/**	Returns the string composed of the characters starting at the current
		position up to the string end.
		*/
	PMStrRef ExtractStringToEnd();

		/**	Returns the string composed of characters starting at the beginning of 
		the string up to the current position.	*/
	PMStrRef ExtractStringFromStart();

	// -----------------------------------------------------------------------
	//	Scan function
	// -----------------------------------------------------------------------

		/**
		Advances the scanner of one position, and returns true if it has advanced.
		*/
	pmbool	Forward();

		/**
		Advances scanner position the specified count.
		Returns true if it has advanced the specified count without reaching
		the end.
		*/
	pmbool Forward(size_t anOffset);

		/**
		Backs scanner up one position,
		returns pmtrue if it has backed up successfully.
		*/
	pmbool	Backward();

		/**
		Rewinds the scanner position of the specified count.
		Returns pmtrue if it has advanced of the specified count without reaching
		the start.
		*/
	pmbool	Backward(size_t anOffset);

		/**	Moves scanner back to the beginning.	*/
	void	GoToStart();

		/**	Advances scanner to the end.	*/
	void	GoToEnd();

		/**
		Sets the specified character with the value of the current scanner character.
		Returns pmtrue if a character was available.
		*/
	pmbool Peek(char* aChar);

		/**
		Moves scanner forward if the character at the current position is the 
		character that was specified.
		Returns true if scanner has advanced.
		*/
	pmbool ForwardAcceptOne(char aChar);

		/**
		Moves scanner forward if the character at the current position is in the specified
		charset; sets the specified character to the value of the character at the current
		position.
		Returns true if scanner has moved forward.
		*/
	pmbool ForwardAcceptOne(const PMCharSet &aSet, char* aCharAccepted = 0);

		/**
		Advances the scanner if the string at the current position is in the specified
		string set; sets the specified index to the index of the string in the string set.
		Returns true if scanner has moved forward.
		*/
	pmbool ForwardAcceptOne(const PMStringSet &aSet, size_t* anIndex = 0);

		/**
		Advances scanner as long as the character at the current position is the specified
		character.
		Returns true if it has moved forward.
		*/
	pmbool ForwardAcceptMany(char aChar);

		/**
		Advances the scanner as long as the character at the current position is in the 
		specified charset.
		Returns true if it has moved forward.
		*/
	pmbool ForwardAcceptMany(const PMCharSet &aSet);

		/**
		Moves scanner forward until the specified character is encountered.
		Does not take the character.
		Returns true if the character was found.
		The scanner is unchanged if the character is not found.
		*/
	pmbool ForwardScanTo(char aChar);

		/**
		Moves scanner forward until a character in the specified charset is encountered
		Does not take the character.
		Returns true if one character was found.
		The scanner is unchanged if no character is found.
		*/
	pmbool ForwardScanTo(const PMCharSet &aSet);

		/**
		Sets the value of the specified 32-bit int from the string at the current
		position.
		Returns true on success, returns false if the string at the current position
		is not the character representation of a signed 32-bit int.
		*/
	pmbool	ScanInt32(pmint32* anInt32);

		/**
		Sets the value of the specified unsigned 32-bit int from the string at the current
		position.
		Returns true on success, returns false if the string at the current position
		is not the character representation of an unsigned 32-bit int.
		*/
	pmbool	ScanUInt32(pmuint32* anUInt32);

		/**
		Moves the scanner forward if the string at the current position is the specified
		string (searches are case-relevant).
		Returns true if it has advanced.
		*/
	pmbool	ForwardAccept(const char *aString);
	
		/**
		Moves the scanner forward until the specified string is encountered.
		Does not take the string.
		Returns true if the string was found.
		*/
	pmbool	ForwardScanTo(const char *aString);

		/**
		Returns the character at the current position; returns -1 if no character is
		available.
		*/
	int		Peek();

		/** 
		Returns the current scanner position expressed as an offset with respect to the 
		beginning of the string.
		*/
	size_t	GetPos();
	
protected:

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

		/**	Only for keeping it retained.	*/
	PMStrRef	itsString;

		/**	Scanner string.	*/
	const char*	itsStringStart;

		/**	Position in buffer.	*/
	const char*	itsPosition;

	void	SetPos(size_t aPos);
};

// ----------------------------------------------------------------------------

#endif	// PMScan_h

