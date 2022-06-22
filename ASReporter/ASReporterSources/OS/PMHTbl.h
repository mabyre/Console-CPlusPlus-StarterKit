// ===========================================================================
//	PMHTbl.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMHTbl_h
#define PMHTbl_h

// ---------------------------------------------------------------------------

#include "PMStr.h"
#include "PMArray.h"

// ---------------------------------------------------------------------------

	/**	Internal use. Successive sizes of hash table.	*/
extern pmint32 gHashTableSize[];	

// ===========================================================================
//	PMMapPair
// ===========================================================================
/**
Internal use.
This class should be embodied in PMMap, but this is not acceptable to 
all compilers.
*/

template <class T, class K> class PMMapPair : public PMRC
{	
public:

		/**	Internal use.	*/
	K GetKey() 
		{ return itsKey; }
	
		/**	Internal use.	*/
	T GetObject() 
		{ return itsObject; }
	
		/**	Internal use.	*/
	void SetObject(const T &anObject)
		{ itsObject = anObject; }
	
		/**	Internal use.	*/
	PMMapPair(const K &aKey, const T &anObject)
		{ itsObject = anObject; itsKey = aKey; }
	 	
		/**	Internal use.	*/
	~PMMapPair()	
		{}

protected:
	K itsKey;
	T itsObject;
};

// ===========================================================================
//	PMMap
// ===========================================================================
/**
Handles mapping between an object and a key. The  object and the key are 
templatized types.
*/

template <class T, class K> class PMMap : public PMRC
{
public:

	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**	Default constructor. The map is empty.	*/
	PMMap();
		
		/**	Destructor.	*/
	virtual ~PMMap();

	// -----------------------------------------------------------------------
	//	Acessing
	// -----------------------------------------------------------------------

		/**
		Attaches 'anObject' to 'aKey'.
		If there is no entry for the key, one is created.
		*/
	T SetObjectForKey(const T &anObject, const K &aKey);
	
		/**
		Returns the object attached to 'aKey'.
		If the key has no entry or no attached object the NULL value (as specified 
		by SetNullValue) is returned.
		*/
	T ObjectForKey(const K &aKey);
	
		/**	Removes a key entry.	*/
	void RemoveKey(const K &aKey);
	
		/**	Returns true if the specified key has already an entry in the map.	*/
	pmbool HasKey(const K &aKey);

		/**	Sets the map's NULL value.	*/
	void SetNullValue(const T &aValue) 
		{ itsNullValue = aValue; }
	
		/**	Returns the map's NULL value; the default NULL value is 0.	*/
	T GetNullValue() 
		{ return itsNullValue; }

		/**	Returns an array made of the map's key entries.	*/
	PMRef<PMArray<K> > GetKeyArray();


	typedef PMMapPair<T, K> Pair;

		//	The xlC compiler on AIX systems needs 'PairRef' to be public
	typedef PMRef<Pair> PairRef;

protected:

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

	class PMMapArray : public PMArray<PairRef>
	{	
	public:
		PairRef GetPairOfKey(const K &aKey)
		{	
			for (size_t i = 0 ; i != GetCount() ; i++)
				if (!Compare(GetObjectAtIndex(i)->GetKey(), aKey))
					return GetObjectAtIndex(i);
			return 0;
		}
		
		void RemoveKey(const K &aKey)
		{	
			for (size_t i = 0 ; i != GetCount() ; i++)
				if (!Compare(GetObjectAtIndex(i)->GetKey(), aKey))
				{	
					RemoveObjectAtIndex(i);
					return;
				}
		}
		
		void AppendKeysTo(const PMRef<PMArray<K> > &anArray)
		{	
			for (size_t i = 0 ; i != GetCount() ; i++)
				anArray->AppendObject(GetObjectAtIndex(i)->GetKey());
		}
	};
	
protected:

	typedef PMRef<PMMapArray> PMMapArrayRef;

	size_t 							itsSizeIndex;
	size_t 							itsSize;
	size_t 							itsMaxSize;
	PMRef<PMArray<PMMapArrayRef> >	itsObjects;

	void _IncSize();
	
	void _InsertPair(const PairRef &aPair);
	
	pmuint32 _GetHashForKey(const K &aKey);
	
	PMMapArrayRef _GetArrayForKey(const K &aKey)
		{ return itsObjects->GetObjectAtIndex(_GetHashForKey(aKey)); }

	PairRef _CreatePair(const K &aKey, const T &anObject)
		{ return new Pair(aKey, anObject); }

		/**	Value returned when no association is found.	*/
	T itsNullValue;		
};


// ---------------------------------------------------------------------------
//	IMPLEMENTATION of PMMap template
// ---------------------------------------------------------------------------

template <class T, class K> PMMap<T,K>::PMMap()
{	
	itsSizeIndex = 0;
	itsMaxSize = gHashTableSize[itsSizeIndex];
	itsSize = 0;
	itsObjects = new PMArray<PMMapArrayRef>;
	itsObjects->ExtendTo(itsMaxSize);
	itsNullValue = (T) 0;
}

