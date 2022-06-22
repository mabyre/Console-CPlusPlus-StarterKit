// ===========================================================================
//	PMArray.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMArray_h
#define PMArray_h

// ---------------------------------------------------------------------------

#include "PMRC.h"
#include "PMTrace.h"

// ---------------------------------------------------------------------------
/**
This is a templatized placement operator.
(Objects in arrays need to be constructed in-place).
*/

template <class T> void *operator new(size_t, T *aPtr) 
	{ return aPtr; }

// ---------------------------------------------------------------------------
	/** Used to destruct array elements. */
template <class T> void PMDestructElement(T& anObject) 
{ 
	(&anObject)->~T();
}

/* Specialization for integral types */
	/** Destructs a 'pmuint16': does nothing. */
void PMDestructElement(pmuint16& a);

	/** Destructs a 'pmint16': does nothing. */
void PMDestructElement(pmint16& a);

	/** Destructs a 'pmuint32': does nothing. */
void PMDestructElement(pmuint32& a);

	/** Destructs a 'pmint32': does nothing. */
void PMDestructElement(pmint32& a);

	/** Destructs a 'char*': does nothing. */
void PMDestructElement(char*& a);
	
// ===========================================================================
//	PMArray
// ===========================================================================
/**
Handles item arrays.
PMArray has been designed to store reference-counted objects or
primitive types. In fact PMArray only handles items that do not require
a deep copy or the use of a copy operator to be copied.
For each item that is stored in an array, a 'NULL item' must be provided.
This 'NULL item' means that there is no object. For reference-counted
objects (and pointers) this value is 0. 
0 is the default value for the 'NULL item'.
Item indexes are zero-based. Every function that uses an index will assert 
if the index is greater than or equal to the number of items in the array.
*/

template <class T> class PMArray : public PMRC
{
public:

	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**	Constructor. The array is empty.	*/
    PMArray();

		/**	Destructor. All items in the array take the value of 'NULL item'.	*/
    virtual ~PMArray();

	// -----------------------------------------------------------------------
	//	Accessing. Generic acces functions.
	// -----------------------------------------------------------------------

		/**	Returns the number of items in the array.	*/
    size_t GetCount();

		/**
		Sets the number of items in the array (new items take the value of 'NULL item').
		*/
    void SetCount(size_t aCount);

		/**	Returns object at 'anIndex', for access only, (zero-based).	*/
    T &GetObjectAtIndex(size_t anIndex);

		/**	Changes object at 'anIndex'.	*/
    void SetObjectAtIndex(size_t anIndex, const T &anObject);

		/**	Inserts object at 'anIndex', (increments the number of items in the array).	*/
    void InsertObjectAtIndex(size_t anIndex, const T &anObject);

		/**	Removes object from 'anIndex', (decrements the number of items in the array).	*/
    void RemoveObjectAtIndex(size_t anIndex);	

		/**	Sets the 'NULL item' value. Default value is 0.	*/
    void SetNullObject(const T &anObject) { itsEmptyElement = anObject; }

		/**	Appends the object in the array, (increments the number of items in the array).	*/
    void AppendObject(const T &anObject);

		/**	Short-cut for 'GetObjectAtIndex', for access only.	*/
    T &operator[](size_t anIndex);                

		/**	Short-cut for 'GetObjectAtIndex', for access only.	*/
    T &At(size_t anIndex);

		/**	Swaps two items specified by their indexes.	*/
    void SwapObjects(size_t anIndex1, size_t anIndex2);

		/**
		Searches for an item.
		Eventually fills 'anIndex' with index reference of the first occurence.
		This method uses the global function 'Compare'. If you need to use this method, you should
		provide a 'Compare' function for the objects you store in the array. 'Compare'
		functions are provided for most framework types as well as basic types.
		*/
    pmbool IsObjectPresent(const T &anObject, size_t *anIndex = 0);

		/**	Reserves space for 'aSize' items (extends the array by inserting the 'NULL item').	*/
    void ExtendTo(size_t aSize);

protected:

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------
    
	T 		itsEmptyElement;
    T 		*itsData;
    size_t 	itsSize;
    size_t 	itsAllocatedSize;

		/**	Asserts if the specified index is invalid.	*/
    void DoCheckIndex(size_t anIndex);

    void CheckAllocate(size_t aNeededSize);
};

