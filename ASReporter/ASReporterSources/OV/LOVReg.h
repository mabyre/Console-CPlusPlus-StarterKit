//=======================================================================
// LOVReg.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================
//
// It's not possible to remove ManageControlParameters from LControl::CreateInstance 
// because it must happen after the new LControl and before theInstance->Init(..)

#ifndef LOVREG_H
#define LOVREG_H

//-----------------------------------------------------------------------
#include <afxwin.h>
#include "LDefines.h"

#ifdef OVF_CF_TIPS
	#include <afxcmn.h>
#endif
#ifdef OVF_CF_SIZABLE
	class LSizable;
#endif

// Declared outside the class scope to be used by other classes for arrays declarations
const	int kClassLenMax = 64;

//-----------------------------------------------------------------------
class LOVRegister
{
protected:
	typedef CWnd*	(*TCreateProc)(HWND aHwnd);

public:
	static void		RegisterClass(const char* const aClassName, CRuntimeClass* aRC);
	static void		UnRegisterClass(const char* const aClassName);

	static void		CreateInstances(CWnd* aParent);
	static void		EndDialogOrView(HWND aHwnd);

protected:
	struct TClassData
	{
		CRuntimeClass*	itsRuntimeClass;
	};

	static CMapStringToPtr	cClassNameMap;

#ifdef OVF_CF_SIZABLE
	static LSizable*	cSizable;

	static void 	ManageDynSize(HWND aHwnd, char* aValue);
#endif

#ifdef OVF_CF_TIPS
	static void		ManageTip(HWND aHwnd, CWnd* aParent, char* aText);
#endif

	static BOOL		CALLBACK CreationCallBack(HWND aHwnd,LPARAM aLParam);
	static CWnd*	InternalCreateControl(HWND aControlHwnd, CWnd* aParent);
	static CWnd*	CreateInstance(char* aClassName, HWND aHwnd);

	static void		ManageParameters(char* aParams, CWnd* aCWnd, HWND aHwnd, CWnd* aParent);
	static BOOL		DoCommonAttribute(CWnd* aCWnd, HWND aHwnd, CWnd* aParent, char* anAttribute, char* aValue);

	static char*	SplitCaption(char* aCaption);
	static int		GetRealClass(char* aText, char* aClassName, UINT aClassLen);
	static char*	FindNextChar(char* aS, char aC);
	static BOOL		GetNextAttribute(char** theArgs, char** theArg, char** theValue);
	
private:
	LOVRegister();
};

//-----------------------------------------------------------------------
#endif // LOVREG_H