// ---------------------------------------------------------------------------

template <class T, class K> PMMap<T,K>::~PMMap()
{
}

// ---------------------------------------------------------------------------

template <class T, class K> void PMMap<T,K>::_IncSize()
{	
	itsSize++;
	if (itsSize > itsMaxSize * 4)
	{		
			//	We empty the table than refill it
		itsSizeIndex++;
		itsMaxSize = gHashTableSize[itsSizeIndex];
		
		PMRef<PMArray<PMMapArrayRef> > theOldObjects = itsObjects;
		
		itsObjects = new PMArray<PMMapArrayRef>;
		itsObjects->ExtendTo(itsMaxSize);

		for (size_t i = 0 ; i != theOldObjects->GetCount() ; i++)
		{	
			PMMapArrayRef theArray = theOldObjects->GetObjectAtIndex(i);
			
			if (!theArray.IsNull())
				for (size_t j = 0 ; j != theArray->GetCount() ; j++)
				{	
					PairRef thePair = theArray->GetObjectAtIndex(j);
					_InsertPair(thePair);
				}
		}
	}
}

// ---------------------------------------------------------------------------

template <class T, class K> void PMMap<T,K>::_InsertPair(const PMRef<PMMapPair<T,K> > &aPair)
{	
	PMMapArrayRef theArray = _GetArrayForKey(aPair->GetKey());
	
	if (theArray.IsNull())
	{	
		theArray = new PMMapArray;
		itsObjects->SetObjectAtIndex(_GetHashForKey(aPair->GetKey()), theArray);
	}
	
	theArray->AppendObject(aPair);
}

// ---------------------------------------------------------------------------

template <class T, class K> pmuint32 PMMap<T,K>::_GetHashForKey(const K &aKey)
{	
	return ::Hash(aKey)%itsMaxSize;
}

// ---------------------------------------------------------------------------

template <class T, class K> T PMMap<T,K>::SetObjectForKey(const T &anObject, const K &aKey)
{	
	PMMapArrayRef theArray = _GetArrayForKey(aKey);
	
	if (!theArray.IsNull())
	{	
		PairRef thePair = theArray->GetPairOfKey(aKey);
		
		if (!thePair.IsNull())
		{	
			T theObject = thePair->GetObject();
			
			thePair->SetObject(anObject);
			return theObject;
		}
	}
	else
	{	
		theArray = new PMMapArray();
		itsObjects->SetObjectAtIndex(_GetHashForKey(aKey), theArray);
	}

	theArray->AppendObject(_CreatePair(aKey, anObject));

	_IncSize();

	return itsNullValue;
}

// ---------------------------------------------------------------------------

template <class T, class K> pmbool PMMap<T,K>::HasKey(const K &aKey)
{	
	PMMapArrayRef theArray = _GetArrayForKey(aKey);
	
	if (theArray.IsNull())
		return pmfalse;
		
	return !theArray->GetPairOfKey(aKey).IsNull();
}

// ---------------------------------------------------------------------------

template <class T, class K> T PMMap<T,K>::ObjectForKey(const K &aKey)
{	
	PMMapArrayRef theArray = _GetArrayForKey(aKey);
	
	if (theArray.IsNull())	
		return itsNullValue;
		
	PairRef thePair = theArray->GetPairOfKey(aKey);
	
	if (thePair.IsNull())	
		return itsNullValue;
		
	return thePair->GetObject();
}

// ---------------------------------------------------------------------------
//	Note: removing an element only destroys its pair, not the arrays.

template <class T, class K> void PMMap<T,K>::RemoveKey(const K &aKey)
{
	PMMapArrayRef theArray = _GetArrayForKey(aKey);
	
	if (theArray.IsNull())
		return ;
		
	theArray->RemoveKey(aKey);
}

// ---------------------------------------------------------------------------

template <class T, class K> PMRef<PMArray<K> > PMMap<T,K>::GetKeyArray()
{	
	PMRef<PMArray<K> > theArray = new PMArray<K>;
	
	for (size_t i = 0 ; i != itsObjects->GetCount() ; i++)
	{	
		PMMapArrayRef theSubArray = itsObjects->GetObjectAtIndex(i);
		if (!theSubArray.IsNull())
			theSubArray->AppendKeysTo(theArray);
	}
	
	return theArray;
}

// ===========================================================================
// PMHashTable
// ===========================================================================
/**
Handles efficient mapping between a string and an object. The object is a
templatized type.
*/

template <class T> class PMHashTable : public PMMap<T, PMStrRef>
{
public:

		/**	Constructor. The hash table is empty.	*/
	PMHashTable() 
		{}
	
		/**	Destructor.	*/
	virtual ~PMHashTable() 
		{}
};

	/**	An hash table of \Ref{PMStrRef}. This hash table is not reference counted.	*/
typedef PMHashTable<PMStrRef> PMStrHashTable;

	/**	An hash table of \Ref{PMStrRef}.	*/
typedef PMRef<PMStrHashTable> PMStrHashTableRef;

// ---------------------------------------------------------------------------

#endif	// PMHtbl_h

