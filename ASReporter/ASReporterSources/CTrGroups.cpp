// ===========================================================================
//	CTrGroups.cpp
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
//	Author  : RNi
// ===========================================================================

#include "stdafx.h"
#include "OSReport.h"
#include "CTrGroups.h"

#include "PMTrace.h"

//-----------------------------------------------------------------------

#define PM_TRACEMODULE	USER0
PM_DEFINE_TRACE_CODE(Info, 1);

//-----------------------------------------------------------------------
// CTraceEntry
//-----------------------------------------------------------------------
CTraceEntry::CTraceEntry(const PMStrRef& aName) :
	PMRC()
{
	PM_ASSERT(!aName.IsNull(), TL("CTraceEntry::CTraceEntry(): Bad parameter"));
	itsfEnabled = pmfalse;
	itsName = aName;
	itsShortName = PMStrRef(itsName + PMSTRSTATIC("      "))->Left(6)->UpperCase();
}

//-----------------------------------------------------------------------
CTraceEntry::~CTraceEntry()
{
}

//-----------------------------------------------------------------------
CTraceEntry::CTraceEntry(const CTraceEntry& anEntry) :
	PMRC()
{
	itsfEnabled = anEntry.itsfEnabled;
	itsName = anEntry.itsName;
	itsShortName = anEntry.itsShortName;
}

//-----------------------------------------------------------------------
PMStrRef CTraceEntry::GetName()
{
	return itsName;
}

//-----------------------------------------------------------------------
PMStrRef CTraceEntry::GetShortName()
{
	return itsShortName;
}

//-----------------------------------------------------------------------
BOOL CTraceEntry::IsEnabled()
{
	return itsfEnabled;
}

//-----------------------------------------------------------------------
void CTraceEntry::Enable(pmbool afEnable)
{
	itsfEnabled = afEnable;
}

//-----------------------------------------------------------------------
//	CTraceGroup
//-----------------------------------------------------------------------
CTraceGroup::CTraceGroup(const PMStrRef& aName) :
	PMRC()
{
	PM_ASSERT(!aName.IsNull(), TL("CTraceGroup::CTraceGroup(): Bad parameter"));
	itsName = aName;
	itsEntries = new PMArray<CTraceEntryRef>;
}

//-----------------------------------------------------------------------
CTraceGroup::~CTraceGroup()
{
}

//-----------------------------------------------------------------------
CTraceGroup::CTraceGroup(const CTraceGroup& aGroup) :
	PMRC()
{
	itsName = aGroup.itsName;
	itsEntries = new PMArray<CTraceEntryRef>;
	for (size_t i = 0; i < aGroup.GetCount(); i++)
		itsEntries->AppendObject(new CTraceEntry(*aGroup.GetTraceEntry(i).Get()));
}

//-----------------------------------------------------------------------
	// This must be implemented for IsObjectPresent.
	int Compare(const CTraceEntryRef& e1, const CTraceEntryRef& e2) 
	{
		if (e1.Get() == e2.Get())
			return 0;
		return 1;
	}

void CTraceGroup::AddTraceEntry(CTraceEntryRef anEntry)
{
	if (!itsEntries->IsObjectPresent(anEntry))
		itsEntries->AppendObject(anEntry);
}

//-----------------------------------------------------------------------
size_t CTraceGroup::GetCount() const
{
	return itsEntries->GetCount();
}

//-----------------------------------------------------------------------
CTraceEntryRef CTraceGroup::GetTraceEntry(size_t anIndex) const
{
	return itsEntries->At(anIndex);
}

//-----------------------------------------------------------------------
PMStrRef CTraceGroup::GetName()
{
	return itsName;
}

//-----------------------------------------------------------------------
//	CTraceConfig
//-----------------------------------------------------------------------
CTraceConfig::CTraceConfig() : 
	PMRC()
{
	itsMap = 0;
	itsGroups = 0;
	itsEntries = 0;
	itsfDisplayAll = pmtrue;
	itsfDisplayNone = pmfalse;
	itsfDisplayName = pmfalse;
	itsfDisplayTS = pmfalse;
	itsfDisplaySpecials = pmtrue;
	itsDisplayTID = 0;
	itsSeparator = ':';
	itsFiltersSection = 0;
	itsFiltersSettingsSection = 0;
	itsfDisplayBAddr = pmfalse;
}