// ===========================================================================
// IMPLEMENTATION of PMArray<T>

template <class T> PMArray<T>::PMArray()
{
	itsData = 0;
	itsSize = 0;
	itsAllocatedSize = 0;
               
		//	Constructs the empty item.
	// #### Is it really needed ? 
	new (&itsEmptyElement)(T);
}

// ---------------------------------------------------------------------------

template <class T> PMArray<T>::~PMArray()
{	
	for (size_t i = 0 ; i != itsSize ; i++)
	//	RNi (27/03/98): Was:
	//	itsData[i] = itsEmptyElement;
		PMDestructElement(itsData[i]);

    ::pm_free((void *)itsData);
}

// ---------------------------------------------------------------------------

template <class T> void PMArray<T>::AppendObject(const T &anObject)
{	
	InsertObjectAtIndex(GetCount(), anObject);
}

// ---------------------------------------------------------------------------

template <class T> T &PMArray<T>::operator[](size_t anIndex)
{	
	return GetObjectAtIndex(anIndex);
}

// ---------------------------------------------------------------------------

template <class T> T &PMArray<T>::At(size_t anIndex)
{	 
	return GetObjectAtIndex(anIndex);
}

// ---------------------------------------------------------------------------

template <class T> void PMArray<T>::SwapObjects(size_t anIndex1, size_t anIndex2)
{	
	DoCheckIndex(anIndex1);
	DoCheckIndex(anIndex2);
	if (anIndex1 != anIndex2)
	{	
		const T theObject = GetObjectAtIndex(anIndex1);
		SetObjectAtIndex(anIndex1, GetObjectAtIndex(anIndex2));
		SetObjectAtIndex(anIndex2, theObject);
	}
}

// ---------------------------------------------------------------------------

template <class T> void PMArray<T>::ExtendTo(size_t aSize)
{	
	while (GetCount() < aSize)
		AppendObject(itsEmptyElement);
}

// ---------------------------------------------------------------------------

template <class T> void PMArray<T>::DoCheckIndex(size_t anIndex)
{	
	PM_ASSERT(anIndex < GetCount(), TL("Out of bounds"));
}

// ---------------------------------------------------------------------------

template <class T> void PMArray<T>::CheckAllocate(size_t aNeededSize)
{	
	const size_t PMArray_kBaseSize = 16;

	size_t theNewAllocatedSize = itsAllocatedSize;

    if (!theNewAllocatedSize)
        theNewAllocatedSize = PMArray_kBaseSize;

    while (aNeededSize > theNewAllocatedSize)
        theNewAllocatedSize *= 2;

	if (theNewAllocatedSize>itsAllocatedSize)
	{   
		T *theNewData = (T *) ::pm_realloc(itsData, theNewAllocatedSize * sizeof(T));
        PM_ASSERT(theNewData, TL("Should have raised"));
        
       		// #### Note: array items will be constructed when they will be accessed
        itsData = theNewData;
        itsAllocatedSize = theNewAllocatedSize;
    }
};

// ---------------------------------------------------------------------------

template <class T> size_t PMArray<T>::GetCount()
{	
	return itsSize;
}

// ---------------------------------------------------------------------------

template <class T> void PMArray<T>::SetCount(size_t aCount)
{	
		//	#### If aCount < itsSize, itsData[aCount..itsSize] = itsEmptyElement do 
		//	destroy objects
	CheckAllocate(aCount);
	for (size_t i = itsSize ; i < aCount ; i++)
	{	
		new(itsData + i)(T);
		itsData[i] = itsEmptyElement;
	}
	itsSize = aCount;
}

