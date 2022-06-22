// ===========================================================================
//	PMRC.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMRC_h
#define PMRC_h

// ----------------------------------------------------------------------------

#define PM_INTERNAL_INCLUDE_ALLOWED
	#include "PMStub.h"
	#include "PMDebug.h"
#undef PM_INTERNAL_INCLUDE_ALLOWED

/* 
These macros are used to increment and decrement reference counter in a 
thread safe manner. They must be defined in PMDefs.h.
They must return change the counter and return the new value.
*/
#ifndef PM_TS_INCREMENT
	#error PM_TS_INCREMENT must be defined in PMDefs.h
#endif
#ifndef PM_TS_DECREMENT
	#error PM_TS_DECREMENT must be defined in PMDefs.h
#endif

class PMCriticalSection;

// ===========================================================================
//	PMRC
// ===========================================================================
/**	Base class for reference-counted objects.	*/

class PMRC
{
public:

	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**
		Constructor.
		PMRC objects should not be allocated on the stack.
		*/
	PMRC();

		/**	Destructor.	*/
	virtual ~PMRC();


	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------

		/**	Increments reference count.	*/
	// Release version is inline for optimization.
#ifndef PMFinalRelease
	void Retain() const;
#else /* PMFinalRelease */
	void Retain() const { PM_TS_INCREMENT(((PMRC*)this)->itsRefCount); }
#endif /* PMFinalRelease */

		/**	Decrements reference count. If the reference count is 0,
		'NotifyRefCountIsNull()' is called.	
		*/
	void Release() const;

		/**	Returns the number of references taken for this object.	*/
	pm_refcount	_GetRefCount()
		{ return itsRefCount; }
	
		/**
		Use this method to register static reference-counted objects for deletion.
		This is necessary because no operations on reference-counted objects can
		happen after 'pm_close()' has been called.
		If you do not register a static reference-counted object, it will be deleted
		when 'main()' exits, which will raise an assertion.
		*/
	static void RegisterRefCleanUp(class PMRefAny *aRefPtr);

		/**
		Use this method to get 'aFunction' called when 'pm_close()' is called. This
		is the mechanism you should use to perform your own cleanup.
		*/
	static void RegisterCleanUp(void (*aFunction)());

		/**
		Internal Use.
		*/
		// Returns a critical section that you can use to initialize 
		// PM global shared objects.
	static PMCriticalSection* GetGlobalCriticalSection();

protected:

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

	/*
pm_refcount is the type that handles the reference counter.
It must be defined in PMDefs.h.
*/
	pm_refcount	itsRefCount;

		/**
		This method is called whenever reference count reachs zero.
		It can be used to hack against cycle reference counts.
		(In the constructor, the object chooses a non-zero reference count (-1 for example).
		Therefore this method will be called when there is still one reference to the object.
		It would be the place to ask neighbour to forget about ourselves, and to delete
		the object...)
		*/
	virtual void NotifyRefCountIsNull()
		{ PMIN("PMRC::NotifyRefCountIsNull()"); delete this; PMOUT; }

	void _IncRefCount()	{ PM_TS_INCREMENT(itsRefCount); }

	void _DecRefCount()	{ PM_TS_DECREMENT(itsRefCount); }

	// -----------------------------------------------------------------------
	//	For PM stub
	// -----------------------------------------------------------------------

	friend pmbool pm_init(void*);
	friend void pm_close(void*);

		/**	Initialize basic structures.	*/
	static void _Init();	
	
		/**	Call clean-up functions.	*/
	static void _CleanUp();
	
		/**	Destroy basic structures.	*/
	static void _Close();		
};


// ===========================================================================
//	PMRefAny
// ===========================================================================
/*
Due to "code bloat" problems, PMRef has been split into two classes:
PMRefAny which implements reference counting, and PMRef<T>, 
the templatized version.
*/

/**	Internal use. Implements generic reference counting. 	*/
class PMRefAny
{
public:
	PMRefAny()
		{ itsPointer = 0; }

	PMRefAny(int aValue)
	{	
		if (aValue != 0)
			BadInitValue();
		itsPointer = 0;
	}

	PMRefAny(const PMRefAny &aRef)
	{	
		if ((itsPointer = aRef.itsPointer) != 0)
			itsPointer->Retain();
	}

	PMRefAny(PMRC *anObject)
	{	
		if ((itsPointer = anObject) != 0)
			itsPointer->Retain();
	}

#ifdef PMF_RC_DEBUG
	virtual
#endif
	~PMRefAny()
	{	
		if (itsPointer != 0)
		{
			itsPointer->Release();
			itsPointer = 0;
		}
	}

	PMRC *Get() const 
		{ return itsPointer; }

	PMRefAny &operator= (const PMRefAny &anOtherObject);

	PMRC *operator->() const
	{	
		if (!itsPointer)
			NullPointerAccess();
		return itsPointer;
	}

	int operator== (const PMRefAny &aRef) const
	{	
		return itsPointer == aRef.itsPointer;
	}

	int operator!=(const PMRefAny &aRef) const
	{	
		return !((*this) == aRef);
	}

	pmbool IsNull() const 
		{ return itsPointer==0; }

protected:
	PMRC *itsPointer;

