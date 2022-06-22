// ===========================================================================
//	PMStr.h
//	Copyright (C) 1996 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMStr_h
#define PMStr_h

// ---------------------------------------------------------------------------

#include "PMRC.h"
#include "PMCharS.h"

// ===========================================================================
//	PMStr
// ===========================================================================

// ---------------------------------------------------------------------------

class PMStrRef;
template <class T> class PMArray;

	/** An array of \Ref{PMStrRef}.	*/
typedef PMRef<PMArray<PMStrRef> > PMStrArrayRef;

// ---------------------------------------------------------------------------

	/**	
	Type used to define how a string is stored in a \Ref{PMStrRef} (by copy,
	by reference, etc).	
	*/
typedef pmuint16 PMStr_eTrait;

	/**	The string is copied.	*/
const PMStr_eTrait PMStr_kCopy		= 0;

	/**	The string will be freed using '::pm_free()'.		*/
const PMStr_eTrait PMStr_kDelete	= 1;

	/**	Only a reference, the string will never be freed.	*/
const PMStr_eTrait PMStr_kRef		= 2;			

	/**	Use this for a string whose size is unknown.	*/
const size_t PMStr_kUnknown = pmnpos;	

/**
Handles a non-mutable (read-only) string.
This is a reference-counted string, and this class should be used only via
the PMStrRef class. A PMStr object cannot be automatically instanciated.
*/

class PMStr : public PMRC
{
friend class PMStrRef;

public:

	// -----------------------------------------------------------------------
	//	Factory. All these functions return new strings
	// -----------------------------------------------------------------------

		/**
		Creates a 'PMStr' from a C string.
		If 'aSize' is 'PMStr_kUnknown' the string length is computed using '::pm_strlen(aCString)'.
		If 'aTrait' is 'PMStr_kDelete', the string should have been allocated using
		'::pm_malloc' as it will be freed using '::pm_free'.
		*/
	static PMStrRef NewFromCString(const char *aCString , PMStr_eTrait aTrait, size_t aSize);

		/**	Creates a PMStr from a C string, copying the string.	*/
	static PMStrRef NewFromCString(const char *aCString);

		/**
		Returns a zero-length string, which is shared (reference-counted). Using
		this method is the most efficient way to deal with empty strings.
		*/
	static PMStrRef Empty();

		/**
		Returns a CRLF string, which is shared (reference-counted). Using
		this method is the most efficient way to deal with strings that contain CRLF.
		*/
	static PMStrRef CRLF();

		/**
		Returns a platform EOL string, which is shared (reference-counted). Using
		this method is the most efficient way to deal with strings that contain a platform EOL.
		*/
	static PMStrRef PlatformEOL();

		/**
		Creates a 'PMStr' from a value, inserting zeros (0) at the beginning of the string so
		that string length matches the number of digits specified.
		For example PMStr::NewFromNumber(1234, 6) generate the string "001234"
		*/
	static PMStrRef NewFromNumber(pmint32 aNumber, size_t aDigitCount);

		/**	Creates a 'PMStr' from an unsigned 32-bit number.	*/
	static PMStrRef NewFromUInt32(pmuint32 aNumber);

		/**
		Creates a random 'PMStr' of the specified length. Characters that make up the string
		are taken from the source string, letting you choose the character set from
		which the random string will be made.
		*/
	static PMStrRef NewRandomString(const PMStrRef &aSource, size_t aLen);


	// -----------------------------------------------------------------------
	//	Modifying. All these functions return new strings
	// -----------------------------------------------------------------------


		/**	Creates a 'PMStr' by appending the specified string.	*/
	PMStrRef Append(const PMStrRef &aStr);

		/**	Creates a 'PMStr' by appending the specified C string.	*/
	PMStrRef Append(const char* aCStr);

		/**	Creates a 'PMStr' which is an upper-case version of the string.	*/
	PMStrRef UpperCase();

		/**	Creates a 'PMStr' which is a lower-case version of the string.	*/
	PMStrRef LowerCase();

		/**
		Creates a 'PMStr' by removing all the first characters  
		equal to the specified one starting at the beginning of the string.
		*/
	PMStrRef SkipLeading(char aChar = ' ');
	
		/**
		Creates a 'PMStr' by removing all the last characters 
		equal to the specified one from the end of the string.
		*/
	PMStrRef SkipTrailing(char aChar = ' ');
		
