//=======================================================================
// LTipInfo.cpp
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
//=======================================================================

#include "LTipInfo.h"

//-----------------------------------------------------------------------
LTipInfo::LTipInfo()
{
	itsfCentered = FALSE;
	itsTipString = itsMessageString = 0;
}

//-----------------------------------------------------------------------
LTipInfo::~LTipInfo()
{
	if (itsTipString != 0)
		delete itsTipString;
	if (itsMessageString != 0)
		delete itsMessageString;
}

//-----------------------------------------------------------------------
void LTipInfo::SetCenter(BOOL afCenter)
{
	itsfCentered = afCenter;
}

//-----------------------------------------------------------------------
void LTipInfo::SetString(LPCSTR aTipString)
{
	if (itsTipString != 0)
	{
		delete itsTipString;
		itsTipString = 0;
	}
	if (itsMessageString != 0)
	{
		delete itsMessageString;
		itsMessageString = 0;
	}

	if (aTipString == 0)
		return;

	char* theCut = (char*)aTipString;
	for (;;)
	{
		theCut = ::strchr(theCut, '~');
		if (theCut == 0)
			break; // Not found

		if (theCut[1] != '~')
			break; // Found

		theCut += 2; // Escaped
	}
	int theLen = ::strlen(aTipString);

	if (theCut != 0)
	{
		theCut++;
		itsMessageString = new char[::strlen(theCut) + 1];
		::strcpy(itsMessageString, theCut);
		theLen -= (::strlen(theCut) + 1);
	}

	if (aTipString[0] == ']' && theLen > 1 && aTipString[1] != ']')
	{
		SetCenter(TRUE);
		aTipString++;
		theLen--;
	}
	itsTipString = new char[theLen + 1];
	::strncpy(itsTipString, aTipString, theLen);
	itsTipString[theLen] = 0;
}

//-----------------------------------------------------------------------
LPCSTR LTipInfo::GetTipString()
{
	return itsTipString;
}

//-----------------------------------------------------------------------
LPCSTR LTipInfo::GetMessageString()
{
	return itsMessageString;
}
//-----------------------------------------------------------------------
BOOL LTipInfo::GetCenter()
{
	return itsfCentered;
}
