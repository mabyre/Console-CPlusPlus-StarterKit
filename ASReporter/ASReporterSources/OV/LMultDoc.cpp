//=======================================================================
// LMultDoc.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

//-----------------------------------------------------------------------
#include "LMultDoc.h"

#include "LDoc.h"
#include "LMDICWnd.h"

//-----------------------------------------------------------------------
LMultiDocTemplate::LMultiDocTemplate(UINT anIDResource, CRuntimeClass* aDocClass, CRuntimeClass* aFrameClass, CRuntimeClass* aViewClass) :
	CMultiDocTemplate(anIDResource, aDocClass, aFrameClass, aViewClass)
{
	itsNextDocCreateParams = 0;
	itsfNextDocCanClose = itsfNextDocCanResize = TRUE;
}

//-----------------------------------------------------------------------
LMultiDocTemplate::~LMultiDocTemplate()
{
}

//-----------------------------------------------------------------------
CDocument* LMultiDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, void* aCreateParams, BOOL afCanClose, BOOL afCanResize, BOOL afMakeVisible)
{
	itsNextDocCreateParams = aCreateParams;
	itsfNextDocCanClose = afCanClose;
	itsfNextDocCanResize = afCanResize;

	return CMultiDocTemplate::OpenDocumentFile(lpszPathName, afMakeVisible);
}

//-----------------------------------------------------------------------
CDocument* LMultiDocTemplate::CreateNewDocument()
{
	CDocument* theDoc = CMultiDocTemplate::CreateNewDocument();

	LDocument* theLDoc = DYNAMIC_DOWNCAST(LDocument, theDoc);
	if (theLDoc != 0)
		theLDoc->SetCreateParams(itsNextDocCreateParams);

	return theDoc;
}

//-----------------------------------------------------------------------
CFrameWnd* LMultiDocTemplate::CreateNewFrame(CDocument* aDoc, CFrameWnd* anOther)
{
	CFrameWnd* theFrame = CMultiDocTemplate::CreateNewFrame(aDoc, anOther);

	LMDIChildWnd* theLFrame = DYNAMIC_DOWNCAST(LMDIChildWnd, theFrame);
	if (theLFrame != 0)
	{
		theLFrame->SetfCanClose(itsfNextDocCanClose);
		theLFrame->SetfCanResize(itsfNextDocCanResize);
	}

	return theFrame;
}