		/**
		Creates a 'PMStr' by quoting the string: add a double-quote (") 
		character at the beginning and at the end of the string.
		*/
	PMStrRef Quote(char aStartQuote = '\"', char aEndQuote = '\"');

		/**
		Creates a 'PMStr' by unquoting the string; this method removes leading and
		trailing spaces and tab characters before performing unquoting.
		*/
	PMStrRef Unquote(char aStartQuote = '\"', char aEndQuote = '\"');

		/**
		Creates a 'PMStr' by removing all the parenthesized strings from the string.
		For example the string "aa (bbb (c)) zz (c)" returns "aa zz".
		This is useful to strip comments from "RFC822-like" strings.
		*/
	PMStrRef StripParenthesis();

		/**
		Creates a 'PMStr' which is a copy of the string, but in which each character in
		'aCharset' is replaced by 'aChar'.
		For example: for the string "abcdef", a charset made of "cd" and the
		replacement character "x" returns "abxxef".
		*/
	PMStrRef ReplaceCharSetByChar(const PMCharSet &aCharSet, char aChar);

		/**
		Creates a 'PMStr' which is a copy of the string but in which each matching 
		character is replaced by the specified one.
		*/
	PMStrRef ReplaceCharByChar(char aChar, char aReplace);

		/**
		Creates a 'PMStr' which is a copy of the string but in which each character in
		'aCharset' is removed.
		For example: for the string "abcdef" with a charset made of "cd", this method
		returns "abef".
		*/
	PMStrRef RemoveCharSet(const PMCharSet &aCharSet);

		/**
		Creates a 'PMStr' which is a copy of the string, but in which the sub-string
		given by 'aStart' and 'aLen' is replaced by the 'aLen' first characters
		in 'aReplace'. If 'aLen' > 'aReplace->size()' the latter is used.
		The returned string will always have the same 'size()' as 'this'.
		*/
	PMStrRef ReplaceSubString(size_t aStart, size_t aLen, const PMStrRef& aReplace);

	// -----------------------------------------------------------------------
	//	Splitting. All these functions return new strings
	// -----------------------------------------------------------------------

		/**
		Creates a 'PMStrArray' by splitting a string on the specified character.
		For example the string "aa:b:ccc" split on ':' returns an array made of 
		3 strings: "aa", "b", "ccc".
		*/
	PMStrArrayRef SplitBy(char aChar);

		/**
		Creates a 'PMStrArray' by splitting a string on the specified string.
		For example the string "aa==b==ccc" splitted on "==" returns an array made of 
		3 strings: "aa", "b", "ccc".
		*/
	PMStrArrayRef SplitBy(const PMStrRef &aSplitString);

		/**
		Creates a 'PMStrArray' by splitting a string on the specified charset.
		Every character in the charset is considered as a delimiter.
		For example the string " aa  b : ccc" split on " :" returns an array of 
		3 strings: "aa", "b", "ccc".
		*/
	PMStrArrayRef SplitBy(const PMCharSet &aCharSet);

		/**
		Creates a 'PMStrArray' by splitting the string on the first occurrence of a specified character.
		For example the string "aa:b:ccc" split on ':' returns an array made of 
		2 strings: "aa", "b:ccc".
		*/
	PMStrArrayRef SplitByFirst(char aChar);

		/**
		Creates a 'PMStrArray' by splitting the string on the last occurrence of a specified character.
		For example the string "aa:b:ccc" split on ':' returns an array of 
		2 strings: "aa:b", "ccc".
		*/
	PMStrArrayRef SplitByLast(char aChar);


	// -----------------------------------------------------------------------
	//	Testing
	// -----------------------------------------------------------------------

		/**
		Returns if the string starts with 'aStr'. If the value of 'afAnyCase' 
		is true, case-irrelevant comparison is performed.
		*/
	pmbool IsPrefix(const PMStrRef &aStr, pmbool afAnyCase = pmfalse);

		/**
		Returns if the string ends with 'aStr'. If the value of 'afAnyCase' 
		is true, case-irrelevant comparison is performed.
		*/
	pmbool IsSuffix(const PMStrRef &aStr, pmbool afAnyCase = pmfalse);


	// -----------------------------------------------------------------------
	//	Searching
	// -----------------------------------------------------------------------

