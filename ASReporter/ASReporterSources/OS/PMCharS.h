// ===========================================================================
//	PMCharS.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMCharS_h
#define PMCharS_h

// ---------------------------------------------------------------------------

#include "PMRC.h"

// ===========================================================================
//	PMCharSet
// ===========================================================================
/**
Handles a set of characters, ranging from 0 to 255. Characters can be added 
or removed from a charset. Basic set operations (negate, union, intersect) 
can be used on a charset. Some default character sets (lower alphas, upper 
alphas, etc.) have been predefined.
*/

class PMCharSet
{
public:
	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**	Default constructor. The charset is empty.	*/
	PMCharSet();
	
		/**
		C string constructor. The charset is made of characters from 
		'aCharSetString'.
		*/
	PMCharSet(const char *aCharSetString);

		/**	Copy constructor.	*/
	PMCharSet(const PMCharSet &aSet);

		/**	Copy operator.	*/
	PMCharSet& operator= (const PMCharSet &aSet);

	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------

		/**	Adds 'aChar' to the charset.	*/
	void Add(char aChar);

		/**	Adds characters from 'aSetString' to the charset.	*/
	void Add(const char *aSetString);

		/**	Removes 'aChar' from the charset.	*/
	void Remove(char aChar);

		/**	Removes characters in 'aSetString' from the charset.	*/
	void Remove(const char *aSetString);

		/**	Performs a union with 'aCharSet'.	*/
	void UnionWith(const PMCharSet &aCharSet);
	
		/**	Performs intersection with 'aCharSet'.	*/
	void IntersectionWith(const PMCharSet &aCharSet);

		/**	Removes characters that are part of 'aCharSet' from the charset.	*/
	void Substract(const PMCharSet &aCharSet);

		/**
		Negates the charset. All characters in the charset are removed while
		all characters not in the charset are added to it.
		*/
	void Negate();

		/**
		Returns if 'aChar' is part of the charset.
		(Inlined and optimized for maximum performance).
		*/
	pmbool IsPresent(char aChar) const
	{ 
		return (itsFlags[ (pmuint32) (((pmuint32)((unsigned char) aChar)) >> (pmuint32) 5) ] 
			& (pmuint32)((pmuint32)1 << (pmuint32)(((pmuint32)((unsigned char) aChar)) & (pmuint32) 31))) 
			!= 0; 
	}

	// -----------------------------------------------------------------------
	//	Factory
	// -----------------------------------------------------------------------

		/**
		Returns an empty charset.
		This charset must not be modified (do not cast it to non const).
		*/
	static const PMCharSet& EmptySet();

		/**
		Returns a charset made only of digits: "0123456789".
		This charset must not be modified (ie: do not cast it to non const).		
		*/
	static const PMCharSet& Digits();
	
		/**
		Returns a charset made only of lower alpha characters:
		"abcdefghijklmnopqrstuvwxyz".
		This charset must not be modified (ie: do not cast it to non const).
		*/
	static const PMCharSet& LowerAlphas();
	
		/**
		Returns a charset made only of upper alpha characters:
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ".
		This charset must not be modified (ie: do not cast it to non const).
		*/
	static const PMCharSet& UpperAlphas();
	
		/**
		Returns a charset made only of upper alpha, lower alpha, and digit 
		characters:
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEF".
		This charset must not be modified (ie: do not cast it to non const).
		*/
	static const PMCharSet& AlphasDigits();
	
		/**
		Returns a charset made only of characters used for hexadecimal
		representation of integers:
		"0123456789ABCDEFabcdef".
		This charset must not be modified (ie: do not cast it to non const).
		*/
	static const PMCharSet& Hex();
	
		/**
		Returns a charset made only of "white spaces":
		ASCII codes from 9 to 13 (inclusive).
		This charset must not be modified (ie: do not cast it to non const).
		*/
	static const PMCharSet& WhiteSpaces();	

		/**
		Returns a charset made only of 7-bit characters:
		ASCII codes from 0 to 127 (inclusive).
		This charset must not be modified (ie: do not cast it to non const).
		*/
	static const PMCharSet& SevenBits();
	
		/**
		Returns a charset made only of "control" characters:
		ASCII codes from 0 to 31 (inclusive).
		This charset must not be modified (ie: do not cast it to non const).
		*/
	static const PMCharSet& Controls();	

protected:
	
	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

	pmuint32 itsFlags[8];
};

// ===========================================================================
//	PMStringSet
// ===========================================================================
/**
Handles a set of strings.
Some default string sets, such as days, months (abbreviated using 3 characters), 
etc. are predefined. Indexes are zero-based.
*/

class PMStringSet
{
public:

	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**
		String array constructor. The array must be terminated by a NULL pointer.
		The array is not copied, and so must not be deallocated or modified.
		*/
	PMStringSet(const char **aStringSet);

	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------

		/**
		Returns a pointer to the string located at the specified index.
		A null pointer is returned if the index is out of range.
		*/
	const char*	StringAtIndex(size_t anIndex) const;
	
	// -----------------------------------------------------------------------
	//	Factory
	// -----------------------------------------------------------------------

		/**
		Returns a string set made only of abbreviated days of the week using 
		a 3 character representation (mon, tue, wed ...).
		*/
	static const PMStringSet& Day_3();	
	
		/**	Returns a string set made only of the names of the days of the week.	*/
	static const PMStringSet& Day_n();

		/**
		Returns a string set made only of abbreviated month names using 
		a 3 character representation (jan, feb, mar ...).
		*/
	static const PMStringSet& Month_3();	

protected:
	
	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

	const char**	itsStringSet;
	size_t			itsStringCount;
};


// ---------------------------------------------------------------------------

#endif	// PMCharS_h

