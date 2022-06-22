//=======================================================================
// LDoc.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

//-----------------------------------------------------------------------
#include "LDoc.h"

IMPLEMENT_SERIAL(LDocument, CDocument, 1)

//-----------------------------------------------------------------------
LDocument::LDocument() :
	CDocument()
{
}

//-----------------------------------------------------------------------
LDocument::~LDocument()
{
}

//-----------------------------------------------------------------------
void LDocument::SetCreateParams(void* aParams)
{
	itsCreateParams = aParams;
}

//-----------------------------------------------------------------------
void* LDocument::GetCreateParams()
{
	return itsCreateParams;
}