		/**
		Returns the offset (zero-based) of the first occurence of 'aChar'
		starting at the position 'aStart'.
		Returns 'pmnpos' if the character is not in the string.
	    Does not search at specified start position.
	    To start search at the beginning, set 'aStart' to 'pmnpos'.
	    */
	size_t FindChar(char aChar, size_t aStart = pmnpos);

		/**
		Returns the offset (zero-based) of the last occurrence of 'aChar'
		starting at the position 'aStart'.
		Returns 'pmnpos', if the character is not in the string.
	    Does not search at specified start position.
	    */
	size_t ReverseFindChar(char aChar, size_t aStart);

		/**
		Returns the offset (zero-based) of the first occurrence of 'aStr'
		starting at the position 'aStart'.
		Returns 'pmnpos', if 'aStr' is not in the string.
	    Does not search at specified start position.
	    To start search at the beginning, set 'aStart' to 'pmnpos'.
	    */
	size_t FindStr(const PMStrRef &aStr, size_t aStart = pmnpos);

 		/**
 		Returns the offset (zero-based) of the first occurrence of the specified string
 		starting at the position 'aStart'.
 		Returns 'pmnpos', if the string is not in the string.
 	    Does not search at specified start position.
	    To start search at the beginning, set 'aStart' to 'pmnpos'.
	    */
 	size_t FindStrncase(const PMStrRef& aStr, size_t aStart = pmnpos);
		
	// -----------------------------------------------------------------------
	//	Getting substring
	// -----------------------------------------------------------------------

		/**
		Creates a substring of the string, starting at the position 'aStart', and of
		'aLen' characters.
		*/
	PMStrRef SubString(size_t aStart, size_t aLen);

		/**	Creates a substring of the string with the first 'aLen' characters.	*/
	PMStrRef Left(size_t aLen);

		/**	Creates a substring of the string with the last 'aLen' characters.	*/
	PMStrRef Right(size_t aLen);


	// -----------------------------------------------------------------------
	//	String access
	// -----------------------------------------------------------------------

		/**	Returns the string size.	*/
	size_t size() { return itsLen; }

		/**
		Returns the string as a const C string. The returned pointer is only
		valid in a const context.
		*/
	const char *c_str() const { return itsData; }


	// -----------------------------------------------------------------------
	//	For HashTable implementation
	// -----------------------------------------------------------------------

		/**	Internal use. Returns the string hash code.	*/
	pmuint32 Hash();
		
		/**	"C"-like comparison, case-relevant, returns 0 if equal.	*/
	int cCompare(const char *aStr);  

protected:

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

	PMStr_eTrait	itsTrait;
	const char 		*itsData;
	size_t 			itsLen;

		/**	Default constructor. Cannot be automatic.	*/
	PMStr();

		/**	Destructor.	*/
	virtual ~PMStr();
};

// ===========================================================================
//	PMStrRef
// ===========================================================================
/**	Convenient reference class for handling strings.	*/

class PMStrRef:public PMRef<PMStr>
{
public:

	// -----------------------------------------------------------------------
	//	Construction / Destruction
	// -----------------------------------------------------------------------

		/**	Default constructor, NULL reference.	*/
	PMStrRef();
	
		/**	int constructor.	*/
	PMStrRef(int aValue); 
	
		/**	Copy constructor.	*/
	PMStrRef(const PMStrRef &aStr);
	
		/**	'PMStr' constructor.	*/
	PMStrRef(PMStr *anObject);

		/**	C string constructor.	*/
	PMStrRef(const char * aCString);

		/**	Buffer constructor.	*/
	PMStrRef(const void * aBuffer, size_t aLength, PMStr_eTrait aTrait = PMStr_kCopy);
		
		/**	Destructor.	*/
	virtual ~PMStrRef();


	// -----------------------------------------------------------------------
	//	Operators
	// -----------------------------------------------------------------------

		/**	'PMStrRef' concatenation operator.	*/
	PMStrRef &operator+=(const PMStrRef &aStr);

		/**	C string concatenation operator.	*/
	PMStrRef &operator+=(const char *aCStr);

		/**	'PMStrRef' concatenation operator.	*/
	PMStrRef operator+(const PMStrRef &aStr);

		/**	Equality operator.	*/
	pmbool operator==(const PMStrRef &aStr) const;

		/**	Non-equality operator.	*/
	pmbool operator!=(const PMStrRef &aStr) const;

