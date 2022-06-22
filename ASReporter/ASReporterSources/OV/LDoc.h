//=======================================================================
// LDoc.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#ifndef LDoc_h
#define LDoc_h

//-----------------------------------------------------------------------
#include <afxwin.h>
#include "LDefines.h"

//-----------------------------------------------------------------------
class LDocument 
	: public CDocument
{
public:
	LDocument();
	virtual ~LDocument();

	virtual void SetCreateParams(void* aParams);
	virtual void* GetCreateParams();

protected:
	void*	itsCreateParams;

	DECLARE_SERIAL(LDocument)

private:
	LDocument(const LDocument&);
	LDocument& operator=(const LDocument&);
};

//-----------------------------------------------------------------------
#endif // LDoc_h