//-----------------------------------------------------------------------
CTraceConfig::~CTraceConfig()
{
}

//-----------------------------------------------------------------------
CTraceConfig::CTraceConfig(const CTraceConfig& aConfig) :
	PMRC()
{
	PM_ASSERT(!aConfig.itsMap.IsNull(), TL("CTraceConfig::CTraceConfig(): Passed Config not loaded."));

	itsfDisplayAll = aConfig.itsfDisplayAll;
	itsfDisplayNone = aConfig.itsfDisplayNone;
	itsfDisplayName = aConfig.itsfDisplayName;
	itsfDisplayTS = aConfig.itsfDisplayTS;
	itsDisplayTID = aConfig.itsDisplayTID;
	itsfDisplaySpecials = aConfig.itsfDisplaySpecials;
	itsfDisplayBAddr = aConfig.itsfDisplayBAddr;
	itsFiltersSection = aConfig.itsFiltersSection;
	itsFiltersSettingsSection = aConfig.itsFiltersSettingsSection;
	itsSeparator = aConfig.itsSeparator;
	itsMap = new TTraceMap;
	itsGroups = new PMArray<CTraceGroupRef>;
	itsEntries = new PMArray<CTraceEntryRef>;

	CTraceEntryRef	theEntry;
	CTraceGroupRef	theGroup;
	size_t			i, j;
	// We must recreate all entries and groups here in order to have them
	// in the right order. We must also enable the entries.
	for (i = 0; i < aConfig.itsGroups->GetCount(); i++)
	{
		theGroup = new CTraceGroup(*aConfig.itsGroups->At(i).Get());
		itsGroups->AppendObject(theGroup);
		for (j = 0; j < theGroup->GetCount(); j++)
			itsEntries->AppendObject(theGroup->GetTraceEntry(j));
	}

	// Remap the codes now.
	PMRef<PMArray<pmuint32> >	theArray = aConfig.itsMap->GetKeyArray();
	pmuint32					theCode;
	TPairRef					thePair;
	for (i = theArray->GetCount(); i > 0 ; i--)
	{
		theCode = theArray->At(i-1);
		thePair = aConfig.itsMap->ObjectForKey(theCode);
		AddCode(theCode, aConfig.GetEntryNameForCode(theCode), aConfig.GetGroupNameForCode(theCode));
	}
}

//-----------------------------------------------------------------------
pmbool CTraceConfig::AddCode(pmuint32 aCode, PMStrRef anEntryName, PMStrRef aGroupName)
{
	PM_ASSERT(!itsMap.IsNull(), TL("CTraceConfig::AddCode(): Not initialized."));

	if (itsMap->HasKey(aCode))
		return pmfalse;

	CTraceEntryRef				theEntry;
	size_t						i;

	// Find if an entry with the same name exists
	for (i = 0; i < itsEntries->GetCount(); i++)
	{
		if (itsEntries->At(i)->GetName()->cCompare(anEntryName) == 0)
		{
			theEntry = itsEntries->At(i);
			break;
		}
	}
	// If no entry with the same name exists, create one.
	if (theEntry.IsNull())
	{
		PM_TRACE(Info, TL("Creating entry [%s] for code [%lx]", anEntryName->c_str(), aCode));
		theEntry = new CTraceEntry(anEntryName);
		// Load the settings for this entry
		theEntry->Enable(::AfxGetApp()->GetProfileInt(itsFiltersSection, theEntry->GetName(), 1));
		itsEntries->AppendObject(theEntry);
	}

	// Find if a group with the same name exists
	CTraceGroupRef	theGroup;
	for (i = 0; i < itsGroups->GetCount(); i++)
	{
		if (itsGroups->At(i)->GetName()->cCompare(aGroupName) == 0)
		{
			theGroup = itsGroups->At(i);
			break;
		}
	}
	// If no group with this name, create one.
	if (theGroup.IsNull())
	{
		PM_TRACE(Info, TL("Creating group [%s] for code [%lx]", aGroupName->c_str(), aCode));
		theGroup = new CTraceGroup(aGroupName);
		itsGroups->AppendObject(theGroup);
	}

	// Add this entry to the group list
	theGroup->AddTraceEntry(theEntry);
	// Add this association to the map
	itsMap->SetObjectForKey(new TPair(theEntry, theGroup), aCode);

	return pmtrue;
}

