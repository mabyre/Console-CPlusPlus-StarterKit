// ===========================================================================
//	PMMonit.cpp
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#include "PMMonit.h"

// ---------------------------------------------------------------------------

#define PM_TRACEMODULE STRUCT
PM_DEFINE_TRACE_CODE(Info, 1);
PM_DEFINE_TRACE_CODE(Debug, 2);

// ===========================================================================
//	PMMonitorable
// ===========================================================================

PMMonitorable::PMMonitorable(pmuint32 aClass) :
	itsClass(aClass),
	itsCallBack(0)
{
}

// ---------------------------------------------------------------------------

PMMonitorable::~PMMonitorable()
{
}

// ---------------------------------------------------------------------------

void PMMonitorable::SetCallBack(PMMonitorCallBack *aCallBack)
{
	itsCallBack = aCallBack;
}

// ---------------------------------------------------------------------------

void PMMonitorable::MonitorIn(const PMMonitorInfoRef &anInfo)
{
	if (itsCallBack != 0)
		itsCallBack->MonitorIn(anInfo);
}

// ---------------------------------------------------------------------------

void PMMonitorable::MonitorOut(const PMMonitorInfoRef &anInfo)
{
	if (itsCallBack != 0)
		itsCallBack->MonitorIn(anInfo);
}

// ---------------------------------------------------------------------------

PMMonitorInfoRef PMMonitorable::MakeTextInfo(const PMStrRef &aText)
{
	return new PMMonitorInfo(this, aText);
}

