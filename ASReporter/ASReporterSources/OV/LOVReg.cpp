//=======================================================================
// LOVReg.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================
//
// How does it work ?
//
// LContainer classes (LDialog, LFormView, LPropertyPage, ...) call CreateInstances.
// CreateInstances call EnumChildWindows that calls CreationCallback for each widget
// CreationCallback just call InternalCreateControl.
// InternalCreateControl finds the Real C++ Class to construct and call
// CreateInstance that creates (new) the C++ instance using MFC RTTI 
// It also subclasses the control and returns
// InternalCreateControl then manages both common and control-specific parameters
// And then calls Init for the new created control.
//
// ---------- Not implemented yet. Will be if necessary -------------------
// It's possible to use this method to dynamically create your own controls:
// Create a Graphical Control with DefWindowProc
// Then call InternalCreateControl. 

#include "LOVReg.h"

#include "LControl.h"
#include "LContain.h"
#include "LOVUtils.h"

#ifdef OVF_CF_SIZABLE
	#include "LSizable.h"
#endif

#include <ctype.h>

//----------------------------------------------------------------------------
#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------------------------------
CMapStringToPtr	LOVRegister::cClassNameMap;

#ifdef OVF_CF_SIZABLE
	LSizable*	LOVRegister::cSizable;
#endif

//----------------------------------------------------------------------------
void LOVRegister::RegisterClass(const char* const aClassName, CRuntimeClass* aRC)
{
	LOVRegister::TClassData* theData = new LOVRegister::TClassData;
	void* thePtr;

	theData->itsRuntimeClass = aRC;
	BOOL theResult = cClassNameMap.Lookup(aClassName, thePtr);
	ASSERT(!theResult); // Class already registered
	cClassNameMap.SetAt(aClassName,(void*)theData);

	if (cClassNameMap.GetCount() == 1)
	{
		WNDCLASS theWC;

		theWC.style = 0;
		theWC.lpfnWndProc = ::DefWindowProc;
		theWC.cbClsExtra = 0;
		theWC.cbWndExtra = 0;
		theWC.hInstance = AfxGetInstanceHandle();
		theWC.hIcon = 0;
		theWC.hCursor = 0;
		theWC.hbrBackground = (HBRUSH)(COLOR_BACKGROUND+1) ;
		theWC.lpszMenuName = 0;
		theWC.lpszClassName = LOVControl::GetCustomClassName();

		::RegisterClass(&theWC);
	}
}

//----------------------------------------------------------------------------
void LOVRegister::UnRegisterClass(const char* const aClassName)
{
	LOVRegister::TClassData* theData;

	BOOL theResult = cClassNameMap.Lookup(aClassName,(void*&)theData);
	ASSERT(theResult != 0); // Class not registered
	cClassNameMap.RemoveKey(aClassName);
	delete theData;

	if (cClassNameMap.IsEmpty())
		::UnregisterClass(LOVControl::GetCustomClassName(), ::AfxGetInstanceHandle());
}

//-----------------------------------------------------------------------
void LOVRegister::CreateInstances(CWnd* aParent)
{
	ASSERT(aParent != 0); // aParent cannot be 0

#ifdef OVF_CF_SIZABLE
	// We must save cSizable here because if the container contains
	// other container cSizable will be invalidated.
	LSizable* theSizable = cSizable;
	cSizable = 0;
	if (aParent != 0)
		cSizable = dynamic_cast<LSizable*>(aParent);
#endif

	::EnumChildWindows(aParent->GetSafeHwnd(), CreationCallBack, (LPARAM)aParent);

#ifdef OVF_CF_SIZABLE
	if (cSizable != 0)
		cSizable->InitAllCtlsPos();
	cSizable = theSizable;
#endif
}

//-----------------------------------------------------------------------
void LOVRegister::EndDialogOrView(HWND aHwnd)
{
	LOVUtils::DestroyCToolTipCtrl(aHwnd);
}

//-----------------------------------------------------------------------
BOOL CALLBACK LOVRegister::CreationCallBack(HWND aHwnd, LPARAM aLParam)
{	
	CWnd* theParent = (CWnd*)aLParam;

	InternalCreateControl(aHwnd, theParent);

	return 1;
}

