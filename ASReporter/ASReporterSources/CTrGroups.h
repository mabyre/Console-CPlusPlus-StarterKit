// ===========================================================================
//	CTrGroups.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef CTrGroups_h
#define CTrGroups_h

//-----------------------------------------------------------------------

#include "PMStr.h"
#include "PMHTbl.h"

//-----------------------------------------------------------------------
/*
	A trace entry represents one line of the CCheckListBox in the
	filters dialog. It may correspond to many codes. It contains a
	name and a BOOLEAN stating if it is enabled or not. It does NOT
	contain its list of code. Mapping is done elsewhere.
	A trace group represents one tab in the filters dialog. It contains
	an array on trace entries.
	A trace config is the whole trace thing. It contains a map between
	code and their corresponding trace entry and an array of groups.

	Filters are described in resources:
	ID: aBaseID (10000) passed to CTraceConfig::Load contains the config
	name and the list of entries' ID.
	Each of these lists can be continued on the next line (the current
	line last element must be a +)
	Each entry line: IDn : Entry Name, Entry Group, IDn+1,... a list
	of trace codes.
*/

//-----------------------------------------------------------------------
//	CTraceEntry
//-----------------------------------------------------------------------
PMARRAYDEFINE(CTraceEntry);

class CTraceEntry : public PMRC
{
public:
	CTraceEntry(const PMStrRef& aName);
	virtual ~CTraceEntry();

	CTraceEntry(const CTraceEntry&);

	PMStrRef	GetName();
	PMStrRef	GetShortName();

	BOOL		IsEnabled();
	void		Enable(pmbool afEnable = pmtrue);

protected:
	pmbool			itsfEnabled;
	PMStrRef		itsName;
	PMStrRef		itsShortName;

private:
	CTraceEntry& operator=(const CTraceEntry&);
};

//-----------------------------------------------------------------------
//	CTraceGroup
//-----------------------------------------------------------------------
PMARRAYDEFINE(CTraceGroup);

class CTraceGroup : public PMRC
{
public:
	CTraceGroup(const PMStrRef& aName);
	virtual ~CTraceGroup();

	CTraceGroup(const CTraceGroup&);

	void			AddTraceEntry(CTraceEntryRef anEntry);
	size_t			GetCount() const;
	CTraceEntryRef	GetTraceEntry(size_t anIndex) const;
	PMStrRef		GetName();

protected:
	PMStrRef			itsName;
	CTraceEntryArrayRef	itsEntries;

private:
	CTraceGroup& operator=(const CTraceGroup&);
};

//-----------------------------------------------------------------------
//	CTraceConfig
//-----------------------------------------------------------------------
// A trace code is a pmuint32. 16 most significants bits are the trace module
// and 16 less significants bits are the code (Info, Debug, ...)
// If you use 0xFFFF for the code, it means all code for that module.

PMDEFINE(CTraceConfig);

class CTraceConfig : public PMRC
{
public:
	CTraceConfig();
	virtual ~CTraceConfig();

	CTraceConfig(const CTraceConfig&);

	PMStrRef		GetEntryNameForCode(pmuint32 aCode) const;
	PMStrRef		GetEntryShortNameForCode(pmuint32 aCode) const;
	PMStrRef		GetGroupNameForCode(pmuint32 aCode) const;
	pmbool			IsEntryEnabledForCode(pmuint32 aCode);

	size_t			GetGroupCount();
	CTraceGroupRef	GetGroup(size_t anIndex);

	pmbool			GetDisplayAll()			{ return itsfDisplayAll; }
	pmbool			GetDisplayNone()		{ return itsfDisplayNone; }
	pmbool			GetDisplayName()		{ return itsfDisplayName; } 
	pmbool			GetDisplayTS()			{ return itsfDisplayTS; } 
	pmbool			GetDisplaySpecials()	{ return itsfDisplaySpecials; }
	pmuint16		GetDisplayTID()			{ return itsDisplayTID; }
	char			GetSeparator()			{ return itsSeparator; }
	pmbool			GetDisplayBAddr()		{ return itsfDisplayBAddr; }

	void			SetDisplayAll(pmbool afDisplay);
	void			SetDisplayNone(pmbool afDisplay);
	void			SetDisplayName(pmbool afDisplay);
	void			SetDisplayTS(pmbool afDisplay);
	/* Returns 0 for None, 4 for 4 hex digits or 8 for 8 hex digits */
	void			SetDisplayTID(pmuint16 aDisplayTID);
	void			SetDisplaySpecials(pmbool afDisplay);
	void			SetSeparator(char aSeparator);
	void			SetDisplayBAddr(pmbool afDisplay);

	pmbool			Load(pmuint32 aBaseID = 10000);
	pmbool			Save();

protected:
	struct TPair : public PMRC
	{
		CTraceEntryRef	itsEntry;
		CTraceGroupRef	itsGroup;

		TPair(CTraceEntryRef anEntry, CTraceGroupRef aGroup)
		{
			itsEntry = anEntry;
			itsGroup = aGroup;
		}
	};
	typedef PMRef<TPair>				TPairRef;
	typedef PMMap<TPairRef,pmuint32>	TTraceMap;
	typedef PMRef<TTraceMap>			TTraceMapRef;

	TTraceMapRef		itsMap;
	CTraceGroupArrayRef	itsGroups;
	CTraceEntryArrayRef	itsEntries;
	pmbool				itsfDisplayAll, itsfDisplayNone, itsfDisplayName, itsfDisplayTS, itsfDisplaySpecials, itsfDisplayBAddr;
	pmuint16			itsDisplayTID;
	char				itsSeparator;

	// FiltersSection contains enabled/disabled boolean for each entry
	// FiltersSettingSection contains other settings (All, None, Name)
	PMStrRef			itsFiltersSection, itsFiltersSettingsSection;

	// Load a list of resource strings starting at ID anID, separated with ,
	// and that may continue on several consecutives lines terminated by +
	// anID is adjusted to the next line after read.
	static PMStrArrayRef	LoadList(UINT* anID);

	pmbool			AddCode(pmuint32 aCode, PMStrRef anEntryName, PMStrRef aGroupName);

private:
	CTraceConfig& operator=(const CTraceConfig&);
};

//-----------------------------------------------------------------------
#endif // CTrGroups_h