//-----------------------------------------------------------------------
PMStrRef CTraceConfig::GetGroupNameForCode(pmuint32 aCode) const
{
	TPairRef thePair = itsMap->ObjectForKey(aCode);
	if (thePair.IsNull())
		thePair = itsMap->ObjectForKey(aCode | 0xFFFF);
	if (thePair.IsNull())
	{
		PM_TRACE(Info, TL("CTraceConfig::GetGroupNameForCode(): No group for code [%lx]", aCode));
		return 0;
	}

	return thePair->itsGroup->GetName();
}

//-----------------------------------------------------------------------
PMStrRef CTraceConfig::GetEntryNameForCode(pmuint32 aCode) const
{
	TPairRef thePair = itsMap->ObjectForKey(aCode);
	if (thePair.IsNull())
		thePair = itsMap->ObjectForKey(aCode | 0xFFFF);
	if (thePair.IsNull())
	{
		PM_TRACE(Info, TL("CTraceConfig::GetEntryNameForCode(): No entry for code [%lx]", aCode));
		return 0;
	}

	return thePair->itsEntry->GetName();
}

//-----------------------------------------------------------------------
PMStrRef CTraceConfig::GetEntryShortNameForCode(pmuint32 aCode) const
{
	TPairRef thePair = itsMap->ObjectForKey(aCode);
	if (thePair.IsNull())
		thePair = itsMap->ObjectForKey(aCode | 0xFFFF);
	if (thePair.IsNull())
	{
		PM_TRACE(Info, TL("CTraceConfig::GetEntryShortNameForCode(): No entry for code [%lx]", aCode));
		return 0;
	}

	return thePair->itsEntry->GetShortName();
}

//-----------------------------------------------------------------------
pmbool CTraceConfig::IsEntryEnabledForCode(pmuint32 aCode)
{
	if (itsfDisplayAll)
		return pmtrue;
	if (itsfDisplayNone)
		return pmfalse;

	// If this exact code if found: Module + Code, use it
	// Else use the "generic" code for that module Module + 0xffff
	TPairRef thePair = itsMap->ObjectForKey(aCode);
	if (thePair.IsNull())
		thePair = itsMap->ObjectForKey(aCode | 0xFFFF);
	if (thePair.IsNull())
	{
		PM_TRACE(Info, TL("CTraceConfig::GetGroupNameForCode(): No entry for code [%lx]", aCode));
		return pmfalse;
	}

	return thePair->itsEntry->IsEnabled();
}

//-----------------------------------------------------------------------
size_t CTraceConfig::GetGroupCount()
{
	return itsGroups->GetCount();
}

//-----------------------------------------------------------------------
CTraceGroupRef CTraceConfig::GetGroup(size_t anIndex)
{
	return itsGroups->At(anIndex);
}

//-----------------------------------------------------------------------
void CTraceConfig::SetDisplayAll(pmbool afDisplay)
{
	itsfDisplayAll = afDisplay;
}

//-----------------------------------------------------------------------
void CTraceConfig::SetDisplayNone(pmbool afDisplay)
{
	itsfDisplayNone = afDisplay;
}

//-----------------------------------------------------------------------
void CTraceConfig::SetDisplayName(pmbool afDisplay)
{
	itsfDisplayName = afDisplay;
}

//-----------------------------------------------------------------------
void CTraceConfig::SetDisplayTS(pmbool afDisplay)
{
	itsfDisplayTS = afDisplay;
}

//-----------------------------------------------------------------------
void CTraceConfig::SetDisplayTID(pmuint16 aDisplayTID)
{
	itsDisplayTID = aDisplayTID;
}

//-----------------------------------------------------------------------
void CTraceConfig::SetDisplaySpecials(pmbool afDisplay)
{
	itsfDisplaySpecials = afDisplay;
}

//-----------------------------------------------------------------------
void CTraceConfig::SetSeparator(char aSeparator)
{
	itsSeparator = aSeparator;
}