//----------------------------------------------------------------------------
CWnd* LOVRegister::InternalCreateControl(HWND aControlHwnd, CWnd* aParent)
{
	char theClassName[kClassLenMax+1], theRealClass[kClassLenMax+1];
	
	GetClassName(aControlHwnd, theClassName, kClassLenMax+1);
	theClassName[kClassLenMax] = '\0';

	char*	theParams = 0; // theParams
	char*	theCaption = 0; // theCaption to set
	char*	theRes = 0; // OV_ATTRIBUTES resource
	char*	theText = 0; // Initial window text

	int						theLen = 0;
	
	theLen = ::GetWindowTextLength(aControlHwnd);
	if (theLen > 0)
	{
		theText = new char[theLen+1];
		::GetWindowText(aControlHwnd, theText, theLen+1);
		theText[theLen] = 0;
		theParams = SplitCaption(theText);
		if (::strlen(theText) != 0)
			theCaption = theText;
	}

	if (theParams == 0)
	{
		HINSTANCE theInstance = ::AfxGetResourceHandle();
		HRSRC theRsrc = ::FindResource(theInstance, MAKEINTRESOURCE(::GetDlgCtrlID(aControlHwnd)), "OV_ATTRIBUTES");
		DWORD theSize = 0;
		if (theRsrc != NULL && (theSize = ::SizeofResource(theInstance, theRsrc)) > 0)
		{
			HGLOBAL theGlobal = ::LoadResource(theInstance, theRsrc);
			void* thePtr = 0;
			if (theGlobal != NULL)
				thePtr = ::LockResource(theGlobal);
			if (thePtr != 0)
			{
				theRes = new char[theSize + 1];
				::memcpy(theRes, thePtr, theSize);
				theRes[theSize] = 0;
				theParams = SplitCaption(theRes);
				if (::strlen(theRes) != 0)
				{
					if (theCaption == 0)
						theCaption = theRes;
					else
						TRACE0("LOVRegister::InternalCreateControl(): A caption was specified both in the control and in the OV_ATTRIBUTES. Later is ignored.\n");
				}
	//			if (theParams == 0) // In case the @ was forgotten
	//				theParams = theRes;
			}
		}
	}

	if (theCaption)
		::SetWindowText(aControlHwnd, theCaption);

	CWnd* theCWnd = 0;

	if (theParams == 0)
		theCWnd = LOVRegister::CreateInstance(theClassName,aControlHwnd);
	else
	{
		switch(GetRealClass(theParams, theRealClass, sizeof(theRealClass)))
		{
			case 1:
				theCWnd = LOVRegister::CreateInstance(theClassName,aControlHwnd);
				break;

			case 2:
				theCWnd = LOVRegister::CreateInstance(theRealClass,aControlHwnd);
				break;
		}
		// No need to Manage parameters if none exist (!thefIsText)
		// Manage parameters even if theCWnd is null for controls which does not create C++ Instance
		// In that case directly use the HWND
		ManageParameters(theParams, theCWnd, aControlHwnd, aParent);
		// theArgs is not still valid because splitted by ManageParameters
	}
	LOVControl* theControl = dynamic_cast<LOVControl*>(theCWnd);
	if (theControl != 0)
		theControl->Init();

	delete[] theText;
	delete[] theRes;

	return theCWnd;
}

//----------------------------------------------------------------------------
CWnd* LOVRegister::CreateInstance(char* aClassName, HWND aHwnd)
{
	LOVRegister::TClassData* theData;
	CWnd* theCWnd = 0;

	if (cClassNameMap.Lookup(aClassName,(void*&)theData) == 0)
		TRACE1("Creation of a '%s' control whose class has not been registered.\n", aClassName);
	else
	{
		theCWnd = CWnd::FromHandlePermanent(aHwnd);
		if (theCWnd != 0)
		{
			theCWnd->Detach();
//			delete theCWnd;   // Must not delete an object inside MFC maps
			TRACE1("Creation of a '%s' Control already attached.\n", aClassName);
		}

		theCWnd = (CWnd*)theData->itsRuntimeClass->CreateObject();
		if (theCWnd == 0)
		{
			TRACE1("Creation of a '%s' control impossible.\n", aClassName);
		}
		else
		{
			theCWnd->SubclassWindow(aHwnd);
			LOVControl* theControl = dynamic_cast<LOVControl*>(theCWnd);
			if (theControl == 0)
			{
				TRACE1("Creation of a '%s' control not derived from LOVControl.\n", aClassName);
			}
			else
				theControl->SetfAutoDelete();
		}
	}

	return theCWnd;
}

