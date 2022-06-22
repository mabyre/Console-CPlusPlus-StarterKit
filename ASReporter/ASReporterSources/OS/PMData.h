// ===========================================================================
//	PMData.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMData_h
#define PMData_h

// ---------------------------------------------------------------------------

#include "PMRC.h"

// ===========================================================================
//	PMData
// ===========================================================================

PMDEFINE(PMData);
PMDEFINE(PMBuffer);

// ---------------------------------------------------------------------------
/**
The PMData class is a low-level wrapper around a buffer of bytes. It has been
designed as an efficient means for adding or removing bytes at the beginning, 
middle, or end of the buffer.
PMData has a size which is the number of bytes in the buffer, and a capacity 
which is the amount of bytes really allocated for the buffer.
*/

class PMData : public PMRC
{
public:
	// -----------------------------------------------------------------------
	//	Construction/Destruction
	// -----------------------------------------------------------------------

		/**	Default constructor. The buffer is empty.	*/
	PMData();

		/**	Capacity constructor, size is set to 0.	*/
	PMData(size_t aCapacity);

		/**
		Buffer constructor. Data is copied.
		Size and capacity are set to 'aSize'.
		*/
	PMData(const void *aBuffer, size_t aSize);

		/**
		Buffer and capacity constructor. Data is copied.
		Size is set to 'aSize'.
		Capacity is set to 'aCapacity'.
		*/
	PMData(const void *aBuffer, size_t aSize, size_t aCapacity);

		/**
		C string constructor. The string is copied (assuming the string is 
		0-terminated). Sets string length size and capacity. 
		Data will not be null-terminated.
		*/
	PMData(const char *aCString);

		/**	Destructor. Deallocates internal buffer.	*/
	virtual ~PMData();

		/**
		Traits constructor, reserved for optimization hints. You may specify 
		PMData behaviors by using this constructor.
		'aMinCapacity' is the minimum allocated capacity (size + overhead).
		'aMinOverhead' is the minimum number of bytes ready for expansion when reallocating data.
		'anOverheadFactor' is the number of bytes reserved for expansion (Usage: set it to 'size * itsOverheadFactor / 100').
		'anOverheadRepartition' is used when reallocating. Buffer starts at (overhead / 2) * (1 - itsOverheadRepartition / 100).
		The following are overhead factor values and meanings.
			   0 means overhead is distributed at both ends.
			 100 means all overhead is at the end.
		    -100 means all overhead is at the beginning.
		  and 50 means more overhead is at the end than at the beginning.	
		 */
	PMData(size_t aMinCapacity, size_t aMinOverhead, pmuint16 anOverheadFactor, pmint16 anOverheadRepartition);

	// -----------------------------------------------------------------------
	//	Accessing
	// -----------------------------------------------------------------------
	
		/**
		Returns a pointer to the buffer, which can only be used in a const context.
		You may read and write in it.
		*/
	void *buffer();

		/**	Returns buffer size (NOTE: this is different from buffer capacity).	*/
	size_t size();		
	
		/**	Truncates or extends the buffer to 'aSize'. The buffer may be reallocated.	*/
	void SetSize(size_t aSize);

		/**	Appends 'aBuffer' of 'aLength' bytes.	*/
	void Append(const void *aBuffer, size_t aLength);

		/**	Appends the contents of 'aData'.	*/
	void Append(const PMDataRef &aData);

		/**	Inserts 'aBuffer' of 'aBufferLength' bytes at 'anOffset'.	*/
	void Insert(size_t anOffset, const void *aBuffer, size_t aBufferLength);

		/**	Inserts the contents of 'aData' at 'anOffset'.	*/
	void Insert(size_t anOffset, const PMDataRef &aData);

		/**	Removes 'aLength' bytes starting from 'anOffset'.	*/
	void Delete(size_t anOffset, size_t aLength);

		/**	Replaces 'aLength' bytes starting at 'anOffset' by 'aBuffer' of 'aBufferLength' bytes.	*/
	void Replace(size_t anOffset, size_t aLength, const void *aBuffer, size_t aBufferLength);

