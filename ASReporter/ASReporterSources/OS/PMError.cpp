// ===========================================================================
//	PMError.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMError.h"
#include "PMOSHelp.h"

// ===========================================================================
//	PMError
// ===========================================================================

PMError::PMError() 
{
	itsError = kPMNoError;
}

// ---------------------------------------------------------------------------

PMError::PMError(const PMError& anError)
{
	itsError = anError.itsError;
}

// ---------------------------------------------------------------------------

PMError::~PMError()
{
}

// ---------------------------------------------------------------------------

PMError& PMError::operator=(const PMError& anError)
{
	if (this != &anError)
		itsError = anError.itsError;

	return *this;
}

// ---------------------------------------------------------------------------

int PMError::operator==(const PMError& anError)
{
	return (itsError == anError.itsError);
}


// ---------------------------------------------------------------------------

PMStrRef PMError::GetAsString() const
{
	return PMOSHelper::GetCurrent()->GetErrorString(*this);
}