// ---------------------------------------------------------------------------

template <class T> T &PMArray<T>::GetObjectAtIndex(size_t anIndex)
{	
	DoCheckIndex(anIndex);
	return itsData[anIndex];
}

// ---------------------------------------------------------------------------

template <class T> void PMArray<T>::SetObjectAtIndex(size_t anIndex, const T &anObject)
{	
	CheckAllocate(anIndex + 1);
    if (anIndex>=itsSize)
	{	
			//	This fills the eventual hole with empty items
			//	(And the place where we copies the item will be blanked
			//  we could optimize that...)
        for (size_t i = itsSize ; i <= anIndex ; i++)
        {	
        		// new(itsData+i)(T)(itsEmptyElement);
			new(itsData + i)(T);
			itsData[i] = itsEmptyElement;
        }
        itsSize = anIndex + 1;
    }
    itsData[anIndex] = anObject;
}

// ---------------------------------------------------------------------------

template <class T> void PMArray<T>::InsertObjectAtIndex(size_t anIndex, const T &anObject)
{	
	CheckAllocate(itsSize + 1);

		//	#### Remove for production code
		//	Acceptable is anIndex valid or equal to size
    if (anIndex != itsSize)                   
		DoCheckIndex(anIndex);
    ::pm_memmove(itsData + anIndex + 1, itsData+anIndex, (itsSize - anIndex) * sizeof(T));
    itsSize++;

		//	This create itsData[anIndex] to be a copy of anObject
   		//	new(itsData+anIndex)(T)(anObject);
    new(itsData + anIndex)(T);
    itsData[anIndex] = anObject;
}

// ---------------------------------------------------------------------------

template <class T> void PMArray<T>::RemoveObjectAtIndex(size_t anIndex)
{	
	DoCheckIndex(anIndex);
	// RNi (27/03/98): Was:
    //itsData[anIndex] = itsEmptyElement;
	PMDestructElement(itsData[anIndex]);

		//	#### Note, this erase 'itsEmptyElement'
		//	Copy operators will not be call correctly for this item
    ::pm_memmove(itsData + anIndex, itsData+anIndex + 1, (itsSize - anIndex - 1) * sizeof(T));
    itsSize--;
}

// ---------------------------------------------------------------------------

template <class T> pmbool PMArray<T>::IsObjectPresent(const T &anObject, size_t *anIndex)
{
	for (size_t i = 0; i < itsSize; i++)
		if (Compare(itsData[i], anObject) == 0)
		{
			if (anIndex != 0)
				*anIndex = i;
				
			return pmtrue;
		}

	return pmfalse;
}

// ---------------------------------------------------------------------------
// This macro siplifies the declaration of a array to a class
// PMARRAYDEFINE(XXX)
// defines the class, the PMRef<> to it and the PMRef<PMArray<PMRef<>>> to it
// Under the respective names of
//    XXXRef and XXXArrayRef
// (Note that XXXArrayRef is _not_ called XXXRefArrayRef)
// ---------------------------------------------------------------------------
// The general strategy, to limit circular reference of header files,
// is, in a new class, to include PMArray.h, and to PMDEFINE/PMARRAYDEFINE
// all the classes that are declared in that header file.
// After that, includes the related files, and declare the classes
// ---------------------------------------------------------------------------

#define PMARRAYDEFINE(aClass)                                                                         \
        PMDEFINE(aClass);                                                                                             \
        typedef PMRef<PMArray<aClass##Ref> > aClass##ArrayRef

// ---------------------------------------------------------------------------

	/**	A reference counted array of 'int'.	*/
typedef PMRef<PMArray<int> > PMIntArrayRef;

// ---------------------------------------------------------------------------

#endif  // PMArray_h