//-----------------------------------------------------------------------
void CTraceConfig::SetDisplayBAddr(pmbool afDisplay)
{
	itsfDisplayBAddr = afDisplay;
}

//-----------------------------------------------------------------------
pmbool CTraceConfig::Load(pmuint32 aBaseID)
{
	CString			theString;
	UINT			theID, theID2;
	PMStrArrayRef	theTraceEntryIDs;
	int				theSep;

	// Load the main filters ID pointing to the filters description
	if (!theString.LoadString(aBaseID) || (theID = ::atoi(theString)) == 0)
	{
		PM_TRACE(Info, TL("CTraceConfig::Load(): Unable to load filters base ID."));
		return pmfalse;
	}

	// Load the description: name of the set (to save), list of IDS of entries
	theTraceEntryIDs = LoadList(&theID);
	if (theTraceEntryIDs.IsNull() || theTraceEntryIDs->GetCount() < 2)
	{
		PM_TRACE(Info, TL("CTraceConfig::Load(): Unable to load filters description."));
		return pmfalse;
	}

	// Registry entry to store filters state (active or not)
	itsFiltersSection = theTraceEntryIDs->At(0);
	// Registry entry to store filters settings (All, No, Display name)
	itsFiltersSettingsSection = itsFiltersSection;

	itsFiltersSection += _T(" Filters");
	itsFiltersSettingsSection += _T(" Filters Settings");

	itsMap = new TTraceMap;
	itsGroups = new PMArray<CTraceGroupRef>;
	itsEntries = new PMArray<CTraceEntryRef>;

	PMStrArrayRef	theParams;
	PMStrRef		theEntryName, theGroupName;
	size_t			i, j;
	pmuint32		theCode;

	// i == 0 is trace name
	for (i = 1; i < theTraceEntryIDs->GetCount(); i++)
	{
		theID = ::atoi(theTraceEntryIDs->At(i));
		if (theID == 0)
		{
			PM_TRACE(Info, TL("CTraceConfig::Load(): Filter ID is 0 for index [%t].", i));
			continue;
		}

		theID2 = theID;
		// Load entry name and group
		theParams = LoadList(&theID);

		if (theParams->GetCount() != 2)
		{
			PM_TRACE(Info, TL("CTraceConfig::Load(): Bad string for Filter ID [%lu].", theID2));
			continue;
		}

		theEntryName = theParams->At(0);
		theGroupName = theParams->At(1);

		// Load trace codes
		theParams = LoadList(&theID);
		for (j = 0; j < theParams->GetCount(); j++)
		{
//			theCode = ::pm_atouint32(theParams->At(j));
			// sscanf can parse hexa too
			theCode = 0; // sscanf will not reset it if an error occurs.
			::sscanf(theParams->At(j)->c_str(), "%x", &theCode);
			if (theCode == 0 || !AddCode(theCode, theEntryName, theGroupName))
				PM_TRACE(Info, TL("CTraceConfig::Load(): Bad String (not a number) [%s] for ID [%lu]", theParams->At(j)->c_str(), theID2));
		}
	}

	if (itsGroups->GetCount() == 0)
	{
		PM_TRACE(Info, TL("CTraceConfig::Load(): No group."));
		return pmfalse;
	}
	if (itsEntries->GetCount() == 0)
	{
		PM_TRACE(Info, TL("CTraceConfig::Load(): No entry."));
		return pmfalse;
	}
	if (itsMap->GetKeyArray()->GetCount() == 0)
	{
		PM_TRACE(Info, TL("CTraceConfig::Load(): No code."));
		return pmfalse;
	}

	itsfDisplayAll = ::AfxGetApp()->GetProfileInt(itsFiltersSettingsSection, COSReportApp::sAllFiltersEntry, 0);
	itsfDisplayNone = ::AfxGetApp()->GetProfileInt(itsFiltersSettingsSection, COSReportApp::sNoFilterEntry, 0);
	itsfDisplayName = ::AfxGetApp()->GetProfileInt(itsFiltersSettingsSection, COSReportApp::sDisplayNameEntry, 0);
	itsfDisplayTS = ::AfxGetApp()->GetProfileInt(itsFiltersSettingsSection, COSReportApp::sDisplayTSEntry, 0);
	itsDisplayTID = ::AfxGetApp()->GetProfileInt(itsFiltersSettingsSection, COSReportApp::sDisplayTIDEntry, 0);
	itsfDisplaySpecials = ::AfxGetApp()->GetProfileInt(itsFiltersSettingsSection, COSReportApp::sDisplaySpecialsEntry, 1);
	itsfDisplayBAddr = ::AfxGetApp()->GetProfileInt(itsFiltersSettingsSection, COSReportApp::sDisplayBAddrEntry, 0);

	if (itsDisplayTID != 0)
	{
		if (itsDisplayTID != 4 && itsDisplayTID != 8)
			itsDisplayTID = 4;
	}
	theSep = ::AfxGetApp()->GetProfileInt(itsFiltersSettingsSection, COSReportApp::sSeparatorEntry, 0);
	if (theSep == 0 || theSep > 255)
		itsSeparator = ':';
	else
		itsSeparator = (char)theSep;

	return pmtrue;
}