//-----------------------------------------------------------------------
void LOVRegister::ManageParameters(char* aParams, CWnd* aCWnd, HWND aHwnd, CWnd* aParent)
{
	if (aParams == 0)
		return;

	LOVControl* theControl = dynamic_cast<LOVControl*>(aCWnd);
		
	char* theAttribute;
	char* theValue;

	while (GetNextAttribute(&aParams,&theAttribute,&theValue))
	{
		if (!DoCommonAttribute(aCWnd, aHwnd, aParent, theAttribute, theValue))
		{
			if (theControl)
				theControl->DoAttribute(theAttribute, theValue);
		}
	}
}

//-----------------------------------------------------------------------
BOOL LOVRegister::DoCommonAttribute(CWnd* aCWnd, HWND aHwnd, CWnd* aParent, char* anAttribute, char* aValue)
{
	// Prevent this two parameters from being received by the control
	if (::strcmp(anAttribute, "CLASS") == 0 || ::strcmp(anAttribute, "NOCPP") == 0)
		return TRUE;

	// Danger: aCwnd may be 0
#ifdef OVF_CF_SIZABLE
	if (::strcmp(anAttribute, "DYNSIZE") == 0)
	{
		if (cSizable == 0)
			TRACE0("Top-level container for dynsize controls must be inherited from LSizable.\n");
		else
			ManageDynSize(aHwnd, aValue);

		return TRUE;
	}
	else
#endif

#ifdef OVF_CF_TIPS
	if (::strcmp(anAttribute, "TIP") == 0)
	{
		ManageTip(aHwnd, aParent, aValue);
		return TRUE;
	}
#endif

	return FALSE;
}

#ifdef OVF_CF_SIZABLE
//-----------------------------------------------------------------------
void LOVRegister::ManageDynSize(HWND aHwnd, char* aValue)
{
	if (aHwnd == 0)
	{
		TRACE0("Null control: Unable to manage Dynsize.\n");
		return;
	}
	// We must not register for dynsizing the edit part of a combobox
	char theClassName[9];
	int theLen = ::GetClassName(aHwnd, theClassName, sizeof(theClassName));
	theClassName[theLen] = 0;
	if (::stricmp(theClassName, "Edit") == 0)
	{
		int theLen = ::GetClassName(::GetParent(aHwnd), theClassName, sizeof(theClassName));
		theClassName[theLen] = 0;
		if (::stricmp(theClassName, "ComboBox") == 0)
			return;
	}

	LSizable::TSizableCtl theSCtl;

	theSCtl.itsHWnd = aHwnd;

	char* theString = strchr(aValue, ',');
	ASSERT(theString != 0); // Dynsize parameter incorrect dynsize=Tx,Ty,Cx,Cy
	*theString = 0;
	theSCtl.itsTrans.cx = ::atoi(aValue);		//Tx
	aValue = theString + 1;
	theString = strchr(aValue, ',');
	ASSERT(theString != 0); // Dynsize parameter incorrect dynsize=Tx,Ty,Cx,Cy
	*theString = 0;
	theSCtl.itsTrans.cy = ::atoi(aValue);		//Ty
	aValue = theString + 1;
	
	theString = strchr(aValue, ',');
	ASSERT(theString != 0); // Dynsize parameter incorrect dynsize=Tx,Ty,Cx,Cy
	*theString = 0;
	theSCtl.itsHom.cx = ::atoi(aValue);			//Cx
	theSCtl.itsHom.cy = ::atoi(theString+1);	//Cy
	
	// We are sure that cSizable != 0 otherwise ManageDynSize is not called
	cSizable->AddSizableCtl(theSCtl);
}
#endif // OVF_CF_SIZABLE