		/**	C string conversion operator.	*/
	operator const char *() const;

		/**	Concatenation operator.	*/
	PMStrRef operator+(const char *aStr);
		
		/**
		Internal use.
		Handles buffer manipulation for system calls.
		The reference now points to a new PMStr with a buffer a aSize+1, allocated
		using '::pm_malloc' which will be deleted (PMStr_kDelete).
		Returns the allocated buffer.
		*/
	char* NewBuffer(size_t aSize);

		/**
		Internal use.
		You must call this method after a call to NewBuffer and manipulating
		the buffer. It allows the string to update its contents.
		If 'aSize' is default or PMStr_kUnknown, '::pm_strlen' is used to obtain the string len.
		If 'aSize' is >=0 a null char is appended at itsBuffer[aSize] and itsLen is set to aSize
		Returns the size (as returned by 'size()').
		*/
	size_t Adjust(size_t aSize = PMStr_kUnknown);

protected:

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

		/**	For Developer Studio debugger	*/
#if defined(PM_DEBUG_STR)
		const char * itsStr;

public:
		PMStrRef operator=(const PMStrRef& aStr);
#endif
};

// ---------------------------------------------------------------------------
/**
Inline for spotting code that creates PMStrRef from static strings.
Do not use this function with non-static strings.
*/

inline PMStrRef PMSTRSTATIC(const char *aStr)	{ return PMStr::NewFromCString(aStr, PMStr_kRef, PMStr_kUnknown); }

// ---------------------------------------------------------------------------

	/**	C function that returns the hash code of the specified PMStr.	*/
pmuint32 Hash(const PMStrRef &aString);

	/**
	C function to compare two PMStr (case-relevant comparison), returns 0 if they 
	are equal.
	*/
int Compare(const PMStrRef &aString1, const PMStrRef &aString2);

// ===========================================================================
//	PMFileNameHelper
// ===========================================================================
/**	Helpers to constrain a file name for a specific operating system.	*/

class PMFileNameHelper
{
public:
		/**
		Cuts to 8.3 uppercase characters; contains only [A-Z0-9_].
		Other characters are replaced by '_'.
		*/
	static PMStrRef ToDOSFileName(const PMStrRef &aFileName);  
		    
		/**
		Cuts to 60.4 chars.
		Replaces control characters and [<>:\"/\\| ] by '_'.
		*/
	static PMStrRef ToWin32FileName(const PMStrRef &aFileName);
	
		/**
		Cuts to 31 chars.
		Replaces control chars and ':' by '_'.
		*/
	static PMStrRef ToMacFileName(const PMStrRef &aFileName);
	
		/**
		Cuts to 31.4 chars.
		Replaces control chars and '/' by '_'.
		*/
	static PMStrRef ToUNIXFileName(const PMStrRef &aFileName);

protected:
	static PMStrRef DoConversion(const PMStrRef &aFileName, const PMCharSet& aCharSet, size_t aNameLen, size_t anExtLen);
};

// ===========================================================================
//	PMStrConverter
// ===========================================================================
/**	Helpers to perform conversion to and from hexadecimal strings.	*/

class PMStrConverter
{
public:
		/**
		Converts a string to its numeric representation.
		Return true if conversion was performed.
		For example: converts the string "41424344" to the long 0x41424344.
		*/
	static pmbool	ToMacType(const PMStrRef &aStr, pmuint32 *outType);

		/**
		Converts a pmuint32 to its string representation.
		For example: converts the long 0x41424344 to "41424344".
		*/
	static PMStrRef	FromMacType(pmuint32 aTypeCreator);

		/**
		Converts an unsigned character to its hexadecimal string representation.
		The specified string size must be at least 2.
		NOTE: the hex string is not zero-terminated.
		For example: converts the long 'A' to "41".
		*/
	static void	ToHex(unsigned char aChar, char* aStr);

		/**
		Converts a two character hexadecimal string to a character value.
		Returns false if the string is not a valid hex string or is not at least
		two characters long.
		NOTE: the hex character letter can be either upper or lower case.
		For example: converts the string "41" to the character 'A'.
		*/
	static pmbool FromHex(const char* aStr, char* aChar);
};

// ---------------------------------------------------------------------------
/**	Include PMArray.h to avoid half-defined template classes.	*/

#include "PMArray.h"    

// ---------------------------------------------------------------------------

#endif  // PMStr_h