//-----------------------------------------------------------------------
pmbool CTraceConfig::Save()
{
	WriteProfileString(itsFiltersSection, 0, 0);

	CTraceEntryRef theEntry;
	for (size_t i = 0; i < itsEntries->GetCount(); i++)
	{
		theEntry = itsEntries->At(i);
		::AfxGetApp()->WriteProfileInt(itsFiltersSection, theEntry->GetName(), theEntry->IsEnabled());
	}

	::AfxGetApp()->WriteProfileInt(itsFiltersSettingsSection, COSReportApp::sAllFiltersEntry, itsfDisplayAll);
	::AfxGetApp()->WriteProfileInt(itsFiltersSettingsSection, COSReportApp::sNoFilterEntry, itsfDisplayNone);
	::AfxGetApp()->WriteProfileInt(itsFiltersSettingsSection, COSReportApp::sDisplayNameEntry, itsfDisplayName);
	::AfxGetApp()->WriteProfileInt(itsFiltersSettingsSection, COSReportApp::sDisplayTSEntry, itsfDisplayTS);
	::AfxGetApp()->WriteProfileInt(itsFiltersSettingsSection, COSReportApp::sDisplayTIDEntry, itsDisplayTID);
	::AfxGetApp()->WriteProfileInt(itsFiltersSettingsSection, COSReportApp::sSeparatorEntry, itsSeparator);
	::AfxGetApp()->WriteProfileInt(itsFiltersSettingsSection, COSReportApp::sDisplaySpecialsEntry, itsfDisplaySpecials);
	::AfxGetApp()->WriteProfileInt(itsFiltersSettingsSection, COSReportApp::sDisplayBAddrEntry, itsfDisplayBAddr);

	return pmtrue;
}

//-----------------------------------------------------------------------
PMStrArrayRef CTraceConfig::LoadList(UINT* anID)
{
	CString			theString;
	PMStrRef		theTempString;
	size_t			theTempCount;
	PMStrArrayRef	theArray = new PMArray<PMStrRef>;
	PMStrArrayRef	theTempArray;
	pmbool			thefGoOn = pmfalse;

	do 
	{
		if (!theString.LoadString(*anID))
		{
			PM_TRACE(Info, TL("CTraceConfig::LoadList(): Failed to load string [%lu]", *anID));
			return 0;
		}
		theTempString = (LPCSTR)theString;
		theTempArray = theTempString->SplitBy(',');
		theTempCount = theTempArray->GetCount();
		if (theTempCount > 0)
		{
			theTempString = theTempArray->At(theTempCount-1)->SkipTrailing()->SkipLeading();
			thefGoOn = (theTempString->cCompare(PMSTRSTATIC("+")) == 0);
			if (thefGoOn)
			{
				theTempArray->RemoveObjectAtIndex(theTempCount-1);
				theTempCount--;
			}
			for (size_t i = 0; i < theTempCount; i++)
				theArray->AppendObject(theTempArray->At(i));
		}
		else
			thefGoOn = pmfalse;
		(*anID)++;
	} 	while (thefGoOn);

	return theArray;
}
