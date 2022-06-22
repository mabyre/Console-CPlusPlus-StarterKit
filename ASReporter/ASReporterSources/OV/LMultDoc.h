//=======================================================================
// LMultDoc.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#ifndef LMultDoc_h
#define LMultDoc_h

//-----------------------------------------------------------------------
#include <afxwin.h>
#include "LDefines.h"

//-----------------------------------------------------------------------
class LMultiDocTemplate : 
	public CMultiDocTemplate
{
public:
	LMultiDocTemplate(UINT anIDResource, CRuntimeClass* aDocClass, CRuntimeClass* aFrameClass, CRuntimeClass* aViewClass);
	virtual ~LMultiDocTemplate();

	virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, void* aCreateParams = 0, BOOL afCanClose = TRUE, BOOL afCanResize = TRUE, BOOL afMakeVisible = TRUE);

protected:
	void*	itsNextDocCreateParams;
	BOOL	itsfNextDocCanClose, itsfNextDocCanResize;

	virtual CDocument* CreateNewDocument();
	virtual CFrameWnd* CreateNewFrame(CDocument* aDoc, CFrameWnd* anOther);

private:
	LMultiDocTemplate(const LMultiDocTemplate&);
	LMultiDocTemplate& operator=(const LMultiDocTemplate&);
};

//-----------------------------------------------------------------------
#endif // LMultDoc_h