	void _SetPointedObject(PMRC *anObject)
		{ itsPointer = anObject; }

	PMRC *_GetPointedObject()
		{ return itsPointer; }

	void NullPointerAccess() const;
	
	void BadInitValue() const;
};

// ===========================================================================
//	PMRef
// ===========================================================================
/**	Smart pointer (reference) to a PMRC subclass.	*/

template <class T> class PMRef : public PMRefAny
{	
public:

	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**	Default constructor. The associated object is null.	*/
	PMRef() : PMRefAny()
		{ PM_INTERNAL_LOG("PMRef()"); }

		/**
		Int constructor, enabling clients to write things like:
		return 0;       // returns null reference
		*/
	PMRef(int value) : PMRefAny(value)
		{ PM_INTERNAL_LOG("PMRef(int aValue)"); }

		/**	PMRC constructor.	*/
	PMRef(T *anObject) : PMRefAny((PMRC *) anObject)
		{ PM_INTERNAL_LOG("PMRef(T *anObject)"); }
 
		/**	Copy constructor.	*/
	PMRef(const PMRef<T> &aRef) : PMRefAny(aRef.itsPointer)
		{ PM_INTERNAL_LOG("PMRef(const PMRef &r)"); }

#ifdef PMF_RC_DEBUG
		/**	Destructor. Releases the object.	*/
	~PMRef()
		{ PM_INTERNAL_LOG("~PMRef()"); }
#endif

	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------
      
		/**	Returns the associated object.	*/
	T *Get() const
	{
		PM_INTERNAL_LOG("T* Get() const");
		return (T*)PMRefAny::Get();
	}

		/**	Copy operator.	*/
	PMRef<T> &operator= (const PMRef<T> &aRef)
	{
		PM_INTERNAL_LOG("PMRef &operator=(const PMRef &aRef)");

		PMRefAny::operator= (aRef);
		return *this;
	}

		/**
		Accesss associated object.
		Applying pointer decay is the only way to access the associated object.
		*/
	T *operator->() const
	{
		PM_INTERNAL_LOG("T *operator->() const");
		return (T*)PMRefAny::operator->();
	}

		/**
		Equality comparison operator. Does not do a deep comparison.
		*/
	int operator== (const PMRef<T> &aRef) const
	{
		PM_INTERNAL_LOG("int operator==(const PMRef &aRef) const");
		return itsPointer == aRef.itsPointer;
	}

		/**
		Non-equality comparison operator. Does not do a deep comparison.
		*/
	int operator!= (const PMRef<T> &aRef) const
	{
		PM_INTERNAL_LOG("int operator!=(const PMRef &aRef) const");
		return itsPointer != aRef.itsPointer;
	}
	
		/**	Tests reference invalidity (i.e tests whether or not the object is NULL)	*/
	pmbool IsNull() const
	{
		PM_INTERNAL_LOG("pmbool IsNull() const");
		return PMRefAny::IsNull();
	}

	// -----------------------------------------------------------------------
	//	Advanced
	// -----------------------------------------------------------------------

		/**
		Changes the pointer without updating reference counts in either direction.
		This method is used to hack around RC bugs.
		(For retaining a new object without releasing the preceding one,
		use _SetPointedObject(0) followed by operator=()...
		However, use of this is discouraged...
		*/
	void _SetPointedObject(T *anObject)
	{
		PM_INTERNAL_LOG("void _SetPointedObject(T *anObject)");
		PMRefAny::_SetPointedObject((PMRC*)anObject);
	}

	T *_GetPointedObject()
	{
		PM_INTERNAL_LOG("T *_GetPointedObject()");
		return (T*)PMRefAny::_GetPointedObject();
	}

	// ----------------------------------------------------------------------------
	//	Any other operation is strictly forbidden.
};

// ----------------------------------------------------------------------------
//	Helpers to perform object comparisons.
// ----------------------------------------------------------------------------

	/** Compares two 'pmuint16'. Returns 0 if equal. */
int Compare(pmuint16 a, pmuint16 b);

	/** Compares two 'pmint16'. Returns 0 if equal. */
int Compare(pmint16 a, pmint16 b);

	/** Compares two 'pmuint32'. Returns 0 if equal. */
int Compare(pmuint32 a, pmuint32 b);

	/** Compares two 'pmint32'. Returns 0 if equal. */
int Compare(pmint32 a, pmint32 b);

	/**	
	Called when a non-implemented comparison occurs (raising an assertion). 
	If you need to compare such objects, you should implement the appropriate 
	compare function.
	*/
void ThrowCompare();

	/**	Function (template) to compare two objects.	Always calls 'ThrowCompare()'.*/
template<class T> int Compare(const T&, const T&) 
	{ ::ThrowCompare(); return 0; }

// ----------------------------------------------------------------------------

	/**	Returns the hash value for an 'int'. In reality just returns 'anInt'.	*/
pmuint32 Hash(int anInt); 

// ----------------------------------------------------------------------------

	/**	Macro used to declare a reference-counted object.	*/
#define PMDEFINE(aClass)				\
	class aClass;						\
	typedef PMRef<aClass> aClass##Ref

// ----------------------------------------------------------------------------

#endif  // PMRC_h