#ifdef OVF_CF_TIPS
//-----------------------------------------------------------------------
void LOVRegister::ManageTip(HWND aHwnd, CWnd* aParent, char* aText)
{
	LContainer* theContainer = dynamic_cast<LContainer*>(aParent);
	if (theContainer == 0)
	{
		TRACE0("Parent is not a LContainer. Unable to manage Tip.\n");
		return;
	}

	theContainer->RegisterTip(aHwnd, aText);
}
#endif // OVF_CF_TIPS

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Return 0 if NOCPP is found
//		  1 if no new class name is found
//		  2 if a new class name is found
int LOVRegister::GetRealClass(char* aText, char* aClassName, UINT aClassLen)
{
	if (aText == 0)
		return 1;
		
	ASSERT(aClassLen > 0);
	ASSERT(aClassName != 0);

	char* theArg;
	char* theValue;

	char* theArgs = new char[::strlen((char*)aText)+1];
	char* theCopy = theArgs;
	::strcpy(theArgs, aText);
	
	while (GetNextAttribute(&theArgs,&theArg,&theValue))
	{
		if (::strcmp(theArg,"CLASS") == 0)
		{
			::strncpy(aClassName,theValue,aClassLen-1);
			aClassName[aClassLen-1] = '\0';

			delete[] theCopy;
			return 2;
		}

		if (::strcmp(theArg,"NOCPP") == 0)
		{
			delete[] theCopy;
			return 0;
		}
	}

	delete[] theCopy;
	return 1;
}

//-----------------------------------------------------------------------
// Does the same as strchr but if aC is doubled makes it single and ignores it.
char* LOVRegister::FindNextChar(char* aS, char aC)
{
	BOOL thefDone = FALSE;
	
	while (!thefDone)
	{
		aS = strchr(aS,aC);

		if (aS == 0 || *(aS+1) != aC)
			thefDone = TRUE;
		else
		{
			char *s;
			for (s = aS + 1; *s != '\0'; s++)
				*s = *(s+1);
			aS++;
		}
	}

	return aS;
}

//-----------------------------------------------------------------------
// Replace the @ by '\0' so the Caption is split in two parts. Return the begin of the 2nd part
char* LOVRegister::SplitCaption(char* aCaption)
{
	char* theLimit = FindNextChar(aCaption,'@');
	if (theLimit == 0)
		return 0;
	
	*theLimit = '\0';
	return (theLimit + 1);
}

//-----------------------------------------------------------------------
// Return sctrue while there are arguments.
BOOL LOVRegister::GetNextAttribute(char** theArgs, char** theArg, char** theValue)
{
	char* theS;
	
// Si pas d'@ ou pas d'arguments fin
	if (*theArgs == 0 || **theArgs == '\0')
		return FALSE;

// absorber espaces et tabulations
	while (**theArgs == ' ' || **theArgs == '\t')
		(*theArgs)++;

// si pas d'arguments fin
	if (**theArgs == '\0')
		return FALSE;

	*theArg = *theArgs;
	*theValue = 0;

// absorber Arg en passant en MAJ
	while (**theArgs != '\0' && ::strchr(" =\t;",**theArgs) == 0)
	{
		**theArgs = ::toupper(**theArgs);
		(*theArgs)++;
	}
	theS = *theArgs;

// absorber espaces et tabulations
	while (**theArgs == ' ' || **theArgs == '\t')
		(*theArgs)++;

	switch (**theArgs)
	{
		case ';':
			*theS = '\0';
			(*theArgs)++;
		case '\0':
			return TRUE;

		case '=':
			*theS = '\0';
			break;

		default:
			ASSERT(0); // Control parameters syntax error
			break;
	}

	**theArgs = '\0';
	(*theArgs)++;
	*theValue = *theArgs;

// Cherche le prochain ; et fait pointer theArgs apres pour continuer.
	*theArgs = FindNextChar(*theValue,';');

	if (*theArgs != 0)
	{
		**theArgs = '\0';
		(*theArgs)++;
	}

	return TRUE;
}