		/**	Replaces 'aLength' bytes starting at 'anOffset' by the contents of 'aData'.	*/
	void Replace(size_t anOffset, size_t aLength, const PMDataRef &aData);

		/**	Removes 'aLength' bytes from the begining of the buffer.	*/
	void TruncateBegin(size_t aLength);

		/**	Removes 'aLength' bytes from the end of the buffer.	*/
	void TruncateEnd(size_t aLength);

		/**	Empties the buffer (i.e. sets buffer size to 0).	*/
	void Clear();
		
		/**
		Returns buffer capacity, which is the allocated size of the internal buffer.
		This may be used for optimization hints.
		*/
	size_t GetCapacity();
	
		/**
		Gives ownership to the caller the buffer, which means that it becomes the 
		caller's responsibility to deallocate the returned buffer. The returned 
		buffer must be deallocated using '::pm_free'.
		NOTE: Due to capacity and overhead configurations, the returned buffer
		will probably not point to the start of data. If you pass in a non-NULL
		pointer for 'aDataStart', it will be initialized with the real start of data.
		*/
	void* TakeBufferOwnership(void **aDataStart);

		/**	Returns a copy of the data.	*/
	PMDataRef	Clone();

		/**
		Creates (allocates) a 'PMBufferRef' and copies into it 'aLength' bytes
		of data located at position 'aFrom' of the 'PMDataRef'.
		If 'aLength' is not specified (default value used) or explicitly called
		with 'pmnpos', the buffer is extracted from 'aFrom' to the end.
		If 'aFrom' is greater than the buffer's length, a NULL reference is returned.
		The same thing happens if the data requested extends beyond the buffer 
		(ie : 'aFrom' + 'aLength' > size()).
		But if 'aLength' is equal to 0 (or 'pmnpos' is used), a valid empty buffer 
		is returned (and not a NULL reference). This simplifies many algorithms.
		*/
	PMBufferRef	ExtractBuffer(size_t aFrom, size_t aLength = pmnpos);

protected:

	// ---------------------------------------------------------------------------
	//	Designed to be overriden
	// ---------------------------------------------------------------------------

		/**	Returns the overhead to reserve for a buffer of 'aSize'.	*/
	virtual size_t OverheadFor(size_t aSize);

		/**	Returns the part of overhead that should be reserved for the start of the buffer.	*/
	virtual size_t HeadOverheadFor(size_t anOverhead);

		/**	Should return false if overhead is excessive.	*/
	virtual pmbool IsOverheadAcceptable(size_t aSize, size_t anOverhead);

	// -----------------------------------------------------------------------
	//	Implementation
	// -----------------------------------------------------------------------

	char*	itsBuffer;
	size_t	itsStart;
	size_t	itsSize;
	size_t	itsCapacity;

		/**
		Control of capacity
		Minimal capacity		[1000]
		*/
	size_t	itsMinCapacity;			

		/**
		Minimal overhead		[10]
		[Only when allocating/re-creating buffer]
		May be called initial overhead
		*/
	size_t	itsMinOverhead;			
									
		/**
		Factor of overhead 
		True overhead will be at most max of
		(size*itsOverheadFactor, itsMinOverhead)
		*/
	pmuint16 itsOverheadFactor;
	
		/**
		0 : Overhead evenly repartited
		100 : All overhead at end
		*/
		//	-100 : All overhead at start
		/**	Used only at allocation/re-creation	*/
	pmint16	itsOverheadRepartition;	

	void DoInit(const void *, size_t, size_t);
	void DoReplace(size_t, size_t, const void *, size_t);
	void _AllocateBufferAndStart(size_t aSize);
	void DoRecreate(const void *aBuffer1, size_t aLen1, const void *aBuffer2, size_t aLen2, const void *aBuffer3, size_t aLen3);
	void _DoReplaceBegin(size_t aStart, size_t aLen, const void *aData, size_t aDataLen);
	void _DoReplaceEnd(size_t aStart, size_t aLen, const void *aData, size_t aDataLen);
	void Defaults();
	void CheckCapacity();
};

// ---------------------------------------------------------------------------

#endif // PMData_h

